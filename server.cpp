#include "server.h"
#include <iostream>
#include "events.h"
#include "gt.hpp"
#include "proton/hash.hpp"
#include "proton/rtparam.hpp"
#include "utils.h"
#include <chrono>
#include "items_dat_decode.h"
#include "print.h"
#include "struct.h"
#include "Pathfinder/PathFinder.h"
#include "proxy.cpp"

using namespace std;
int getState() {
    int val = 0;
    val |= gt::noclip ? 1 : 2;
    val |= gt::doublejump ? 1 : 1;
    return val;
}

bool collecting = true;



void kirmaknk(int x, int y)
{
    gameupdatepacket_t packet{};
    packet.m_type = 3;
    packet.m_int_data = 18;
    packet.m_vec_x = g_server->m_world.local.pos.m_x;
    packet.m_vec_y = g_server->m_world.local.pos.m_y;
    packet.m_state1 = x;
    packet.m_state2 = y;
    packet.m_packet_flags = 2560;
    g_server->send(false, NET_MESSAGE_GAME_PACKET, (uint8_t*)&packet, sizeof(gameupdatepacket_t));
}

void placeknk(int x, int y, int id)
{
    gameupdatepacket_t packet{};
    packet.m_type = 3;
    packet.m_int_data = id;
    packet.m_vec_x = g_server->m_world.local.pos.m_x;
    packet.m_vec_y = g_server->m_world.local.pos.m_y;
    packet.m_state1 = x;
    packet.m_state2 = y;
    g_server->send(false, NET_MESSAGE_GAME_PACKET, (uint8_t*)&packet, sizeof(gameupdatepacket_t));
}

void server::Set_Pos(int x, int y)
{
    vector2_t pos;
    pos.m_x = x * 32;
    pos.m_y = y * 32;

    variantlist_t varlist{ "OnSetPos" };
    varlist[1] = pos;
    m_world.local.pos = pos;
    send(true, varlist, m_world.local.netid, -1);
}


void save_log_to_file(const std::string& log) {

    std::string filePath = "C:\\Users\\Public\\Nexus_Stuff\\Logs\\Nexus_DropCollect.txt";

    std::ofstream file(filePath, std::ios::app);

    if (file.is_open()) {
        file << log << "\n";
        file.close();
    }
    else {
    }
}

void collect_log(gameupdatepacket_t* packet) {
    auto object = g_server->m_world.object_map.get_objectlol(packet->m_int_data);
    if (object == NULL) return;
    if (object->id == 112) return;

    std::string adam = "";
    auto& players = g_server->m_world.players;

    for (size_t i = 0; i < players.size(); i++) {
        auto& player = players[i];
        if (player.netid == packet->m_player_flags) {
            adam = player.name;
        }
    }

    std::string log_message = "`#" + adam + " `9Collected `2" + std::to_string(object->amount) + " " + index[object->id].name;

    if (collect_logs.size() > 150 && !logs_no_limited) {
        collect_logs.clear();
    }

    gt::log_to_console(log_message);
}

std::string DCurrentTime() {
    auto now = std::chrono::system_clock::now();
    std::time_t now_c = std::chrono::system_clock::to_time_t(now);
    std::tm* current_time = std::localtime(&now_c);

    std::stringstream timeStream;
    timeStream << std::put_time(current_time, "%T");
    return timeStream.str();
}

void collectlogs(gameupdatepacket_t* packet) {
    auto object = g_server->m_world.object_map.get_objectlol(packet->m_int_data);
    if (object == NULL) return;
    if (object->id == 112) return;
    std::string adam = "";
    auto& players = g_server->m_world.players;
    for (size_t i = 0; i < players.size(); i++) {
        auto& player = players[i];
        if (player.netid == packet->m_player_flags) {
            adam = player.name;
        }
    }
    std::string currentTime = DCurrentTime();
    std::string log_message = "`9[`2" + currentTime + "`9] `#" + adam + " `9Collected `2" + std::to_string(object->amount) + " " + index[object->id].name + " `9In World: `2" + g_server->m_world.name;
    if (collect_logs.size() > 150 && !logs_no_limited) {
        collect_logs.clear();
    }
    save_log_to_file(log_message);
}

BYTE* packPlayerMovings(PlayerMovings* dataStruct) {
    BYTE* data = new BYTE[56];
    for (int i = 0; i < 56; i++) {
        data[i] = 0;
    }
    memcpy(data, &dataStruct->packetType, 4);
    memcpy(data + 4, &dataStruct->netID, 4);
    memcpy(data + 8, &dataStruct->SecondaryNetID, 4);
    memcpy(data + 12, &dataStruct->characterState, 4);
    memcpy(data + 20, &dataStruct->plantingTree, 4);
    memcpy(data + 24, &dataStruct->x, 4);
    memcpy(data + 28, &dataStruct->y, 4);
    memcpy(data + 32, &dataStruct->XSpeed, 4);
    memcpy(data + 36, &dataStruct->YSpeed, 4);
    memcpy(data + 44, &dataStruct->punchX, 4);
    memcpy(data + 48, &dataStruct->punchY, 4);
    return data;
}

PlayerMovings* unpackRaw(BYTE* data) {
    PlayerMovings* p = new PlayerMovings;
    memcpy(&p->packetType, data, 4);
    memcpy(&p->netID, data + 4, 4);
    memcpy(&p->characterState, data + 12, 4);
    memcpy(&p->plantingTree, data + 20, 4);
    memcpy(&p->x, data + 24, 4);
    memcpy(&p->y, data + 28, 4);
    memcpy(&p->XSpeed, data + 32, 4);
    memcpy(&p->YSpeed, data + 36, 4);
    memcpy(&p->punchX, data + 44, 4);
    memcpy(&p->punchY, data + 48, 4);
    return p;
}

void server::NoclipState(int netid) {
    PlayerMovings data;
    data.packetType = 0x14;
    data.characterState = 0; // animation
    data.x = 1000;
    data.y = 100;
    data.punchX = 500;
    data.punchY = 0;
    data.XSpeed = 2000;
    data.YSpeed = 500;
    data.netID = netid;
    data.plantingTree = getState();
    BYTE* raw = packPlayerMovings(&data);
    int var = 0x808000;
    float water = 125.0f;
    memcpy(raw + 1, &var, 3);
    memcpy(raw + 16, &water, 4);
    g_server->SendPacketRaw(true, 4, raw, 56, 0, ENET_PACKET_FLAG_RELIABLE);
}

ENetPeer* server::getServerPeer() const {
    return m_server_peer;
}

// Doors, portals and vents are identified by the item's action category, read from
// items.dat. The old code asked an ItemData map that nothing ever populated, so every
// door and vent counted as solid and the pathfinder could not route through them.
static bool item_is_door(const item& info) {
    // 2 = Door, 13 = Main Door, 26 = Portal
    return info.itemCategory == 2 || info.itemCategory == 13 || info.itemCategory == 26;
}

static bool item_is_entrance(const item& info) {
    // 9 covers air vents and jail doors: passable only while switched on
    return info.itemCategory == 9;
}

bool pathfinder_tile_blocked(world_t& world, tile_t* tile) {
    if (!tile)
        return true;                                   // outside the world
    if (tile->foreground == 0)
        return false;                                  // open air
    if (!items || tile->foreground >= itemCount)
        return true;                                   // unknown item, assume solid

    const item& info = items[tile->foreground];

    if (info.collisionType == 0)
        return false;                                  // decoration
    if (item_is_door(info) && tile->opened)
        return false;
    if (item_is_entrance(info) && tile->enabled)
        return false;

    switch (info.collisionType) {
    case 1:  return true;                              // solid block
    case 2:  return false;                             // platform, can be jumped through
    case 3:                                            // access controlled, e.g. jail door
        if (world.check_access_name(world.local.name))
            return false;
        return (tile->flag & 0x90) == 0;
    case 4:  return tile->flag_int != 64;              // toggled block, open at 64
    default: return true;
    }
}

