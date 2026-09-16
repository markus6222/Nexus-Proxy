#pragma once
// Proton-style float vectors. Members are m_x/m_y/m_z to match the
#include <cstdint>
// Growtopia packet structs this proxy was written against.

struct vector2_t {
    float m_x{ 0.f }, m_y{ 0.f };

    vector2_t() = default;
    template <typename X, typename Y>
    vector2_t(X x, Y y) : m_x(static_cast<float>(x)), m_y(static_cast<float>(y)) {}

    bool operator==(const vector2_t& o) const { return m_x == o.m_x && m_y == o.m_y; }
    bool operator!=(const vector2_t& o) const { return !(*this == o); }
    vector2_t operator+(const vector2_t& o) const { return { m_x + o.m_x, m_y + o.m_y }; }
    vector2_t operator-(const vector2_t& o) const { return { m_x - o.m_x, m_y - o.m_y }; }
};

struct vector3_t {
    float m_x{ 0.f }, m_y{ 0.f }, m_z{ 0.f };

    vector3_t() = default;
    template <typename X, typename Y, typename Z>
    vector3_t(X x, Y y, Z z)
        : m_x(static_cast<float>(x)), m_y(static_cast<float>(y)), m_z(static_cast<float>(z)) {}

    bool operator==(const vector3_t& o) const { return m_x == o.m_x && m_y == o.m_y && m_z == o.m_z; }
    bool operator!=(const vector3_t& o) const { return !(*this == o); }
};

// Integer variant, used for tile/world coordinates.
struct vector2i_t {
    int32_t m_x{ 0 }, m_y{ 0 };

    vector2i_t() = default;
    template <typename X, typename Y>
    vector2i_t(X x, Y y) : m_x(static_cast<int32_t>(x)), m_y(static_cast<int32_t>(y)) {}

    bool operator==(const vector2i_t& o) const { return m_x == o.m_x && m_y == o.m_y; }
    bool operator!=(const vector2i_t& o) const { return !(*this == o); }
    vector2i_t operator+(const vector2i_t& o) const { return { m_x + o.m_x, m_y + o.m_y }; }
    vector2i_t operator-(const vector2i_t& o) const { return { m_x - o.m_x, m_y - o.m_y }; }
};
