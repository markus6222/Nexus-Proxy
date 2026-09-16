// Check the items.dat decoder against the real file. Build and run:
//   cl /EHsc /std:c++17 /D_CRT_SECURE_NO_WARNINGS test_items_dat.cpp && test_items_dat.exe
//
// Mirrors the field walk in items_dat_decode.h. A one-byte drift here is silent --
// every item name and collisionType after it becomes garbage -- so this asserts that
// all records align in order and that well-known item names decode exactly.
#include <cassert>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <fstream>
#include <string>
#include <vector>

#define ITEMS_DAT_FULL_PARSE_VERSION 18

static const char* g_data = nullptr;
static int g_size = 0;
static int g_pos = 0;

static int16_t i16at(int p) { int16_t v; memcpy(&v, g_data + p, 2); return v; }
static uint16_t u16at(int p) { uint16_t v; memcpy(&v, g_data + p, 2); return v; }

static void skip_string() { g_pos += 2 + i16at(g_pos); }

static bool looks_like_item_record(const char* data, int size, int pos, int expected_id) {
    if (pos < 0 || pos + 12 > size) return false;
    int32_t id = 0; memcpy(&id, data + pos, 4);
    if (id != expected_id) return false;
    int16_t name_len = 0; memcpy(&name_len, data + pos + 8, 2);
    if (name_len < 0 || name_len > 256) return false;
    const int texture_at = pos + 10 + name_len;
    if (texture_at + 2 > size) return false;
    int16_t texture_len = 0; memcpy(&texture_len, data + texture_at, 2);
    return texture_len >= 0 && texture_len <= 256;
}

static int find_next_item_record(const char* data, int size, int from, int expected_id) {
    for (int pos = from; pos + 12 <= size; pos++)
        if (looks_like_item_record(data, size, pos, expected_id)) return pos;
    return -1;
}

int main() {
    char* local = nullptr; size_t len = 0;
    if (_dupenv_s(&local, &len, "LOCALAPPDATA") || !local) { printf("no LOCALAPPDATA\n"); return 1; }
    const std::string path = std::string(local) + "\\Growtopia\\cache\\items.dat";

    std::ifstream file(path, std::ios::binary | std::ios::ate);
    if (!file) { printf("SKIP: items.dat not found at %s\n", path.c_str()); return 0; }
    g_size = static_cast<int>(file.tellg());
    std::vector<char> buf(g_size);
    file.seekg(0);
    file.read(buf.data(), g_size);
    g_data = buf.data();

    int16_t version = 0; int32_t count = 0;
    memcpy(&version, g_data, 2);
    memcpy(&count, g_data + 2, 4);
    printf("items.dat version=%d count=%d size=%d\n", version, count, g_size);

    const std::string secret = "PBG892FXX982ABC*";
    std::vector<std::string> names(count);
    int resyncs = 0;
    g_pos = 6;

    for (int i = 0; i < count; i++) {
        const int itemStart = g_pos;
        assert(looks_like_item_record(g_data, g_size, g_pos, i) && "record did not validate");

        int32_t id = 0; memcpy(&id, g_data + g_pos, 4);
        g_pos += 4 + 1 + 1 + 1 + 1;

        int16_t name_len = i16at(g_pos); g_pos += 2;
        std::string name;
        for (int j = 0; j < name_len; j++)
            name += g_data[g_pos + j] ^ secret[(j + id) % secret.length()];
        names[i] = name;
        g_pos += name_len;

        skip_string();                                   // texture
        g_pos += 4 + 1 + 4;                              // textureHash, itemKind, val1
        g_pos += 1 + 1 + 1 + 1 + 1 + 1;                  // tx, ty, spread, stripey, collision, breakHits
        g_pos += 4 + 1 + 2 + 1;                          // restoreTime, clothingType, rarity, maxAmount
        skip_string();                                   // extraFile
        g_pos += 4 + 4;                                  // extraFileHash, audioVolume
        for (int k = 0; k < 4; k++) skip_string();       // pet strings
        g_pos += 1 + 1 + 1 + 1 + 4 + 4;                  // seed/tree bytes, seedColor, seedOverlayColor
        g_pos += 4;                                      // deleted ingredients
        g_pos += 4 + 2 + 2;                              // growTime, val2, isRayman
        skip_string(); skip_string(); skip_string();     // extraOptions, texture2, extraOptions2
        skip_string();                                   // extraFieldUnk_4
        g_pos += 4;
        const uint16_t value = u16at(g_pos); g_pos += 2;
        g_pos += 2;                                      // value2
        g_pos += 2 + (16 - value);                       // unkValueShort1 + odd skip
        g_pos += 2;                                      // unkValueShort2
        g_pos += 50;
        if (version >= 11) skip_string();                // punchOptions
        if (version >= 12) g_pos += 4 + 9;
        if (version >= 13) g_pos += 4;
        if (version >= 14) g_pos += 4;
        if (version >= 15) g_pos += u16at(g_pos + 25) + 27;
        if (version >= 16) skip_string();                // newField
        if (version >= 17) g_pos += 4;
        if (version >= 18) g_pos += 4;
        if (version > ITEMS_DAT_FULL_PARSE_VERSION) {
            g_pos += 11;
            skip_string();                               // description
            g_pos += 12;
        }

        if (version > ITEMS_DAT_FULL_PARSE_VERSION && i + 1 < count
            && !looks_like_item_record(g_data, g_size, g_pos, i + 1)) {
            int next = find_next_item_record(g_data, g_size, g_pos, i + 1);
            if (next < 0) next = find_next_item_record(g_data, g_size, itemStart + 12, i + 1);
            assert(next >= 0 && "lost the next record");
            resyncs++;
            g_pos = next;
        }
    }

    printf("aligned all %d records (%d resync%s)\n", count, resyncs, resyncs == 1 ? "" : "s");
    for (int i = 0; i < 6; i++) printf("  [%d] '%s'\n", i, names[i].c_str());
    printf("  [18] '%s'  [32] '%s'\n", names[18].c_str(), names[32].c_str());
    fflush(stdout);

    // Known ids: if the walk drifts by even one byte these stop matching.
    assert(names[0] == "Blank");
    assert(names[1] == "Blank Seed");
    assert(names[2] == "Dirt");
    assert(names[3] == "Dirt Seed");
    assert(names[4] == "Lava");
    assert(names[18] == "Fist");
    assert(names[32] == "Wrench");

    int printable = 0;
    for (const std::string& n : names) {
        bool ok = true;
        for (unsigned char c : n) if (c < 32 || c > 126) { ok = false; break; }
        if (ok) printable++;
    }
    printf("names printable: %d/%d\n", printable, count);
    assert(printable == count && "garbled names -- the walk drifted");

    printf("items.dat: all checks passed\n");
    return 0;
}