void server::sendState(int netid2, bool pathfinds = false) {
    int32_t netid = netid2;
    int type, charstate, plantingtree, punchx, punchy;
    float x, y, xspeed, yspeed, mspeed;
    type = PACKET_SET_CHARACTER_STATE;
    charstate = 0;
    int state = 0;
    state |= (gt::ghost) << 0; //ghost
    state |= doublejump << 1; //double jump
    state |= false << 1; //spirit form
    state |= false << 1; //hoveration
    state |= false << 1; //aurora
    state |= false << 2; //one ringe
    state |= false << 4; //mark of growganoth
    state |= false << 7; //halo
    state |= false << 13; //duct tape
    state |= false << 15; //lucky
    state |= false << 19; //geiger effect
    state |= false << 20; //spotlight
    state |= ssup << 24; //super suporter
    plantingtree = state;
    punchx = 0;
    punchy = 0;
    x = 1000.0f;
    y = 400.0f;
    xspeed = speed_x;

    yspeed = speed_y; //gravity

    mspeed = modfly;

    BYTE* data = new BYTE[56];
    memset(data, 0, 56);
    memcpy(data + 0, &type, 4);
    memcpy(data + 4, &netid, 4);
    memcpy(data + 12, &charstate, 4);
    memcpy(data + 20, &plantingtree, 4);
    memcpy(data + 24, &x, 4);
    memcpy(data + 28, &y, 4);
    memcpy(data + 32, &xspeed, 4);
    memcpy(data + 36, &yspeed, 4);
    memcpy(data + 36, &mspeed, 4);
    memcpy(data + 44, &punchx, 4);
    memcpy(data + 48, &punchy, 4);


    memcpy(data + 1, &punch_effect, 3);

    if (serverlongpunch == true)
    {
        uint8_t build_range = (pathfinds ? -1 : 128);
        uint8_t punch_range = (pathfinds ? -1 : 128);
        memcpy(data + 2, &build_range, 1);
        memcpy(data + 3, &punch_range, 1);
    }
    else
    {
        PACKET_SET_CHARACTER_STATE;
        memcpy(data + 2, &build_range, 1);
        memcpy(data + 3, &punch_range, 1);
    }
    float waterspeed = 200.0f;
    memcpy(data + 16, &waterspeed, 4);
    g_server->SendPacketRaw(true, 4, data, 56, 0, ENET_PACKET_FLAG_RELIABLE);
}

void server::speedy(int netid, int XSpeed) {
    PlayerMovings data;
    data.packetType = 0x14;
    data.characterState = 0; // animation
    data.x = 1000;
    data.y = 100;
    data.punchX = 500;
    data.punchY = 0;
    data.XSpeed = 300;
    data.YSpeed = XSpeed;
    data.netID = netid;
    data.plantingTree = getState();
    BYTE* raw = packPlayerMovings(&data);
    int var = 0x808000;
    float water = 125.0f;
    memcpy(raw + 1, &var, 3);
    memcpy(raw + 16, &water, 4);
    g_server->SendPacketRaw(true, 4, raw, 56, 0, ENET_PACKET_FLAG_RELIABLE);
}
void server::stateReset(int netid) {
    PlayerMovings data;
    data.packetType = 0x14;
    data.characterState = 0; // animation
    data.x = 1000;
    data.y = 100;
    data.punchX = 500;
    data.punchY = 0;
    data.XSpeed = 300;
    data.YSpeed = 600;
    data.netID = netid;
    data.plantingTree = getState();
    BYTE* raw = packPlayerMovings(&data);
    int var = 0x808000;
    float water = 125.0f;
    memcpy(raw + 1, &var, 3);
    memcpy(raw + 16, &water, 4);
    g_server->SendPacketRaw(true, 4, raw, 56, 0, ENET_PACKET_FLAG_RELIABLE);
}


void server::addBlock(bool client, int tile, int x, int y) {
    PlayerMovings data;
    data.packetType = 0x3;
    data.characterState = 0x0; // animation
    data.x = x;
    data.y = y;
    data.punchX = x;
    data.punchY = y;
    data.XSpeed = 0;
    data.YSpeed = 0;
    data.netID = g_server->m_world.local.netid;
    data.plantingTree = tile;
    g_server->SendPacketRaw(true, 4, packPlayerMovings(&data), 56, 0, ENET_PACKET_FLAG_RELIABLE); // If client = false then auto ban
}

void server::breakBlock(int x, int y) {
    PlayerMovings data;
    data.packetType = 0x3;
    data.characterState = 0x0; // animation
    data.x = g_server->m_world.local.pos.m_x;
    data.y = g_server->m_world.local.pos.m_y;
    data.punchX = x;
    data.punchY = y;
    data.XSpeed = 0;
    data.YSpeed = 0;
    data.netID = g_server->m_world.local.netid;
    data.plantingTree = 18;
    g_server->SendPacketRaw(false, 4, packPlayerMovings(&data), 56, 0, ENET_PACKET_FLAG_RELIABLE);
}

void server::placeBlock(bool client, int tile, int x, int y) {
    PlayerMovings data;
    data.packetType = PACKET_TILE_CHANGE_REQUEST;
    data.characterState = 0x0; // animation
    data.x = g_server->m_world.local.pos.m_x;
    data.y = g_server->m_world.local.pos.m_y;
    data.punchX = x;
    data.punchY = y;
    data.XSpeed = 0;
    data.XSpeed = 0;
    data.YSpeed = 0; gameupdatepacket_t* packet;
    data.netID = g_server->m_world.local.netid;
    data.plantingTree = tile;
    g_server->SendPacketRaw(client, 4, packPlayerMovings(&data), 56, 0, ENET_PACKET_FLAG_RELIABLE);
}

void server::kirisblock(int tile, int x, int y) {
    PlayerMovings data;
    data.packetType = 0x3;
    data.characterState = 0x0; // animation
    data.x = g_server->m_world.local.pos.m_x;
    data.y = g_server->m_world.local.pos.m_y;
    data.punchX = x;
    data.punchY = y;
    data.XSpeed = 0;
    data.XSpeed = 0;
    data.YSpeed = 0;
    data.netID = g_server->m_world.local.netid;
    data.plantingTree = tile;
    g_server->SendPacketRaw(false, 4, packPlayerMovings(&data), 56, 0, ENET_PACKET_FLAG_RELIABLE);
}

void server::wearItem(int itemid) {
    variantlist_t varlist{ "OnEquipNewItem" };
    varlist[1] = itemid;
    g_server->send(false, varlist, g_server->m_world.local.netid, -1);

    PlayerMovings data;
    data.packetType = 10;
    data.characterState = 0x0; // animation local player sorunlu iþte kuloda amk
    data.x = g_server->m_world.local.pos.m_x;
    data.y = g_server->m_world.local.pos.m_y;
    data.punchX = NULL;
    data.punchY = NULL;
    data.XSpeed = NULL;
    data.YSpeed = NULL;
    data.netID = g_server->m_world.local.netid;
    data.plantingTree = itemid;
    g_server->SendPacketRaw(false, 4, packPlayerMovings(&data), 56, 0, ENET_PACKET_FLAG_RELIABLE);
}


BYTE* GetStructPointerFromTankPacket(ENetPacket* packet)
{
    const unsigned int packetLenght = packet->dataLength;
    BYTE* result = nullptr;
    if (packetLenght >= 0x3C)
    {
        BYTE* packetData = packet->data;
        result = packetData + 4;
        if (*static_cast<BYTE*>(packetData + 16) & 8)
        {
            if (packetLenght < *reinterpret_cast<int*>(packetData + 56) + 60)
            {
                result = nullptr;
            }
        }
        else
        {
            int zero = 0;
            memcpy(packetData + 56, &zero, 4);
        }
    }
    return result;
}

void server::kapigiris(bool client, int x, int y) {
    PlayerMovings data;
    data.packetType = 0x7;
    data.characterState = 0x0; // animation
    data.x = g_server->m_world.local.pos.m_x;
    data.y = g_server->m_world.local.pos.m_y;
    data.punchX = x;
    data.punchY = y;
    data.XSpeed = 0;
    data.YSpeed = 0;
    data.netID = g_server->m_world.local.netid;
    data.plantingTree = 18;
    g_server->SendPacketRaw(client, 4, packPlayerMovings(&data), 56, 0, ENET_PACKET_FLAG_RELIABLE);
}

