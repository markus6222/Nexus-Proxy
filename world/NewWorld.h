#pragma once
#include <cstdint>
#include <cstring>
#include <string>
#include <vector>
#include "../proton/vector.hpp"
#include "../player.h"

// World state model. The tile parser below follows the current packet layout:
// tile-extra presence comes from the flag bit, not from a hardcoded item-id list.

enum eTileExtraType : uint8_t {
    None = 0, Door = 1, Sign = 2, Lock = 3, Seed = 4,
    Dice = 8, Provider = 9, AchievementBlock = 10, HeartMonitor = 11,
    Mannequin = 14, GameGrave = 16, GameGenerator = 17, Xenonite = 18,
    PhoneBooth = 19, Spotlight = 22, DisplayBlock = 23,
    Vending = 24,
    FishTankPort = 25, Forge = 27, GivingTree = 28, SewingMachine = 32,
    CountryFlag = 33, LobsterTrap = 34, PaintingEasel = 35, WeatherMachine = 40,
    DataBedrock = 42, Shelf = 43, VipEntrance = 44, ChallengeTimer = 45,
    Portrait = 48, GuildWeatherMachine = 49, DnaExtractor = 51, Howler = 52,
    StorageBlock = 54, CookingOven = 55, AudioRack = 56, AdventureBegin = 58,
    TombRobber = 59, TrainingPort = 61, GuildItem = 65,
    KrakenGalaticBlock = 80, FriendsEntrance = 81,
};

enum eTileFlag : uint16_t {
    TILEFLAG_EXTRA   = 1 << 0,
    TILEFLAG_LOCKED  = 1 << 1,
    TILEFLAG_SEED    = 1 << 4,
    TILEFLAG_FLIPPED = 1 << 5,
    TILEFLAG_OPEN    = 1 << 6,
    TILEFLAG_PUBLIC  = 1 << 7,
};

struct tile_extra_t {
    eTileExtraType type{ None };
    // Only the vending fields are read by features; other types are walked past.
    struct { uint32_t item_id{ 0 }; int32_t price{ 0 }; } Vending;
};

struct tile_t {
    uint16_t foreground{ 0 }, background{ 0 };
    uint16_t parent_tile_index{ 0 };
    uint16_t lock_index{ 0 };
    // Same flag word under the three names the feature code uses.
    uint16_t flags_{ 0 }, flags_1{ 0 }, flag{ 0 };
    int flag_int{ 0 };
    bool enabled{ false }, opened{ false };
    vector2i_t pos{};
    tile_extra_t tile_extra{};
};

struct object_t {
    uint32_t id{ 0 };
    vector2_t pos{};
    uint32_t amount{ 0 };
    uint8_t flags{ 0 };
    uint32_t oid{ 0 };
};

struct world_tile_map_t {
    vector2i_t size{};
    std::vector<tile_t*> tiles{};

    tile_t* get_tile(const vector2i_t& pos) {
        if (pos.m_x < 0 || pos.m_y < 0 || pos.m_x >= size.m_x || pos.m_y >= size.m_y)
            return nullptr;
        const size_t index = static_cast<size_t>(pos.m_y) * size.m_x + pos.m_x;
        return index < tiles.size() ? tiles[index] : nullptr;
    }

    void clear() {
        for (tile_t* tile : tiles) delete tile;
        tiles.clear();
        size = {};
    }
    ~world_tile_map_t() { clear(); }

    // Callers take whole-world copies (gt.cpp pathfinding does `auto world = m_world`),
    // so the tiles must be deep-copied or both copies would free the same pointers.
    world_tile_map_t() = default;
    world_tile_map_t(const world_tile_map_t& other) { copy_from(other); }
    world_tile_map_t& operator=(const world_tile_map_t& other) {
        if (this != &other) { clear(); copy_from(other); }
        return *this;
    }

private:
    void copy_from(const world_tile_map_t& other) {
        size = other.size;
        tiles.reserve(other.tiles.size());
        for (const tile_t* tile : other.tiles)
            tiles.push_back(tile ? new tile_t(*tile) : nullptr);
    }
};

