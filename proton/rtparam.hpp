#pragma once
#include <cstdint>
#include <string>
#include <vector>

// Growtopia's text protocol: newline-separated records, each "key|value|value2|..."
// m_key is the field before the first '|', m_values holds the values after it, and
// m_value is the first of those. The key is deliberately NOT in m_values: callers index
// values directly, e.g. "posXY|100|200" -> m_values[0]=x, m_values[1]=y, and a chat line
// "|text|/command" (which really does start with '|') -> m_key="", m_values[1]="/command".
struct rtvar_t {
    std::string m_key{};
    std::string m_value{};
    std::vector<std::string> m_values{};
};

class rtvar {
public:
    static rtvar parse(const std::string& input) {
        rtvar out{};
        size_t start = 0;

        while (start <= input.length()) {
            size_t end = input.find('\n', start);
            if (end == std::string::npos) end = input.length();

            std::string line = input.substr(start, end - start);
            start = end + 1;

            // Strip trailing CR and any stray NULs from packet padding.
            while (!line.empty() && (line.back() == '\r' || line.back() == '\0'))
                line.pop_back();
            if (line.empty())
                continue;

            rtvar_t var{};
            size_t field_start = 0;
            bool first = true;
            while (field_start <= line.length()) {
                size_t field_end = line.find('|', field_start);
                if (field_end == std::string::npos) field_end = line.length();
                std::string field = line.substr(field_start, field_end - field_start);
                if (first) {
                    var.m_key = std::move(field);
                    first = false;
                }
                else {
                    var.m_values.push_back(std::move(field));
                }
                field_start = field_end + 1;
            }

            if (!var.m_values.empty())
                var.m_value = var.m_values[0];
            out.m_vars.push_back(std::move(var));
        }
        return out;
    }

    // Returns nullptr when absent -- callers test it directly.
    rtvar_t* find(const std::string& key) {
        for (auto& var : m_vars)
            if (var.m_key == key) return &var;
        return nullptr;
    }

    std::string get(const std::string& key) {
        rtvar_t* var = find(key);
        return var ? var->m_value : std::string{};
    }

    int32_t get_int(const std::string& key) {
        rtvar_t* var = find(key);
        if (!var) return 0;
        try { return std::stoi(var->m_value); }
        catch (...) { return 0; }
    }

    // True only when the key exists AND its value actually parses as an int.
    bool validate_int(const std::string& key) {
        rtvar_t* var = find(key);
        if (!var || var->m_value.empty()) return false;
        try { size_t used = 0; (void)std::stoi(var->m_value, &used); return used == var->m_value.length(); }
        catch (...) { return false; }
    }

    rtvar_t& get(size_t index) { return m_vars[index]; }
    size_t size() const { return m_vars.size(); }
    bool valid() const { return !m_vars.empty(); }

    // Rebuilds the exact input, leading '|' on value-only lines included.
    std::string serialize() const {
        std::string out{};
        for (size_t i = 0; i < m_vars.size(); i++) {
            out += m_vars[i].m_key;
            for (const std::string& value : m_vars[i].m_values)
                out += "|" + value;
            if (i + 1 < m_vars.size()) out += "\n";
        }
        return out;
    }

private:
    std::vector<rtvar_t> m_vars{};
};