void server::enterDoor(bool client, int x, int y) {

    PlayerMovings data2{};
    data2.packetType = 8;
    data2.plantingTree = 6;
    data2.characterState = 0; // animation
    data2.x = g_server->m_world.local.pos.m_x;
    data2.y = g_server->m_world.local.pos.m_y;
    data2.punchX = x;
    data2.punchY = y;
    data2.XSpeed = 0;
    data2.YSpeed = 0;
    data2.netID = g_server->m_world.local.netid;
    g_server->SendPacketRaw(client, 4, packPlayerMovings(&data2), 56, 0, ENET_PACKET_FLAG_RELIABLE);
}

void agabe(ENetPacket* packet) {
    auto lalal = GetStructPointerFromTankPacket(packet);
    PlayerMovings* data = unpackRaw(GetStructPointerFromTankPacket(packet));
    cout << "agabe: " << int(data->plantingTree) << std::endl;
    cout << "agabe 2: " << int(data->XSpeed) << std::endl;
    cout << "agabe 3: " << int(data->YSpeed) << std::endl;
    cout << "agabe 4: " << int(data->punchX) << std::endl;
    cout << "agabe 5: " << int(data->punchY) << std::endl;
    cout << "agabe 6: " << int(data->characterState) << std::endl;
    cout << "agabe 7: " << int(data->netID) << std::endl;
    cout << "agabe 8: " << int(data->SecondaryNetID) << std::endl;
    cout << "agabe 9: " << int(data->x) << std::endl;
    cout << "agabe 10: " << int(data->y) << std::endl;
    cout << "agabe 11: " << int(data->packetType) << std::endl;

}

void server::handle_outgoing() {
    ENetEvent evt;
    while (enet_host_service(m_proxy_server, &evt, 0) > 0) {
        // m_gt_peer used to be assigned here for every event, which made a disconnect
        // from an old connection indistinguishable from the current one. Track it on
        // connect and receive instead, so the disconnect handler can tell them apart.
        switch (evt.type) {
        case ENET_EVENT_TYPE_CONNECT: {
            m_gt_peer = evt.peer;
            PRINTS("Growtopia client connected to the proxy (%x:%u)\n",
                evt.peer->address.host, evt.peer->address.port);
            // Reaching here means any pending redirect finished: the client came back.
            gt::connecting = false;
            if (!this->connect()) {
                PRINTS("Upstream connect failed; dropping the client.\n");
                return;
            }
        } break;
        case ENET_EVENT_TYPE_RECEIVE: {
            m_gt_peer = evt.peer;
            int packet_type = get_packet_type(evt.packet);

            switch (packet_type) {
            case NET_MESSAGE_GENERIC_TEXT:
                if (events::out::generictext(utils::get_text(evt.packet))) {
                    PRINTS("^ handled locally, NOT forwarded to the server\n");
                    enet_packet_destroy(evt.packet);
                    return;
                }
                break;
            case NET_MESSAGE_GAME_MESSAGE:
                if (events::out::gamemessage(utils::get_text(evt.packet))) {
                    enet_packet_destroy(evt.packet);
                    return;
                }
                break;

            case NET_MESSAGE_GAME_PACKET: {
                auto packet = utils::get_struct(evt.packet);
                auto packetGUP = utils::get_structGUP(evt.packet);
                //agabe(evt.packet);
                //agabe(evt.packet);
                if (!packet)
                    break;

                if (!packetGUP)
                    break;

                switch (packet->m_type) {

                case PACKET_STATE:
                    if (events::out::state(packet, packetGUP)) {
                        enet_packet_destroy(evt.packet);
                        return;
                    }
                    break;
                case 0x3: {

                    if (GetAsyncKeyState(VK_SHIFT) && pathfinder) {
                        enet_packet_destroy(evt.packet);
                        return;
                    }
                    if (gemcount)
                    {
                        enet_packet_destroy(evt.packet);
                        return;
                    }
                } break;
                case PACKET_ITEM_ACTIVATE_REQUEST: {
                    bool varmi = true;
                    if (packet->m_int_data != 1796 && packet->m_int_data != 242 && packet->m_int_data != 7188) varmi = false;
                    if (iswear && varmi == false) {

                        auto& world = g_server->m_world;
                        types types;
                        auto type = items[packet->m_int_data];
                        if (type.name.find("Ancestral") != -1 || type.name.find("Samille") != -1 || type.name.find("Chakram") != -1) {
                            ances = type.itemID == ances ? 0000.0 : type.itemID;
                        }
                        else
                            switch (type.clothingType) {
                            case 0:
                                hair = type.itemID == hair ? 0000.0 : type.itemID;
                                break;
                            case 1:
                                shirt = type.itemID == shirt ? 0000.0 : type.itemID;
                                break;
                            case 2:
                                pants = type.itemID == pants ? 0000.0 : type.itemID;
                                break;
                            case 3:
                                shoe = type.itemID == shoe ? 0000.0 : type.itemID;
                                break;
                            case 4:
                                face = type.itemID == face ? 0000.0 : type.itemID;
                                break;
                            case 5:
                                hand = type.itemID == hand ? 0000.0 : type.itemID;
                                break;
                            case 6:
                                back = type.itemID == back ? 0000.0 : type.itemID;

                                break;
                            case 7:
                                hair = type.itemID == hair ? 0000.0 : type.itemID;
                                break;
                            case 8:
                                neck = type.itemID == neck ? 0000.0 : type.itemID;
                                break;
                            default:
                                hair = type.itemID == hair ? 0000.0 : type.itemID;
                                break;
                            }


                        if (events::out::get_punch_id(packet->m_int_data) != 0) {
                            punch_effect = events::out::get_punch_id(packet->m_int_data);
                        }
                        if (back != 0000.0) {
                            doublejump = true;
                        }
                        sendState(world.local.netid);
                        variantlist_t liste{ "OnSetClothing" };
                        liste[1] = vector3_t{ hair,  shirt,  pants };
                        liste[2] = vector3_t{ shoe,  face,  hand };
                        liste[3] = vector3_t{ back,  mask,  neck };
                        liste[4] = skincolor;
                        liste[5] = vector3_t{ ances , 1.f, 0.f };
                        g_server->send(true, liste, world.local.netid, -1);

                        gameupdatepacket_t t{ 0 };
                        return;
                    }
                    // This used to drop the packet outright when the proxy's own inventory
                    // tracking showed a count of zero. That tracking desyncs easily, and a
                    // dropped activate looks like the game ignoring your wrench or item.
                    // The server validates item usage itself, so let it through.
                } break;   // without this, an activate request fell through into
                           // PACKET_CALL_FUNCTION and was parsed as a variant list
                case PACKET_CALL_FUNCTION:
                    if (events::out::variantlist(packet)) {
                        enet_packet_destroy(evt.packet);
                        return;
                    }
                    break;

                case PACKET_PING_REPLY:
                    if (events::out::pingreply(packet)) {
                        enet_packet_destroy(evt.packet);
                        std::this_thread::sleep_for(std::chrono::milliseconds(100));
                        return;
                    }
                    break;
                case PACKET_GOT_PUNCHED:
                {
                    if (antizombie)
                    {
                        return;
                    }
                }break;
                case PACKET_DISCONNECT:
                    // Must reach the server. Swallowing it left the session open on their
                    // side, so the next login collided with a connection they still
                    // believed was live -- the client then hung on "Connecting...".
                    PRINTS("Client requested disconnect; forwarding to the server.\n");
                    break;
                case PACKET_APP_INTEGRITY_FAIL:
                    // Deliberately not forwarded: this is the client reporting that it
                    // detected tampering, and relaying it invites a ban.
                    if (gt::in_game)
                        return;
                    break;

                default: PRINTS("Packet Type: %d\n", packet->m_type);
                }
            } break;
            case NET_MESSAGE_TRACK: //track one should never be used, but its not bad to have it in case.
            case NET_MESSAGE_CLIENT_LOG_RESPONSE: return;

            default: PRINTS("Got unknown packet of type %d.\n", packet_type); break;
            }

            if (!m_server_peer || !m_real_server) {
                PRINTS("DROPPED a client packet: no upstream connection yet.\n");
                return;
            }
            if (enet_peer_send(m_server_peer, 0, evt.packet) != 0)
                PRINTS("enet_peer_send to the server FAILED.\n");
            enet_host_flush(m_real_server);
        } break;
        case ENET_EVENT_TYPE_DISCONNECT: {
            // On a world hop the client opens its new connection before the old one
            // finishes tearing down, so this event can arrive for a peer we already
            // replaced. Acting on it killed the upstream link we had just built, and
            // every packet after that was dropped with nowhere to forward it.
            if (m_gt_peer && evt.peer != m_gt_peer) {
                PRINTS("Ignoring disconnect from a superseded client connection.\n");
                break;
            }
            if (gt::connecting) {
                // Sub-server hop: the client drops us and comes straight back, so keep
                // the redirect target that redirect_server just stored.
                this->disconnectsr(false);
                gt::connecting = false;
                return;
            }
            // The client really went away (game closed or logged out). This used to bail
            // out early whenever we were in game, which left the upstream session and the
            // stale peer behind, so the next connect never completed.
            PRINTS("Growtopia client disconnected from the proxy (enet data=%u).\n", evt.data);
            gt::in_game = false;
            m_gt_peer = nullptr;
            this->disconnectsr(true);
        } break;
        default: PRINTS("UNHANDLED\n"); break;
        }
    }



    if (fastblink)
    {
        if (g_server->m_world.connected)
        {
            uint64_t _time = GetTickCount64();
            if (_time >= fastblink_sleep_end)
            {
                fastblink_sleep_end = _time + 200;
                std::vector<uint32_t> Skinlist;
                Skinlist = { 1348237567, 1685231359, 2022356223, 2190853119, 2527912447, 2864971775, 3033464831, 3370516479 };
                string color = "action|setSkin\ncolor|" + std::to_string(Skinlist.at(utils::random(0, 7)));
                g_server->send(false, color);
            }
        }
    }

    if (blink)
    {
        if (g_server->m_world.connected)
        {
            uint64_t _time = GetTickCount64();
            if (_time >= blink_sleep_end)
            {
                blink_sleep_end = _time + 550;
                std::vector<uint32_t> Skinlist;
                Skinlist = { 1348237567, 1685231359, 2022356223, 2190853119, 2527912447, 2864971775, 3033464831, 3370516479 };
                string color = "action|setSkin\ncolor|" + std::to_string(Skinlist.at(utils::random(0, 7)));
                g_server->send(false, color);
            }
        }
    }

    if (son)
    {
        if (g_server->m_world.connected)
        {
            uint64_t _time = GetTickCount64();
            if (_time >= spam_sleep_end)
            {
                spam_sleep_end = _time + spamdelay;
                const string colored_text_array[12] = { "`2", "`3", "`4", "`#", "`9", "`8", "`c", "`6", "`^" , "`b" , "`p" , "`5" };
                int baba = (rand() % 11) + 1;
                string send_ = colored_text_array[baba];
                if (enabled_color == true)
                    g_server->send(false, "action|input\n|text|" + send_ + aspam);
                else
                    g_server->send(false, "action|input\n|text|" + aspam);
            }
        }
    }

    if (gem_message) {
        uint64_t _time = GetTickCount64();

        if (_time >= gem_sleep_end) {

            if (last_gems_collect != 0) {
                gt::log_to_console("`9Collected `2+" + std::to_string(last_gems_collect) + "`` Gems");
                gt::talkbubble("`9Collected `2+" + std::to_string(last_gems_collect) + "`` Gems");
                last_gems_collect = 0;
            }
            gem_sleep_end = _time + 1500;
        }
    }

    if (crystal)
    {
        uint64_t _time = GetTickCount64();
        if (_time >= crystal_sleep)
        {
            crystal_sleep = _time + 25;

            auto pos = g_server->m_world.local.pos;
            std::vector<uint32_t> csid;
            csid = { 2242, 2244, 2246, 2248, 2250 };

            GameUpdatePacket packet{ 0 };
            packet.type = PACKET_STATE;
            packet.int_data = csid.at(utils::random(0, 4));
            packet.int_x = utils::random(0, 100);
            packet.int_y = utils::random(0, 60);
            if (gt::right_left == 48)
            {
                packet.flags = 48 | (1 << 10) | (1 << 11);
            }
            else
            {
                packet.flags = 32 | (1 << 10) | (1 << 11);
            }
            packet.vec_x = pos.m_x;
            packet.vec_y = pos.m_y;
            g_server->send(false, 4, (uint8_t*)&packet, sizeof(gameupdatepacket_t));
        }
    }

    if (farmauto)
    {
        uint64_t _time = GetTickCount64();
        if (_time >= farm_sleep)
        {
            std::string filePath = "C:\\Users\\Public\\Nexus_Stuff\\selectfarm.txt";
            std::vector<int> x_coords;
            std::vector<int> y_coords;

            std::ifstream file(filePath);
            if (!file.is_open()) {
                return;
            }

            std::string line;
            while (std::getline(file, line)) {
                if (line.empty()) continue;

                int x, y;
                if (sscanf(line.c_str(), "x:%d | y:%d", &x, &y) == 2) {
                    x_coords.push_back(x);
                    y_coords.push_back(y);
                }
            }

            file.close();
            for (size_t i = 0; i < x_coords.size(); ++i) {
                int x = x_coords[i];
                int y = y_coords[i];

                placeknk(x, y, farmid);
                farm_sleep = _time + 3000;

                gameupdatepacket_t pl;
                pl.m_type = GAME_SELECT_TILE_INDEX;
                pl.m_player_flags = HashCoord(x, y);
                g_server->send(true, NET_MESSAGE_GAME_PACKET, (uint8_t*)&pl, sizeof(GameUpdatePacket));

                for (int j = 0; j < farmhit; ++j) {
                    kirmaknk(x, y);
                    farm_sleep = _time + 3000;

                    gameupdatepacket_t pl_hit;
                    pl_hit.m_type = GAME_SELECT_TILE_INDEX;
                    pl_hit.m_player_flags = HashCoord(x, y);
                    g_server->send(true, NET_MESSAGE_GAME_PACKET, (uint8_t*)&pl_hit, sizeof(GameUpdatePacket));
                }
            }
            farm_sleep = _time + farmdelay;
        }
    }

}
std::vector<std::string> ItemDataContainer::arr, ItemDataContainer::itd, ItemDataContainer::itr, ItemDataContainer::itt, ItemDataContainer::itx, ItemDataContainer::ity, ItemDataContainer::ita, ItemDataContainer::ith;