struct world_object_map_t {
    std::vector<object_t*> objects{};
    uint32_t drop_id{ 0 };

    // The autocollect thread walks this list while the network thread adds and deletes
    // entries. Copy what you need out first -- holding a pointer across that is a
    // use-after-free.
    std::vector<object_t> snapshot() const {
        std::vector<object_t> out;
        out.reserve(objects.size());
        for (const object_t* object : objects)
            if (object) out.push_back(*object);
        return out;
    }

    void add_object(object_t* object) { objects.push_back(object); }

    object_t* get_objectlol(uint32_t oid) {
        for (object_t* object : objects)
            if (object->oid == oid) return object;
        return nullptr;
    }

    void delete_object_oid(uint32_t oid) {
        for (size_t i = 0; i < objects.size(); i++) {
            if (objects[i]->oid != oid) continue;
            delete objects[i];
            objects.erase(objects.begin() + i);
            return;
        }
    }

    void clear() {
        for (object_t* object : objects) delete object;
        objects.clear();
        drop_id = 0;
    }
    ~world_object_map_t() { clear(); }

    world_object_map_t() = default;
    world_object_map_t(const world_object_map_t& other) { copy_from(other); }
    world_object_map_t& operator=(const world_object_map_t& other) {
        if (this != &other) { clear(); copy_from(other); }
        return *this;
    }

private:
    void copy_from(const world_object_map_t& other) {
        drop_id = other.drop_id;
        objects.reserve(other.objects.size());
        for (const object_t* object : other.objects)
            objects.push_back(object ? new object_t(*object) : nullptr);
    }
};

// Bounds-checked cursor. Every read is guarded so a malformed or truncated
// world packet aborts the parse instead of walking off the buffer.
class world_reader_t {
public:
    world_reader_t(uint8_t* begin, uint8_t* end) : m_it(begin), m_end(end) {}

    bool can_read(size_t bytes) const { return m_it && m_it + bytes <= m_end; }

    template <typename T>
    T read() {
        T value{};
        if (!can_read(sizeof(T))) { m_overflow = true; return value; }
        memcpy(&value, m_it, sizeof(T));
        m_it += sizeof(T);
        return value;
    }

    bool skip(size_t bytes) {
        if (!can_read(bytes)) { m_overflow = true; return false; }
        m_it += bytes;
        return true;
    }

    // Length-prefixed string: u16 length followed by that many bytes.
    bool skip_string() {
        const uint16_t length = read<uint16_t>();
        return !m_overflow && skip(length);
    }

    bool overflow() const { return m_overflow; }

private:
    uint8_t* m_it;
    uint8_t* m_end;
    bool m_overflow{ false };
};

struct world_t {
    std::string name{};
    bool connected{ false };
    player local{};
    std::vector<player> players{};
    world_tile_map_t tile_map{};
    world_object_map_t object_map{};

    // A world name carrying these colour codes means the local player has access.
    bool check_access_name(const std::string& player_name) const {
        return player_name.find("`^") != std::string::npos
            || player_name.find("`2") != std::string::npos;
    }

    bool serialize(uint8_t* data, uint8_t* end);

private:
    bool parse(world_reader_t& reader);
    bool parse_tile_extra(world_reader_t& reader, tile_t& tile);
};

// The packet sometimes carries a version+flags header ahead of the name and
// sometimes does not, so try each start offset and keep whichever validates.
inline bool world_t::serialize(uint8_t* data, uint8_t* end) {
    if (!data || !end || end <= data)
        return false;

    const size_t offsets[] = { 6, 10, 0 };
    for (size_t offset : offsets) {
        if (data + offset >= end)
            continue;
        world_reader_t reader{ data + offset, end };
        if (parse(reader))
            return true;
    }

    tile_map.clear();
    object_map.clear();
    return false;
}

