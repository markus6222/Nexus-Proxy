#include "gt.hpp"
#include "packet.h"
#include "server.h"
#include "utils.h"
#include "events.h"
#include "Pathfinder/PathFinder.h"
#include "items_dat_decode.h"
#include "proxy.cpp"

std::string gt::version = "5.57";
std::string gt::flag = "mm";
bool gt::resolving_uid2 = false;
bool gt::connecting = false;
bool gt::in_game = false;
bool gt::ghost = false;
bool gt::antighost = false;
bool gt::resolving_uid = false;
bool gt::aapbypass = false;
bool gt::Nexus = false;
bool gt::noclip = false;
bool gt::doublejump = false;
bool gt::autocollect = false;
int gt::ac_range = 10;
int gt::path_amount = 0;
int gt::right_left = 48;
string gt::hotkey1 = "";
string gt::hotkey2 = "";
string gt::hotkey3 = "";
string gt::hotkey4 = "";
string gt::hotkey5 = "";
string gt::hotkey6 = "";
string gt::hotkey7 = "";
string gt::hotkey8 = "";
string gt::hotkey9 = "";
string gt::hotkey10 = "";
string gt::hotkey11 = "";
string gt::hotkey12 = "";
bool gt::noclip2 = false;
int gt::block_id = 0;
int gt::max_dropped_block = 0;
extern int max_dropped_block;
bool gt::showxy = false;
std::string gt::macaddr = "16:69:f9:t6:ga:6b";

void gt::log_to_console(std::string text) {
    g_server->send(true, "action|log\nmsg|" + text, NET_MESSAGE_GAME_MESSAGE);
}

std::string gt::generate_hash() {
    std::random_device random;
    std::mt19937 gen(random());
    std::uniform_int_distribution<int> range(INT_MIN, INT_MAX);
    return std::to_string(range(gen));
}

std::string gt::generate_mac_address() {
    std::random_device random;
    std::mt19937 gen(random());
    std::uniform_int_distribution<> range(0x00, 0xFF);
    std::stringstream mac;
    mac << std::hex << std::setfill('0');
    for (int i = 0; i < 6; ++i) {
        if (i != 0)
            mac << ':';
        mac << std::setw(2) << range(gen);
    }
    return mac.str();
}

std::string gt::generate_wk() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(0, 255);

    std::string rid_str;
    for (int i = 0; i < 16; i++) {
        rid_str += utils::hex_str(static_cast<unsigned char>(distrib(gen)));
    }

    std::transform(rid_str.begin(), rid_str.end(), rid_str.begin(), [](unsigned char c) {
        return std::toupper(c);
        });
    return rid_str;
}


void gt::solve_captcha(std::string text) {
    //Get the sum :D
    utils::replace(text,
        "set_default_color|`o\nadd_label_with_icon|big|`wAre you Human?``|left|206|\nadd_spacer|small|\nadd_textbox|What will be the sum of the following "
        "numbers|left|\nadd_textbox|",
        "");
    utils::replace(text, "|left|\nadd_text_input|captcha_answer|Answer:||32|\nend_dialog|captcha_submit||Submit|", "");
    auto number1 = text.substr(0, text.find(" +"));
    auto number2 = text.substr(number1.length() + 3, text.length());
    int result = atoi(number1.c_str()) + atoi(number2.c_str());
    gt::log_to_console("Solved captcha as `2" + std::to_string(result) + "``");
    g_server->send(false, "action|dialog_return\ndialog_name|captcha_submit\ncaptcha_answer|" + std::to_string(result));
}

string toString(int number) {
    std::ostringstream ss;
    ss << number;
    return ss.str();
}