bool get_data(BYTE* data) {
    if (!iswear) {
        memcpy(&punch_effect, data + 1, 3);
    }
    memcpy(&build_range, data + 2, 1);
    memcpy(&punch_range, data + 3, 1);

    if (back != 0.0) {
        doublejump = true;
    }

    return false;
}

void server::handle_incoming() {
    //TO-DO Fix Here
    ENetEvent event;

    while (enet_host_service(m_real_server, &event, 0) > 0) {
        switch (event.type) {
        case ENET_EVENT_TYPE_CONNECT:
            PRINTC("Connected to the real Growtopia server %s:%d\n", m_server.c_str(), m_port);
            break;
        case ENET_EVENT_TYPE_DISCONNECT:
            PRINTC("Disconnected from the real server %s:%d\n", m_server.c_str(), m_port);
            this->disconnectsr(true);
            return;
        case ENET_EVENT_TYPE_RECEIVE: {
            if (event.packet->data) {
                int packet_type = get_packet_type(event.packet);
                switch (packet_type) {
                case NET_MESSAGE_GENERIC_TEXT:
                    if (events::in::generictext(utils::get_text(event.packet))) {
                        enet_packet_destroy(event.packet);
                        return;
                    }
                    break;
                case NET_MESSAGE_GAME_MESSAGE:
                    if (events::in::gamemessage(utils::get_text(event.packet))) {
                        enet_packet_destroy(event.packet);
                        return;
                    }
                    break;

                case NET_MESSAGE_GAME_PACKET: {
                    auto packet = utils::get_struct(event.packet);
                    if (!packet)
                        break;

                    switch (packet->m_type) {
                    case PACKET_PING_REQUEST: {
                        gameupdatepacket_t pkt = {};
                        pkt.m_type = PACKET_PING_REPLY;
                        pkt.m_int_data = packet->m_int_data;
                        pkt.m_vec_x = 64.f;
                        pkt.m_vec_y = 64.f;
                        pkt.m_vec2_x = 1000.f;
                        pkt.m_vec2_y = 250.f;
                        g_server->send(true, NET_MESSAGE_GAME_PACKET, (uint8_t*)&pkt, sizeof(gameupdatepacket_t));
                    } break;
                    case PACKET_SET_CHARACTER_STATE: {
                        if (packet->m_player_flags == g_server->m_world.local.netid) {
                            build_range = packet->m_jump_amount;
                            punch_range = packet->m_count;

                        }
                    }break;
                    case PACKET_SEND_INVENTORY_STATE: {
                        local_player.inventory.slotCount = 0;
                        local_player.inventory.itemCount = 0;
                        local_player.inventory.items.clear();

                        std::vector<Item> invBuf;
                        LPBYTE extended_ptr = utils::get_extended(packet);

                        memcpy(&local_player.inventory.slotCount, extended_ptr + 5, 4);
                        memcpy(&local_player.inventory.itemCount, extended_ptr + 9, 2);
                        invBuf.resize(local_player.inventory.itemCount);

                        memcpy(invBuf.data(), extended_ptr + 11, invBuf.capacity() * sizeof(Item));

                        for (Item& item : invBuf) {
                            local_player.inventory.items.operator[](item.id) = item;
                        }
                    }break;
                    case 8: {
                        if (diceroll)
                        {
                            if (!packet->m_int_data) {
                                std::string dice_roll = std::to_string(packet->m_count + 1);
                                gt::log_to_console("`9The Dice Will Roll a `2" + dice_roll);
                                gt::talkbubble("`9The Dice Will Roll a `2" + dice_roll);
                            }
                        }
                    }break;
                    case PACKET_MODIFY_ITEM_INVENTORY: {
                        auto packetGUP = utils::get_structGUP(event.packet);
                        auto s_ptr = &local_player.inventory.items;
                        if (packetGUP->gained_item_count != 0) s_ptr->operator[](packet->m_int_data).count += (packetGUP->gained_item_count);
                        else if (s_ptr->operator[](packet->m_int_data).count > packet->m_jump_amount) s_ptr->operator[](packet->m_int_data).count -= (packet->m_jump_amount);
                        else  s_ptr->erase(packet->m_int_data);
                    }break;

                    case PACKET_TILE_CHANGE_REQUEST: {
                        auto& world = g_server->m_world;
                        // Coordinates and item id come straight off the wire. This block used
                        // to index the tile vector via HashCoord (which assumes a 100-wide
                        // world), index the item table unchecked, and write gtmap[x][y] when
                        // every other site uses gtmap[y][x] on an int[60][100]. Any of those
                        // walks off its buffer on an unexpected packet.
                        const int tile_x = static_cast<int>(packet->m_state1);
                        const int tile_y = static_cast<int>(packet->m_state2);
                        tile_t* changed = world.tile_map.get_tile(vector2i_t(tile_x, tile_y));
                        if (!changed)
                            break;

                        const bool gtmap_ok = tile_y >= 0 && tile_y < 60 && tile_x >= 0 && tile_x < 100;

                        if (packet->m_int_data == 18) {
                            if (changed->foreground != 0) {
                                changed->foreground = 0;
                                if (gtmap_ok) g_server->gtmap[tile_y][tile_x] = 0;
                            }
                            else
                                changed->background = 0;
                        }
                        else {
                            if (packet->m_int_data >= index.size())
                                break;
                            auto item = index[packet->m_int_data];
                            if (item.itemCategory == 18)
                                changed->background = packet->m_int_data;
                            else {
                                changed->foreground = packet->m_int_data;
                                if (gtmap_ok) g_server->gtmap[tile_y][tile_x] = 1;
                            }

                            if (packet->m_player_flags == m_world.local.netid) {
                                auto s_items_ptr = &local_player.inventory.items;
                                auto it = s_items_ptr->find(packet->m_int_data);
                                if (it != s_items_ptr->end()) {
                                    if (it->second.count > 1)
                                        it->second.count -= 1;
                                    else
                                        s_items_ptr->erase(packet->m_int_data);
                                }

                            }
                        }
                        //  printf("tile data is C: %d %d --- F: %d B: %d\n", tile->second.pos.m_x, tile->second.pos.m_y, tile->second->foreground, tile->second->background);
                    }break;
                    case PACKET_ITEM_CHANGE_OBJECT: {
                        if (packet->m_vec_x == 0 && packet->m_vec_y == 0) {
                            if (track)
                            {
                                collect_log(packet);
                            }
                            collectlogs(packet);
                            if (packet->m_player_flags == m_world.local.netid) {
                                auto object = m_world.object_map.get_objectlol(packet->m_int_data);
                                // Drops the proxy never saw (dropped before we joined, or
                                // ones it stopped tracking) return null here. The other two
                                // callers already guard; this one dereferenced regardless,
                                // which crashed the proxy on /daw.
                                if (!object) {
                                    m_world.object_map.delete_object_oid(packet->m_int_data);
                                    break;
                                }
                                if (object->id != 112) {
                                    auto s_items_ptr = &local_player.inventory.items;
                                    //int buffer = local_player.inventory.getItemCount(object->oid);
                                    if (!local_player.inventory.doesItemExistUnsafe(object->id)) { //haven't tested yet.         
                                        Item item{};
                                        item.id = object->id;
                                        item.count = object->amount;
                                        item.type = 0;
                                        s_items_ptr->operator[](item.id) = item;
                                    }

                                    else s_items_ptr->operator[](object->id).count += object->amount;
                                    if (game_started) {

                                        if (object->id == 242) {
                                            total_bet += object->amount;
                                        }
                                        if (object->id == 1796) {
                                            total_bet += (object->amount * 100);
                                        }
                                    }

                                    if (s_items_ptr->operator[](242).count >= 100) {
                                        gameupdatepacket_t compress{ 0 };
                                        compress.m_type = PACKET_ITEM_ACTIVATE_REQUEST;
                                        compress.m_int_data = 242;
                                        g_server->send(false, NET_MESSAGE_GAME_PACKET, (uint8_t*)&compress, sizeof(gameupdatepacket_t));

                                    }
                                }
                                else {
                                    local_player.gems_balance += object->amount;
                                    if (gem_message)
                                    {
                                        last_gems_collect += object->amount;
                                    }
                                }

                            }
                            m_world.object_map.delete_object_oid(packet->m_int_data);
                        }
                        else {
                            if (packet->m_player_flags == -1) {
                                object_t item;
                                item.id = packet->m_int_data;
                                item.pos = vector2_t(packet->m_vec_x, packet->m_vec_y);
                                item.amount = uint32_t(packet->m_struct_flags);
                                item.flags = 0;
                                item.oid = ++(m_world.object_map.drop_id);
                                m_world.object_map.add_object(new object_t(item));
                                if (track)
                                {
                                    std::string adam = "";
                                    if (drop_logs.size() > 150 && !logs_no_limited) drop_logs.clear();
                                    auto& players = g_server->m_world.players;
                                    for (size_t i = 0; i < players.size(); i++) {
                                        auto& player = players[i];
                                        if (player.netid == packet->m_item) {
                                            adam = player.name;
                                        }
                                    }
                                    gt::log_to_console("`#" + adam + " `9Dropped `2" + std::to_string(item.amount) + " " + index[item.id].name);
                                }
                                std::string adam = "";
                                if (drop_logs.size() > 150 && !logs_no_limited) drop_logs.clear();
                                auto& players = g_server->m_world.players;
                                for (size_t i = 0; i < players.size(); i++) {
                                    auto& player = players[i];
                                    if (player.netid == packet->m_item) {
                                        adam = player.name;
                                    }
                                }
                                std::string IdkcurrentTime = DCurrentTime();
                                std::string log_message = "`9[`2" + IdkcurrentTime + "`9] `#" + adam + " `9Dropped `2" + std::to_string(item.amount) + " " + index[item.id].name + " `9In World: `2" + g_server->m_world.name;
                                save_log_to_file(log_message);
                            }
                            else if (packet->m_player_flags == -3) {
                                for (auto& obj : g_server->m_world.object_map.objects) {
                                    if (obj->id == packet->m_int_data && obj->pos.m_x == packet->m_vec_x && obj->pos.m_y == packet->m_vec_y) {
                                        obj->amount = static_cast<uint8_t>(packet->m_struct_flags);
                                        break;
                                    }
                                }
                            }
                            else if (packet->m_player_flags > 0) {
                                for (int i = 0; i < g_server->m_world.object_map.objects.size(); i++) {
                                    if (g_server->m_world.object_map.objects[i]->oid == packet->m_int_data) {
                                        if (packet->m_player_flags == g_server->m_world.local.netid) {
                                            if (g_server->m_world.object_map.objects[i]->id == 112) {
                                                gems += g_server->m_world.object_map.objects[i]->amount;
                                            }
                                        }
                                        m_world.object_map.delete_object_oid(packet->m_int_data);
                                        break;
                                    }
                                }
                            }
                        }

                    }break;


                    case PACKET_CALL_FUNCTION:
                        //
                        if (events::in::variantlist(packet)) {
                            enet_packet_destroy(event.packet);
                            return;
                        }
                        break;
                    case PACKET_SEND_MAP_DATA:
                        if (events::in::sendmapdata(event, packet)) {
                            enet_packet_destroy(event.packet);
                            return;
                        }
                        break;
                    case PACKET_STATE:
                        if (events::in::state(packet)) {
                            enet_packet_destroy(event.packet);
                            return;
                        }
                        break;

                        //no need to print this for handled packet types such as func call, because we know its 1
                    default: PRINTC("Packet Type: %d\n", packet->m_type); break;
                    }
                } break;

                    //ignore tracking packet, and request of client crash log
                case NET_MESSAGE_TRACK:
                    if (events::in::tracking(utils::get_text(event.packet))) {
                        enet_packet_destroy(event.packet);
                        return;
                    }
                    break;
                case NET_MESSAGE_CLIENT_LOG_REQUEST: return;

                default: PRINTS("Got unknown packet of type %d.\n", packet_type); break;
                }
            }

            if (!m_gt_peer || !m_proxy_server)
                return;
            enet_peer_send(m_gt_peer, 0, event.packet);
            enet_host_flush(m_proxy_server);

        } break;

        default: PRINTC("UNKNOWN event: %d\n", event.type); break;
        }
    }
}

