#pragma once
#include <cstdint>
#include <cstring>
#include <string>
#include <vector>
#include "vector.hpp"

// Proton VariantList wire format, as Growtopia speaks it:
//   [u8 count] then per entry: [u8 index][u8 type][payload]
// Type ids are fixed by the protocol -- do not renumber.
enum variant_type_t : uint8_t {
    VARIANT_UNUSED  = 0,
    VARIANT_FLOAT   = 1,
    VARIANT_STRING  = 2,
    VARIANT_VECTOR2 = 3,
    VARIANT_VECTOR3 = 4,
    VARIANT_UINT32  = 5,
    VARIANT_INT32   = 9,
};

class variant_t {
public:
    variant_t() = default;
    variant_t(const char* v) { set(std::string(v)); }
    variant_t(const std::string& v) { set(v); }
    variant_t(float v) { set(v); }
    variant_t(int32_t v) { set(v); }
    variant_t(uint32_t v) { set(v); }
    variant_t(const vector2_t& v) { set(v); }
    variant_t(const vector3_t& v) { set(v); }

    void set(const std::string& v) { type = VARIANT_STRING; m_string = v; }
    void set(const char* v) { set(std::string(v)); }
    void set(float v) { type = VARIANT_FLOAT; m_vec.m_x = v; }
    void set(int32_t v) { type = VARIANT_INT32; m_int = v; }
    void set(uint32_t v) { type = VARIANT_UINT32; m_uint = v; }
    void set(const vector2_t& v) { type = VARIANT_VECTOR2; m_vec = { v.m_x, v.m_y, 0.f }; }
    void set(const vector3_t& v) { type = VARIANT_VECTOR3; m_vec = v; }

    variant_t& operator=(const char* v) { set(std::string(v)); return *this; }
    variant_t& operator=(const std::string& v) { set(v); return *this; }
    variant_t& operator=(float v) { set(v); return *this; }
    variant_t& operator=(int32_t v) { set(v); return *this; }
    variant_t& operator=(uint32_t v) { set(v); return *this; }
    variant_t& operator=(const vector2_t& v) { set(v); return *this; }
    variant_t& operator=(const vector3_t& v) { set(v); return *this; }

    std::string& get_string() { return m_string; }
    const char* c_str() const { return m_string.c_str(); }
    float get_float() const { return m_vec.m_x; }
    int32_t get_int32() const { return type == VARIANT_UINT32 ? static_cast<int32_t>(m_uint) : m_int; }
    uint32_t get_uint32() const { return type == VARIANT_INT32 ? static_cast<uint32_t>(m_int) : m_uint; }
    vector2_t get_vector2() const { return { m_vec.m_x, m_vec.m_y }; }
    vector3_t get_vector3() const { return m_vec; }

    // Human-readable form, used for the packet log.
    std::string str() const {
        switch (type) {
        case VARIANT_FLOAT:   return std::to_string(m_vec.m_x);
        case VARIANT_STRING:  return m_string;
        case VARIANT_VECTOR2: return std::to_string(m_vec.m_x) + ", " + std::to_string(m_vec.m_y);
        case VARIANT_VECTOR3: return std::to_string(m_vec.m_x) + ", " + std::to_string(m_vec.m_y) + ", " + std::to_string(m_vec.m_z);
        case VARIANT_UINT32:  return std::to_string(m_uint);
        case VARIANT_INT32:   return std::to_string(m_int);
        default:              return "unused";
        }
    }

    uint8_t type{ VARIANT_UNUSED };

private:
    std::string m_string{};
    vector3_t m_vec{};
    int32_t m_int{ 0 };
    uint32_t m_uint{ 0 };
};

class variantlist_t {
public:
    variantlist_t() = default;
    template <typename... Args>
    variantlist_t(Args&&... args) {
        int i = 0;
        ((m_items[i++] = variant_t(std::forward<Args>(args))), ...);
    }

    variant_t& operator[](size_t index) { return m_items[index]; }
    variant_t& get(size_t index) { return m_items[index]; }

