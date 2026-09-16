// Round-trip check for the Proton wire format. Build and run standalone:
//   cl /EHsc /std:c++17 proton\test_variant.cpp && test_variant.exe
#include <cassert>
#include <cstdio>
#include "variant.hpp"
#include "rtparam.hpp"
#include "hash.hpp"

int main() {
    // --- variantlist round-trip, one of every type ---
    variantlist_t out{ "OnSendToServer" };
    out[1] = uint32_t{ 17191 };
    out[2] = int32_t{ -42 };
    out[3] = 3.5f;
    out[4] = vector2_t{ 12.f, 34.f };
    out[5] = vector3_t{ 1.f, 2.f, 3.f };
    out[6] = std::string("127.0.0.1|door");

    uint32_t size = 0;
    uint8_t* buf = static_cast<uint8_t*>(out.serialize_to_mem(&size, nullptr));
    assert(size > 0);
    assert(buf[0] == 7); // count byte leads the buffer

    variantlist_t in{};
    in.serialize_from_mem(buf);
    assert(in[0].get_string() == "OnSendToServer");
    assert(in[1].get_uint32() == 17191);
    assert(in[2].get_int32() == -42);
    assert(in[3].get_float() == 3.5f);
    assert(in[4].get_vector2().m_x == 12.f && in[4].get_vector2().m_y == 34.f);
    assert(in[5].get_vector3().m_z == 3.f);
    assert(in[6].get_string() == "127.0.0.1|door");
    assert(in.size() == 7);

    // Type tags are fixed by the protocol -- catch any renumbering.
    assert(in[0].type == VARIANT_STRING && in[1].type == VARIANT_UINT32);
    assert(in[2].type == VARIANT_INT32 && in[3].type == VARIANT_FLOAT);
    assert(in[4].type == VARIANT_VECTOR2 && in[5].type == VARIANT_VECTOR3);

    // --- rtvar parsing ---
    // m_values excludes the key. The chat line really does begin with '|', so its key is
    // empty and the command lands at m_values[1] -- which is where events.cpp reads it.
    rtvar var = rtvar::parse("action|input\n|text|/pull bob\nnetID|123\nname|`2Bob``\nposXY|100|200\n");
    assert(var.valid());
    assert(var.get(0).m_key == "action" && var.get(0).m_value == "input");
    assert(var.get(1).m_key.empty());
    assert(var.get(1).m_values.size() == 2);
    assert(var.get(1).m_values[0] == "text");
    assert(var.get(1).m_values[1] == "/pull bob");   // the command itself

    // "posXY|100|200": callers read x from m_values[0] and y from m_values[1]
    rtvar_t* pos = var.find("posXY");
    assert(pos && pos->m_values.size() == 2);
    assert(pos->m_values[0] == "100" && pos->m_values[1] == "200");

    // serialize must rebuild the input byte-for-byte, leading '|' included
    assert(var.serialize() == "action|input\n|text|/pull bob\nnetID|123\nname|`2Bob``\nposXY|100|200");
    assert(var.get_int("netID") == 123);
    assert(var.validate_int("netID"));
    assert(!var.validate_int("name"));   // present but not an int
    assert(!var.validate_int("absent")); // missing entirely
    assert(var.find("name") && var.find("name")->m_value == "`2Bob``");
    assert(var.find("nope") == nullptr);

    // --- hash agreement: the event switch depends on these matching ---
    assert(fnv32("OnSendToServer") == hs::hash32("OnSendToServer"));
    assert(fnv32("OnSpawn") == hs::hash32("OnSpawn"));
    assert(fnv32("OnConsoleMessage") == hs::hash32("OnConsoleMessage"));

    printf("proton: all checks passed\n");
    return 0;
}