void server::lockThread()
{
    if (threadID != std::hash<std::thread::id>{}(std::this_thread::get_id())) {
        if (mutexStatus.load()) {
            mutexStatus.store(false);
            this->cv.notify_all();
        }
    }
}
void server::unlockThread()
{
    if (threadID != std::hash<std::thread::id>{}(std::this_thread::get_id())) {
        if (!mutexStatus.load()) {
            mutexStatus.store(true);
            this->cv.notify_all();
        }
    }
}

void server::queue_to_server(const uint8_t* data, int length) {
    if (!data || length <= 0)
        return;
    std::lock_guard<std::mutex> lock(m_outbox_mutex);
    m_outbox.emplace_back(data, data + length);
}

void server::queue_command(const std::string& text) {
    if (text.empty())
        return;
    std::lock_guard<std::mutex> lock(m_outbox_mutex);
    m_command_queue.push_back(text);
}

// Runs on the polling thread, which is the only thread allowed to touch the ENet hosts.
void server::flush_queued() {
    std::vector<std::vector<uint8_t>> pending;
    std::vector<std::string> commands;
    {
        std::lock_guard<std::mutex> lock(m_outbox_mutex);
        pending.swap(m_outbox);
        commands.swap(m_command_queue);
    }

    for (const std::string& text : commands)
        events::out::generictext(text);

    if (pending.empty())
        return;

    if (!m_server_peer || !m_real_server)
        return;   // no upstream yet; drop them rather than queue up stale actions

    for (const auto& message : pending)
        send(false, NET_MESSAGE_GAME_PACKET, const_cast<uint8_t*>(message.data()),
            static_cast<int32_t>(message.size()));
}

