#pragma once
#include <cstdint>
#include <cstddef>

// Proton's string hash. fnv32() is the constexpr form used in `case` labels;
// hs::hash32() is the runtime form applied to the incoming function name.
// Both must produce identical values or the event switch silently stops matching.
constexpr uint32_t fnv32(const char* str, uint32_t hash = 0x811c9dc5) {
    return *str ? fnv32(str + 1, (hash ^ static_cast<uint32_t>(*str)) * 0x01000193) : hash;
}

namespace hs {
    inline uint32_t hash32(const char* str) {
        uint32_t hash = 0x811c9dc5;
        if (!str) return hash;
        for (; *str; ++str)
            hash = (hash ^ static_cast<uint32_t>(*str)) * 0x01000193;
        return hash;
    }
}