    // Highest used slot + 1; trailing unused slots are not transmitted.
    uint8_t size() const {
        uint8_t n = 0;
        for (uint8_t i = 0; i < MAX_ITEMS; i++)
            if (m_items[i].type != VARIANT_UNUSED) n = i + 1;
        return n;
    }

    void* serialize_to_mem(uint32_t* size_out, uint8_t* dest) {
        const uint8_t count = size();
        m_buffer.clear();
        m_buffer.push_back(count);

        for (uint8_t i = 0; i < count; i++) {
            variant_t& v = m_items[i];
            if (v.type == VARIANT_UNUSED)
                continue;
            m_buffer.push_back(i);
            m_buffer.push_back(v.type);

            switch (v.type) {
            case VARIANT_FLOAT:   write_raw(v.get_float()); break;
            case VARIANT_STRING: {
                const std::string& s = v.get_string();
                write_raw(static_cast<uint32_t>(s.length()));
                m_buffer.insert(m_buffer.end(), s.begin(), s.end());
            } break;
            case VARIANT_VECTOR2: {
                vector2_t vec = v.get_vector2();
                write_raw(vec.m_x); write_raw(vec.m_y);
            } break;
            case VARIANT_VECTOR3: {
                vector3_t vec = v.get_vector3();
                write_raw(vec.m_x); write_raw(vec.m_y); write_raw(vec.m_z);
            } break;
            case VARIANT_UINT32:  write_raw(v.get_uint32()); break;
            case VARIANT_INT32:   write_raw(v.get_int32()); break;
            default: break;
            }
        }

        if (size_out) *size_out = static_cast<uint32_t>(m_buffer.size());
        if (dest) memcpy(dest, m_buffer.data(), m_buffer.size());
        return m_buffer.data();
    }

    void serialize_from_mem(uint8_t* data) {
        if (!data) return;
        reset();
        const uint8_t count = *data++;

        for (uint8_t n = 0; n < count; n++) {
            const uint8_t index = *data++;
            const uint8_t type = *data++;
            if (index >= MAX_ITEMS)
                return; // malformed; refuse to write past the slots

            switch (type) {
            case VARIANT_FLOAT:   m_items[index] = read_raw<float>(data); break;
            case VARIANT_STRING: {
                const uint32_t len = read_raw<uint32_t>(data);
                m_items[index] = std::string(reinterpret_cast<char*>(data), len);
                data += len;
            } break;
            case VARIANT_VECTOR2: {
                const float x = read_raw<float>(data), y = read_raw<float>(data);
                m_items[index] = vector2_t{ x, y };
            } break;
            case VARIANT_VECTOR3: {
                const float x = read_raw<float>(data), y = read_raw<float>(data), z = read_raw<float>(data);
                m_items[index] = vector3_t{ x, y, z };
            } break;
            case VARIANT_UINT32:  m_items[index] = read_raw<uint32_t>(data); break;
            case VARIANT_INT32:   m_items[index] = read_raw<int32_t>(data); break;
            default: return; // unknown type -> rest of the buffer is unparseable
            }
        }
    }

    std::string print() {
        std::string out{};
        const uint8_t count = size();
        for (uint8_t i = 0; i < count; i++)
            out += "[" + std::to_string(i) + "] " + m_items[i].str() + " ";
        return out;
    }

    void reset() { for (auto& item : m_items) item = variant_t{}; }

    static constexpr size_t MAX_ITEMS = 16;

private:
    template <typename T>
    void write_raw(T value) {
        const auto* bytes = reinterpret_cast<const uint8_t*>(&value);
        m_buffer.insert(m_buffer.end(), bytes, bytes + sizeof(T));
    }

    template <typename T>
    static T read_raw(uint8_t*& data) {
        T value{};
        memcpy(&value, data, sizeof(T));
        data += sizeof(T);
        return value;
    }

    variant_t m_items[MAX_ITEMS]{};
    std::vector<uint8_t> m_buffer{};
};