void server::poll() {
    //outgoing packets going to real server that are intercepted by our proxy server
    this->handle_outgoing();

    if (!m_real_server)
        return;

    //ingoing packets coming to gt client intercepted by our proxy client
    this->handle_incoming();

    // anything background threads asked us to send
    this->flush_queued();
}

bool server::start() {

    ENetAddress address;
    enet_address_set_host(&address, "0.0.0.0");
    address.port = m_proxyport;
    m_proxy_server = enet_host_create(&address, 1024, 10, 0, 0);
    this->threadID = std::hash<std::thread::id>{}(std::this_thread::get_id());
    if (!m_proxy_server) {
        PRINTS("failed to start the proxy server on port %d!\n", m_proxyport);
        return false;
    }
    // This host faces the Growtopia client, so it needs the server-side flag.
    // It previously set usingNewPacket (the client-side one) to false, which left the
    // new packet format off for traffic from the client and failed the handshake.
    m_proxy_server->usingNewPacketForServer = 1;
    m_proxy_server->checksum = enet_crc32;
    auto code = enet_host_compress_with_range_coder(m_proxy_server);
    if (code != 0)
        PRINTS("enet host compressing failed\n");
    PRINTS("started the enet server.\n");
    return setup_client();
}

void server::quit() {
    gt::in_game = false;
    this->disconnectsr(true);
}

bool server::setup_client() {
    // A previous session's host may still be around on reconnect; drop it first.
    if (m_real_server) {
        enet_host_destroy(m_real_server);
        m_real_server = nullptr;
        m_server_peer = nullptr;
    }
    m_real_server = enet_host_create(0, 1, 2, 0, 0);
    if (!m_real_server) {
        PRINTC("failed to start the client\n");
        return false;
    }
    // This host faces the real Growtopia server, so it uses the client-side flag.
    m_real_server->usingNewPacket = 1;
    m_real_server->checksum = enet_crc32;
    auto code = enet_host_compress_with_range_coder(m_real_server);
    if (code != 0)
        PRINTC("enet host compressing failed\n");
    enet_host_flush(m_real_server);
    PRINTC("Started enet client\n");
    return true;
}

bool server::reconnect() {
    disconnectsr(true);
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    connect();
    return true;
}

void server::redirect_server(variantlist_t& varlist) {
    m_port = varlist[1].get_uint32();
    m_token = varlist[2].get_uint32();
    m_user = varlist[3].get_uint32();
    auto str = varlist[4].get_string();

    auto doorid = str.substr(str.find("|"));
    m_server = str.erase(str.find("|")); //remove | and doorid from end
    PRINTC("port: %d token %d user %d server %s doorid %s\n", m_port, m_token, m_user, m_server.c_str(), doorid.c_str());
    varlist[1] = m_proxyport;
    varlist[4] = "127.0.0.1" + doorid;

    gt::connecting = true;
    send(true, varlist);
    if (m_real_server) {
        enet_host_destroy(m_real_server);
        m_real_server = nullptr;
        // The peer lived inside that host, so it is freed memory now. Clearing it stops
        // the disconnect path from calling enet_peer_disconnect on a dangling pointer.
        m_server_peer = nullptr;
    }
}

void server::disconnectsr(bool reset) {
    m_world.connected = false;
    m_world.local = {};
    m_world.players.clear();
    LoginNotification = 0;
    MacAdrName = "";
    if (m_server_peer) {
        // enet_peer_disconnect only queues the notice; destroying the host on the next
        // line threw it away, so the server kept the session open and the next login
        // collided with a connection it still thought was live. _now sends immediately.
        enet_peer_disconnect_now(m_server_peer, 0);
        if (m_real_server)
            enet_host_flush(m_real_server);
        m_server_peer = nullptr;
        enet_host_destroy(m_real_server);
        m_real_server = nullptr;
        PRINTS("Upstream session torn down (reset=%s)\n", reset ? "yes" : "no");
    }
    if (reset) {
        m_user = 0;
        m_token = 0;
        // Back to the address setgtserver() resolved at startup. This used to reset to a
        // hardcoded IP that is long dead, so any reconnect dialled a black hole and the
        // client sat on "Connecting..." until both it and the proxy were restarted.
        m_server = serverz;
        m_port = portz;
        PRINTS("Session reset; next connection goes to %s:%d\n", m_server.c_str(), m_port);
    }
}

bool server::connect() {
    system("Color 0B");
    PRINTS("Connecting To Grotwopia Server.\n");


    ENetAddress address;
    enet_address_set_host(&address, m_server.c_str());
    address.port = m_port;

    PRINTS("port is %d and server is %s\n", m_port, m_server.c_str());

    if (!this->setup_client()) {
        PRINTS("Failed to setup client when trying to connect to server!\n");
        return false;
    }


    m_server_peer = enet_host_connect(m_real_server, &address, 2, 0);

    if (!m_server_peer) {
        PRINTS("Failed to connect to real server.\n");
        return false;
    }

    return true;
}