inline bool world_t::parse(world_reader_t& reader) {
    const uint16_t name_length = reader.read<uint16_t>();
    if (reader.overflow() || name_length == 0 || name_length > 100)
        return false;
    if (!reader.can_read(name_length))
        return false;

    std::string parsed_name(name_length, '\0');
    for (uint16_t i = 0; i < name_length; i++)
        parsed_name[i] = reader.read<char>();

    const uint32_t width = reader.read<uint32_t>();
    const uint32_t height = reader.read<uint32_t>();
    const uint32_t tile_count = reader.read<uint32_t>();
    if (reader.overflow())
        return false;

    // These three agreeing is what tells us the start offset was the right one.
    if (width == 0 || height == 0 || tile_count == 0 || tile_count > 0xFE01)
        return false;
    if (width * height != tile_count)
        return false;

    if (!reader.skip(5))
        return false;

    tile_map.clear();
    tile_map.size = vector2i_t(width, height);
    tile_map.tiles.reserve(tile_count);

    for (uint32_t i = 0; i < tile_count; i++) {
        tile_t* tile = new tile_t{};
        tile->pos = vector2i_t(i % width, i / width);

        tile->foreground = reader.read<uint16_t>();
        tile->background = reader.read<uint16_t>();
        tile->parent_tile_index = reader.read<uint16_t>();

        const uint16_t flags = reader.read<uint16_t>();
        tile->flags_ = tile->flags_1 = tile->flag = flags;
        tile->flag_int = static_cast<int>(flags);
        tile->enabled = (flags & TILEFLAG_PUBLIC) != 0;
        tile->opened = (flags & TILEFLAG_OPEN) != 0;

        if (reader.overflow()) { delete tile; tile_map.clear(); return false; }

        if (flags & TILEFLAG_LOCKED)
            tile->lock_index = reader.read<uint16_t>();

        if (flags & TILEFLAG_EXTRA) {
            tile->tile_extra.type = static_cast<eTileExtraType>(reader.read<uint8_t>());
            if (!parse_tile_extra(reader, *tile)) { delete tile; tile_map.clear(); return false; }
        }

        if (reader.overflow()) { delete tile; tile_map.clear(); return false; }
        tile_map.tiles.push_back(tile);
    }

    // The tiles are what the features actually need, so commit them here. A short or
    // truncated drop table below must not throw the whole world away -- doing that left
    // an empty grid, which silently disabled /scan, /growscan and the pathfinder.
    name = parsed_name;
    connected = true;

    // 12 bytes of trailer, then the dropped-item table.
    if (!reader.skip(12))
        return true;

    const uint32_t dropped_count = reader.read<uint32_t>();
    object_map.clear();
    object_map.drop_id = reader.read<uint32_t>();
    if (reader.overflow())
        return true;

    for (uint32_t i = 0; i < dropped_count; i++) {
        if (!reader.can_read(16))
            break; // trailing drops truncated; keep the tiles already parsed
        object_t* object = new object_t{};
        object->id = reader.read<uint16_t>();
        object->pos.m_x = reader.read<float>();
        object->pos.m_y = reader.read<float>();
        object->amount = reader.read<uint8_t>();
        object->flags = reader.read<uint8_t>();
        object->oid = reader.read<uint32_t>();
        object_map.add_object(object);
    }

    return true;
}