void gt::MoveXY(int x, int y, int lowx, int lowy) {
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
                for (std::size_t i = 0; i < path.size(); i += 5)
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
                    std::this_thread::sleep_for(std::chrono::milliseconds(1ms));
                }
            }
            if (path.size() > 1000 && path.size() < 1799)
            {
                for (std::size_t i = 0; i < path.size(); i += 7)
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
                    std::this_thread::sleep_for(std::chrono::milliseconds(1ms));
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
                    std::this_thread::sleep_for(std::chrono::milliseconds(1ms));
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
            notif[1] = "`2Pathfinding `9" + std::to_string(path.size()) + " `2Blocks `9(0." + toString(elapsedMilliseconds) + "sec)";
            g_server->send(true, notif, -1, -1);

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

void gt::findpath(int x, int y)
{
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
            if (path.size() > 35) {
                for (int i = 0; i < path.size(); i++) {
                    GameUpdatePacket packet{ 0 };
                    packet.type = PACKET_STATE;
                    packet.int_data = 0;
                    packet.int_x = path[i].first;
                    packet.int_y = path[i].second;
                    packet.pos_x = path[i].first * 32;
                    packet.pos_y = path[i].second * 32;
                    packet.flags = 32 | (1 << 10) | (1 << 11);
                    g_server->send(false, NET_MESSAGE_GAME_PACKET, (uint8_t*)&packet, sizeof(GameUpdatePacket));
                    std::this_thread::sleep_for(std::chrono::milliseconds(2));
                }
            }
            else {
                for (std::size_t i = 0; i < path.size(); i += 2) {
                    gameupdatepacket_t packet{ 0 };
                    packet.m_type = PACKET_STATE;
                    packet.m_int_data = 0;
                    packet.m_vec_x = path[i].first * 32;
                    packet.m_vec_y = path[i].second * 32;
                    packet.m_state1 = path[i].first;
                    packet.m_state2 = path[i].second;
                    packet.m_packet_flags = 32 | (1 << 10) | (1 << 11);
                    g_server->send(false, NET_MESSAGE_GAME_PACKET, (uint8_t*)&packet, sizeof(gameupdatepacket_t));
                    std::this_thread::sleep_for(std::chrono::milliseconds(2));
                }
            }
            auto EndTime = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::microseconds>(EndTime - StartTime);
            double elapsedMilliseconds = static_cast<double>(duration.count()) / 1000.0;

            variantlist_t notif{ "OnTextOverlay" };
            notif[1] = "`2Pathfinding `9" + std::to_string(path.size()) + " `2Blocks `9(0.0" + toString(elapsedMilliseconds) + "sec)";
            g_server->send(true, notif, -1, -1);

            variantlist_t lost{ "OnSetPos" };
            vector2_t pos;
            pos.m_x = x * 32;
            pos.m_y = y * 32;
            lost[1] = pos;
            
            g_server->send(true, lost, g_server->m_world.local.netid, -1);

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
        notif[1] = "`8Something Goes Wrong";
        notif[4] = 0;
        g_server->send(true, notif, -1, -1);

    }
    gt::path_amount = 0;
}

void gt::autohost(int x, int y)
{
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
    if (path.size() < 35) {
        for (int i = 0; i < path.size(); i++) {
            GameUpdatePacket packet{ 0 };
            packet.type = PACKET_STATE;
            packet.int_data = 2250;
            packet.int_x = path[i].first;
            packet.int_y = path[i].second;
            packet.pos_x = path[i].first * 32;
            packet.pos_y = path[i].second * 32;
            packet.flags = 32 | (1 << 10) | (1 << 11);
            g_server->send(false, NET_MESSAGE_GAME_PACKET, (uint8_t*)&packet, sizeof(GameUpdatePacket));

        }
    }
    else {
        for (int i = 0; i < path.size(); i++) {
            GameUpdatePacket packet{ 0 };
            packet.type = PACKET_STATE;
            packet.int_data = 2250;
            packet.int_x = path[i].first;
            packet.int_y = path[i].second;
            packet.pos_x = path[i].first * 32;
            packet.pos_y = path[i].second * 32;
            packet.flags = 32 | (1 << 10) | (1 << 11);

            g_server->send(false, NET_MESSAGE_GAME_PACKET, (uint8_t*)&packet, sizeof(GameUpdatePacket));
        }
    }
    gt::path_amount = 0;
}



void gt::denemectrl(int x, int y)
{
    try
    {
        vector2_t pos;
        variantlist_t lost{ "OnSetPos" };
        pos.m_x = x * 32;
        pos.m_y = y * 32;
        lost[1] = pos;
        g_server->send(true, lost, g_server->m_world.local.netid, -1);
        gt::log_to_console("`9Teleporting To `2" + std::to_string(x) + " , " + std::to_string(y));
    }
    catch (const std::exception& ex)
    {

    }
    gt::path_amount = 0;
}

void gt::talkbubble(std::string text) {

    variantlist_t va{ "OnTalkBubble" };
    va[1] = g_server->m_world.local.netid;
    va[2] = text;
    va[3] = 0;

    g_server->send(true, va);
}