bool server::sendEnetPacket(ENetPacket* packet, bool client)
{
    auto peer = client ? m_gt_peer : m_server_peer;
    auto host = client ? m_proxy_server : m_real_server;


    if (!peer || !host)
    {
        printf("The packet could not be sent due to the peer or host its null. Type %s", (client == true) ? "Growtopia" : "Local");
        enet_packet_destroy(packet);
        goto failed;
    }
    else if (peer->state != ENET_PEER_STATE_CONNECTED) {
        /*auto packet2 = utils::get_struct(packet);
        cout << (int)packet2->m_type << endl;
        if (packet2->m_type == 1) {
            variantlist_t varlist{};
            auto extended = utils::get_extended(packet2);
            extended += 4; //since it casts to data size not data but too lazy to fix this
            varlist.serialize_from_mem(extended);
            auto func = varlist[0].get_string();
            cout << varlist[0].get_string() << endl;
        }*/
        enet_host_flush(host);
        std::cout << "ENET_PEER_STATE_CONNECTED is not connected!" << std::endl;
    }
    else  if (enet_list_size(&host->peers->sentReliableCommands) > 45)
    {
        printf("Packets have been cleared due to an excessive accumulation of packets.\n");
        enet_list_clear(&host->peers->sentReliableCommands);
        enet_packet_destroy(packet);
        goto failed;
    }
    else if (enet_peer_send(peer, 0, packet) != 0)
    {

        printf("The packet could not be sent due to the enet_peer_send function return false\n");
        enet_packet_destroy(packet);
        goto failed;
    }
    else
        enet_host_flush(host);
failed:
    return false;
}

void server::SendPacketRaw2(bool client, int a1, void* packetData, size_t packetDataSize, void* a4, int packetFlag)
{
    ENetPacket* p;
    auto peer = client ? m_gt_peer : m_server_peer;
    if (peer) {
        if (a1 == 4 && *((BYTE*)packetData + 12) & 8)
        {
            p = enet_packet_create(0, packetDataSize + *((DWORD*)packetData + 13) + 5, packetFlag);
            int four = 4;
            memcpy(p->data, &four, 4);
            memcpy((char*)p->data + 4, packetData, packetDataSize);
            memcpy((char*)p->data + packetDataSize + 4, a4, *((DWORD*)packetData + 13));
            enet_peer_send(peer, 0, p);
        }
        else
        {
            p = enet_packet_create(0, packetDataSize + 5, packetFlag);
            memcpy(p->data, &a1, 4);
            memcpy((char*)p->data + 4, packetData, packetDataSize);
            enet_peer_send(peer, 0, p);
        }
    }
}
void server::SendPacketRaw(bool client, int a1, void* packetData, size_t packetDataSize, void* a4, int packetFlag) {
    ENetPacket* p;
    auto peer = client ? m_gt_peer : m_server_peer;
    if (peer) {
        if (a1 == 4 && *((BYTE*)packetData + 12) & 8) {
            p = enet_packet_create(0, packetDataSize + *((DWORD*)packetData + 13) + 5, packetFlag);
            int four = 4;
            memcpy(p->data, &four, 4);
            memcpy((char*)p->data + 4, packetData, packetDataSize);
            memcpy((char*)p->data + packetDataSize + 4, a4, *((DWORD*)packetData + 13));
            enet_peer_send(peer, 0, p);
        }
        else {
            p = enet_packet_create(0, packetDataSize + 5, packetFlag);
            memcpy(p->data, &a1, 4);
            memcpy((char*)p->data + 4, packetData, packetDataSize);
            enet_peer_send(peer, 0, p);
        }
    }
    delete (char*)packetData;
}

void server::send(bool client, int32_t type, uint8_t* data, int32_t len) {
    auto peer = client ? m_gt_peer : m_server_peer;
    auto host = client ? m_proxy_server : m_real_server;

    if (!peer || !host)
        return;
    auto packet = enet_packet_create(0, len + 5, ENET_PACKET_FLAG_RELIABLE);
    auto game_packet = (gametextpacket_t*)packet->data;
    game_packet->m_type = type;
    if (data)
        memcpy(&game_packet->m_data, data, len);

    memset(&game_packet->m_data + len, 0, 1);
    int code = enet_peer_send(peer, 0, packet);
    if (code != 0)
        PRINTS("Error Sending Packet! Code: %d\n", code);
    enet_host_flush(host);
}

//bool client: true - sends to growtopia client    false - sends to gt server
void server::send(bool client, variantlist_t& list, int32_t netid, int32_t delay) {
    auto peer = client ? m_gt_peer : m_server_peer;
    auto host = client ? m_proxy_server : m_real_server;

    if (!peer || !host)
        return;

    uint32_t data_size = 0;
    void* data = list.serialize_to_mem(&data_size, nullptr);

    //optionally we wouldnt allocate this much but i dont want to bother looking into it
    auto update_packet = MALLOC(gameupdatepacket_t, +data_size);
    auto game_packet = MALLOC(gametextpacket_t, +sizeof(gameupdatepacket_t) + data_size);

    if (!game_packet || !update_packet)
        return;

    memset(update_packet, 0, sizeof(gameupdatepacket_t) + data_size);
    memset(game_packet, 0, sizeof(gametextpacket_t) + sizeof(gameupdatepacket_t) + data_size);
    game_packet->m_type = NET_MESSAGE_GAME_PACKET;

    update_packet->m_type = PACKET_CALL_FUNCTION;
    update_packet->m_player_flags = netid;
    update_packet->m_packet_flags |= 8;
    update_packet->m_int_data = delay;
    memcpy(&update_packet->m_data, data, data_size);
    update_packet->m_data_size = data_size;
    memcpy(&game_packet->m_data, update_packet, sizeof(gameupdatepacket_t) + data_size);
    free(update_packet);

    auto packet = enet_packet_create(game_packet, data_size + sizeof(gameupdatepacket_t), ENET_PACKET_FLAG_RELIABLE);
    enet_peer_send(peer, 0, packet);
    enet_host_flush(host);
    free(game_packet);
}

//bool client: true - sends to growtopia client    false - sends to gt server
void server::send(bool client, std::string text, int32_t type) {
    auto peer = client ? m_gt_peer : m_server_peer;
    auto host = client ? m_proxy_server : m_real_server;

    if (!peer || !host)
        return;
    auto packet = enet_packet_create(0, text.length() + 5, ENET_PACKET_FLAG_RELIABLE);
    auto game_packet = (gametextpacket_t*)packet->data;
    game_packet->m_type = type;
    memcpy(&game_packet->m_data, text.c_str(), text.length());

    memset(&game_packet->m_data + text.length(), 0, 1);
    int code = enet_peer_send(peer, 0, packet);
    if (code != 0)
        //PRINTS("Error Sending Packet! Code: %d\n", code);
        enet_host_flush(host);
}



string pfstring(int number) {
    std::ostringstream ss;
    ss << number;
    return ss.str();
}




