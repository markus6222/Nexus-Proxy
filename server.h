#pragma once
#include <string>
#include "proton/variant.hpp"
#include "enet/include/enet.h"
#include "world.h"
#include "world/NewWorld.h"
#include <queue>
#include "struct.h"
#include <thread>
#include <mutex>
using namespace std;
// True when the pathfinder must route around this tile. Shared by every pathfinding
// loop (gt.cpp and server.cpp) so they cannot drift apart again.
bool pathfinder_tile_blocked(world_t& world, tile_t* tile);

class server {
private:
    ENetHost* m_proxy_server;
    ENetHost* m_real_server;
    ENetPeer* m_server_peer;
    ENetPeer* m_gt_peer;

    void handle_outgoing();
    void handle_incoming();

    bool connect();
public:

    void disconnectsr(bool reset);
    void quit();
    void redirect_server(variantlist_t& varlist);
    void send(bool client, int32_t type, uint8_t* data, int32_t len);
    void send(bool client, variantlist_t& list, int32_t netid = -1, int32_t delay = 0);
    void send(bool client, std::string packet, int32_t type = 2);


    void MoveXY(int x, int y);
    void AutoFindPath(int x, int y);
    void SendPacketRaw(bool client, int a1, void* packetData, size_t packetDataSize, void* a4, int packetFlag);
    void SendPacketRaw2(bool client, int a1, void* packetData, size_t packetDataSize, void* a4, int packetFlag);
    void sendState(int netid2, bool pathfinds);
    void Set_Pos(int x, int y);
    void breakBlock(int x, int y);
    void kirisblock(int itemID, int x, int y);
    void kapigiris(bool client, int x, int y);
    void NoclipState(int netid);
    void stateReset(int netid);
    void poll();
    void lockThread();
    void unlockThread();

    static void addBlock(bool client, int a, int b, int c);
    static void placeBlock(bool client, int a, int b, int c);
    static void enterDoor(bool client, int x, int y);
    static void wearItem(int itemid);
    static void speedy(int netid, int XSpeed);

    LocalPlayer local_player;
    world n_world;
    world_t m_world;
     
    // server.h da bi�eylerin eksik gibi g�z�k�yo la ne mesela bekle bak�cam
    std::string m_server = "213.179.209.168";
    std::string serverz = m_server;
    std::string ipserver = "127.0.0.1";
    std::string create = "0.0.0.0";
    std::string meta = "defined";
    ENetPeer* getServerPeer() const;

    std::mutex mutex;

    // ENet hosts are not thread-safe. Background threads (autocollect and friends) must
    // not call enet_peer_send directly -- doing so corrupts the range coder's state and
    // it divides by zero mid-compress. Queue here instead; poll() sends it.
    void queue_to_server(const uint8_t* data, int length);
    // Hotkeys fire on their own thread and run command handlers, which send packets.
    // Queue the command text instead and let poll() run it.
    void queue_command(const std::string& text);
    void flush_queued();

    std::mutex m_outbox_mutex;
    std::vector<std::vector<uint8_t>> m_outbox;
    std::vector<std::string> m_command_queue;

    int m_user = 0;
    int m_token = 0;
    int gtmap[60][100];
    int m_port = 17182;
    int portz = m_port;
    int m_proxyport = 17191;

    int puncheffect = 0;
    bool reconnect();
    bool start();
    bool setup_client();
    bool sendEnetPacket(ENetPacket* packet, bool client);

    std::atomic<bool> mutexStatus;
    std::condition_variable cv;

    uint32_t threadID;
};
extern server* g_server;