// Each case walks past one extra-data record. Only the vending record has
// fields the features actually read; the rest are measured and skipped.
inline bool world_t::parse_tile_extra(world_reader_t& reader, tile_t& tile) {
    switch (tile.tile_extra.type) {
    case None: break;
    case Door:   if (!reader.skip_string()) return false; return reader.skip(4);
    case Sign:   if (!reader.skip_string()) return false; return reader.skip(1);
    case Lock: {
        if (!reader.skip(1 + 4)) return false;
        const uint32_t access_count = reader.read<uint32_t>();
        if (reader.overflow() || !reader.skip(static_cast<size_t>(access_count) * 4)) return false;
        return reader.skip(1 + 7);
    }
    case Seed: return reader.skip(4 + 1);
    case 6: case 7: case 12:
        if (!reader.skip_string()) return false;
        return reader.skip(1);
    case Dice: return reader.skip(1);
    case Provider: return reader.skip(4);
    case AchievementBlock: return reader.skip(4 + 1);
    case HeartMonitor:
        if (!reader.skip(4)) return false;
        return reader.skip_string();
    case Mannequin:
        if (!reader.skip_string()) return false;
        return reader.skip(1 + 4 + 2 * 9);
    case 15: return reader.skip(4);
    case GameGrave: return reader.skip(1);
    case GameGenerator: break;
    case Xenonite: return reader.skip(1 + 4);
    case PhoneBooth: return reader.skip(2 * 9);
    case 20: break;
    case 21: return reader.skip_string();
    case Spotlight:
        if (!reader.skip_string()) return false;
        return reader.skip(4 + 1);
    case DisplayBlock: return reader.skip(4);
    case Vending:
        tile.tile_extra.Vending.item_id = reader.read<uint32_t>();
        tile.tile_extra.Vending.price = reader.read<int32_t>();
        return !reader.overflow();
    case FishTankPort: {
        if (!reader.skip(1)) return false;
        const uint32_t fish_count = reader.read<uint32_t>();
        if (reader.overflow()) return false;
        return reader.skip(static_cast<size_t>(fish_count) * 8);
    }
    case 26: return reader.skip(5);
    case Forge: return reader.skip(4);
    case GivingTree: return reader.skip(2 + 4);
    case 29: return reader.skip(1 + 4);
    case 30:
        if (!reader.skip(1)) return false;
        if (!reader.skip_string()) return false;
        return reader.skip(16);
    case SewingMachine: {
        const uint32_t bolt_count = reader.read<uint32_t>();
        if (reader.overflow()) return false;
        return reader.skip(static_cast<size_t>(bolt_count) * 4);
    }
    case CountryFlag: return reader.skip_string();
    case LobsterTrap: break;
    case PaintingEasel:
        if (!reader.skip(4)) return false;
        return reader.skip_string();
    case WeatherMachine: return reader.skip(4);
    case DataBedrock: break;
    case Shelf: return reader.skip(16);
    case VipEntrance: {
        if (!reader.skip(1 + 4)) return false;
        const uint32_t count = reader.read<uint32_t>();
        if (reader.overflow()) return false;
        return reader.skip(static_cast<size_t>(count) * 4);
    }
    case ChallengeTimer: break;
    case 46:
        if (!reader.skip_string()) return false;
        return reader.skip(4 + 1);
    case Portrait:
        if (!reader.skip_string()) return false;
        return reader.skip(28);
    case GuildWeatherMachine: return reader.skip(4 + 4 + 1);
    case 50: return reader.skip(4);
    case DnaExtractor: break;
    case Howler: break;
    case 53: return reader.skip(8);
    case StorageBlock: {
        const uint32_t item_count = reader.read<uint32_t>();
        if (reader.overflow()) return false;
        return reader.skip(static_cast<size_t>(item_count) * 8);
    }
    case CookingOven: {
        if (!reader.skip(4)) return false;
        const uint32_t ingredient_count = reader.read<uint32_t>();
        if (reader.overflow()) return false;
        if (!reader.skip(static_cast<size_t>(ingredient_count) * 8)) return false;
        return reader.skip(12);
    }
    case AudioRack:
        if (!reader.skip_string()) return false;
        return reader.skip(4);
    case 57: return reader.skip(4);
    default:
        // Unknown record: its length is unknown, so the rest of the buffer
        // cannot be trusted. Fail rather than emit scrambled tiles.
        return false;
    }
    return !reader.overflow();
}