void server::AutoFindPath(int x, int y) {
    try {
        auto StartTime = std::chrono::high_resolution_clock::now();
        auto& world = g_server->m_world;

        PathFinder pf(world.tile_map.size.m_x, world.tile_map.size.m_y);

        for (int xx = 0; xx < world.tile_map.size.m_x; xx++) {
            for (int yy = 0; yy < world.tile_map.size.m_y; yy++) {
                tile_t* tile = world.tile_map.get_tile(vector2i_t(xx, yy));
                if (pathfinder_tile_blocked(world, tile))
                    pf.setBlocked(xx, yy);
            }
        }
        pf.setNeighbors({ -1, 0, 1, 0 }, { 0, 1, 0, -1 });
        vector<pair<int, int>> path = pf.aStar(g_server->m_world.local.pos.m_x / 32, g_server->m_world.local.pos.m_y / 32, x, y);
        static std::chrono::time_point<std::chrono::system_clock> timer = std::chrono::system_clock::now();

        if (path.size() > 0) {
            if (path.size() < 400)
            {
                for (auto& p : path)
                {
                    gameupdatepacket_t packet{ 0 };
                    packet.m_type = PACKET_STATE;
                    packet.m_int_data = 0;
                    packet.m_vec_x = p.first * 32;
                    packet.m_vec_y = p.second * 32;
                    packet.m_state1 = p.first;
                    packet.m_state2 = p.second;
                    packet.m_packet_flags = 32 | (1 << 10) | (1 << 11);
                    g_server->send(false, NET_MESSAGE_GAME_PACKET, (uint8_t*)&packet, sizeof(gameupdatepacket_t));
                }
            }
            if (path.size() > 400 && path.size() < 999)
            {
                for (std::size_t i = 0; i < path.size(); i += 6)
                {
                    gameupdatepacket_t packet{ 0 };
                    packet.m_type = PACKET_STATE;
                    packet.m_int_data = 0;
                    packet.m_vec_x = path[i].first * 32;
                    packet.m_vec_y = path[i].second * 32;
                    packet.m_state1 = path[i].first;
                    packet.m_state2 = path[i].second;
                    packet.m_packet_flags = 32 | (1 << 10) | (1 << 11);
                    g_server->send(false, NET_MESSAGE_GAME_PACKET, (uint8_t*)&packet, sizeof(gameupdatepacket_t));
                }
            }
            if (path.size() > 1000 && path.size() < 1799)
            {
                for (std::size_t i = 0; i < path.size(); i += 8)
                {
                    gameupdatepacket_t packet{ 0 };
                    packet.m_type = PACKET_STATE;
                    packet.m_int_data = 0;
                    packet.m_vec_x = path[i].first * 32;
                    packet.m_vec_y = path[i].second * 32;
                    packet.m_state1 = path[i].first;
                    packet.m_state2 = path[i].second;
                    packet.m_packet_flags = 32 | (1 << 10) | (1 << 11);
                    g_server->send(false, NET_MESSAGE_GAME_PACKET, (uint8_t*)&packet, sizeof(gameupdatepacket_t));
                }
            }
            if (path.size() > 1800)
            {
                for (std::size_t i = 0; i < path.size(); i += 9)
                {
                    gameupdatepacket_t packet{ 0 };
                    packet.m_type = PACKET_STATE;
                    packet.m_int_data = 0;
                    packet.m_vec_x = path[i].first * 32;
                    packet.m_vec_y = path[i].second * 32;
                    packet.m_state1 = path[i].first;
                    packet.m_state2 = path[i].second;
                    packet.m_packet_flags = 32 | (1 << 10) | (1 << 11);
                    g_server->send(false, NET_MESSAGE_GAME_PACKET, (uint8_t*)&packet, sizeof(gameupdatepacket_t));
                }
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(1ms));

            variantlist_t lost{ "OnSetPos" };
            vector2_t pos;
            pos.m_x = x * 32;
            pos.m_y = y * 32;
            lost[1] = pos;
            g_server->send(true, lost, g_server->m_world.local.netid, -1);
        }
    }
    catch (exception ex)
    {
    }

}

void server::MoveXY(int x, int y) {
    try {
        auto StartTime = std::chrono::high_resolution_clock::now();
        auto& world = g_server->m_world;

        PathFinder pf(world.tile_map.size.m_x, world.tile_map.size.m_y);

        int blocked_count = 0;
        for (int xx = 0; xx < world.tile_map.size.m_x; xx++) {
            for (int yy = 0; yy < world.tile_map.size.m_y; yy++) {
                tile_t* tile = world.tile_map.get_tile(vector2i_t(xx, yy));
                if (pathfinder_tile_blocked(world, tile)) {
                    pf.setBlocked(xx, yy);
                    blocked_count++;
                }
            }
        }
        pf.setNeighbors({ -1, 0, 1, 0 }, { 0, 1, 0, -1 });

        const int start_x = (int)(g_server->m_world.local.pos.m_x / 32);
        const int start_y = (int)(g_server->m_world.local.pos.m_y / 32);
        vector<pair<int, int>> path = pf.aStar(start_x, start_y, x, y);

        PRINTS("Pathfind (%d,%d) -> (%d,%d) | world %dx%d | blocked %d | start_blocked=%d dest_blocked=%d | path=%d\n",
            start_x, start_y, x, y, world.tile_map.size.m_x, world.tile_map.size.m_y,
            blocked_count, (int)pf.isBlocked(start_x, start_y), (int)pf.isBlocked(x, y), (int)path.size());
        static std::chrono::time_point<std::chrono::system_clock> timer = std::chrono::system_clock::now();

        if (path.size() > 0) {
            if (path.size() < 400)
            {
                for (auto& p : path)
                {
                    gameupdatepacket_t packet{ 0 };
                    packet.m_type = PACKET_STATE;
                    packet.m_int_data = 0;
                    packet.m_vec_x = p.first * 32;
                    packet.m_vec_y = p.second * 32;
                    packet.m_state1 = p.first;
                    packet.m_state2 = p.second;
                    packet.m_packet_flags = 32 | (1 << 10) | (1 << 11);
                    g_server->send(false, NET_MESSAGE_GAME_PACKET, (uint8_t*)&packet, sizeof(gameupdatepacket_t));
                }
            }
            if (path.size() > 400 && path.size() < 999)
            {
                for (std::size_t i = 0; i < path.size(); i += 6)
                {
                    gameupdatepacket_t packet{ 0 };
                    packet.m_type = PACKET_STATE;
                    packet.m_int_data = 0;
                    packet.m_vec_x = path[i].first * 32;
                    packet.m_vec_y = path[i].second * 32;
                    packet.m_state1 = path[i].first;
                    packet.m_state2 = path[i].second;
                    packet.m_packet_flags = 32 | (1 << 10) | (1 << 11);
                    g_server->send(false, NET_MESSAGE_GAME_PACKET, (uint8_t*)&packet, sizeof(gameupdatepacket_t));
                }
            }
            if (path.size() > 1000 && path.size() < 1799)
            {
                for (std::size_t i = 0; i < path.size(); i += 8)
                {
                    gameupdatepacket_t packet{ 0 };
                    packet.m_type = PACKET_STATE;
                    packet.m_int_data = 0;
                    packet.m_vec_x = path[i].first * 32;
                    packet.m_vec_y = path[i].second * 32;
                    packet.m_state1 = path[i].first;
                    packet.m_state2 = path[i].second;
                    packet.m_packet_flags = 32 | (1 << 10) | (1 << 11);
                    g_server->send(false, NET_MESSAGE_GAME_PACKET, (uint8_t*)&packet, sizeof(gameupdatepacket_t));
                }
            }
            if (path.size() > 1800)
            {
                for (std::size_t i = 0; i < path.size(); i += 9)
                {
                    gameupdatepacket_t packet{ 0 };
                    packet.m_type = PACKET_STATE;
                    packet.m_int_data = 0;
                    packet.m_vec_x = path[i].first * 32;
                    packet.m_vec_y = path[i].second * 32;
                    packet.m_state1 = path[i].first;
                    packet.m_state2 = path[i].second;
                    packet.m_packet_flags = 32 | (1 << 10) | (1 << 11);
                    g_server->send(false, NET_MESSAGE_GAME_PACKET, (uint8_t*)&packet, sizeof(gameupdatepacket_t));
                }
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(1ms));

            variantlist_t lost{ "OnSetPos" };
            vector2_t pos;
            pos.m_x = x * 32;
            pos.m_y = y * 32;
            lost[1] = pos;
            g_server->send(true, lost, g_server->m_world.local.netid, -1);

            auto EndTime = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::microseconds>(EndTime - StartTime);
            double elapsedMilliseconds = static_cast<double>(duration.count()) / 10000.0;

            variantlist_t notif{ "OnTextOverlay" };
            notif[1] = "`2Traveling " + std::to_string(path.size()) + " `2Blocks.";
            g_server->send(true, notif, -1, -1);

            //variantlist_t notif{ "OnTextOverlay" };
            //notif[1] = "`2Pathfinding `9" + std::to_string(path.size()) + " `2Blocks `9(0." + pfstring(elapsedMilliseconds) + "sec)";
            //g_server->send(true, notif, -1, -1);

        }
        else {
            variantlist_t notif{ "OnTextOverlay" };
            notif[1] = "`8Path Not Found.";
            notif[4] = 0;
            g_server->send(true, notif, -1, -1);
        }
    }
    catch (exception ex)
    {
        variantlist_t notif{ "OnTextOverlay" };
        notif[1] = "`#Something Goes Wrong";
        notif[4] = 0;
        g_server->send(true, notif, -1, -1);
    }

}