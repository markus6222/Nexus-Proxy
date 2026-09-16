#include "events.h"
#include "dialog.h"
#include "json.hpp"
#include "gt.hpp"
#include "proton/hash.hpp"
#include "proton/rtparam.hpp"
#include "proton/variant.hpp"
#include "server.h"
#include <vector> 
#include "utils.h"
#include <thread>
#include <limits.h>
#include "HTTPRequest.hpp"
#include "items_dat_decode.h"
#include "namespace_class.h"
#define CPPHTTPLIB_OPENSSL_SUPPORT
#include "httplib.h"
#include "player.h"
#include "world.h"
#include "Pathfinder/PathFinder.h"
#include <future>
#include "struct.h"
#include "packet.h"
#include "discord_rpc.h"
#include "proxy.cpp"
#include <stack>
#include "magic_enum/include/magic_enum/magic_enum.hpp"
#include <tlhelp32.h>
#include "klv.h"
#include "ProxyHandlers.h"

void read_logs_and_add_to_dialog(Dialog& droplog) {

    std::string filePath = "C:\\Users\\Public\\Nexus_Stuff\\Logs\\Nexus_DropCollect.txt";

    std::ifstream file(filePath);
    std::string line;

    while (std::getline(file, line)) {
        droplog.addSmallText(line.c_str());
    }

    file.close();
}

void send_vset() {
    if (iswear) {
        int skincolor = 1685231359;
        variantlist_t liste{ "OnSetClothing" };
        liste[1] = vector3_t{ hair,  shirt,  pants };
        liste[2] = vector3_t{ shoe,  face,  hand };
        liste[3] = vector3_t{ back,  mask,  neck };
        liste[4] = skincolor;
        liste[5] = vector3_t{ ances , 1.f, 0.f };

        g_server->send(true, liste, g_server->m_world.local.netid, -1);

    }
}

std::string directoryPaths = "C:\\Users\\Public\\Nexus_Stuff\\save\\clothes";
std::string fileNames = "";
std::string filePaths = directoryPaths + "\\" + fileNames;
std::ofstream files(filePaths, std::ios::app);

void SendWarn(std::string text)
{
    variantlist_t varlist{ "OnAddNotification" };
    varlist[1] = "interface/atomic_button.rttex";
    varlist[2] = text;
    varlist[3] = "audio/hub_open.wav";
    g_server->send(true, varlist);
}

void autoacca() {
    Sleep(500);
    nopulldialog = false;
    autoaccc = false;
}
bool events::out::variantlist(gameupdatepacket_t* packet) {
    variantlist_t varlist{};
    varlist.serialize_from_mem(utils::get_extended(packet));
    /*if (iswear) {
        int skincolor = 1685231359;
        variantlist_t liste{ "OnSetClothing" };
        liste[1] = vector3_t{ hair,  shirt,  pants };
        liste[2] = vector3_t{ shoe,  face,  hand };
        liste[3] = vector3_t{ back,  mask,  neck };
        liste[4] = skincolor;
        liste[5] = vector3_t{ ances , 1.f, 0.f };

        g_server->send(true, liste, g_server->m_world.local.netid, -1);
        g_server->sendState(g_server->m_world.local.netid);
    }*/
    if (debug)
        PRINTS("varlist: %s\n", varlist.print().c_str());
    return false;
}

bool events::out::pingreply(gameupdatepacket_t* packet) {
    // The client's ping reply reports its gravity, move speed and punch/build ranges, and
    // the server checks them. This used to overwrite them on every ping with values no
    // real client sends (ranges of 64 against a normal 5-8, gravity 1000, speed 250) and
    // zero out the jump/effect flags. Current servers reject that: you enter a world and
    // get dropped a couple of seconds later.
    //
    // Range and state changes belong to the toggles that already implement them
    // (superpunch in events::out::state, serverlongpunch in server::sendState), so the
    // ping reply is forwarded untouched.
    (void)packet;
    return false;
}
bool hostermode = false;

string toStringDouble(double number) {
    std::ostringstream ss;
    ss << number;
    return ss.str();
}

void save_enter_exit_log(const std::string& log) {

    std::string filePath = "C:\\Users\\Public\\Nexus_Stuff\\Logs\\Nexus_EnterExit.txt";

    std::ofstream file(filePath, std::ios::app);

    if (file.is_open()) {
        file << log << "\n";
        file.close();
    }
    else {
    }
}

void save_details() {
    std::wstring dosyaYolu = L"C:\\Users\\Public\\Nexus_Stuff\\save\\config.json";
    std::ifstream ifs(dosyaYolu);
    if (ifs.is_open()) {
        json j;
        ifs >> j;
        j["auto_surg"] = auto_surg;
        j["fastdrop"] = fastdrop;
        j["fasttrash"] = fasttrash;
        j["rqqmode"] = Nexusqq;
        j["rremeemode"] = Nexusreme;
        j["namenumber"] = namenumber;
        j["pinglatency"] = pinglatency;
        j["denemectrl"] = denemectrl;
        j["autobgl"] = autobgl;
        j["autoacc"] = autoacc;
        j["pathfinder"] = pathfinder;
        j["antigravity"] = antigravity;
        j["antighost"] = gt::antighost;
        j["ghost"] = gt::ghost;
        j["autobanfire"] = autobanfire;
        j["PocketLighterBan"] = PocketLighterBan;
        j["modas"] = modas;

        std::ofstream f_(dosyaYolu);
        f_ << j;
        f_.close();
    }
}
void save_details2() {
    std::wstring dosyaYolu = L"C:\\Users\\Public\\Nexus_Stuff\\save\\config2.json";
    std::ifstream ifs(dosyaYolu);
    if (ifs.is_open()) {
        json j;
        ifs >> j;
        j["auto_surg"] = auto_surg;
        j["fastdrop"] = fastdrop;
        j["fasttrash"] = fasttrash;
        j["rqqmode"] = Nexusqq;
        j["rremeemode"] = Nexusreme;
        j["namenumber"] = namenumber;
        j["pinglatency"] = pinglatency;
        j["denemectrl"] = denemectrl;
        j["autobgl"] = autobgl;
        j["autoacc"] = autoacc;
        j["pathfinder"] = pathfinder;
        j["antigravity"] = antigravity;
        j["antighost"] = gt::antighost;
        j["ghost"] = gt::ghost;
        j["autobanfire"] = autobanfire;
        j["PocketLighterBan"] = PocketLighterBan;
        j["modas"] = modas;

        std::ofstream f_(dosyaYolu);
        f_ << j;
        f_.close();
    }
}
void save_details3() {
    std::wstring dosyaYolu = L"C:\\Users\\Public\\Nexus_Stuff\\save\\config3.json";
    std::ifstream ifs(dosyaYolu);
    if (ifs.is_open()) {
        json j;
        ifs >> j;
        j["auto_surg"] = auto_surg;
        j["fastdrop"] = fastdrop;
        j["fasttrash"] = fasttrash;
        j["rqqmode"] = Nexusqq;
        j["rremeemode"] = Nexusreme;
        j["namenumber"] = namenumber;
        j["pinglatency"] = pinglatency;
        j["denemectrl"] = denemectrl;
        j["autobgl"] = autobgl;
        j["autoacc"] = autoacc;
        j["pathfinder"] = pathfinder;
        j["antigravity"] = antigravity;
        j["antighost"] = gt::antighost;
        j["ghost"] = gt::ghost;
        j["autobanfire"] = autobanfire;
        j["PocketLighterBan"] = PocketLighterBan;
        j["modas"] = modas;

        std::ofstream f_(dosyaYolu);
        f_ << j;
        f_.close();
    }
}

// Dialog and chat values are parsed with stoi throughout this file, and most call sites
// are unguarded. stoi throws on anything non-numeric, and an uncaught throw aborts the
// whole process -- enabling autosurg did exactly that. These return a fallback instead.
static int safe_stoi(const std::string& text, int fallback = 0) {
    try { return std::stoi(text); } catch (...) { return fallback; }
}
static float safe_stof(const std::string& text, float fallback = 0.f) {
    try { return std::stof(text); } catch (...) { return fallback; }
}
static double safe_stod(const std::string& text, double fallback = 0.0) {
    try { return std::stod(text); } catch (...) { return fallback; }
}

int item_count(uint32_t itemID)
{
    int countitem = 0;
    const auto& items = g_server->local_player.inventory.items;

    auto it = items.find(itemID);
    if (it != items.end())
    {
        countitem = it->second.count;
    }

    return countitem;
}

void DoChat(string text) {
    g_server->send(false, "action|input\n|text|" + text);
    Sleep(5);
    return;
}

int drop_count(int itemid) {
    int wl = g_server->local_player.inventory.getItemCount(itemid);
    return wl;
}

int Balance()
{
    int wl = 0;

    wl += item_count(242);
    wl += item_count(1796) * 100;
    wl += item_count(7188) * 10000;

    return wl;
}

void DropItem(int itemID, int count)
{
    g_server->send(false, "action|drop\n|itemID|" + to_string(itemID));
    g_server->send(false, "action|dialog_return\ndialog_name|drop_item\nitemID|" + to_string(itemID) + "\ncount|" + to_string(count) + "\n");
}

bool daw()
{
    if (item_count(242) > 0) {
        dropwl = true;
        g_server->send(false, "action|drop\n|itemID|242");
        g_server->send(false, "action|dialog_return\ndialog_name|drop_item\nitemID|242|\ncount|" + to_string(item_count(242))); //242
    }
    if (item_count(1796) > 0) {
        dropdl = true;
        g_server->send(false, "action|drop\n|itemID|1796");
        g_server->send(false, "action|dialog_return\ndialog_name|drop_item\nitemID|1796|\ncount|" + to_string(item_count(1796))); //242
    }
    if (item_count(7188) > 0) {
        dropwl = true;
        g_server->send(false, "action|drop\nitemID|7188");
        g_server->send(false, "action|dialog_return\ndialog_name|drop_item\nitemID|7188|\ncount|" + to_string(item_count(7188))); //242
    }
    return true;
}

std::string generateRandomMAC() {
    std::srand(std::time(0));
    std::string mac = "02";
    for (int i = 0; i < 5; ++i) {
        mac += ":";
        int byte = std::rand() % 256;
        char buffer[3];
        snprintf(buffer, sizeof(buffer), "%02X", byte);
        mac += buffer;
    }
    return mac;
}

void changeMACAddress(const std::string& deneme, const std::string& newMAC) {
    std::string command = "reg add \"HKLM\\SYSTEM\\CurrentControlSet\\Control\\Class\\{4d36e972-e325-11ce-bfc1-08002be10318}\\0001\" /v NetworkAddress /d " + newMAC + " /f";
    system(command.c_str());
    command = "netsh interface set interface name=\"" + deneme + "\" admin=disable";
    system(command.c_str());
    command = "netsh interface set interface name=\"" + deneme + "\" admin=enable";
    system(command.c_str());
}


std::set<std::string> loadAutoPullNames(const std::string& filepath) {
    std::set<std::string> names;
    std::ifstream file(filepath);
    std::string line;
    while (std::getline(file, line)) {
        if (!line.empty()) {
            names.insert(line);
        }
    }
    return names;
}

std::string readFileContents(const std::string& filePath) {
    std::ifstream file(filePath);
    std::stringstream buffer;
    if (file) {
        buffer << file.rdbuf();
        file.close();
    }
    else {
        std::cerr << "Unable to open file: " << filePath << std::endl;
    }
    return buffer.str();
}

bool is_numeric(const std::string& str) {
    return std::all_of(str.begin(), str.end(), ::isdigit);
}

// --- casino helpers -------------------------------------------------------------
// The betting commands need to know what is lying on a betting square and to pick it
// up. Both walk the world's drop list, which the proxy already tracks.

static bool near_pos(const vector2_t& a, const vector2_t& b, int range_tiles) {
    const float dx = a.m_x - b.m_x;
    const float dy = a.m_y - b.m_y;
    const float r = static_cast<float>(range_tiles * 32);
    return (dx * dx + dy * dy) <= (r * r);
}

// Worth of the locks lying on a betting square, in world-lock units.
static int bet_value_at(const vector2_t& pos, int range_tiles = 5) {
    int total = 0;
    for (const auto& object : g_server->m_world.object_map.snapshot()) {
        if (!near_pos(object.pos, pos, range_tiles))
            continue;
        switch (object.id) {
        case 242:  total += object.amount; break;              // World Lock
        case 1796: total += object.amount * 100; break;         // Diamond Lock
        case 7188: total += object.amount * 10000; break;       // Blue Gem Lock
        default: break;
        }
    }
    return total;
}

// A game packet on the wire is 56 bytes. sizeof(GameUpdatePacket) is 60 because the
// struct carries an extra trailing field, so sending sizeof() overruns it by four.
void build_collect_packet(uint32_t oid, float x, float y, uint8_t* out) {
    memset(out, 0, GAME_PACKET_SIZE);
    out[0] = PACKET_ITEM_ACTIVATE_OBJECT_REQUEST;

    // The collect request has to name the player doing the collecting. This was sent as
    // -1, which the server simply ignored -- that is why nothing was ever picked up.
    const int32_t netid = g_server->m_world.local.netid;
    memcpy(out + 4, &netid, 4);
    memcpy(out + 20, &oid, 4);
    memcpy(out + 24, &x, 4);
    memcpy(out + 28, &y, 4);
}

// Ask the server to pick up every drop on a betting square. Same request the
// autocollect thread uses.
static int collect_objects_at(const vector2_t& pos, int range_tiles = 5) {
    int sent = 0;
    for (const auto& object : g_server->m_world.object_map.snapshot()) {
        if (!near_pos(object.pos, pos, range_tiles))
            continue;
        uint8_t packet[GAME_PACKET_SIZE];
        build_collect_packet(object.oid, object.pos.m_x, object.pos.m_y, packet);
        g_server->send(false, NET_MESSAGE_GAME_PACKET, packet, GAME_PACKET_SIZE);
        sent++;
    }
    return sent;
}

void mal_pf(int x, int y) {
    int deger = 0;
    if (gt::right_left == 32) {
        deger = rand() % 10;
    }
    else {
        deger = (rand() % 10) + -1.1;
    }
    gameupdatepacket_t packet{ 0 };
    packet.m_type = PACKET_STATE;
    packet.m_int_data = 526;
    packet.m_vec_x = (x * 32) + deger;
    packet.m_vec_y = y * 32;
    packet.m_state1 = x;
    packet.m_state2 = y;
    packet.m_packet_flags = gt::right_left | (1 << 10) | (1 << 11);
    g_server->send(false, NET_MESSAGE_GAME_PACKET, (uint8_t*)&packet, sizeof(gameupdatepacket_t));
}

void aharvest()
{
    while (true) {
        Sleep(1);
        if (harvestauto)
        {
            for (auto& tile : g_server->m_world.tile_map.tiles)
            {
                if (!harvestauto)
                    break;
                if (tile->foreground == harvestid)
                {
                    Sleep(harvestdelay);
                    g_server->AutoFindPath(tile->pos.m_x, tile->pos.m_y);
                    Sleep(harvestdelay);
                    gt::log_to_console("`3[" + to_string(tile->pos.m_x) + "," + to_string(tile->pos.m_y) + "]" + " Found closest open path, now breaking...");
                    gameupdatepacket_t packet{};
                    packet.m_type = PACKET_TILE_CHANGE_REQUEST;
                    packet.m_int_data = 18;
                    packet.m_vec_x = g_server->m_world.local.pos.m_x;
                    packet.m_vec_y = g_server->m_world.local.pos.m_y;
                    packet.m_state1 = tile->pos.m_x;
                    packet.m_state2 = tile->pos.m_y;
                    g_server->send(false, NET_MESSAGE_GAME_PACKET, (uint8_t*)&packet, sizeof(gameupdatepacket_t));
                    Sleep(harvestdelay);
                    gt::log_to_console("`2Sent Break!");
                }
            }
        }
    }
}

void aplant()
{
    while (true) {
        Sleep(1);

        if (plantauto)
        {
            for (auto& tile : g_server->m_world.tile_map.tiles)
            {
                if (!plantauto) break;
                if (tile->foreground == 0)
                {
                    vector2i_t below_pos(tile->pos.m_x, tile->pos.m_y + 1);
                    auto* below_tile = g_server->m_world.tile_map.get_tile(below_pos);
                    if (below_tile->foreground == 2 || below_tile->foreground == 10)
                    {
                        Sleep(plantdelay);
                        g_server->AutoFindPath(tile->pos.m_x, tile->pos.m_y);
                        Sleep(plantdelay);
                        gt::log_to_console("`3[" + to_string(tile->pos.m_x) + "," + to_string(tile->pos.m_y) + "]" + " Found closest open path, now placing...");
                        gameupdatepacket_t packet{};
                        packet.m_type = PACKET_TILE_CHANGE_REQUEST;
                        packet.m_int_data = plantseed;
                        packet.m_vec_x = g_server->m_world.local.pos.m_x;
                        packet.m_vec_y = g_server->m_world.local.pos.m_y;
                        packet.m_state1 = g_server->m_world.local.pos.m_x / 32;
                        packet.m_state2 = g_server->m_world.local.pos.m_y / 32;
                        g_server->send(false, NET_MESSAGE_GAME_PACKET, (uint8_t*)&packet, sizeof(gameupdatepacket_t));
                        Sleep(plantdelay);
                        gt::log_to_console("`2Sent Place!");
                    }
                }
            }
        }
    }
}

void punch(int x, int y)
{
    gameupdatepacket_t packet{};
    packet.m_type = 3;
    packet.m_int_data = 18;
    packet.m_vec_x = g_server->m_world.local.pos.m_x;
    packet.m_vec_y = g_server->m_world.local.pos.m_y;
    packet.m_state1 = x;
    packet.m_state2 = y;
    g_server->send(false, NET_MESSAGE_GAME_PACKET, (uint8_t*)&packet, sizeof(gameupdatepacket_t));
}

void place(int x, int y, int id)
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

void CopyWorld()
{
    std::ofstream file("C:\\Users\\Public\\Nexus_Stuff\\copyworld.txt");
    std::vector<int> foregroundIgnoreList = { 6, 8, 9640, 242, 1796, 7188 };

    for (auto& tile : g_server->m_world.tile_map.tiles) {
        bool ignoreTile = false;
        for (int ignoreValue : foregroundIgnoreList) {
            if (tile->foreground == ignoreValue) {
                ignoreTile = true;
                break;
            }
        }

        if (!ignoreTile && (tile->foreground > 0 || tile->background > 0)) {
            file << "x: " << tile->pos.m_x << " - y: " << tile->pos.m_y
                << " - fg: " << tile->foreground << " - bg: " << tile->background << std::endl;
        }
    }

    file.close();
}

void PasteWorld()
{
    while (true) {
        Sleep(1);
        if (pasteworld)
        {
            if (!pasteworld) break;
            std::string dosya_yolu = "C:\\Users\\Public\\Nexus_Stuff\\copyworld.txt";
            std::ifstream dosya(dosya_yolu);
            std::string satir;
            while (std::getline(dosya, satir)) {
                std::istringstream iss(satir);

                std::string token;
                int x = 0, y = 0, fg = 0, bg = 0;
                while (iss >> token) {
                    if (token == "x:") {
                        iss >> x;
                    }
                    else if (token == "y:") {
                        iss >> y;
                    }
                    else if (token == "fg:") {
                        iss >> fg;
                    }
                    else if (token == "bg:") {
                        iss >> bg;
                    }
                }
                /*
                if (item_count(fg || bg) == 0)
                {
                    Sleep(500);
                    bool found = false;
                    gt::log_to_console("`8No stock in inventory, looking for dropped blocks...");
                    for (const auto& object : g_server->m_world.object_map.objects) {
                        if (object->id == fg || bg) {
                            int posX = object->pos.m_x / 32;
                            int posY = object->pos.m_y / 32;
                            g_server->AutoFindPath(posX, posY);
                            gt::log_to_console("`5Collected dropped blocks at [" + to_string(posX) + "," + to_string(posY) + "]");
                            found = true;
                        }
                    }
                    if (!found)
                    {
                        gt::log_to_console("`4Dropped item not found.");
                    }
                }
                */
                Sleep(350);
                modfly = 0;
                g_server->sendState(g_server->m_world.local.netid, false);
                Sleep(350);
                g_server->AutoFindPath(x + 1, y);
                gt::log_to_console("`3[" + to_string(x) + "," + to_string(y) + "]" + " Found closest open path, now placing...");
                Sleep(350);
                place(x, y, bg);
                Sleep(350);
                place(x, y, fg);
                gt::log_to_console("`2Sent Place!");;
                Sleep(350);
                modfly = 1000;
            }
            dosya.close();
        }
    }
}


void kirma(int x, int y)
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

void aclear()
{
    while (true) {
        Sleep(1);
        if (clearauto)
        {
            for (auto& tile : g_server->m_world.tile_map.tiles)
            {
                if (!clearauto) break;
                if (openfly)
                {
                    modfly = 0;
                }
                if (tile->foreground == 2)
                {
                    Sleep(cleardelay);
                    g_server->AutoFindPath(tile->pos.m_x, tile->pos.m_y - 1);
                    gt::log_to_console("`3[" + to_string(tile->pos.m_x) + "," + to_string(tile->pos.m_y) + "]" + " Found closest open path, now breaking...");
                    Sleep(cleardelay);
                    for (int i = 0; i < 4; i++)
                    {
                        kirma(tile->pos.m_x, tile->pos.m_y);
                        gt::log_to_console("`2Sent Break!");
                        Sleep(cleardelay);    
                    }
                }
                if (tile->foreground == 4)
                {
                    Sleep(cleardelay);
                    g_server->AutoFindPath(tile->pos.m_x, tile->pos.m_y - 1);
                    gt::log_to_console("`3[" + to_string(tile->pos.m_x) + "," + to_string(tile->pos.m_y) + "]" + " Found closest open path, now breaking...");
                    Sleep(cleardelay);
                    for (int i = 0; i < 4; i++)
                    {
                        kirma(tile->pos.m_x, tile->pos.m_y);
                        gt::log_to_console("`2Sent Break!");
                        Sleep(cleardelay);
                    }
                }
                if (tile->background == 14)
                {
                    Sleep(cleardelay);
                    g_server->AutoFindPath(tile->pos.m_x, tile->pos.m_y - 1);
                    gt::log_to_console("`3[" + to_string(tile->pos.m_x) + "," + to_string(tile->pos.m_y) + "]" + " Found closest open path, now breaking...");
                    Sleep(cleardelay);
                    for (int i = 0; i < 4; i++)
                    {
                        kirma(tile->pos.m_x, tile->pos.m_y);
                        gt::log_to_console("`2Sent Break!");
                        Sleep(cleardelay);
                    }
                }
                if (tile->foreground == 10)
                {
                    Sleep(cleardelay);
                    g_server->AutoFindPath(tile->pos.m_x, tile->pos.m_y - 1);
                    gt::log_to_console("`3[" + to_string(tile->pos.m_x) + "," + to_string(tile->pos.m_y) + "]" + " Found closest open path, now breaking...");
                    Sleep(cleardelay);
                    for (int i = 0; i < 12; i++)
                    {
                        kirma(tile->pos.m_x, tile->pos.m_y);
                        gt::log_to_console("`2Sent Break!");
                        Sleep(cleardelay);
                    }
                }
                modfly = 1000;
            }
        }
    }
}

void getfindresults(std::string text) {
    if (iswear == true) {
        bc = "1";
    }
    else {
        bc = "0";
    }
    string find_listx = "";

    for (int i = 0; i < index.size(); i++) {
        item_id = index[i].itemID;
        if (to_lower(index[item_id].name).find(text) != string::npos) {
            if (index[item_id].name.find(" Seed") != std::string::npos) continue;
            //gt::log_to_console("" + to_string(item_id) + ": " + index[item_id].name + "");
            //find_list += "\nadd_label_with_icon|small|" + to_string(item_id) + " -> " + index[item_id].name + "|left|" + to_string(item_id) + "|";
            find_listx += "\nadd_label_with_icon_button|small|`w" + index[item_id].name + " `w: " + to_string(item_id) + "|left|" + to_string(item_id) + "|spare_btn_" + to_string(item_id) + "|noflags|0|0|";
        }
    }
    std::string paket;
    paket =
        "\nadd_label_with_icon|big|`wResults For `#" + text + "|left|2242|"
        "\nadd_spacer|small"
        "\nadd_textbox|`wClick On Item `#Icon Of The Item `wTo Add It To You Inventory|left|2480|" +
        bc +
        "|"
        "\n" + find_listx + ""
        "\nadd_quick_exit|"
        "\nend_dialog|find_dialog|Cancel|Okay|";
    variantlist_t liste{ "OnDialogRequest" };
    liste[1] = paket;
    g_server->send(true, liste);
}



int scangemscount = 0;
void itemScanDialog(int x, int y) {
    bool moi = false;
    for (auto& object0 : g_server->m_world.object_map.objects) {
        int posX = object0->pos.m_x / 32;
        int posY = object0->pos.m_y / 32;
        if (posX == x && posY == y || posX == x && posY == y + 1 || posX == x + 1 && posY == y || posX == x - 1 && posY == y) {
            moi = true;
        }
    }
    if (!moi) {
        variantlist_t v{ "OnTextOverlay" };
        v[1] = "`4No Floating Items found in this Tile.";
        g_server->send(true, v);
        return;
    }
    Dialog a;
    a.addLabelWithIcon("`wExtract-O-Snap", 6140, LABEL_BIG);
    a.addSpacer(SPACER_SMALL);
    a.addTextBox("GrowTech: use the Extract-O-Snap to pick out the items from the floating items in your world! - Thanks, Technician Dave.");
    a.addSpacer(SPACER_SMALL);
    a.addTextBox("Press on the icon to extract the item into your inventory.");
    a.addSpacer(SPACER_SMALL);
    int gemtoplam = 0;
    bool moi3 = false;
    for (auto& object0 : g_server->m_world.object_map.objects) {
        int posX = object0->pos.m_x / 32;
        int posY = object0->pos.m_y / 32;
        if (posX == x && posY == y || posX == x && posY == y + 1 || posX == x + 1 && posY == y || posX == x - 1 && posY == y) {
            if (object0->id == 112) {
                gemtoplam += object0->amount;
            }
        }
    }
    a.addLabelWithIcon("`wTotal Gems: " + std::to_string(gemtoplam), 9438, LABEL_SMALL);
    a.addSpacer(SPACER_SMALL);
    a.addTextBox("`wItem List:");
    bool moi2 = false;
    for (auto& object0 : g_server->m_world.object_map.objects) {
        int posX = object0->pos.m_x / 32;
        int posY = object0->pos.m_y / 32;
        if (posX == x && posY == y || posX == x && posY == y + 1 || posX == x + 1 && posY == y || posX == x - 1 && posY == y) {
            if (object0->id) {
                a.addLabelWithIconButton("`w" + index[object0->id].name + " `w: " + std::to_string(object0->amount), object0->id, "ext_" + std::to_string(object0->id) + "_x_" + to_string(object0->pos.m_x) + "_y_" + to_string(object0->pos.m_y));
            }
        }
    }
    scangemscount = 0;
    a.endDialog("itemscanner", "", "Cancel");
    variantlist_t liste{ "OnDialogRequest" };
    liste[1] = a.finishDialog();
    g_server->send(true, liste);
}

void GemTake(int x, int y) {
    bool moi = false;
    for (auto& object0 : g_server->m_world.object_map.objects) {
        int posX = object0->pos.m_x / 32;
        int posY = object0->pos.m_y / 32;
        if (posX == x && posY == y || posX == x && posY == y + 1 || posX == x + 1 && posY == y || posX == x - 1 && posY == y) {
            moi = true;
        }
    }
    if (!moi) {
        variantlist_t v{ "OnTextOverlay" };
        v[1] = "`4No Floating Gems found in this Tile.";
        g_server->send(true, v);
        return;
    }
    int gemtoplam = 0;
    bool moi2 = false;
    for (auto& object0 : g_server->m_world.object_map.objects) {
        int posX = object0->pos.m_x / 32;
        int posY = object0->pos.m_y / 32;
        if (posX == x && posY == y || posX == x && posY == y + 1 || posX == x + 1 && posY == y || posX == x - 1 && posY == y) {
            if (object0->id == 112) {
                gemtoplam += object0->amount;
            }
        }
    }
    variantlist_t varlist{ "OnTextOverlay" };
    varlist[1] = "`#Tile Gems Count: `2" + std::to_string(gemtoplam);
    g_server->send(true, varlist);
}

void sendGemEffect(int x, int y, int count) {
    if (count != 0) {
        variantlist_t varlist{ "OnParticleEffect" };
        varlist[1] = 181;
        varlist[2] = vector2_t{ float(x), float(y) };
        varlist[3] = float(count);
        varlist[4] = float(0.000000);
        g_server->send(true, varlist);
    }
}

void showTotalGemsInTiles(int x, int y, int count) {
    sendGemEffect(x, y, count);
    return;
}
int getGemsFromPos(int x, int y) {
    int count = 0;
    auto objects = g_server->m_world.object_map.objects;
    for (auto& object : objects) {
        if (object->id == 112) {
        }
    }
    return count;
}

void showGemsInTiles1() {
    auto objects = g_server->m_world.object_map.objects;
    for (auto& obj : objects) {
        if (obj->id == 112) {
            int posX = obj->pos.m_x / 32;
            int posY = obj->pos.m_y / 32;
            showTotalGemsInTiles(posX * 32, posY * 32, getGemsFromPos(posX * 32, posY * 32));
        }
    }
}

inline void dropitemCasual(int itemid, int count) {
    g_server->send(false, "action|drop\nitemID|" + std::to_string(itemid));
    g_server->send(false, "action|dialog_return\ndialog_name|drop_item\nitemID|" + std::to_string(itemid) + "|\ncount|" + std::to_string(count));
}

void dropAll() {
    if (g_server->m_world.connected) {
        dAll = true;
        auto itemler = g_server->local_player.inventory.items;
        int rmbaba = 0;
        for (auto item : itemler) {
            if (items[item.first].itemProps2 >= 0 && rmbaba <= 19) {
                dropitemCasual(item.first, item.second.count);
                gt::log_to_console("`9Dropped `2" + to_string(item.second.count) + " " + items[item.first].name + "`9.");
                rmbaba++;
            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(1250));
        dAll = false;
    }
}

/*
void dropalla() {
    dropall = true;
    drop_finish = false;
    for (int i = 0; i < items_count; i++) {

        if (g_server->local_player.inventory.getItemCount(i) > 0 && i != 18 && i != 32 && i != 6336 && !drop_finish) {

            g_server->send(false, "action|drop\nitemID|" + std::to_string(i));
            std::this_thread::sleep_for(std::chrono::milliseconds(300));
            g_server->send(false, "action|dialog_return\ndialog_name|drop_item\nitemID|" + to_string(i) + "|\ncount|" + to_string(drop_count(i))); //242
        }
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(200));
    dropall = false;
    after = true;
    gt::log_to_console("Dropped all items.");
}
*/

int betauto = 0; // wls
double taxauto = 10.0; // tax %
int autohosttp = 0;
int bets[2] = { 0, 0 };
int casino_payout = 0;   // what /win1 and /win2 pay out, fixed when /tp runs

int Tax(int bet, double tax) {
    return floor(tax / 100 * bet);
}



void placeaga(int id, int posx, int posy) {
    if (posx == 0 || posy == 0)
    {
        gt::log_to_console("position cannot be null!");
        return;
    }
    auto pos = g_server->m_world.local.pos;
    GameUpdatePacket legitpacket{ 0 };
    legitpacket.type = PACKET_STATE;
    legitpacket.item_id = id;
    legitpacket.int_x = posx;
    legitpacket.int_y = posy;
    legitpacket.vec_x = pos.m_x;
    legitpacket.vec_y = pos.m_y;
    legitpacket.flags = 2592;

    g_server->send(false, NET_MESSAGE_GAME_PACKET, (uint8_t*)&legitpacket, sizeof(GameUpdatePacket));

}

std::string random_string(int length) {
    const std::string charset = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    const int charsetLength = charset.length();

    // Initialize random seed
    std::srand(std::time(nullptr));

    std::string result;
    for (int i = 0; i < length; ++i) {
        result += charset[std::rand() % charsetLength];
    }

    return result;
}

void place_tile(int id, int posx, int posy) {
    if (posx == 0 || posy == 0)
    {
        gt::log_to_console("position cannot be null!");
        return;
    }
    auto pos = g_server->m_world.local.pos;

    g_server->enterDoor(false, posx, posy);

    GameUpdatePacket packet{ 0 };
    packet.type = PACKET_TILE_CHANGE_REQUEST;
    packet.item_id = id;
    packet.int_x = posx;
    packet.int_y = posy;
    packet.vec_x = pos.m_x;
    packet.vec_y = pos.m_y;
    g_server->send(false, NET_MESSAGE_GAME_PACKET, (uint8_t*)&packet, sizeof(GameUpdatePacket));
}

void send_effect_real() {


    if (events::out::get_punch_id(hand) != 0) {
        punch_effect = events::out::get_punch_id(hand);
    }
    else if (events::out::get_punch_id(face) != 0) {
        punch_effect = events::out::get_punch_id(face);
    }
    else  if (events::out::get_punch_id(shirt) != 0) {
        punch_effect = events::out::get_punch_id(shirt);
    }
    else  if (events::out::get_punch_id(mask) != 0) {
        punch_effect = events::out::get_punch_id(mask);
    }
    else  if (events::out::get_punch_id(neck) != 0) {
        punch_effect = events::out::get_punch_id(neck);
    }
    else if (events::out::get_punch_id(neck) != 0) {
        punch_effect = events::out::get_punch_id(neck);
    }
    else {
        punch_effect = 8421376;
    }

    if (back != 0000.0) {
        doublejump = true;
    }

}

void break_tile(int id, int posx, int posy) {
    //g_server->breakBlock(false, posx, posy);
    place_tile(18, posx, posy);
    std::this_thread::sleep_for(std::chrono::milliseconds(250));
    place_tile(18, posx, posy);
    std::this_thread::sleep_for(std::chrono::milliseconds(250));
    place_tile(18, posx, posy);
    std::this_thread::sleep_for(std::chrono::milliseconds(250));
    place_tile(18, posx, posy);
}

void TpRetard(float x, float y)
{
    vector2_t pos;
    pos.m_x = x;
    pos.m_y = y;
    variantlist_t varlist{ "OnSetPos" };
    varlist[1] = pos;
    g_server->m_world.local.pos = pos;
    g_server->send(true, varlist, g_server->m_world.local.netid, -1);
}

void do_punch_roulette() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dist(500, 1000);

    for (auto& tile : g_server->m_world.tile_map.tiles)
    {
        if (tile->foreground == 758)
        {
            gameupdatepacket_t packet{};
            packet.m_type = PACKET_TILE_CHANGE_REQUEST;
            packet.m_int_data = 6326;
            packet.m_state1 = tile->pos.m_x;
            packet.m_state2 = tile->pos.m_y;
            packet.m_vec_x = g_server->m_world.local.pos.m_x;
            packet.m_vec_y = g_server->m_world.local.pos.m_y;

            g_server->send(false, NET_MESSAGE_GAME_PACKET, (uint8_t*)&packet, sizeof(gameupdatepacket_t));

            auto delay = std::chrono::milliseconds(dist(gen));
            std::this_thread::sleep_for(delay);
        }
    }
}

int events::out::get_punch_id(const int id_) {
    switch (id_) {
    case 138: return 1;
    case 366:
    case 1464:
        return 2;
    case 472: return 3;
    case 594:
    case 10130:
    case 5424:
    case 5456:
    case 4136:
    case 10052:
        return 4;
    case 768: return 5;
    case 900:
    case 7760:
    case 9272:
    case 5002:
    case 7758:
        return 6;
    case 910:
    case 4332:
        return 7;
    case 930:
    case 1010:
    case 6382:
        return 8;
    case 1016:
    case 6058:
        return 9;
    case 1204:
    case 9534:
        return 10;
    case 1378: return 11;
    case 1440: return 12;
    case 1484:
    case 5160:
    case 9802:
        return 13;
    case 1512:
    case 1648:
        return 14;
    case 1542: return 15;
    case 1576: return 16;
    case 1676:
    case 7504:
        return 17;
    case 1748:
    case 8006:
    case 8008:
    case 8010:
    case 8012:
        return 19;
    case 1710:
    case 4644:
    case 1714:
    case 1712:
    case 6044:
    case 1570:
        return 18;
    case 1780: return 20;
    case 1782:
    case 5156:
    case 9776:
    case 9782:
    case 9810:
        return 21;
    case 1804:
    case 5194:
    case 9784:
        return 22;
    case 1868:
    case 1998:
        return 23;
    case 1874: return 24;
    case 1946:
    case 2800:
        return 25;
    case 1952:
    case 2854:
        return 26;
    case 1956: return 27;
    case 1960: return 28;
    case 2908:
    case 6312:
    case 9496:
    case 8554:
    case 3162:
    case 9536:
    case 4956:
    case 3466:
    case 4166:
    case 4506:
    case 2952:
    case 9520:
    case 9522:
    case 8440:
    case 3932:
    case 3934:
    case 8732:
    case 3108:
    case 9766:
    case 12368:
        return 29;
    case 1980: return 30;
    case 2066:
    case 4150:
    case 11082:
    case 11080:
    case 11078:
        return 31;
    case 2212:
    case 5174:
    case 5004:
    case 5006:
    case 5008:
        return 32;
    case 2218: return 33;
    case 2220: return 34;
    case 2266: return 35;
    case 2386: return 36;
    case 2388: return 37;
    case 2450:
        return 38;
    case 2476:
    case 4208:
    case 12308:
    case 10336:
    case 9804:
        return 39;
    case 4748:
    case 4294:
        return 40;
    case 2512:
    case 9732:
    case 6338:
        return 41;
    case 2572: return 42;
    case 2592:
    case 9396:
    case 2596:
    case 9548:
    case 9812:
        return 43;
    case 2720: return 44;
    case 2752: return 45;
    case 2754: return 46;
    case 2756: return 47;
    case 2802: return 49;
    case 2866: return 50;
    case 2876: return 51;
    case 2878:
    case 2880:
        return 52;
    case 2906:
    case 4170:
    case 4278:
        return 53;
    case 2886: return 54;
    case 2890: return 55;
    case 2910: return 56;
    case 3066: return 57;
    case 3124: return 58;
    case 3168: return 59;
    case 3214:
    case 9194:
        return 60;
    case 7408:
    case 3238:
        return 61;
    case 3274: return 62;
    case 3300: return 64;
    case 3418: return 65;
    case 3476: return 66;
    case 3596: return 67;
    case 3686: return 68;
    case 3716: return 69;
    case 4290: return 71;
    case 4474: return 72;
    case 4464:
    case 9500:
        return 73;
    case 4746: return 75;
    case 4778:
    case 6026: case 7784:
        return 76;
    case 4996:
    case 3680:
    case 5176:
        return 77;
    case 4840: return 78;
    case 5206: return 79;
    case 5480:
    case 9770:
    case 9772:
        return 80;
    case 6110: return 81;
    case 6308: return 82;
    case 6310: return 83;
    case 6298: return 84;
    case 6756: return 85;
    case 7044: return 86;
    case 6892: return 87;
    case 6966: return 88;
    case 7088:
    case 11020:
        return 89;
    case 7098:
    case 9032:
        return 90;
    case 7192: return 91;
    case 7136:
    case 9738:
        return 92;
    case 3166: return 93;
    case 7216: return 94;
    case 7196:
    case 9340:
        return 95;
    case 7392:
    case 9604:
        return 96;
    case 7384: return 98;
    case 7414: return 99;
    case 7402: return 100;
    case 7424: return 101;
    case 7470: return 102;
    case 7488: return 103;
    case 7586:
    case 7646:
    case 9778:
        return 104;
    case 7650: return 105;
    case 6804:
    case 6358:
        return 106;
    case 7568:
    case 7570:
    case 7572:
    case 7574:
        return 107;
    case 7668: return 108;
    case 7660:
    case 9060:
        return 109;
    case 7584:
        return 110;
    case 7736:
    case 9116:
    case 9118:
    case 7826:
    case 7828:
    case 11440:
    case 11442:
    case 11312:
    case 7830:
    case 7832:
    case 10670:
    case 9120:
    case 9122:
    case 10680:
    case 10626:
    case 10578:
    case 10334:
    case 11380:
    case 11326:
    case 7912:
    case 11298:
    case 10498:
    case 12342:
        return 111;
    case 7836:
    case 7838:
    case 7840:
    case 7842:
        return 112;
    case 7950: return 113;
    case 8002: return 114;
    case 8022: return 116;
    case 8036: return 118;
    case 9348:
    case 8372:
        return 119;
    case 8038: return 120;
    case 8816:
    case 8818:
    case 8820:
    case 8822:
        return 128;
    case 8910: return 129;
    case 8942: return 130;
    case 8944:
    case 5276:
        return 131;
    case 8432:
    case 8434:
    case 8436:
    case 8950:
        return 132;
    case 8946: case 9576: return 133;
    case 8960: return 134;
    case 9006: return 135;
    case 9058: return 136;
    case 9082:
    case 9304:
        return 137;
    case 9066:
        return 138;
    case 9136: return 139;
    case 9138:
        return 140;
    case 9172: return 141;
    case 9254: return 143;
    case 9256: return 144;
    case 9236: return 145;
    case 9342: return 146;
    case 9542: return 147;
    case 9378: return 148;
    case 9376: return 149;
    case 9410: return 150;
    case 9462: return 151;
    case 9606:
        return 152;
    case 9716:
    case 5192:
        return 153;
    case 10048: return 167;
    case 10064: return 168;
    case 10046: return 169;
    case 10050: return 170;
    case 10128: return 171;
    case 10210:
    case 9544:
        return 172;
    case 10330: return 178;
    case 10398: return 179;
    case 10388:
    case 9524:
    case 9598:
        return 180;
    case 10442: return 184;
    case 10506: return 185;
    case 10652: return 188;
    case 10676: return 191;
    case 10694: return 193;
    case 10714: return 194;
    case 10724: return 195;
    case 10722: return 196;
    case 10754: return 197;
    case 10800: return 198;
    case 10888: return 199;
    case 10886:
    case 11308:
        return 200;
    case 10890: return 202;
    case 10922: case 9550: return 203;
    case 10990: return 205;
    case 10998: return 206;
    case 10952: return 207;
    case 11000: return 208;
    case 11006: return 209;
    case 11046: return 210;
    case 11052: return 211;
    case 10960: return 212;
    case 10956:
    case 9774:
        return 213;
    case 10958: return 214;
    case 10954: return 215;
    case 11076: return 216;
    case 11084: return 217;
    case 11118:
    case 9546:
    case 9574:
        return 218;
    case 11120: return 219;
    case 11116: return 220;
    case 11158: return 221;
    case 11162: return 222;
    case 11142: return 223;
    case 11232: return 224;
    case 11140: return 225;
    case 11248:
    case 9596:
    case 9636:
        return 226;
    case 11240: return 227;
    case 11250: return 228;
    case 11284: return 229;
    case 11292: return 231;
    case 11314: return 233;
    case 11316: return 234;
    case 11324: return 235;
    case 11354: return 236;
    case 11760:
    case 11464:
    case 11438:
    case 12230:
    case 11716:
    case 11718:
    case 11674:
    case 11630:
    case 11786:
    case 11872:
    case 11762:
    case 11994:
    case 12172:
    case 12184:
    case 11460:
    case 12014:
    case 12016:
    case 12018:
    case 12020:
    case 12022:
    case 12024:
    case 12246:
    case 12248:
    case 12176:
    case 12242:
    case 11622:
    case 12350:
    case 12300:
    case 12374:
    case 12356:
        return 237;
    case 11814:
    case 12232:
    case 12302:
        return 241;
    case 11548:
    case 11552:
        return 242;
    case 11704:
    case 11706:
        return 243;
    case 12180:
    case 12346:
    case 12344:
        return 244;
    case 11506:
    case 11508:
    case 11562:
    case 11768:
    case 11882:
    case 11720:
    case 11884:
        return 245;
    case 12432:
    case 12434:
        return 246;
    case 11818:
    case 11876:
    case 12000:
    case 12240:
    case 12642:
    case 12644:
        return 248;

    }
    return 0;
}

bool custom_drop(int sayi, vector2_t pos, float m_x, float m_y) {

    std::this_thread::sleep_for(std::chrono::milliseconds(1));
    string cdropcount = to_string(sayi);
    int wl;
    int dl;
    int bgl;
    wl = item_count(242);
    dl = item_count(1796) * 100;
    bgl = item_count(7188) * 10000;
    int total = wl + dl + bgl;
    if (total < sayi) {
        gt::log_to_console("`9Dont have `#balance`9. balance: " + to_string(total) + ".");
        return true;
    }
    if (sayi < 100) {

        if (item_count(242) < sayi) {
            gameupdatepacket_t drop{ 0 };
            drop.m_type = PACKET_ITEM_ACTIVATE_REQUEST;
            drop.m_int_data = 1796;
            g_server->send(false, NET_MESSAGE_GAME_PACKET, (uint8_t*)&drop, sizeof(gameupdatepacket_t));
            std::this_thread::sleep_for(std::chrono::milliseconds(5));
        }

        dropwl = true;
        int weel = safe_stoi(cdropcount) % 100;
        int deel = safe_stoi(cdropcount) / 100;
        g_server->send(false, "action|drop\n|itemID|242");
        std::this_thread::sleep_for(std::chrono::milliseconds(15));
        g_server->send(false, "action|dialog_return\ndialog_name|drop_item\nitemID|242|\ncount|" + cdropcount); //242
        gt::log_to_console("`9Dropping `2" + to_string(deel) + " `9Dls & `2" + to_string(weel) + " `9Wls.");
    }

    else if (sayi > 10000) {


        int sayi1 = (sayi / 10000);

        int kalan = ((sayi / 100) - (sayi1 * 100));
        int kalan2 = sayi - ((kalan * 100) + (sayi1 * 10000));
        if (kalan > item_count(1796)) {
            gameupdatepacket_t drop{ 0 };
            drop.m_type = PACKET_ITEM_ACTIVATE_REQUEST;
            drop.m_int_data = 7188;
            g_server->send(false, NET_MESSAGE_GAME_PACKET, (uint8_t*)&drop, sizeof(gameupdatepacket_t));
            std::this_thread::sleep_for(std::chrono::milliseconds(5));
        }
        else if (item_count(242) < kalan2) {
            gameupdatepacket_t drop{ 0 };
            drop.m_type = PACKET_ITEM_ACTIVATE_REQUEST;
            drop.m_int_data = 1796;
            g_server->send(false, NET_MESSAGE_GAME_PACKET, (uint8_t*)&drop, sizeof(gameupdatepacket_t));
            std::this_thread::sleep_for(std::chrono::milliseconds(5));
        }
        dropbgl = true;
        g_server->send(false, "action|drop\n|itemID|7188");
        std::this_thread::sleep_for(std::chrono::milliseconds(5));

        g_server->send(false, "action|dialog_return\ndialog_name|drop_item\nitemID|7188|\ncount|" + std::to_string(sayi1)); //242

        dropdl = true;
        g_server->send(false, "action|drop\n|itemID|1796");
        std::this_thread::sleep_for(std::chrono::milliseconds(5));

        g_server->send(false, "action|dialog_return\ndialog_name|drop_item\nitemID|1796|\ncount|" + std::to_string(kalan)); //242

        dropwl = true;
        int weel = safe_stoi(cdropcount) % 100;
        int deel = safe_stoi(cdropcount) / 100;
        g_server->send(false, "action|drop\n|itemID|242");
        std::this_thread::sleep_for(std::chrono::milliseconds(5));

        g_server->send(false, "action|dialog_return\ndialog_name|drop_item\nitemID|242|\ncount|" + std::to_string(kalan2)); //242

        gt::log_to_console("`9Dropping `2" + to_string(deel) + " `9Dls & `2" + to_string(weel) + " `9Wls.");
    }
    else {
        int sayi1 = (sayi / 100);
        int kalan = (sayi % 100);

        if (item_count(242) < kalan) {
            gameupdatepacket_t drop{ 0 };
            drop.m_type = PACKET_ITEM_ACTIVATE_REQUEST;
            drop.m_int_data = 1796;
            g_server->send(false, NET_MESSAGE_GAME_PACKET, (uint8_t*)&drop, sizeof(gameupdatepacket_t));
            std::this_thread::sleep_for(std::chrono::milliseconds(1));

        }
        else if (item_count(1796) < sayi1) {
            gameupdatepacket_t drop{ 0 };
            drop.m_type = PACKET_ITEM_ACTIVATE_REQUEST;
            drop.m_int_data = 242;
            g_server->send(false, NET_MESSAGE_GAME_PACKET, (uint8_t*)&drop, sizeof(gameupdatepacket_t));
            std::this_thread::sleep_for(std::chrono::milliseconds(1));

        }
        dropdl = true;
        g_server->send(false, "action|drop\n|itemID|1796");
        std::this_thread::sleep_for(std::chrono::milliseconds(1));

        g_server->send(false, "action|dialog_return\ndialog_name|drop_item\nitemID|1796|\ncount|" + std::to_string(sayi1)); //242

        dropwl = true;
        int weel = safe_stoi(cdropcount) % 100;
        int deel = safe_stoi(cdropcount) / 100;
        g_server->send(false, "action|drop\n|itemID|242");
        std::this_thread::sleep_for(std::chrono::milliseconds(1));

        g_server->send(false, "action|dialog_return\ndialog_name|drop_item\nitemID|242|\ncount|" + std::to_string(kalan)); //242

        gt::log_to_console("`9Dropping `2" + to_string(deel) + " `9Dls & `2" + to_string(weel) + " `9Wls.");
    }
    total_bet = 0;
}
std::mutex mtx;
static auto timer = std::chrono::system_clock::now();

#include <string>
#include <algorithm>
#include <cctype>

bool find_command_force_id(std::string chat, std::string name) {
    std::transform(chat.begin(), chat.end(), chat.begin(), ::tolower);
    std::transform(name.begin(), name.end(), name.begin(), ::tolower);

    size_t found = chat.find("" + name);

    if (found != std::string::npos && found == 0) {
        gt::log_to_console("`6" + chat);
        return true;
    }

    return false;
}

bool find_command_force(std::string chat, std::string command) {
    std::transform(chat.begin(), chat.end(), chat.begin(), ::tolower);

    std::string searchCommand = "" + command;
    size_t found = chat.find(searchCommand);

    if (found != std::string::npos && found == 0) {
        if (chat.length() == searchCommand.length() || chat[searchCommand.length()] == ' ') {
            gt::log_to_console("`6" + chat);
            return true;
        }
    }

    return false;
}

void ban_everyone() {
    gt::log_to_console("`9You have `4(Ban All)`9 When Mod joins option `2Enabled");
    gt::log_to_console("`4Banning `9Everyone in the world...");
    std::string username = "all";
    for (auto& player : g_server->m_world.players) {
        auto nik = player.name.substr(2); //remove color
        string banint = player.name.substr(2).substr(0, player.name.length() - 4);
        if (nik.find(username)) {
            g_server->send(false, "action|input\n|text|/ban " + banint);
        }
    }
}

void gosaveworld() {
    gt::log_to_console("`0Warping To `# Save World!");
    g_server->send(false, "action|join_request\nname|" + saveworld + "\ninvitedWorld|0", 3);
}

bool check_number(string num) {
    for (int i = 0; i < num.length(); i++)
        if (isdigit(num[i]) == false)
            return false;
    return true;
}

void anti_gravity() {
    GameUpdatePacket packet{ 0 };
    packet.type = PACKET_TILE_CHANGE_REQUEST;
    packet.item_id = 4992;
    packet.int_x = 99;
    packet.int_y = 59;
    g_server->send(true, NET_MESSAGE_GAME_PACKET, (uint8_t*)&packet, sizeof(GameUpdatePacket));
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));

    auto pos = g_server->m_world.local.pos;

    g_server->enterDoor(true, 99, 59);


    GameUpdatePacket legitpacket{ 0 };
    legitpacket.type = PACKET_STATE;
    legitpacket.item_id = 18;
    legitpacket.int_x = 99;
    legitpacket.int_y = 59;
    legitpacket.vec_x = pos.m_x;
    legitpacket.vec_y = pos.m_y;
    legitpacket.flags = 2592;
    g_server->send(true, NET_MESSAGE_GAME_PACKET, (uint8_t*)&legitpacket, sizeof(GameUpdatePacket));
}

bool events::out::worldoptions(std::string option)
{
    std::string username = "all";
    for (auto& player : g_server->m_world.players) {
        auto name_2 = player.name.substr(2); //remove color
        std::transform(name_2.begin(), name_2.end(), name_2.begin(), ::tolower);
        if (name_2.find(username)) {
            auto& bruh = g_server->m_world.local;
            if (option == "pull")
            {
                string plyr = player.name.substr(2).substr(0, player.name.length() - 4);
                if (plyr != bruh.name.substr(2).substr(0, player.name.length() - 4))
                {
                    g_server->send(false, "action|input\n|text|/pull " + plyr);
                }
            }
            if (option == "kick")
            {
                string plyr = player.name.substr(2).substr(0, player.name.length() - 4);
                if (plyr != bruh.name.substr(2).substr(0, player.name.length() - 4))
                {
                    g_server->send(false, "action|input\n|text|/kick " + plyr);

                }
            }
            if (option == "ban")
            {
                string plyr = player.name.substr(2).substr(0, player.name.length() - 4);
                if (plyr != bruh.name.substr(2).substr(0, player.name.length() - 4))
                {
                    g_server->send(false, "action|input\n|text|/ban " + plyr);

                }
            }
        }
    }

    return true;
}

int get_tax_amount(int total) {
    double tax_yuzde = total / 100.0;
    double total_tax = tax_yuzde * taxamount;

    double get_total = total - total_tax;

    return get_total;
}
bool events::out::generictext(std::string packet) {
    //if (packet == "action|refresh_item_data\n") {
    //    g_server->send(false, "action|enter_game\n", 3);
    //    PRINTS("Skipping Updating Items\n");
    //    return true;
    //}
    if (iswear) send_vset();
    PRINTS("Generic text: %s\n", packet.c_str());
    auto& world = g_server->m_world;
    rtvar var = rtvar::parse(packet);
    if (!var.valid())
        return false;
    /*if (iswear) {
        int skincolor = 1685231359;
        variantlist_t liste{ "OnSetClothing" };
        liste[1] = vector3_t{ hair,  shirt,  pants };
        liste[2] = vector3_t{ shoe,  face,  hand };
        liste[3] = vector3_t{ back,  mask,  neck };
        liste[4] = skincolor;
        liste[5] = vector3_t{ ances , 1.f, 0.f };

        g_server->send(true, liste, g_server->m_world.local.netid, -1);
        g_server->sendState(g_server->m_world.local.netid);
    }*/
    if (packet.find("buttonClicked|wblocks") != -1) {
        auto round = [](double n) {
            return n - floor(n) >= 0.5 ? ceil(n) : floor(n);
            };

        std::map<int, std::map<std::string, int>> store1;

        for (auto tile : g_server->m_world.tile_map.tiles) {
            int foreground_id = tile->foreground;
            int background_id = tile->background;

            if (foreground_id != 0) {
                if (store1[foreground_id].empty()) {
                    store1[foreground_id] = { {"id", foreground_id}, {"qty", 1} };
                }
                else {
                    store1[foreground_id]["qty"] += 1;
                }
            }

            if (background_id != 0) {
                if (store1[background_id].empty()) {
                    store1[background_id] = { {"id", background_id}, {"qty", 1} };
                }
                else {
                    store1[background_id]["qty"] += 1;
                }
            }
        }

        std::string placed_items = "add_spacer|small|";

        for (auto& tile : store1) {
            int id = tile.second["id"];
            if (id == 0) {
                continue;
            }

            int count = round(tile.second["qty"]);
            int idplaced = floor(tile.second["id"]);

            placed_items += "\nadd_label_with_icon|small|`w" + index[idplaced].name + " : " + std::to_string(count) + "``|left|" + std::to_string(idplaced);
        }

        std::string paket =
            "\nadd_label_with_icon|big|World Blocks|left|170|"
            "\n" + placed_items + ""
            "\nadd_quick_exit|"
            "\nend_dialog|growscan_dialog|Cancel|Okay|";

        variantlist_t liste{ "OnDialogRequest" };
        liste[1] = paket;
        g_server->send(true, liste);
        return true;

    }
    if (packet.find("buttonClicked|wfloating") != -1) {
        std::unordered_map<int, int> totalAmounts;
        for (const auto& object : g_server->m_world.object_map.objects) {
            int itemId = object->id;
            int amount = object->amount;
            if (totalAmounts.find(itemId) != totalAmounts.end()) {
                totalAmounts[itemId] += amount;
            }
            else {
                totalAmounts[itemId] = amount;
            }
        }
        std::string purgesexy = "";
        for (const auto& entry : totalAmounts) {
            int itemId = entry.first;
            int totalAmount = entry.second;
            purgesexy += "\nadd_label_with_icon_button|small|`w" + index[itemId].name + " `w: " + std::to_string(totalAmount) + "|left|" + std::to_string(itemId) + "|floating_" + std::to_string(itemId) + "|noflags|0|0|";
        }
        std::string paket;
        paket =
            "\nadd_label_with_icon|big|Floating Items|left|6016|"
            "\n" + purgesexy + ""
            "\nadd_quick_exit|"
            "\nend_dialog|growscan_dialog|Cancel|Okay|";
        variantlist_t liste{ "OnDialogRequest" };
        liste[1] = paket;
        g_server->send(true, liste);
        return true;
    }
    if (packet.find("buttonClicked|showinfo") != -1) {
        Dialog account;
        account.addLabelWithIcon("`9Your Account Info", 262, LABEL_BIG);
        account.addSpacer(SPACER_SMALL);
        if (isim == "")
        {
            account.addTextBox("`3Your GrowID: `w" + gisim);
        }
        else
        {
            account.addTextBox("`3Your GrowID: `w" + isim);
            account.addTextBox("`3Your Password: `w" + sifre);
        }
        //account.addTextBox("`3Your NetID: `w");
        account.addTextBox("`3Your UserID: `w" + uid);
        account.addTextBox("`3Your Mac Address: `w" + macaddress);
        account.addTextBox("`3Your RID: `w" + yourrid);
        account.addTextBox("`3Your KLV: `w" + yourklv);
        account.endDialog("end", "Okay", "Cancel");
        variantlist_t liste{ "OnDialogRequest" };
        liste[1] = account.finishDialog();
        g_server->send(true, liste);

        return true;
    }

    if (packet.find("buttonClicked|ruletlogs") != -1) {
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
        std::string dosyaYolu = "C:\\Users\\Public\\Nexus_Stuff\\Logs\\Nexus_RouletteLogs.txt";
        std::ifstream dosya(dosyaYolu);
        if (!dosya.is_open()) {
            return 1;
        }
        std::string satir;
        Dialog logs;
        logs.addLabelWithIcon("`2Roulete Logs", 758, LABEL_BIG);
        logs.addSmallText("`wHere only `w[`2REAL`w] `wSpins will be Displayed.");
        while (std::getline(dosya, satir)) {
            logs.addSmallText(satir);
        }
        dosya.close();
        logs.addQuickExit();
        logs.endDialog("end", "Okay", "Cancel");
        variantlist_t liste{ "OnDialogRequest" };
        liste[1] = logs.finishDialog();
        g_server->send(true, liste);
        return true;
    }

    if (packet.find("buttonClicked|dclogs") != -1) {
        Dialog droplog;
        droplog.addLabelWithIcon("`2Collect/Drop Logs", 242, LABEL_BIG);
        read_logs_and_add_to_dialog(droplog);
        droplog.endDialog("end", "Okay", "Cancel");
        variantlist_t liste{ "OnDialogRequest" };
        liste[1] = droplog.finishDialog();
        g_server->send(true, liste);

        return true;
    }
    if (packet.find("iso11pul2|") != -1) {
        try {
            std::string aaa = packet.substr(packet.find("ul2|") + 4, packet.size());
            std::string number = aaa.c_str();
            while (!number.empty() && isspace(number[number.size() - 1]))
                number.erase(number.end() - (76 - 0x4B));
            hostermode = safe_stoi(number);
        }
        catch (exception a)
        {
            gt::log_to_console("`4Critical Error: `2override detected");

        }
    }
    if (packet.find("fastdoor|") != -1) {
        try {
            std::string aaa = packet.substr(packet.find("oor|") + 4, packet.size());
            std::string number = aaa.c_str();
            while (!number.empty() && isspace(number[number.size() - 1]))
                number.erase(number.end() - (76 - 0x4B));
            fast_door = safe_stoi(number);
        }
        catch (exception a)
        {
            gt::log_to_console("`4Critical Error: `2override detected");

        }
    }
    if (packet.find("buttonClicked|iso11pul1") != -1) {
        mode = "`5Pull"; //bannj
        hostermode = false;
        return true;
    }
    if (packet.find("buttonClicked|wrenchenable") != -1) {
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
        wrench = true;
        Dialog a;
        a.addLabelWithIcon("`2Wrench Mode", 32, LABEL_BIG);
        if (wrench == false) {
            a.addSpacer(SPACER_SMALL);
            a.addSmallText("`wCurrently Wrench Mode is `4Disabled");
            a.addSpacer(SPACER_SMALL);
        }
        else {
            a.addSpacer(SPACER_SMALL);
        }
        a.addSmallText("`wClick On The Wrench Mode You Want To `2Enable`w: " + mode);
        a.addSpacer(SPACER_SMALL);
        a.addStaticPurpleFrameWithIdText("32", "`4     Ban     ``", "bannj", false);
        a.addStaticPurpleFrameWithIdText("32", "`8     Kick     ``", "kicjlabas", false);
        a.addStaticPurpleFrameWithIdText("32", "`5     Pull     ``", "iso11pul1", false);
        a.addStaticPurpleFrameWithIdText("32", "`^     Trade     ``", "opentrade", false);
        a.addStaticPurpleFrameWithIdText("170", "`4     Disable     ``", "wrenchdisable", false);
        a.addStaticCustomFrameENDLine();
        a.addSmallText("");
        a.addCheckbox("iso11pul2", "`2Enable `wRight Click `8Kick", hostermode);
        a.addCustomMargin2();
        a.addCustomText("`7Grants The Ability To `8Kick `7A Person With Right Mouse Click.");
        a.addCustomMargin1();
        a.endDialog("end", "Okey", "Cancel");
        variantlist_t liste{ "OnDialogRequest" };
        liste[1] = a.finishDialog();
        g_server->send(true, liste);

        return true;
    }
    if (packet.find("buttonClicked|wrenchdisable") != -1) {
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
        wrench = false;
        Dialog a;
        a.addLabelWithIcon("`2Wrench Mode", 32, LABEL_BIG);
        if (wrench == false) {
            a.addSpacer(SPACER_SMALL);
            a.addSmallText("`wCurrently Wrench Mode is `4Disabled");
            a.addSpacer(SPACER_SMALL);
        }
        else {
            a.addSpacer(SPACER_SMALL);
        }
        a.addSmallText("`wClick On The Wrench Mode You Want To `2Enable`w: " + mode);
        a.addSpacer(SPACER_SMALL);
        a.addStaticPurpleFrameWithIdText("32", "`4     Ban     ``", "bannj", false);
        a.addStaticPurpleFrameWithIdText("32", "`8     Kick     ``", "kicjlabas", false);
        a.addStaticPurpleFrameWithIdText("32", "`5     Pull     ``", "iso11pul1", false);
        a.addStaticPurpleFrameWithIdText("32", "`^     Trade     ``", "opentrade", false);
        a.addStaticPurpleFrameWithIdText("176", "`2     Enable     ``", "wrenchenable", false);
        a.addStaticCustomFrameENDLine();
        a.addSmallText("");
        a.addCheckbox("iso11pul2", "`2Enable `wRight Click `8Kick", hostermode);
        a.addCustomMargin2();
        a.addCustomText("`7Grants The Ability To `8Kick `7A Person With Right Mouse Click.");
        a.addCustomMargin1();
        a.endDialog("end", "Okey", "Cancel");
        variantlist_t liste{ "OnDialogRequest" };
        liste[1] = a.finishDialog();
        g_server->send(true, liste);

        return true;
    }
    if (packet.find("iso11pul2|") != -1) {
        try {
            std::string aaa = packet.substr(packet.find("ul2|") + 4, packet.size());
            std::string number = aaa.c_str();
            while (!number.empty() && isspace(number[number.size() - 1]))
                number.erase(number.end() - (76 - 0x4B));
            hostermode = safe_stoi(number);
        }
        catch (exception a)
        {
            gt::log_to_console("`4Critical Error: `2override detected");

        }
    }
    if (packet.find("buttonClicked|bannj") != -1) {
        mode = "`4Ban";
        hostermode = false;
        return true;
    }
    if (packet.find("buttonClicked|opentrade") != -1) {
        mode = "`^Trade";
        hostermode = false;
        return true;
    }
    if (packet.find("buttonClicked|kicjlabas") != -1) {
        mode = "`4Kick";
        hostermode = false;
        return true;
    } 
    if (packet.find("buttonClicked|beachblast") != -1) {
        variantlist_t varlist{ "OnSetCurrentWeather" };
        varlist[1] = 1;
        g_server->send(true, varlist);
        return true;
    }
    if (packet.find("buttonClicked|night") != -1) {
        variantlist_t varlist{ "OnSetCurrentWeather" };
        varlist[1] = 2;
        g_server->send(true, varlist);
        return true;
    }
    if (packet.find("buttonClicked|arid") != -1) {
        variantlist_t varlist{ "OnSetCurrentWeather" };
        varlist[1] = 3;
        g_server->send(true, varlist);
        return true;
    }
    if (packet.find("buttonClicked|Sunny") != -1) {
        variantlist_t varlist{ "OnSetCurrentWeather" };
        varlist[1] = 4;
        g_server->send(true, varlist);
        return true;
    }
    if (packet.find("buttonClicked|RainyCity") != -1) {
        variantlist_t varlist{ "OnSetCurrentWeather" };
        varlist[1] = 5;
        g_server->send(true, varlist);
        gt::log_to_console("`9Weather Change To : `2" + 5);
        return true;
    }
    if (packet.find("buttonClicked|Harvest") != -1) {
        variantlist_t varlist{ "OnSetCurrentWeather" };
        varlist[1] = 6;
        g_server->send(true, varlist);
        return true;
    }
    if (packet.find("buttonClicked|Mars") != -1) {
        variantlist_t varlist{ "OnSetCurrentWeather" };
        varlist[1] = 7;
        g_server->send(true, varlist);
        return true;
    }
    if (packet.find("buttonClicked|Spooky") != -1) {
        variantlist_t varlist{ "OnSetCurrentWeather" };
        varlist[1] = 8;
        g_server->send(true, varlist);
        return true;
    }
    if (packet.find("buttonClicked|Imperial") != -1) {
        variantlist_t varlist{ "OnSetCurrentWeather" };
        varlist[1] = 9;
        g_server->send(true, varlist);
        return true;
    }
    if (packet.find("buttonClicked|Nothingness") != -1) {
        variantlist_t varlist{ "OnSetCurrentWeather" };
        varlist[1] = 10;
        g_server->send(true, varlist);
        return true;
    }
    if (packet.find("buttonClicked|Snowy") != -1) {
        variantlist_t varlist{ "OnSetCurrentWeather" };
        varlist[1] = 11;
        g_server->send(true, varlist);
        return true;
    }
    if (packet.find("buttonClicked|Boat") != -1) {
        variantlist_t varlist{ "OnSetCurrentWeather" };
        varlist[1] = 12;
        g_server->send(true, varlist);
        return true;
    }
    if (packet.find("buttonClicked|Growchimilco") != -1) {
        variantlist_t varlist{ "OnSetCurrentWeather" };
        varlist[1] = 13;
        g_server->send(true, varlist);
        return true;
    }
    if (packet.find("buttonClicked|Undersea") != -1) {
        variantlist_t varlist{ "OnSetCurrentWeather" };
        varlist[1] = 14;
        g_server->send(true, varlist);
        return true;
    }
    if (packet.find("buttonClicked|Warp") != -1) {
        variantlist_t varlist{ "OnSetCurrentWeather" };
        varlist[1] = 15;
        g_server->send(true, varlist);
        return true;
    }
    if (packet.find("buttonClicked|Comet") != -1) {
        variantlist_t varlist{ "OnSetCurrentWeather" };
        varlist[1] = 16;
        g_server->send(true, varlist);
        return true;
    }
    if (packet.find("buttonClicked|Howling") != -1) {
        variantlist_t varlist{ "OnSetCurrentWeather" };
        varlist[1] = 17;
        g_server->send(true, varlist);
        return true;
    }
    if (packet.find("buttonClicked|Party") != -1) {
        variantlist_t varlist{ "OnSetCurrentWeather" };
        varlist[1] = 18;
        g_server->send(true, varlist);
        return true;
    }
    if (packet.find("buttonClicked|Pineapples") != -1) {
        variantlist_t varlist{ "OnSetCurrentWeather" };
        varlist[1] = 19;
        g_server->send(true, varlist);
        return true;
    }
    if (packet.find("buttonClicked|snowynight") != -1) {
        variantlist_t varlist{ "OnSetCurrentWeather" };
        varlist[1] = 20;
        g_server->send(true, varlist);
        return true;
    }
    if (packet.find("buttonClicked|Bountiful") != -1) {
        variantlist_t varlist{ "OnSetCurrentWeather" };
        varlist[1] = 21;
        g_server->send(true, varlist);
        return true;
    }
    if (packet.find("buttonClicked|Howsky") != -1) {
        variantlist_t varlist{ "OnSetCurrentWeather" };
        varlist[1] = 22;
        g_server->send(true, varlist);
        return true;
    }
    if (packet.find("buttonClicked|Suny") != -1) {
        variantlist_t varlist{ "OnSetCurrentWeather" };
        varlist[1] = 23;
        g_server->send(true, varlist);
        return true;
    }
    if (packet.find("buttonClicked|Heatwave") != -1) {
        variantlist_t varlist{ "OnSetCurrentWeather" };
        varlist[1] = 24;
        g_server->send(true, varlist);
        return true;
    }
    if (packet.find("buttonClicked|htwave") != -1) {
        variantlist_t varlist{ "OnSetCurrentWeather" };
        varlist[1] = 25;
        g_server->send(true, varlist);
        return true;
    }
    if (packet.find("buttonClicked|hawave") != -1) {
        variantlist_t varlist{ "OnSetCurrentWeather" };
        varlist[1] = 26;
        g_server->send(true, varlist);
        return true;
    }
    if (packet.find("buttonClicked|hewave") != -1) {
        variantlist_t varlist{ "OnSetCurrentWeather" };
        varlist[1] = 27;
        g_server->send(true, varlist);
        return true;
    }
    if (packet.find("buttonClicked|heawave") != -1) {
        variantlist_t varlist{ "OnSetCurrentWeather" };
        varlist[1] = 28;
        g_server->send(true, varlist);
        return true;
    }
    if (packet.find("buttonClicked|Stuff") != -1) {
        variantlist_t varlist{ "OnSetCurrentWeather" };
        varlist[1] = 29;
        g_server->send(true, varlist);
        return true;
    }
    if (packet.find("buttonClicked|Pagoda") != -1) {
        variantlist_t varlist{ "OnSetCurrentWeather" };
        varlist[1] = 30;
        g_server->send(true, varlist);
        return true;
    }
    if (packet.find("buttonClicked|Apocalypse") != -1) {
        variantlist_t varlist{ "OnSetCurrentWeather" };
        varlist[1] = 31;
        g_server->send(true, varlist);
        return true;
    }
    if (packet.find("buttonClicked|Jungle") != -1) {
        variantlist_t varlist{ "OnSetCurrentWeather" };
        varlist[1] = 32;
        g_server->send(true, varlist);
        return true;
    }
    if (packet.find("buttonClicked|Balloon") != -1) {
        variantlist_t varlist{ "OnSetCurrentWeather" };
        varlist[1] = 33;
        g_server->send(true, varlist);
        return true;
    }
    if (packet.find("buttonClicked|Background") != -1) {
        variantlist_t varlist{ "OnSetCurrentWeather" };
        varlist[1] = 34;
        g_server->send(true, varlist);
        return true;
    }
    if (packet.find("buttonClicked|Autumn") != -1) {
        variantlist_t varlist{ "OnSetCurrentWeather" };
        varlist[1] = 35;
        g_server->send(true, varlist);
        return true;
    }
    if (packet.find("buttonClicked|Valentine") != -1) {
        variantlist_t varlist{ "OnSetCurrentWeather" };
        varlist[1] = 36;
        g_server->send(true, varlist);
        return true;
    }
    if (packet.find("buttonClicked|Paddy") != -1) {
        variantlist_t varlist{ "OnSetCurrentWeather" };
        varlist[1] = 37;
        g_server->send(true, varlist);
        return true;
    }
    if (packet.find("buttonClicked|epochice") != -1) {
        variantlist_t varlist{ "OnSetCurrentWeather" };
        varlist[1] = 38;
        g_server->send(true, varlist);
        return true;
    }
    if (packet.find("buttonClicked|Volcano") != -1) {
        variantlist_t varlist{ "OnSetCurrentWeather" };
        varlist[1] = 39;
        g_server->send(true, varlist);
        return true;
    }
    if (packet.find("buttonClicked|Island") != -1) {
        variantlist_t varlist{ "OnSetCurrentWeather" };
        varlist[1] = 40;
        g_server->send(true, varlist);
        return true;
    }
    if (packet.find("buttonClicked|Snuy") != -1) {
        variantlist_t varlist{ "OnSetCurrentWeather" };
        varlist[1] = 41;
        g_server->send(true, varlist);
        return true;
    }
    if (packet.find("buttonClicked|Digital") != -1) {
        variantlist_t varlist{ "OnSetCurrentWeather" };
        varlist[1] = 42;
        g_server->send(true, varlist);
        return true;
    }
    if (packet.find("buttonClicked|Monochrome") != -1) {
        variantlist_t varlist{ "OnSetCurrentWeather" };
        varlist[1] = 43;
        g_server->send(true, varlist);
        return true;
    }
    if (packet.find("buttonClicked|Frozen") != -1) {
        variantlist_t varlist{ "OnSetCurrentWeather" };
        varlist[1] = 44;
        g_server->send(true, varlist);
        return true;
    }
    if (packet.find("buttonClicked|Surgworld") != -1) {
        variantlist_t varlist{ "OnSetCurrentWeather" };
        varlist[1] = 45;
        g_server->send(true, varlist);
        return true;
    }
    if (packet.find("buttonClicked|botiful") != -1) {
        variantlist_t varlist{ "OnSetCurrentWeather" };
        varlist[1] = 46;
        g_server->send(true, varlist);
        return true;
    }
    if (packet.find("buttonClicked|nayt") != -1) {
        variantlist_t varlist{ "OnSetCurrentWeather" };
        varlist[1] = 47;
        g_server->send(true, varlist);
        return true;
    }
    if (packet.find("buttonClicked|Speed") != -1) {
        variantlist_t varlist{ "OnSetCurrentWeather" };
        varlist[1] = 48;
        g_server->send(true, varlist);
        return true;
    }
    if (packet.find("buttonClicked|Stellarix") != -1) {
        variantlist_t varlist{ "OnSetCurrentWeather" };
        varlist[1] = 49;
        g_server->send(true, varlist);
        return true;
    }
    if (packet.find("buttonClicked|HyperTech") != -1) {
        variantlist_t varlist{ "OnSetCurrentWeather" };
        varlist[1] = 50;
        g_server->send(true, varlist);
        return true;
    }
    if (packet.find("buttonClicked|Celebrity") != -1) {
        variantlist_t varlist{ "OnSetCurrentWeather" };
        varlist[1] = 51;
        g_server->send(true, varlist);
        return true;
    }
    /*if (packet.find("buttonClicked|Howsky") != -1) {
        variantlist_t varlist{ "OnSetCurrentWeather" };
        varlist[1] = 52;
        g_server->send(true, varlist);
        return true;
    }*/
    if (packet.find("buttonClicked|Dragon") != -1) {
        variantlist_t varlist{ "OnSetCurrentWeather" };
        varlist[1] = 53;
        g_server->send(true, varlist);
        return true;
    }
    if (packet.find("buttonClicked|Blood") != -1) {
        variantlist_t varlist{ "OnSetCurrentWeather" };
        varlist[1] = 54;
        g_server->send(true, varlist);
        return true;
    }
    if (packet.find("buttonClicked|Prince") != -1) {
        variantlist_t varlist{ "OnSetCurrentWeather" };
        varlist[1] = 55;
        g_server->send(true, varlist);
        return true;
    }
    if (packet.find("buttonClicked|sany") != -1) {
        variantlist_t varlist{ "OnSetCurrentWeather" };
        varlist[1] = 56;
        g_server->send(true, varlist);
        return true;
    }
    if (packet.find("buttonClicked|soly") != -1) {
        variantlist_t varlist{ "OnSetCurrentWeather" };
        varlist[1] = 57;
        g_server->send(true, varlist);
        return true;
    }
    if (packet.find("buttonClicked|Radical") != -1) {
        variantlist_t varlist{ "OnSetCurrentWeather" };
        varlist[1] = 58;
        g_server->send(true, varlist);
        return true;
    }
    if (packet.find("buttonClicked|Plaza") != -1) {
        variantlist_t varlist{ "OnSetCurrentWeather" };
        varlist[1] = 59;
        g_server->send(true, varlist);
        return true;
    }
    if (packet.find("buttonClicked|Nebula") != -1) {
        variantlist_t varlist{ "OnSetCurrentWeather" };
        varlist[1] = 60;
        g_server->send(true, varlist);
        return true;
    }
    if (packet.find("buttonClicked|Protostar") != -1) {
        variantlist_t varlist{ "OnSetCurrentWeather" };
        varlist[1] = 61;
        g_server->send(true, varlist);
        return true;
    }
    if (packet.find("buttonClicked|Dark") != -1) {
        variantlist_t varlist{ "OnSetCurrentWeather" };
        varlist[1] = 62;
        g_server->send(true, varlist);
        return true;
    }
    if (packet.find("buttonClicked|Assasin") != -1) {
        variantlist_t varlist{ "OnSetCurrentWeather" };
        varlist[1] = 63;
        g_server->send(true, varlist);
        return true;
    }
    if (packet.find("buttonClicked|Growmore") != -1) {
        variantlist_t varlist{ "OnSetCurrentWeather" };
        varlist[1] = 64;
        g_server->send(true, varlist);
        return true;
    }
    if (packet.find("buttonClicked|Reality") != -1) {
        variantlist_t varlist{ "OnSetCurrentWeather" };
        varlist[1] = 65;
        g_server->send(true, varlist);
        return true;
    }
    if (packet.find("buttonClicked|Nian") != -1) {
        variantlist_t varlist{ "OnSetCurrentWeather" };
        varlist[1] = 66;
        g_server->send(true, varlist);
        return true;
    }
    if (packet.find("buttonClicked|Rayman") != -1) {
        variantlist_t varlist{ "OnSetCurrentWeather" };
        varlist[1] = 67;
        g_server->send(true, varlist);
        return true;
    }
    if (packet.find("buttonClicked|Steampunk") != -1) {
        variantlist_t varlist{ "OnSetCurrentWeather" };
        varlist[1] = 68;
        g_server->send(true, varlist);
        return true;
    }
    if (packet.find("buttonClicked|Realm") != -1) {
        variantlist_t varlist{ "OnSetCurrentWeather" };
        varlist[1] = 69;
        g_server->send(true, varlist);
        return true;
    }
    if (packet.find("buttonClicked|Black") != -1) {
        variantlist_t varlist{ "OnSetCurrentWeather" };
        varlist[1] = 70;
        g_server->send(true, varlist);
        return true;
    }
    if (packet.find("buttonClicked|Rainin") != -1) {
        variantlist_t varlist{ "OnSetCurrentWeather" };
        varlist[1] = 71;
        g_server->send(true, varlist);
        return true;
    }
    if (packet.find("buttonClicked|ext") != -1) {
        string itemid = packet.substr(packet.find("ext_") + 4, packet.length() - packet.find("ext_") - 1);
        itemid.erase(itemid.begin() + itemid.find("_x_"), itemid.end());
        string x = packet.substr(packet.find("x_") + 2, packet.length() - packet.find("x_") - 1);
        x.erase(x.begin() + x.find(".000000_y"), x.end());
        string y = packet.substr(packet.find("y_") + 2, packet.length() - packet.find("y_") - 1);
        y.erase(y.begin() + y.find(".000000"), y.end());
        return true;
    }
    if (packet.find("surg_page") != -1) {
        if (packet.find("legitsurg|") != -1) {
            try {
                std::string aaa = packet.substr(packet.find("urg|") + 4, packet.size());
                std::string number = aaa.c_str();
                while (!number.empty() && isspace(number[number.size() - 1]))
                    number.erase(number.end() - (76 - 0x4B));
                legit_surg = safe_stoi(number);
            }
            catch (exception a)
            {
                gt::log_to_console("`4Critical Error: `2override detected");

            }
        }
        if (packet.find("surgauto|") != -1) {
            try {
                std::string aaa = packet.substr(packet.find("uto|") + 4, packet.size());
                std::string number = aaa.c_str();
                while (!number.empty() && isspace(number[number.size() - 1]))
                    number.erase(number.end() - (76 - 0x4B));
                auto_surg = safe_stoi(number);
            }
            catch (exception a)
            {
                gt::log_to_console("`4Critical Error: `2override detected");

            }
        }
    }
    if (packet.find("farm_page") != -1) {
        if (packet.find("autofarm|") != -1) {
            try {
                farmauto = safe_stoi(var.get("autofarm"));
                if (!startedFarm) {
                    startedFarm = true;
                    //std::thread(afarm).detach();
                }
            }
            catch (exception a)
            {
                gt::log_to_console("`4Critical Error: `2override detected");

            }
        }
        if (packet.find("farmstop|") != -1) {
            try {
                std::string aaa = packet.substr(packet.find("top|") + 4, packet.size());
                std::string number = aaa.c_str();
                while (!number.empty() && isspace(number[number.size() - 1]))
                    number.erase(number.end() - (76 - 0x4B));
                stopfarm = safe_stoi(number);
            }
            catch (exception a)
            {
                gt::log_to_console("`4Critical Error: `2override detected");

            }
        }
        if (packet.find("farmid") != -1)
        {
            std::string x = packet.substr(packet.find("mid|") + 4, packet.size());
            farmid = safe_stoi(x.c_str());
            return true;
        }
        if (packet.find("farmhit") != -1)
        {
            std::string x = packet.substr(packet.find("hit|") + 4, packet.size());
            farmhit = safe_stoi(x.c_str());
            return true;
        }
        if (packet.find("farmi_msg|") != -1) {
            std::string msg = packet.substr(packet.find("farmi_msg|") + 10, packet.length() - packet.find("farmi_msg") - 1);
            farmdelay = safe_stoi(msg);
        }
    }
    if (packet.find("clear_page") != -1) {
        if (packet.find("clearauto|") != -1) {
            try {
                clearauto = safe_stoi(var.get("clearauto"));
                if (!startedClear) {
                    startedClear = true;
                    std::thread(aclear).detach();
                }
            }
            catch (exception a)
            {
                gt::log_to_console("`4Critical Error: `2override detected");

            }
        }
        if (packet.find("modfly|") != -1) {
            try {
                std::string aaa = packet.substr(packet.find("fly|") + 4, packet.size());
                std::string number = aaa.c_str();
                while (!number.empty() && isspace(number[number.size() - 1]))
                    number.erase(number.end() - (76 - 0x4B));
                openfly = safe_stoi(number);
            }
            catch (exception a)
            {
                gt::log_to_console("`4Critical Error: `2override detected");

            }
        }
        if (packet.find("annen_msg|") != -1) {
            std::string msg = packet.substr(packet.find("annen_msg|") + 10, packet.length() - packet.find("annen_msg") - 1);
            cleardelay = safe_stoi(msg);
        }
    }
    if (packet.find("harvest_page") != -1) {
        if (packet.find("harvestauto|") != -1) {
            try {
                harvestauto = safe_stoi(var.get("harvestauto"));
                if (!startedHarvest) {
                    startedHarvest = true;
                    std::thread(aharvest).detach();
                }
            }
            catch (exception a)
            {
                gt::log_to_console("`4Critical Error: `2override detected");

            }
        }
        if (packet.find("harvestid") != -1)
        {
            std::string x = packet.substr(packet.find("tid|") + 4, packet.size());
            harvestid = safe_stoi(x.c_str());
            return true;
        }
        if (packet.find("bilme_msg|") != -1) {
            std::string msg = packet.substr(packet.find("bilme_msg|") + 10, packet.length() - packet.find("bilme_msg") - 1);
            harvestdelay = safe_stoi(msg);
        }
    }
    if (packet.find("plant_page") != -1) {
        if (packet.find("autoplant|") != -1) {
            try {
                plantauto = safe_stoi(var.get("autoplant"));
                if (!startedPlant) {
                    startedPlant = true;
                    std::thread(aplant).detach();
                }
            }
            catch (exception a)
            {
                gt::log_to_console("`4Critical Error: `2override detected");

            }
        }
        if (packet.find("takeseedp|") != -1) {
            try {
                std::string aaa = packet.substr(packet.find("edp|") + 4, packet.size());
                std::string number = aaa.c_str();
                while (!number.empty() && isspace(number[number.size() - 1]))
                    number.erase(number.end() - (76 - 0x4B));
                ptakeseed = safe_stoi(number);
            }
            catch (exception a)
            {
                gt::log_to_console("`4Critical Error: `2override detected");

            }
        }
        if (packet.find("plantseed") != -1)
        {
            std::string x = packet.substr(packet.find("eed|") + 4, packet.size());
            plantseed = safe_stoi(x.c_str());
            return true;
        }
        if (packet.find("senin_msg|") != -1) {
            std::string msg = packet.substr(packet.find("senin_msg|") + 10, packet.length() - packet.find("senin_msg") - 1);
            plantdelay = safe_stoi(msg);
        }
    }
    if (packet.find("paint_page") != -1) {
        if (packet.find("paintauto|") != -1) {
            try {
                std::string aaa = packet.substr(packet.find("uto|") + 4, packet.size());
                std::string number = aaa.c_str();
                while (!number.empty() && isspace(number[number.size() - 1]))
                    number.erase(number.end() - (76 - 0x4B));
                paintauto = safe_stoi(number);
            }
            catch (exception a)
            {
                gt::log_to_console("`4Critical Error: `2override detected");

            }
        }
        if (packet.find("paintid") != -1)
        {
            std::string x = packet.substr(packet.find("tid|") + 4, packet.size());
            paintid = safe_stoi(x.c_str());
            return true;
        }
        if (packet.find("benim_msg|") != -1) {
            std::string msg = packet.substr(packet.find("benim_msg|") + 10, packet.length() - packet.find("benim_msg") - 1);
            paintdelay = safe_stoi(msg);
        }
        if (packet.find("paintspecial|") != -1) {
            try {
                std::string aaa = packet.substr(packet.find("ial|") + 4, packet.size());
                std::string number = aaa.c_str();
                while (!number.empty() && isspace(number[number.size() - 1]))
                    number.erase(number.end() - (76 - 0x4B));
                paintspecial = safe_stoi(number);
            }
            catch (exception a)
            {
                gt::log_to_console("`4Critical Error: `2override detected");

            }
        }
        if (packet.find("paintblock") != -1)
        {
            std::string x = packet.substr(packet.find("ock|") + 4, packet.size());
            paintblock = safe_stoi(x.c_str());
            return true;
        }
    }
    if (packet.find("ac_page") != -1) {
        if (packet.find("ac_enable|") != -1) {
            try {
                std::string aaa = packet.substr(packet.find("ble|") + 4, packet.size());
                std::string number = aaa.c_str();
                while (!number.empty() && isspace(number[number.size() - 1]))
                    number.erase(number.end() - (76 - 0x4B));
                gt::autocollect = safe_stoi(number);
            }
            catch (exception a)
            {
                gt::log_to_console("`4Critical Error: `2override detected");

            }
        }
        if (packet.find("ac_coloredmessage|") != -1) {
            try {
                std::string aaa = packet.substr(packet.find("age|") + 4, packet.size());
                std::string number = aaa.c_str();
                while (!number.empty() && isspace(number[number.size() - 1]))
                    number.erase(number.end() - (76 - 0x4B));
                ac_message = safe_stoi(number);
            }
            catch (exception a)
            {
                gt::log_to_console("`4Critical Error: `2override detected");

            }
        }
        if (packet.find("ac_range") != -1) {
            try
            {
                std::string aaa = packet.substr(packet.find("nge|") + 4, packet.size());
                std::string number = aaa.c_str();
                while (!number.empty() && isspace(number[number.size() - 1]))
                    number.erase(number.end() - (76 - 0x4B));
                gt::ac_range = safe_stoi(number);
            }
            catch (exception a)
            {
                gt::log_to_console("`4Critical Error: `2override detected");

            }
        }
        if (packet.find("ac_itemid") != -1) {
            try
            {
                std::string aaa = packet.substr(packet.find("mid|") + 4, packet.size());
                std::string number = aaa.c_str();
                while (!number.empty() && isspace(number[number.size() - 1]))
                    number.erase(number.end() - (76 - 0x4B));
                acitemid = safe_stoi(number);
            }
            catch (exception a)
            {
                gt::log_to_console("`4Critical Error: `2override detected");

            }
        }
        if (packet.find("ac_second") != -1) {
            try
            {
                std::string aaa = packet.substr(packet.find("ond|") + 4, packet.size());
                std::string number = aaa.c_str();
                while (!number.empty() && isspace(number[number.size() - 1]))
                    number.erase(number.end() - (76 - 0x4B));
                acms = safe_stoi(number);
            }
            catch (exception a)
            {
                gt::log_to_console("`4Critical Error: `2override detected");

            }
        }
        return true;
    }
    if (packet.find("buttonClicked|kadaryt") != -1) {
        try {
            Dialog a;
            a.addLabelWithIcon("Auto Mod Detect Settings", 278, LABEL_BIG);
            a.addTextBox("`9When `#@Moderator`9 Joins The World You will:");
            a.addCheckbox("leaveworld", "`2Auto `9Exit World", exitmod);
            a.addCustomMargin2();
            a.addCustomText("`7Automatically Exit World When Moderator Come.");
            a.addCustomMargin1();
            a.addCheckbox("unacclsa", "`2Auto `9Unaccess", unaccmod);
            a.addCustomMargin2();
            a.addCustomText("`7Automatically Unaccess Yourself in The World.");
            a.addCustomMargin1();
            a.addCheckbox("acmodNexus", "`2Auto `9Collect (10 Range)", aacNexus);
            a.addCustomMargin2();
            a.addCustomText("`7Automatically Throw A 10 Ranged Collect.");
            a.addCustomMargin1();
            a.addCheckbox("isjunkvisusnx", "`2Ban `9Everyone In World", banas);
            a.addCustomMargin2();
            a.addCustomText("`7Automatically Ban Everyone In World.");
            a.addCustomMargin1();
            a.addCheckbox("saveworldxd", "`2Auto `9Warp To Save World", saveworldmod);
            a.addCustomMargin2();
            a.addCustomText("`7Automatically Warp To Save World (/setsave).");
            a.addCustomMargin1();
            a.endDialog("mod_settings_spare", "Okey", "Cancel");
            variantlist_t liste{ "OnDialogRequest" };
            liste[1] = a.finishDialog();
            g_server->send(true, liste);
        }
        catch (exception a) {
            gt::log_to_console("`4Critical Error: `2override detected");
        }
        return true;
    }
    if (packet.find("vend_page") != -1) {
        if (packet.find("stockadd|") != -1) {
            try {
                std::string aaa = packet.substr(packet.find("add|") + 4, packet.size());
                std::string number = aaa.c_str();
                while (!number.empty() && isspace(number[number.size() - 1]))
                    number.erase(number.end() - (76 - 0x4B));
                addstock = safe_stoi(number);
            }
            catch (exception a)
            {
                gt::log_to_console("`4Critical Error: `2override detected");

            }
        }
        if (packet.find("stockpull|") != -1) {
            try {
                std::string aaa = packet.substr(packet.find("ull|") + 4, packet.size());
                std::string number = aaa.c_str();
                while (!number.empty() && isspace(number[number.size() - 1]))
                    number.erase(number.end() - (76 - 0x4B));
                pullstock = safe_stoi(number);
            }
            catch (exception a)
            {
                gt::log_to_console("`4Critical Error: `2override detected");

            }
        }
        if (packet.find("fastvend|") != -1) {
            try {
                std::string aaa = packet.substr(packet.find("end|") + 4, packet.size());
                std::string number = aaa.c_str();
                while (!number.empty() && isspace(number[number.size() - 1]))
                    number.erase(number.end() - (76 - 0x4B));
                fastvend = safe_stoi(number);
            }
            catch (exception a)
            {
                gt::log_to_console("`4Critical Error: `2override detected");

            }
        }
        if (packet.find("vend_count|") != -1) {
            std::string msg = packet.substr(packet.find("vend_count|") + 11, packet.length() - packet.find("vend_count") - 1);
            to_string(vendcount) = msg;
        }
    }

    if (packet.find("mod_settings_spare") != -1) {
        if (packet.find("leaveworld|") != -1) {
            try {
                std::string aaa = packet.substr(packet.find("rld|") + 4, packet.size());
                std::string number = aaa.c_str();
                while (!number.empty() && isspace(number[number.size() - 1]))
                    number.erase(number.end() - (76 - 0x4B));
                exitmod = safe_stoi(number);
            }
            catch (exception a)
            {
                gt::log_to_console("`4Critical Error: `2override detected");

            }
        }
        if (packet.find("unacclsa|") != -1) {
            try {
                std::string aaa = packet.substr(packet.find("lsa|") + 4, packet.size());
                std::string number = aaa.c_str();
                while (!number.empty() && isspace(number[number.size() - 1]))
                    number.erase(number.end() - (76 - 0x4B));
                unaccmod = safe_stoi(number);
            }
            catch (exception a)
            {
                gt::log_to_console("`4Critical Error: `2override detected");

            }
        }
        if (packet.find("acmodNexus|") != -1) {
            try {
                std::string aaa = packet.substr(packet.find("ove|") + 4, packet.size());
                std::string number = aaa.c_str();
                while (!number.empty() && isspace(number[number.size() - 1]))
                    number.erase(number.end() - (76 - 0x4B));
                aacNexus = safe_stoi(number);
            }
            catch (exception a)
            {
                gt::log_to_console("`4Critical Error: `2override detected");

            }
        }
        if (packet.find("saveworldxd|") != -1) {
            try {
                std::string aaa = packet.substr(packet.find("dxd|") + 4, packet.size());
                std::string number = aaa.c_str();
                while (!number.empty() && isspace(number[number.size() - 1]))
                    number.erase(number.end() - (76 - 0x4B));
                saveworldmod = safe_stoi(number);
            }
            catch (exception a)
            {
                gt::log_to_console("`4Critical Error: `2override detected");

            }
        }
        if (packet.find("isjunkvisusnx|") != -1) {
            try {
                std::string aaa = packet.substr(packet.find("snx|") + 4, packet.size());
                std::string number = aaa.c_str();
                while (!number.empty() && isspace(number[number.size() - 1]))
                    number.erase(number.end() - (76 - 0x4B));
                banas = safe_stoi(number);
            }
            catch (exception a)
            {
                std::cout << "ERROR: " << a.what() << '\n';
                gt::log_to_console("`4Critical Error: `2override detected");

            }
        }
        return true;
    }

    if (packet.find("autoacc|") != -1) {
        std::string aaa = packet.substr(packet.find("acc|") + 4, packet.size());
        std::string number = aaa.c_str();
        autoacc = safe_stoi(number);
    }
    if (packet.find("speed_page") != -1) {
        try {
            if (packet.find("ed_x|") != -1) {
                std::string aaa = packet.substr(packet.find("d_x|") + 4, packet.size());
                std::string number = aaa.c_str();
                // cout << number << endl;
                while (!number.empty() && isspace(number[number.size() - 1]))
                    number.erase(number.end() - (76 - 0x4B));
                speed_x = safe_stof(number);
                g_server->sendState(world.local.netid, false);
            }
            if (packet.find("ed_y|") != -1) {
                std::string aaa = packet.substr(packet.find("d_y|") + 4, packet.size());
                std::string number = aaa.c_str();
                //cout << number << endl;
                while (!number.empty() && isspace(number[number.size() - 1]))
                    number.erase(number.end() - (76 - 0x4B));
                speed_y = safe_stof(number);
                g_server->sendState(world.local.netid, false);
            }

        }
        catch (exception a) {
            std::cout << "error?";
            std::cout << a.what();
        }
        return true;
    }
    if (packet.find("changecntr") != -1) {
        g_server->disconnectsr(true);
        if (packet.find("buttonClicked|showallcntr") != -1)
        {
            std::string paket;
            paket =
                "\nadd_label_with_icon|big|`2All Country IDs|left|3394|"
                "\nadd_spacer|small"

                "\nadd_textbox|`1af: `2Afghanistan|left|2480|"
                "\nadd_textbox|`1al: `2Albania|left|2480|"
                "\nadd_textbox|`1dz: `2Algeria|left|2480|"
                "\nadd_textbox|`1as: `2American Samoa|left|2480|"
                "\nadd_textbox|`1ad: `2Andorra|left|2480|"
                "\nadd_textbox|`1ao: `2Angola|left|2480|"
                "\nadd_textbox|`1ai: `2Anguilla|left|2480|"
                "\nadd_textbox|`1aq: `2Antarctica|left|2480|"
                "\nadd_textbox|`1ag: `2Antigua and Barbuda|left|2480|"
                "\nadd_textbox|`1ar: `2Argentina|left|2480|"
                "\nadd_textbox|`1am: `2Armenia|left|2480|"
                "\nadd_textbox|`1aw: `2Aruba|left|2480|"
                "\nadd_textbox|`1au: `2Australia|left|2480|"
                "\nadd_textbox|`1at: `2Austria|left|2480|"
                "\nadd_textbox|`1az: `2Azerbaijan|left|2480|"
                "\nadd_textbox|`1bs: `2Bahamas|left|2480|"
                "\nadd_textbox|`1bh: `2Bahrain|left|2480|"
                "\nadd_textbox|`1bd: `2Bangladesh|left|2480|"
                "\nadd_textbox|`1bb: `2Barbados|left|2480|"
                "\nadd_textbox|`1by: `2Belarus|left|2480|"
                "\nadd_textbox|`1be: `2Belgium|left|2480|"
                "\nadd_textbox|`1bz: `2Belize|left|2480|"
                "\nadd_textbox|`1bj: `2Benin|left|2480|"
                "\nadd_textbox|`1bm: `2Bermuda|left|2480|"
                "\nadd_textbox|`1bt: `2Bhutan|left|2480|"
                "\nadd_textbox|`1bo: `2Bolivia|left|2480|"
                "\nadd_textbox|`1ba: `2Bosnia and Herzegovina|left|2480|"
                "\nadd_textbox|`1bw: `2Botswana|left|2480|"
                "\nadd_textbox|`1br: `2Brazil|left|2480|"
                "\nadd_textbox|`1io: `2British Indian Ocean Territory|left|2480|"
                "\nadd_textbox|`1bn: `2Brunei|left|2480|"
                "\nadd_textbox|`1bg: `2Bulgaria|left|2480|"
                "\nadd_textbox|`1bf: `2Burkina Faso|left|2480|"
                "\nadd_textbox|`1bi: `2Burundi|left|2480|"
                "\nadd_textbox|`1kh: `2Cambodia|left|2480|"
                "\nadd_textbox|`1cm: `2Cameroon|left|2480|"
                "\nadd_textbox|`1ca: `2Canada|left|2480|"
                "\nadd_textbox|`1cv: `2Cape Verde|left|2480|"
                "\nadd_textbox|`1ky: `2Cayman Islands|left|2480|"
                "\nadd_textbox|`1cf: `2Central African Republic|left|2480|"
                "\nadd_textbox|`1td: `2Chad|left|2480|"
                "\nadd_textbox|`1cl: `2Chile|left|2480|"
                "\nadd_textbox|`1cn: `2China|left|2480|"
                "\nadd_textbox|`1cx: `2Christmas Island|left|2480|"
                "\nadd_textbox|`1cc: `2Cocos (Keeling)Islands|left|2480|"
                "\nadd_textbox|`1co: `2Colombia|left|2480|"
                "\nadd_textbox|`1km: `2Comoros|left|2480|"
                "\nadd_textbox|`1cg: `2Republic of the Congo|left|2480|"
                "\nadd_textbox|`1cd: `2Democratic Republic of the Congo|left|2480|"
                "\nadd_textbox|`1ck: `2Cook Islands|left|2480|"
                "\nadd_textbox|`1cr: `2Costa Rica|left|2480|"
                "\nadd_textbox|`1ci: `2C¶te dIvoire|left|2480|"
                "\nadd_textbox|`1hr: `2Croatia|left|2480|"
                "\nadd_textbox|`1cu: `2Cuba|left|2480|"
                "\nadd_textbox|`1cw: `2Curaao|left|2480|"
                "\nadd_textbox|`1cy: `2Cyprus|left|2480|"
                "\nadd_textbox|`1cz: `2Czech Republic|left|2480|"
                "\nadd_textbox|`1dk: `2Denmark|left|2480|"
                "\nadd_textbox|`1dj: `2Djibouti|left|2480|"
                "\nadd_textbox|`1dm: `2Dominica|left|2480|"
                "\nadd_textbox|`1do: `2Dominican Republic|left|2480|"
                "\nadd_textbox|`1ec: `2Ecuador|left|2480|"
                "\nadd_textbox|`1eg: `2Egypt|left|2480|"
                "\nadd_textbox|`1sv: `2El Salvador|left|2480|"
                "\nadd_textbox|`1gq: `2Equatorial Guinea|left|2480|"
                "\nadd_textbox|`1er: `2Eritrea|left|2480|"
                "\nadd_textbox|`1ee: `2Estonia|left|2480|"
                "\nadd_textbox|`1et: `2Ethiopia|left|2480|"
                "\nadd_textbox|`1fk: `2Falkland Islands|left|2480|"
                "\nadd_textbox|`1fo: `2Faroe Islands|left|2480|"
                "\nadd_textbox|`1fj: `2Fiji|left|2480|"
                "\nadd_textbox|`1fi: `2Finland|left|2480|"
                "\nadd_textbox|`1fr: `2France|left|2480|"
                "\nadd_textbox|`1pf: `2French Polynesia|left|2480|"
                "\nadd_textbox|`1ga: `2Gabon|left|2480|"
                "\nadd_textbox|`1gm: `2Gambia|left|2480|"
                "\nadd_textbox|`1ge: `2Georgia|left|2480|"
                "\nadd_textbox|`1de: `2Germany|left|2480|"
                "\nadd_textbox|`1gh: `2Ghana|left|2480|"
                "\nadd_textbox|`1gi: `2Gibraltar|left|2480|"
                "\nadd_textbox|`1gr: `2Greece|left|2480|"
                "\nadd_textbox|`1gl: `2Greenland|left|2480|"
                "\nadd_textbox|`1gd: `2Grenada|left|2480|"
                "\nadd_textbox|`1gu: `2Guam|left|2480|"
                "\nadd_textbox|`1gt: `2Guatemala|left|2480|"
                "\nadd_textbox|`1gg: `2Guernsey|left|2480|"
                "\nadd_textbox|`1gn: `2Guinea|left|2480|"
                "\nadd_textbox|`1gw: `2Guinea-Bissau|left|2480|"
                "\nadd_textbox|`1gy: `2Guyana|left|2480|"
                "\nadd_textbox|`1ht: `2Haiti|left|2480|"
                "\nadd_textbox|`1hn: `2Honduras|left|2480|"
                "\nadd_textbox|`1hk: `2Hong Kong|left|2480|"
                "\nadd_textbox|`1hu: `2Hungary|left|2480|"
                "\nadd_textbox|`1is: `2Iceland|left|2480|"
                "\nadd_textbox|`1in: `2India|left|2480|"
                "\nadd_textbox|`1id: `2Indonesia|left|2480|"
                "\nadd_textbox|`1ir: `2Iran|left|2480|"
                "\nadd_textbox|`1iq: `2Iraq|left|2480|"
                "\nadd_textbox|`1ie: `2Ireland|left|2480|"
                "\nadd_textbox|`1im: `2Isle of Man|left|2480|"
                "\nadd_textbox|`1il: `2Israel|left|2480|"
                "\nadd_textbox|`1it: `2Italy|left|2480|"
                "\nadd_textbox|`1jm: `2Jamaica|left|2480|"
                "\nadd_textbox|`1jp: `2Japan|left|2480|"
                "\nadd_textbox|`1je: `2Jersey|left|2480|"
                "\nadd_textbox|`1jo: `2Jordan|left|2480|"
                "\nadd_textbox|`1kz: `2Kazakhstan|left|2480|"
                "\nadd_textbox|`1ke: `2Kenya|left|2480|"
                "\nadd_textbox|`1ki: `2Kiribati|left|2480|"
                "\nadd_textbox|`1kp: `2North Korea|left|2480|"
                "\nadd_textbox|`1kr: `2South Korea|left|2480|"
                "\nadd_textbox|`1kw: `2Kuwait|left|2480|"
                "\nadd_textbox|`1kg: `2Kyrgyzstan|left|2480|"
                "\nadd_textbox|`1la: `2Laos|left|2480|"
                "\nadd_textbox|`1lv: `2Latvia|left|2480|"
                "\nadd_textbox|`1lb: `2Lebanon|left|2480|"
                "\nadd_textbox|`1ls: `2Lesotho|left|2480|"
                "\nadd_textbox|`1lr: `2Liberia|left|2480|"
                "\nadd_textbox|`1ly: `2Libya|left|2480|"
                "\nadd_textbox|`1li: `2Liechtenstein|left|2480|"
                "\nadd_textbox|`1lt: `2Lithuania|left|2480|"
                "\nadd_textbox|`1lu: `2Luxembourg|left|2480|"
                "\nadd_textbox|`1mo: `2Macau|left|2480|"
                "\nadd_textbox|`1mk: `2North Macedonia|left|2480|"
                "\nadd_textbox|`1mg: `2Madagascar|left|2480|"
                "\nadd_textbox|`1mw: `2Malawi|left|2480|"
                "\nadd_textbox|`1my: `2Malaysia|left|2480|"
                "\nadd_textbox|`1mv: `2Maldives|left|2480|"
                "\nadd_textbox|`1ml: `2Mali|left|2480|"
                "\nadd_textbox|`1mt: `2Malta|left|2480|"
                "\nadd_textbox|`1mh: `2Marshall Islands|left|2480|"
                "\nadd_textbox|`1mr: `2Mauritania|left|2480|"
                "\nadd_textbox|`1mu: `2Mauritius|left|2480|"
                "\nadd_textbox|`1yt: `2Mayotte|left|2480|"
                "\nadd_textbox|`1mx: `2Mexico|left|2480|"
                "\nadd_textbox|`1fm: `2Micronesia|left|2480|"
                "\nadd_textbox|`1md: `2Moldova|left|2480|"
                "\nadd_textbox|`1mc: `2Monaco|left|2480|"
                "\nadd_textbox|`1mn: `2Mongolia|left|2480|"
                "\nadd_textbox|`1me: `2Montenegro|left|2480|"
                "\nadd_textbox|`1ms: `2Montserrat|left|2480|"
                "\nadd_textbox|`1ma: `2Morocco|left|2480|"
                "\nadd_textbox|`1mz: `2Mozambique|left|2480|"
                "\nadd_textbox|`1mm: `2Myanmar|left|2480|"
                    "\nadd_textbox|`1na: `2Namibia|left|2480|"
                    "\nadd_textbox|`1nr: `2Nauru|left|2480|"
                    "\nadd_textbox|`1np: `2Nepal|left|2480|"
                    "\nadd_textbox|`1nl: `2Netherlands|left|2480|"
                    "\nadd_textbox|`1nc: `2New Caledonia|left|2480|"
                    "\nadd_textbox|`1nz: `2New Zealand|left|2480|"
                    "\nadd_textbox|`1ni: `2Nicaragua|left|2480|"
                    "\nadd_textbox|`1ne: `2Niger|left|2480|"
                    "\nadd_textbox|`1ng: `2Nigeria|left|2480|"
                    "\nadd_textbox|`1ng: `2Nigeria|left|2480|"
                    "\nadd_textbox|`1nu: `2Niue|left|2480|"
                    "\nadd_textbox|`1nf: `2Norfolk Island|left|2480|"
                    "\nadd_textbox|`1mp: `2Northern Mariana Islands|left|2480|"
                    "\nadd_textbox|`1no: `2Norway|left|2480|"
                    "\nadd_textbox|`1om: `2Oman|left|2480|"
                    "\nadd_textbox|`1pk: `2Pakistan|left|2480|"
                    "\nadd_textbox|`1pw: `2Palau|left|2480|"
                    "\nadd_textbox|`1ps: `2Palestine|left|2480|"
                    "\nadd_textbox|`1pa: `2Panama|left|2480|"
                    "\nadd_textbox|`1pg: `2Papua New Guinea|left|2480|"
                    "\nadd_textbox|`1py: `2Paraguay|left|2480|"
                    "\nadd_textbox|`1pe: `2Peru|left|2480|"
                    "\nadd_textbox|`1ph: `2Philippines|left|2480|"
                    "\nadd_textbox|`1pn: `2Pitcairn Islands|left|2480|"
                    "\nadd_textbox|`1pl: `2Poland|left|2480|"
                    "\nadd_textbox|`1pt: `2Portugal|left|2480|"
                    "\nadd_textbox|`1pr: `2Puerto Rico|left|2480|"
                    "\nadd_textbox|`1qa: `2Qatar|left|2480|"
                    "\nadd_textbox|`1re: `2RÚunion|left|2480|"
                    "\nadd_textbox|`1ro: `2Romania|left|2480|"
                    "\nadd_textbox|`1ru: `2Russia|left|2480|"
                    "\nadd_textbox|`1rw: `2Rwanda|left|2480|"
                    "\nadd_textbox|`1bl: `2Saint BarthÚlemy|left|2480|"
                    "\nadd_textbox|`1sh: `2Saint Helena|left|2480|"
                    "\nadd_textbox|`1kn: `2Saint Kitts and Nevis|left|2480|"
                    "\nadd_textbox|`1lc: `2Saint Lucia|left|2480|"
                    "\nadd_textbox|`1mf: `2Saint Martin|left|2480|"
                    "\nadd_textbox|`1pm: `2Saint Pierre and Miquelon|left|2480|"
                    "\nadd_textbox|`1vc: `2Saint Vincent and the Grenadines|left|2480|"
                    "\nadd_textbox|`1ws: `2Samoa|left|2480|"
                    "\nadd_textbox|`1sm: `2San Marino|left|2480|"
                    "\nadd_textbox|`1st: `2SÒo TomÚ and Prÿncipe|left|2480|"
                    "\nadd_textbox|`1sa: `2Saudi Arabia|left|2480|"
                    "\nadd_textbox|`1sn: `2Senegal|left|2480|"
                    "\nadd_textbox|`1rs: `2Serbia|left|2480|"
                    "\nadd_textbox|`1sc: `2Seychelles|left|2480|"
                    "\nadd_textbox|`1sl: `2Sierra Leone|left|2480|"
                    "\nadd_textbox|`1sg: `2Singapore|left|2480|"
                    "\nadd_textbox|`1sx: `2Sint Maarten|left|2480|"
                    "\nadd_textbox|`1sk: `2Slovakia|left|2480|"
                    "\nadd_textbox|`1si: `2Slovenia|left|2480|"
                    "\nadd_textbox|`1sb: `2Solomon Islands|left|2480|"
                    "\nadd_textbox|`1so: `2Somalia|left|2480|"
                    "\nadd_textbox|`1za: `2South Africa|left|2480|"
                    "\nadd_textbox|`1gs: `2South Georgia and the South Sandwich Islands|left|2480|"
                    "\nadd_textbox|`1ss: `2South Sudan|left|2480|"
                    "\nadd_textbox|`1es: `2Spain|left|2480|"
                    "\nadd_textbox|`1lk: `2Sri Lanka|left|2480|"
                    "\nadd_textbox|`1sd: `2Sudan|left|2480|"
                    "\nadd_textbox|`1sr: `2Suriname|left|2480|"
                    "\nadd_textbox|`1sj: `2Svalbard and Jan Mayen|left|2480|"
                    "\nadd_textbox|`1sz: `2Eswatini|left|2480|"
                    "\nadd_textbox|`1se: `2Sweden|left|2480|"
                    "\nadd_textbox|`1ch: `2Switzerland|left|2480|"
                    "\nadd_textbox|`1sy: `2Syria|left|2480|"
                    "\nadd_textbox|`1tw: `2Taiwan|left|2480|"
                    "\nadd_textbox|`1tj: `2Tajikistan|left|2480|"
                    "\nadd_textbox|`1tz: `2Tanzania|left|2480|"
                    "\nadd_textbox|`1th: `2Thailand|left|2480|"
                    "\nadd_textbox|`1tl: `2Timor-Leste|left|2480|"
                    "\nadd_textbox|`1tg: `2Togo|left|2480|"
                    "\nadd_textbox|`1tk: `2Tokelau|left|2480|"
                    "\nadd_textbox|`1to: `2Tonga|left|2480|"
                    "\nadd_textbox|`1tt: `2Trinidad and Tobago|left|2480|"
                    "\nadd_textbox|`1tn: `2Tunisia|left|2480|"
                    "\nadd_textbox|`1tr: `2Turkey|left|2480|"
                    "\nadd_textbox|`1tm: `2Turkmenistan|left|2480|"
                    "\nadd_textbox|`1tc: `2Turks and Caicos Islands|left|2480|"
                    "\nadd_textbox|`1tv: `2Tuvalu|left|2480|"
                    "\nadd_textbox|`1ug: `2Uganda|left|2480|"
                    "\nadd_textbox|`1ua: `2Ukraine|left|2480|"
                    "\nadd_textbox|`1ae: `2United Arab Emirates|left|2480|"
                    "\nadd_textbox|`1gb: `2United Kingdom|left|2480|"
                    "\nadd_textbox|`1us: `2United States|left|2480|"
                    "\nadd_textbox|`1um: `2United States Minor Outlying Islands|left|2480|"
                    "\nadd_textbox|`1uy: `2Uruguay|left|2480|"
                    "\nadd_textbox|`1uz: `2Uzbekistan|left|2480|"
                    "\nadd_textbox|`1vu: `2Vanuatu|left|2480|"
                    "\nadd_textbox|`1va: `2Vatican City|left|2480|"
                    "\nadd_textbox|`1ve: `2Venezuela|left|2480|"
                    "\nadd_textbox|`1vn: `2Vietnam|left|2480|"
                    "\nadd_textbox|`1vg: `2British Virgin Islands|left|2480|"
                    "\nadd_textbox|`1vi: `2United States Virgin Islands|left|2480|"
                    "\nadd_textbox|`1wf: `2Wallis and Futuna|left|2480|"
                    "\nadd_textbox|`1eh: `2Western Sahara|left|2480|"
                    "\nadd_textbox|`1ye: `2Yemen|left|2480|"
                    "\nadd_textbox|`1zm: `2Zambia|left|2480|"
                    "\nadd_textbox|`1zw: `2Zimbabwe|left|2480|"
                    "\nadd_quick_exit|"
                    "\nend_dialog|cntrids|Cancel||";
            variantlist_t liste{ "OnDialogRequest" };
            liste[1] = paket;
            g_server->send(true, liste);
            return true;
        }
        if (packet.find("flag_nam|") != -1) {
            std::string msg = packet.substr(packet.find("flag_nam|") + 9, packet.length() - packet.find("flag_nam") - 1);
            gt::flag = msg;
        }
    }
    if (packet.find("auto_dialog") != -1) {
        try {
            if (packet.find("autopuli") != -1) {
                std::string filePath = "C:\\Users\\Public\\Nexus_Stuff\\autopull.txt";
                std::string fileContents = readFileContents(filePath);
                std::istringstream fileStream(fileContents);
                std::string line;
                std::vector<std::string> lines;
                while (std::getline(fileStream, line)) {
                    lines.push_back(line);
                }

                Dialog autos;
                autos.addLabelWithIcon("Auto Pull Options", 2246, LABEL_BIG);
                autos.addSmallText("`9Name has to be fully written, for autopull to work.");
                autos.addSmallText("`9Name has to be at least 3 letters long & without spaces");
                autos.addCheckbox("pull_enable", "`2Enable `#Auto Pull", specficpull);
                autos.addSpacer(SPACER_SMALL);

                for (const auto& aline : lines) {
                    autos.addLabelWithIconButton("`w" + aline, 2246, "deletepull");
                }

                autos.addInputBox("pull_nam", "`5Name: ", "", 18);
                autos.addButton("autopullknk", "`2Add To List");
                autos.addQuickExit();
                autos.endDialog("auto_dialog", "Okay", "Cancel");
                variantlist_t packet{ "OnDialogRequest" };
                packet[1] = autos.finishDialog();
                g_server->send(true, packet);
                return true;
            }

            if (packet.find("pull_nam|") != -1) {
                std::string msg = packet.substr(packet.find("pull_nam|") + 9, packet.length() - packet.find("pull_nam") - 1);
                pname1 = msg;
            }

            if (packet.find("buttonClicked|autopullknk") != std::string::npos) {
                std::ofstream outFile("C:\\Users\\Public\\Nexus_Stuff\\autopull.txt", std::ios_base::app);
                if (outFile.is_open()) {
                    outFile << pname1; // std::endl yerine sadece pname1 yazıyoruz.
                    outFile.close();
                }

                std::string filePath = "C:\\Users\\Public\\Nexus_Stuff\\autopull.txt";
                std::string fileContents = readFileContents(filePath);
                std::istringstream fileStream(fileContents);
                std::string line;
                std::vector<std::string> lines;
                while (std::getline(fileStream, line)) {
                    lines.push_back(line);
                }

                Dialog autos;
                autos.addLabelWithIcon("Auto Pull Options", 2246, LABEL_BIG);
                autos.addSmallText("`9Name has to be fully written, for autopull to work.");
                autos.addSmallText("`9Name has to be at least 3 letters long & without spaces");
                autos.addCheckbox("pull_enable", "`2Enable `#Auto Pull", specficpull);
                autos.addSpacer(SPACER_SMALL);

                for (const auto& aline : lines) {
                    autos.addLabelWithIconButton("`w" + aline, 2246, "deletepull");
                }

                autos.addInputBox("pull_nam", "`5Name: ", "", 18);
                autos.addButton("autopullknk", "`2Add To List");
                autos.addQuickExit();
                autos.endDialog("auto_dialog", "Okay", "Cancel");
                variantlist_t packet{ "OnDialogRequest" };
                packet[1] = autos.finishDialog();
                g_server->send(true, packet);
                return true;
            }





            if (packet.find("autobani") != -1) {
                std::string filePath = "C:\\Users\\Public\\Nexus_Stuff\\autoban.txt";
                std::string fileContents = readFileContents(filePath);
                std::istringstream fileStream(fileContents);
                std::string line;
                std::vector<std::string> lines;
                while (std::getline(fileStream, line)) {
                    lines.push_back(line);
                }

                Dialog autos;
                autos.addLabelWithIcon("Auto Ban Options", 2246, LABEL_BIG);
                autos.addSmallText("`9Name has to be fully written, for autoban to work.");
                autos.addSmallText("`9Name has to be at least 3 letters long & without spaces");
                autos.addCheckbox("enable_ban", "`2Enable `#Auto Ban", specficban);
                autos.addSpacer(SPACER_SMALL);

                for (const auto& aline : lines) {
                    autos.addLabelWithIconButton("`w" + aline, 2246, "deleteban");
                }

                autos.addInputBox("ban_nam", "`5Name: ", "", 18);
                autos.addButton("autobanknk", "`2Add To List");
                autos.addQuickExit();
                autos.endDialog("auto_dialog", "Okay", "Cancel");
                variantlist_t packet{ "OnDialogRequest" };
                packet[1] = autos.finishDialog();
                g_server->send(true, packet);
                return true;
            }

            if (packet.find("ban_nam|") != -1) {
                std::string msg = packet.substr(packet.find("ban_nam|") + 8, packet.length() - packet.find("ban_nam") - 1);
                bname1 = msg;
            }

            if (packet.find("buttonClicked|autobanknk") != std::string::npos) {
                std::ofstream outFile("C:\\Users\\Public\\Nexus_Stuff\\autoban.txt", std::ios_base::app);
                if (outFile.is_open()) {
                    outFile << bname1;
                    outFile.close();
                }

                std::string filePath = "C:\\Users\\Public\\Nexus_Stuff\\autoban.txt";
                std::string fileContents = readFileContents(filePath);
                std::istringstream fileStream(fileContents);
                std::string line;
                std::vector<std::string> lines;
                while (std::getline(fileStream, line)) {
                    lines.push_back(line);
                }

                Dialog autos;
                autos.addLabelWithIcon("Auto Ban Options", 2246, LABEL_BIG);
                autos.addSmallText("`9Name has to be fully written, for autoban to work.");
                autos.addSmallText("`9Name has to be at least 3 letters long & without spaces");
                autos.addCheckbox("enable_ban", "`2Enable `#Auto Ban", specficban);
                autos.addSpacer(SPACER_SMALL);

                for (const auto& aline : lines) {
                    autos.addLabelWithIconButton("`w" + aline, 2246, "deleteban");
                }

                autos.addInputBox("ban_nam", "`5Name: ", "", 18);
                autos.addButton("autobanknk", "`2Add To List");
                autos.addQuickExit();
                autos.endDialog("auto_dialog", "Okay", "Cancel");
                variantlist_t packet{ "OnDialogRequest" };
                packet[1] = autos.finishDialog();
                g_server->send(true, packet);
                return true;
            }




            if (packet.find("pull_enable|") != -1) {
                try {
                    std::string aaa = packet.substr(packet.find("ble|") + 4, packet.size());
                    std::string number = aaa.c_str();
                    while (!number.empty() && isspace(number[number.size() - 1]))
                        number.erase(number.end() - (76 - 0x4B));
                    specficpull = safe_stoi(number);
                }
                catch (exception a)
                {
                    gt::log_to_console("`4Critical Error: `2override detected");

                }
            }
            if (packet.find("enable_ban|") != -1) {
                try {
                    std::string aaa = packet.substr(packet.find("ban|") + 4, packet.size());
                    std::string number = aaa.c_str();
                    while (!number.empty() && isspace(number[number.size() - 1]))
                        number.erase(number.end() - (76 - 0x4B));
                    specficban = safe_stoi(number);
                }
                catch (exception a)
                {
                    gt::log_to_console("`4Critical Error: `2override detected");

                }
            }
            if (packet.find("enable_autop|") != -1) {
                try {
                    std::string aaa = packet.substr(packet.find("top|") + 4, packet.size());
                    std::string number = aaa.c_str();
                    while (!number.empty() && isspace(number[number.size() - 1]))
                        number.erase(number.end() - (76 - 0x4B));
                    enter_pull = safe_stoi(number);
                }
                catch (exception a)
                {
                    gt::log_to_console("`4Critical Error: `2override detected");

                }
            }
            if (packet.find("enable_autob|") != -1) {
                try {
                    std::string aaa = packet.substr(packet.find("tob|") + 4, packet.size());
                    std::string number = aaa.c_str();
                    while (!number.empty() && isspace(number[number.size() - 1]))
                        number.erase(number.end() - (76 - 0x4B));
                    enter_ban = safe_stoi(number);
                }
                catch (exception a)
                {
                    gt::log_to_console("`4Critical Error: `2override detected");

                }
            }
            if (packet.find("buttonClicked|deletepull") != std::string::npos) {

            }
        }
        catch (std::exception& e) {
            std::cout << "error: " << e.what();
        }
        return true;
    }

    if (packet.find("spam_text") != -1) {
        try {
            if (packet.find("c_text|") != -1) {
                std::string aaa = packet.substr(packet.find("ext|") + 4, packet.size());
                std::string number = aaa.c_str();
                while (!number.empty() && isspace(number[number.size() - 1]))
                    number.erase(number.end() - (76 - 0x4B));
                enabled_color = safe_stoi(number);
            }
            if (packet.find("spam_msg|") != -1) {
                std::string msg = packet.substr(packet.find("spam_msg|") + 9, packet.length() - packet.find("spam_msg") - 1);
                aspam = msg;
            }
            if (packet.find("delay_msg|") != -1) {
                std::string msg = packet.substr(packet.find("delay_msg|") + 10, packet.length() - packet.find("delay_msg") - 1);
                delay = safe_stoi(msg);
            }

        }
        catch (exception a) {
            std::cout << "error?";
            std::cout << a.what();
        }
        return true;
    }
    if (packet.find("buttonClicked|SendCloth1") != -1)
    {
        std::ifstream vr0;
        std::ifstream vr1;
        std::ifstream vr2;
        std::ifstream vr3;
        std::ifstream vr4;
        std::ifstream vr5;
        std::ifstream vr6;
        std::ifstream vr7;
        std::ifstream vr8;

        vr0.open(filePaths + "hat1.txt");
        vr0 >> hat;
        vr0.close();
        vr1.open(filePaths + "shoes1.txt");
        vr1 >> shoe;
        vr1.close();
        vr2.open(filePaths + "wing1.txt");
        vr2 >> back;
        vr2.close();
        vr3.open(filePaths + "ances1.txt");
        vr3 >> ances;
        vr3.close();
        vr4.open(filePaths + "shirt1.txt");
        vr4 >> shirt;
        vr4.close();
        vr5.open(filePaths + "neck1.txt");
        vr5 >> face;
        vr5.close();
        vr6.open(filePaths + "hair1.txt");
        vr6 >> hair;
        vr6.close();
        vr7.open(filePaths + "pant1.txt");
        vr7 >> pants;
        vr7.close();
        vr8.open(filePaths + "hand1.txt");
        vr8 >> hand;
        vr8.close();

        if (hand > 0)
        {
            std::string iID = to_string(hand);
            gameupdatepacket_t xp{ 0 };
            xp.m_type = PACKET_MODIFY_ITEM_INVENTORY;
            xp.m_count = 0;
            xp.m_int_data = atoi(iID.c_str());
            g_server->send(true, NET_MESSAGE_GAME_PACKET, (uint8_t*)&xp, sizeof(gameupdatepacket_t));
            auto& item = items[atoi(iID.c_str())];
        }

        if (pants > 0)
        {
            std::string iID0 = to_string(pants);
            gameupdatepacket_t xp0{ 0 };
            xp0.m_type = PACKET_MODIFY_ITEM_INVENTORY;
            xp0.m_count = 0;
            xp0.m_int_data = atoi(iID0.c_str());
            g_server->send(true, NET_MESSAGE_GAME_PACKET, (uint8_t*)&xp0, sizeof(gameupdatepacket_t));
            auto& item0 = items[atoi(iID0.c_str())];
        }

        if (hair > 0)
        {
            std::string iID1 = to_string(hair);
            gameupdatepacket_t xp1{ 0 };
            xp1.m_type = PACKET_MODIFY_ITEM_INVENTORY;
            xp1.m_count = 0;
            xp1.m_int_data = atoi(iID1.c_str());
            g_server->send(true, NET_MESSAGE_GAME_PACKET, (uint8_t*)&xp1, sizeof(gameupdatepacket_t));
            auto& item1 = items[atoi(iID1.c_str())];
        }


        if (face > 0)
        {
            std::string iID2 = to_string(face);
            gameupdatepacket_t xp2{ 0 };
            xp2.m_type = PACKET_MODIFY_ITEM_INVENTORY;
            xp2.m_count = 0;
            xp2.m_int_data = atoi(iID2.c_str());
            g_server->send(true, NET_MESSAGE_GAME_PACKET, (uint8_t*)&xp2, sizeof(gameupdatepacket_t));
            auto& item2 = items[atoi(iID2.c_str())]; // nerde oluyodu bu bek şey
        }// buldum bekle

        if (shirt > 0)
        {
            std::string iID3 = to_string(shirt);
            gameupdatepacket_t xp3{ 0 };
            xp3.m_type = PACKET_MODIFY_ITEM_INVENTORY;
            xp3.m_count = 0;
            xp3.m_int_data = atoi(iID3.c_str());
            g_server->send(true, NET_MESSAGE_GAME_PACKET, (uint8_t*)&xp3, sizeof(gameupdatepacket_t));
            auto& item3 = items[atoi(iID3.c_str())];
        }


        if (ances > 0)
        {
            std::string iID4 = to_string(ances);
            gameupdatepacket_t xp4{ 0 };
            xp4.m_type = PACKET_MODIFY_ITEM_INVENTORY;
            xp4.m_count = 0;
            xp4.m_int_data = atoi(iID4.c_str());
            g_server->send(true, NET_MESSAGE_GAME_PACKET, (uint8_t*)&xp4, sizeof(gameupdatepacket_t));
            auto& item4 = items[atoi(iID4.c_str())];
        }

        if (back > 0)
        {
            std::string iID6 = to_string(back);
            gameupdatepacket_t xp6{ 0 };
            xp6.m_type = PACKET_MODIFY_ITEM_INVENTORY;
            xp6.m_count = 0;
            xp6.m_int_data = atoi(iID6.c_str());
            g_server->send(true, NET_MESSAGE_GAME_PACKET, (uint8_t*)&xp6, sizeof(gameupdatepacket_t));
            auto& item6 = items[atoi(iID6.c_str())];
        }

        if (shoe > 0)
        {
            std::string iID7 = to_string(shoe);
            gameupdatepacket_t xp7{ 0 };
            xp7.m_type = PACKET_MODIFY_ITEM_INVENTORY;
            xp7.m_count = 0;
            xp7.m_int_data = atoi(iID7.c_str());
            g_server->send(true, NET_MESSAGE_GAME_PACKET, (uint8_t*)&xp7, sizeof(gameupdatepacket_t));
            auto& item7 = items[atoi(iID7.c_str())];
        }

        if (hat > 0)
        {
            std::string iID8 = to_string(hat);
            gameupdatepacket_t xp8{ 0 };
            xp8.m_type = PACKET_MODIFY_ITEM_INVENTORY;
            xp8.m_count = 0;
            xp8.m_int_data = atoi(iID8.c_str());
            g_server->send(true, NET_MESSAGE_GAME_PACKET, (uint8_t*)&xp8, sizeof(gameupdatepacket_t));
            auto& item8 = items[atoi(iID8.c_str())];
        }


        /*
        std::string iID5 = to_string(ances);
        gameupdatepacket_t xp5{ 0 };
        xp5.m_type = PACKET_MODIFY_ITEM_INVENTORY;
        xp5.m_count = 0;
        xp5.m_int_data = atoi(iID5.c_str());
        g_server->send(true, NET_MESSAGE_GAME_PACKET, (uint8_t*)&xp5, sizeof(gameupdatepacket_t));
        auto& item5 = items[atoi(iID5.c_str())];
        */

        iswear = true;
        send_vset();
        send_effect_real();
        return true;
    }

    if (packet.find("buttonClicked|SendCloth2") != -1)
    {
        std::ifstream vr0;
        std::ifstream vr1;
        std::ifstream vr2;
        std::ifstream vr3;
        std::ifstream vr4;
        std::ifstream vr5;
        std::ifstream vr6;
        std::ifstream vr7;
        std::ifstream vr8;
        vr0.open(filePaths + "hat2.txt");
        vr0 >> hat;
        vr0.close();
        vr1.open(filePaths + "shoes2.txt");
        vr1 >> shoe;
        vr1.close();
        vr2.open(filePaths + "wing2.txt");
        vr2 >> back;
        vr2.close();
        vr3.open(filePaths + "ances2.txt");
        vr3 >> ances;
        vr3.close();
        vr4.open(filePaths + "shirt2.txt");
        vr4 >> shirt;
        vr4.close();
        vr5.open(filePaths + "neck2.txt");
        vr5 >> face;
        vr5.close();
        vr6.open(filePaths + "hair2.txt");
        vr6 >> hair;
        vr6.close();
        vr7.open(filePaths + "pant2.txt");
        vr7 >> pants;
        vr7.close();
        vr8.open(filePaths + "hand2.txt");
        vr8 >> hand;
        vr8.close();

        if (hand > 0)
        {
            std::string iID = to_string(hand);
            gameupdatepacket_t xp{ 0 };
            xp.m_type = PACKET_MODIFY_ITEM_INVENTORY;
            xp.m_count = 0;
            xp.m_int_data = atoi(iID.c_str());
            g_server->send(true, NET_MESSAGE_GAME_PACKET, (uint8_t*)&xp, sizeof(gameupdatepacket_t));
            auto& item = items[atoi(iID.c_str())];
        }

        if (pants > 0)
        {
            std::string iID0 = to_string(pants);
            gameupdatepacket_t xp0{ 0 };
            xp0.m_type = PACKET_MODIFY_ITEM_INVENTORY;
            xp0.m_count = 0;
            xp0.m_int_data = atoi(iID0.c_str());
            g_server->send(true, NET_MESSAGE_GAME_PACKET, (uint8_t*)&xp0, sizeof(gameupdatepacket_t));
            auto& item0 = items[atoi(iID0.c_str())];
        }

        if (hair > 0)
        {
            std::string iID1 = to_string(hair);
            gameupdatepacket_t xp1{ 0 };
            xp1.m_type = PACKET_MODIFY_ITEM_INVENTORY;
            xp1.m_count = 0;
            xp1.m_int_data = atoi(iID1.c_str());
            g_server->send(true, NET_MESSAGE_GAME_PACKET, (uint8_t*)&xp1, sizeof(gameupdatepacket_t));
            auto& item1 = items[atoi(iID1.c_str())];
        }


        if (face > 0)
        {
            std::string iID2 = to_string(face);
            gameupdatepacket_t xp2{ 0 };
            xp2.m_type = PACKET_MODIFY_ITEM_INVENTORY;
            xp2.m_count = 0;
            xp2.m_int_data = atoi(iID2.c_str());
            g_server->send(true, NET_MESSAGE_GAME_PACKET, (uint8_t*)&xp2, sizeof(gameupdatepacket_t));
            auto& item2 = items[atoi(iID2.c_str())];
        }

        if (shirt > 0)
        {
            std::string iID3 = to_string(shirt);
            gameupdatepacket_t xp3{ 0 };
            xp3.m_type = PACKET_MODIFY_ITEM_INVENTORY;
            xp3.m_count = 0;
            xp3.m_int_data = atoi(iID3.c_str());
            g_server->send(true, NET_MESSAGE_GAME_PACKET, (uint8_t*)&xp3, sizeof(gameupdatepacket_t));
            auto& item3 = items[atoi(iID3.c_str())];
        }


        if (ances > 0)
        {
            std::string iID4 = to_string(ances);
            gameupdatepacket_t xp4{ 0 };
            xp4.m_type = PACKET_MODIFY_ITEM_INVENTORY;
            xp4.m_count = 0;
            xp4.m_int_data = atoi(iID4.c_str());
            g_server->send(true, NET_MESSAGE_GAME_PACKET, (uint8_t*)&xp4, sizeof(gameupdatepacket_t));
            auto& item4 = items[atoi(iID4.c_str())];
        }

        if (back > 0)
        {
            std::string iID6 = to_string(back);
            gameupdatepacket_t xp6{ 0 };
            xp6.m_type = PACKET_MODIFY_ITEM_INVENTORY;
            xp6.m_count = 0;
            xp6.m_int_data = atoi(iID6.c_str());
            g_server->send(true, NET_MESSAGE_GAME_PACKET, (uint8_t*)&xp6, sizeof(gameupdatepacket_t));
            auto& item6 = items[atoi(iID6.c_str())];
        }

        if (shoe > 0)
        {
            std::string iID7 = to_string(shoe);
            gameupdatepacket_t xp7{ 0 };
            xp7.m_type = PACKET_MODIFY_ITEM_INVENTORY;
            xp7.m_count = 0;
            xp7.m_int_data = atoi(iID7.c_str());
            g_server->send(true, NET_MESSAGE_GAME_PACKET, (uint8_t*)&xp7, sizeof(gameupdatepacket_t));
            auto& item7 = items[atoi(iID7.c_str())];
        }

        if (hat > 0)
        {
            std::string iID8 = to_string(hat);
            gameupdatepacket_t xp8{ 0 };
            xp8.m_type = PACKET_MODIFY_ITEM_INVENTORY;
            xp8.m_count = 0;
            xp8.m_int_data = atoi(iID8.c_str());
            g_server->send(true, NET_MESSAGE_GAME_PACKET, (uint8_t*)&xp8, sizeof(gameupdatepacket_t));
            auto& item8 = items[atoi(iID8.c_str())];
        }

        iswear = true;
        send_vset();
        send_effect_real();
        return true;
    }

    if (packet.find("buttonClicked|SendCloth3") != -1)
    {
        std::ifstream vr0;
        std::ifstream vr1;
        std::ifstream vr2;
        std::ifstream vr3;
        std::ifstream vr4;
        std::ifstream vr5;
        std::ifstream vr6;
        std::ifstream vr7;
        std::ifstream vr8;
        vr0.open(filePaths + "hat3.txt");
        vr0 >> hat;
        vr0.close();
        vr1.open(filePaths + "shoes3.txt");
        vr1 >> shoe;
        vr1.close();
        vr2.open(filePaths + "wing3.txt");
        vr2 >> back;
        vr2.close();
        vr3.open(filePaths + "ances3.txt");
        vr3 >> ances;
        vr3.close();
        vr4.open(filePaths + "shirt3.txt");
        vr4 >> shirt;
        vr4.close();
        vr5.open(filePaths + "neck3.txt");
        vr5 >> face;
        vr5.close();
        vr6.open(filePaths + "hair3.txt");
        vr6 >> hair;
        vr6.close();
        vr7.open(filePaths + "pant3.txt");
        vr7 >> pants;
        vr7.close();
        vr8.open(filePaths + "hand3.txt");
        vr8 >> hand;
        vr8.close();

        if (hand > 0)
        {
            std::string iID = to_string(hand);
            gameupdatepacket_t xp{ 0 };
            xp.m_type = PACKET_MODIFY_ITEM_INVENTORY;
            xp.m_count = 0;
            xp.m_int_data = atoi(iID.c_str());
            g_server->send(true, NET_MESSAGE_GAME_PACKET, (uint8_t*)&xp, sizeof(gameupdatepacket_t));
            auto& item = items[atoi(iID.c_str())];
        }

        if (pants > 0)
        {
            std::string iID0 = to_string(pants);
            gameupdatepacket_t xp0{ 0 };
            xp0.m_type = PACKET_MODIFY_ITEM_INVENTORY;
            xp0.m_count = 0;
            xp0.m_int_data = atoi(iID0.c_str());
            g_server->send(true, NET_MESSAGE_GAME_PACKET, (uint8_t*)&xp0, sizeof(gameupdatepacket_t));
            auto& item0 = items[atoi(iID0.c_str())];
        }

        if (hair > 0)
        {
            std::string iID1 = to_string(hair);
            gameupdatepacket_t xp1{ 0 };
            xp1.m_type = PACKET_MODIFY_ITEM_INVENTORY;
            xp1.m_count = 0;
            xp1.m_int_data = atoi(iID1.c_str());
            g_server->send(true, NET_MESSAGE_GAME_PACKET, (uint8_t*)&xp1, sizeof(gameupdatepacket_t));
            auto& item1 = items[atoi(iID1.c_str())];
        }


        if (face > 0)
        {
            std::string iID2 = to_string(face);
            gameupdatepacket_t xp2{ 0 };
            xp2.m_type = PACKET_MODIFY_ITEM_INVENTORY;
            xp2.m_count = 0;
            xp2.m_int_data = atoi(iID2.c_str());
            g_server->send(true, NET_MESSAGE_GAME_PACKET, (uint8_t*)&xp2, sizeof(gameupdatepacket_t));
            auto& item2 = items[atoi(iID2.c_str())];
        }

        if (shirt > 0)
        {
            std::string iID3 = to_string(shirt);
            gameupdatepacket_t xp3{ 0 };
            xp3.m_type = PACKET_MODIFY_ITEM_INVENTORY;
            xp3.m_count = 0;
            xp3.m_int_data = atoi(iID3.c_str());
            g_server->send(true, NET_MESSAGE_GAME_PACKET, (uint8_t*)&xp3, sizeof(gameupdatepacket_t));
            auto& item3 = items[atoi(iID3.c_str())];
        }


        if (ances > 0)
        {
            std::string iID4 = to_string(ances);
            gameupdatepacket_t xp4{ 0 };
            xp4.m_type = PACKET_MODIFY_ITEM_INVENTORY;
            xp4.m_count = 0;
            xp4.m_int_data = atoi(iID4.c_str());
            g_server->send(true, NET_MESSAGE_GAME_PACKET, (uint8_t*)&xp4, sizeof(gameupdatepacket_t));
            auto& item4 = items[atoi(iID4.c_str())];
        }

        if (back > 0)
        {
            std::string iID6 = to_string(back);
            gameupdatepacket_t xp6{ 0 };
            xp6.m_type = PACKET_MODIFY_ITEM_INVENTORY;
            xp6.m_count = 0;
            xp6.m_int_data = atoi(iID6.c_str());
            g_server->send(true, NET_MESSAGE_GAME_PACKET, (uint8_t*)&xp6, sizeof(gameupdatepacket_t));
            auto& item6 = items[atoi(iID6.c_str())];
        }

        if (shoe > 0)
        {
            std::string iID7 = to_string(shoe);
            gameupdatepacket_t xp7{ 0 };
            xp7.m_type = PACKET_MODIFY_ITEM_INVENTORY;
            xp7.m_count = 0;
            xp7.m_int_data = atoi(iID7.c_str());
            g_server->send(true, NET_MESSAGE_GAME_PACKET, (uint8_t*)&xp7, sizeof(gameupdatepacket_t));
            auto& item7 = items[atoi(iID7.c_str())];
        }

        if (hat > 0)
        {
            std::string iID8 = to_string(hat);
            gameupdatepacket_t xp8{ 0 };
            xp8.m_type = PACKET_MODIFY_ITEM_INVENTORY;
            xp8.m_count = 0;
            xp8.m_int_data = atoi(iID8.c_str());
            g_server->send(true, NET_MESSAGE_GAME_PACKET, (uint8_t*)&xp8, sizeof(gameupdatepacket_t));
            auto& item8 = items[atoi(iID8.c_str())];
        }
        iswear = true;
        send_vset();
        send_effect_real();
        return true;
    }

    if (packet.find("buttonClicked|SendCloth4") != -1)
    {

        std::ifstream vr0;
        std::ifstream vr1;
        std::ifstream vr2;
        std::ifstream vr3;
        std::ifstream vr4;
        std::ifstream vr5;
        std::ifstream vr6;
        std::ifstream vr7;
        std::ifstream vr8;
        vr0.open(filePaths + "hat4.txt");
        vr0 >> hat;
        vr0.close();
        vr1.open(filePaths + "shoes4.txt");
        vr1 >> shoe;
        vr1.close();
        vr2.open(filePaths + "wing4.txt");
        vr2 >> back;
        vr2.close();
        vr3.open(filePaths + "ances4.txt");
        vr3 >> ances;
        vr3.close();
        vr4.open(filePaths + "shirt4.txt");
        vr4 >> shirt;
        vr4.close();
        vr5.open(filePaths + "neck4.txt");
        vr5 >> face;
        vr5.close();
        vr6.open(filePaths + "hair4.txt");
        vr6 >> hair;
        vr6.close();
        vr7.open(filePaths + "pant4.txt");
        vr7 >> pants;
        vr7.close();
        vr8.open(filePaths + "hand4.txt");
        vr8 >> hand;
        vr8.close();

        if (hand > 0)
        {
            std::string iID = to_string(hand);
            gameupdatepacket_t xp{ 0 };
            xp.m_type = PACKET_MODIFY_ITEM_INVENTORY;
            xp.m_count = 0;
            xp.m_int_data = atoi(iID.c_str());
            g_server->send(true, NET_MESSAGE_GAME_PACKET, (uint8_t*)&xp, sizeof(gameupdatepacket_t));
            auto& item = items[atoi(iID.c_str())];
        }

        if (pants > 0)
        {
            std::string iID0 = to_string(pants);
            gameupdatepacket_t xp0{ 0 };
            xp0.m_type = PACKET_MODIFY_ITEM_INVENTORY;
            xp0.m_count = 0;
            xp0.m_int_data = atoi(iID0.c_str());
            g_server->send(true, NET_MESSAGE_GAME_PACKET, (uint8_t*)&xp0, sizeof(gameupdatepacket_t));
            auto& item0 = items[atoi(iID0.c_str())];
        }

        if (hair > 0)
        {
            std::string iID1 = to_string(hair);
            gameupdatepacket_t xp1{ 0 };
            xp1.m_type = PACKET_MODIFY_ITEM_INVENTORY;
            xp1.m_count = 0;
            xp1.m_int_data = atoi(iID1.c_str());
            g_server->send(true, NET_MESSAGE_GAME_PACKET, (uint8_t*)&xp1, sizeof(gameupdatepacket_t));
            auto& item1 = items[atoi(iID1.c_str())];
        }


        if (face > 0)
        {
            std::string iID2 = to_string(face);
            gameupdatepacket_t xp2{ 0 };
            xp2.m_type = PACKET_MODIFY_ITEM_INVENTORY;
            xp2.m_count = 0;
            xp2.m_int_data = atoi(iID2.c_str());
            g_server->send(true, NET_MESSAGE_GAME_PACKET, (uint8_t*)&xp2, sizeof(gameupdatepacket_t));
            auto& item2 = items[atoi(iID2.c_str())];
        }

        if (shirt > 0)
        {
            std::string iID3 = to_string(shirt);
            gameupdatepacket_t xp3{ 0 };
            xp3.m_type = PACKET_MODIFY_ITEM_INVENTORY;
            xp3.m_count = 0;
            xp3.m_int_data = atoi(iID3.c_str());
            g_server->send(true, NET_MESSAGE_GAME_PACKET, (uint8_t*)&xp3, sizeof(gameupdatepacket_t));
            auto& item3 = items[atoi(iID3.c_str())];
        }


        if (ances > 0)
        {
            std::string iID4 = to_string(ances);
            gameupdatepacket_t xp4{ 0 };
            xp4.m_type = PACKET_MODIFY_ITEM_INVENTORY;
            xp4.m_count = 0;
            xp4.m_int_data = atoi(iID4.c_str());
            g_server->send(true, NET_MESSAGE_GAME_PACKET, (uint8_t*)&xp4, sizeof(gameupdatepacket_t));
            auto& item4 = items[atoi(iID4.c_str())];
        }

        if (back > 0)
        {
            std::string iID6 = to_string(back);
            gameupdatepacket_t xp6{ 0 };
            xp6.m_type = PACKET_MODIFY_ITEM_INVENTORY;
            xp6.m_count = 0;
            xp6.m_int_data = atoi(iID6.c_str());
            g_server->send(true, NET_MESSAGE_GAME_PACKET, (uint8_t*)&xp6, sizeof(gameupdatepacket_t));
            auto& item6 = items[atoi(iID6.c_str())];
        }

        if (shoe > 0)
        {
            std::string iID7 = to_string(shoe);
            gameupdatepacket_t xp7{ 0 };
            xp7.m_type = PACKET_MODIFY_ITEM_INVENTORY;
            xp7.m_count = 0;
            xp7.m_int_data = atoi(iID7.c_str());
            g_server->send(true, NET_MESSAGE_GAME_PACKET, (uint8_t*)&xp7, sizeof(gameupdatepacket_t));
            auto& item7 = items[atoi(iID7.c_str())];
        }

        if (hat > 0)
        {
            std::string iID8 = to_string(hat);
            gameupdatepacket_t xp8{ 0 };
            xp8.m_type = PACKET_MODIFY_ITEM_INVENTORY;
            xp8.m_count = 0;
            xp8.m_int_data = atoi(iID8.c_str());
            g_server->send(true, NET_MESSAGE_GAME_PACKET, (uint8_t*)&xp8, sizeof(gameupdatepacket_t));
            auto& item8 = items[atoi(iID8.c_str())];
        }

        iswear = true;
        send_vset();
        send_effect_real();
        return true;
    }

    if (packet.find("buttonClicked|SaveCloth1") != -1)
    {
        std::ifstream vr;
        if (!vr) {
            std::ofstream vw;
            vw.open(filePaths + "gaspull.txt");
            vw << doublejump;
            vw.close();
        }
        vr >> doublejump;
        vr.close();
        if (!vr) {
            std::ofstream vw;
            vw.open(filePaths + "hat1.txt");
            vw << hat;
            vw.close();
        }
        vr >> hat;
        vr.close();
        if (!vr) {
            std::ofstream vw;
            vw.open(filePaths + "wing1.txt");
            vw << back;
            vw.close();

        }
        vr >> back;
        vr.close();
        if (!vr) {
            std::ofstream vw;
            vw.open(filePaths + "shoes1.txt");
            vw << shoe;
            vw.close();
        }
        vr >> shoe;
        vr.close();

        if (!vr) {
            std::ofstream vw;
            vw.open(filePaths + "ances1.txt");
            vw << ances;
            vw.close();
        }
        vr >> ances;
        vr.close();

        if (!vr) {
            std::ofstream vw;
            vw.open(filePaths + "shirt1.txt");
            vw << shirt;
            vw.close();
        }
        vr >> shirt;
        vr.close();

        if (!vr) {
            std::ofstream vw;
            vw.open(filePaths + "neck1.txt");
            vw << face;
            vw.close();
        }
        vr >> face;
        vr.close();

        if (!vr) {
            std::ofstream vw;
            vw.open(filePaths + "hair1.txt");
            vw << hair;
            vw.close();
        }
        vr >> hair;
        vr.close();

        if (!vr) {
            std::ofstream vw;
            vw.open(filePaths + "pant1.txt");
            vw << pants;
            vw.close();
        }
        vr >> pants;
        vr.close();

        if (!vr) {
            std::ofstream vw;
            vw.open(filePaths + "hand1.txt");
            vw << hand;
            vw.close();
        }
        vr >> hand;
        vr.close();
        return true;
    }


    if (packet.find("buttonClicked|SaveCloth2") != -1)
    {
        std::ifstream vr;
        if (!vr) {
            std::ofstream vw;
            vw.open(filePaths + "gaspull.txt");
            vw << doublejump;
            vw.close();
        }
        vr >> doublejump;
        vr.close();
        if (!vr) {
            std::ofstream vw;
            vw.open(filePaths + "wing2.txt");
            vw << back;
            vw.close();
        }
        vr >> back;
        vr.close();
        if (!vr) {
            std::ofstream vw;
            vw.open(filePaths + "hat2.txt");
            vw << hat;
            vw.close();
        }
        vr >> hat;
        vr.close();
        if (!vr) {
            std::ofstream vw;
            vw.open(filePaths + "shoes2.txt");
            vw << shoe;
            vw.close();
        }
        vr >> shoe;
        vr.close();
        if (!vr) {
            std::ofstream vw;
            vw.open(filePaths + "ances2.txt");
            vw << ances;
            vw.close();
        }
        vr >> ances;
        vr.close();
        if (!vr) {
            std::ofstream vw;
            vw.open(filePaths + "shirt2.txt");
            vw << shirt;
            vw.close();
        }
        vr >> shirt;
        vr.close();
        if (!vr) {
            std::ofstream vw;
            vw.open(filePaths + "neck2.txt");
            vw << face;
            vw.close();
        }
        vr >> face;
        vr.close();
        if (!vr) {
            std::ofstream vw;
            vw.open(filePaths + "hair2.txt");
            vw << hair;
            vw.close();
        }
        vr >> hair;
        vr.close();
        if (!vr) {
            std::ofstream vw;
            vw.open(filePaths + "pant2.txt");
            vw << pants;
            vw.close();
        }
        vr >> pants;
        vr.close();
        if (!vr) {
            std::ofstream vw;
            vw.open(filePaths + "hand2.txt");
            vw << hand;
            vw.close();
        }
        vr >> hand;
        vr.close();
        return true;
    }

    if (packet.find("buttonClicked|SaveCloth3") != -1)
    {
        std::ifstream vr;
        if (!vr) {
            std::ofstream vw;
            vw.open(filePaths + "gaspull.txt");
            vw << doublejump;
            vw.close();
        }
        vr >> doublejump;
        vr.close();
        if (!vr) {
            std::ofstream vw;
            vw.open(filePaths + "wing3.txt");
            vw << back;
            vw.close();
        }
        vr >> back;
        vr.close();
        if (!vr) {
            std::ofstream vw;
            vw.open(filePaths + "hat3.txt");
            vw << hat;
            vw.close();
        }
        vr >> hat;
        vr.close();

        if (!vr) {
            std::ofstream vw;
            vw.open(filePaths + "shoes3.txt");
            vw << shoe;
            vw.close();
        }
        vr >> shoe;
        vr.close();

        if (!vr) {
            std::ofstream vw;
            vw.open(filePaths + "ances3.txt");
            vw << ances;
            vw.close();
        }
        vr >> ances;
        vr.close();

        if (!vr) {
            std::ofstream vw;
            vw.open(filePaths + "shirt3.txt");
            vw << shirt;
            vw.close();
        }
        vr >> shirt;
        vr.close();

        if (!vr) {
            std::ofstream vw;
            vw.open(filePaths + "neck3.txt");
            vw << face;
            vw.close();
        }
        vr >> face;
        vr.close();

        if (!vr) {
            std::ofstream vw;
            vw.open(filePaths + "hair3.txt");
            vw << hair;
            vw.close();
        }
        vr >> hair;
        vr.close();

        if (!vr) {
            std::ofstream vw;
            vw.open(filePaths + "pant3.txt");
            vw << pants;
            vw.close();
        }
        vr >> pants;
        vr.close();

        if (!vr) {
            std::ofstream vw;
            vw.open(filePaths + "hand3.txt");
            vw << hand;
            vw.close();
        }
        vr >> hand;
        vr.close();
        return true;
    }

    if (packet.find("buttonClicked|SaveCloth4") != -1)
    {
        std::ifstream vr;
        if (!vr) {
            std::ofstream vw;
            vw.open(filePaths + "gaspull.txt");
            vw << doublejump;
            vw.close();
        }
        vr >> doublejump;
        vr.close();
        if (!vr) {
            std::ofstream vw;
            vw.open(filePaths + "wing4.txt");
            vw << back;
            vw.close();
        }
        vr >> back;
        vr.close();
        if (!vr) {
            std::ofstream vw;
            vw.open(filePaths + "hat4.txt");
            vw << hat;
            vw.close();
        }
        vr >> hat;
        vr.close();

        if (!vr) {
            std::ofstream vw;
            vw.open(filePaths + "shoes4.txt");
            vw << shoe;
            vw.close();
        }
        vr >> shoe;
        vr.close();

        if (!vr) {
            std::ofstream vw;
            vw.open(filePaths + "ances4.txt");
            vw << ances;
            vw.close();
        }
        vr >> ances;
        vr.close();

        if (!vr) {
            std::ofstream vw;
            vw.open(filePaths + "shirt4.txt");
            vw << shirt;
            vw.close();
        }
        vr >> shirt;
        vr.close();

        if (!vr) {
            std::ofstream vw;
            vw.open(filePaths + "neck4.txt");
            vw << face;
            vw.close();
        }
        vr >> face;
        vr.close();

        if (!vr) {
            std::ofstream vw;
            vw.open(filePaths + "hair4.txt");
            vw << hair;
            vw.close();
        }
        vr >> hair;
        vr.close();

        if (!vr) {
            std::ofstream vw;
            vw.open(filePaths + "pant4.txt");
            vw << pants;
            vw.close();
        }
        vr >> pants;
        vr.close();

        if (!vr) {
            std::ofstream vw;
            vw.open(filePaths + "hand4.txt");
            vw << hand;
            vw.close();
        }
        vr >> hand;
        vr.close();
        return true;
    }


    if (packet.find("buttonClicked|saveslot1") != -1)
    {
        save_details();
        return true;
    }
    if (packet.find("buttonClicked|saveslot2") != -1)
    {
        save_details2();
        return true;
    }
    if (packet.find("buttonClicked|saveslot3") != -1)
    {
        save_details3();
        return true;
    }
    if (packet.find("buttonClicked|TakeDisc") != -1)
    {
        std::string iID = to_string(6326);
        gameupdatepacket_t xp{ 0 };
        xp.m_type = PACKET_MODIFY_ITEM_INVENTORY;
        xp.m_count = 1;
        xp.m_int_data = atoi(iID.c_str());
        g_server->send(true, NET_MESSAGE_GAME_PACKET, (uint8_t*)&xp, sizeof(gameupdatepacket_t));
        auto& item = items[atoi(iID.c_str())];
        return true;
    }   
    if (packet.find("wrenchpull|") != -1) {
        try {
            std::string aaa = packet.substr(packet.find("ull|") + 4, packet.size());
            std::string number = aaa.c_str();
            while (!number.empty() && isspace(number[number.size() - 1]))
                number.erase(number.end() - (76 - 0x4B));
            wrench = safe_stoi(number);
        }
        catch (exception a)
        {
            gt::log_to_console("`4Critical Error: `2override detected");

        }
        return true;
    }
    if (packet.find("buttonClicked|sendfond") != -1) {
        try
        {
            std::string number = packet.substr(packet.find("message_xd|") + 11, packet.length() - packet.find("message_xd") - 1);
            std::string find_value = duzeltilmisStringForFond(number);
            getfindresults(find_value);
            return true;
        }
        catch (exception a)
        {
            gt::log_to_console("`4Critical Error: `2Override detected");

        }
    }
    if (packet.find("find_dialog") != -1) {
        if (packet.find("buttonClicked|spare_btn_") != -1) {
            std::string iID = packet.substr(packet.find("buttonClicked|spare_btn_") + 24, packet.length() - packet.find("buttonClicked|spare_btn_") - 1);
            gameupdatepacket_t xp{ 0 };
            xp.m_type = PACKET_MODIFY_ITEM_INVENTORY;
            xp.m_count = 1;
            xp.m_int_data = atoi(iID.c_str());
            g_server->send(true, NET_MESSAGE_GAME_PACKET, (uint8_t*)&xp, sizeof(gameupdatepacket_t));
            auto& item = items[atoi(iID.c_str())];
            gt::log_to_console("`9Succesfully Give `#" + item.name);
        }
        if (packet.find("vclothes") != -1) {
            try {
                std::string aaa = packet.substr(packet.find("hes|") + 4, packet.size());
                std::string number = aaa.c_str();
                while (!number.empty() && isspace(number[number.size() - 1]))
                    number.erase(number.end() - (76 - 0x4B));
                iswear = safe_stoi(number);
                if (iswear == true) {

                }
            }
            catch (exception a)
            {
                gt::log_to_console("`4Critical Error: `2override detected");

            }
        }
        return true;
    }
    
    //---- 1. BUTON -----//
    if (packet.find("buttonClicked|selecthotkeys1_f1") != -1) {
        std::this_thread::sleep_for(std::chrono::milliseconds(300));
        std::string convertedstr = std::string(1, globalhotkey1);
        Dialog a;
        a.addLabelWithIcon("`#Select Hotkey", 2724, LABEL_BIG);
        if (globalhotkey1 == 0)
            a.addSmallText("`wCurrent Selected Hotkey: None");
        else
            a.addSmallText("`wCurrent Selected Hotkey: " + convertedstr);
        a.addTextBox("`wPress `#One `wOr `# Multiple `wKeys On Your Keyboard");
        a.addButton("selecthotkeys1", "`wSelect Hotkey!");
        a.addButton("clearhotkeys1", "`wClear Hotkey!");
        a.addQuickExit();
        a.endDialog("end", "Okey", "Cancel");
        variantlist_t liste{ "OnDialogRequest" };
        liste[1] = a.finishDialog();
        g_server->send(true, liste);
        return true;
    }
    if (packet.find("buttonClicked|clearhotkeys1") != -1) {
        std::this_thread::sleep_for(std::chrono::milliseconds(300));
        globalhotkey1 = 0;
        hotkeys1 = "None";

        return true;
    }
    if (packet.find("buttonClicked|selecthotkeys1") != -1) {
        try
        {

            // Klavye girişi için bir hook oluşturun
            gt::log_to_console("`2Go Select Hotkey!");
            //std::cout << "[DEV-LOG] Waiting To Prees Key" << std::endl;

            // Klavye girişi için bir hook oluşturun
            keyboardHook = SetWindowsHookEx(WH_KEYBOARD_LL, LowLevelKeyboardProcFor1, GetModuleHandle(nullptr), 0);

            // Message döngüsü
            MSG msg;
            while (GetMessage(&msg, nullptr, 0, 0)) {
                TranslateMessage(&msg);
                DispatchMessage(&msg);

                // Eğer bir tuşa basıldıysa, programı sonlandırmayı iptal etmek için döngüden çıkın
                if (keyPressed) {
                    break;
                }
            }
            std::string myString = std::string(1, globalhotkey1);
            keyPressed = false;
            // Klavye hook'u kaldırıldıktan sonra geri kalan kodlara devam edebilirsiniz
            //std::cout << "[DEV-LOG] Application Contiunening" << std::endl;
            gt::log_to_console("`2Pressed Key : " + myString);
            hotkey1 = globalhotkey1;

            std::this_thread::sleep_for(std::chrono::milliseconds(50));
            Dialog a;
            a.addLabelWithIcon("`#Select Hotkey", 2724, LABEL_BIG);
            a.addSmallText("`wCurrent Selected Hotkey: " + myString);
            a.addInputBox("message_9", "`#Command Shortcut `0:", "", 20);
            a.addQuickExit();
            a.addButton("hotkeys1", "`wSave");
            variantlist_t liste{ "OnDialogRequest" };
            liste[1] = a.finishDialog();
            g_server->send(true, liste);
            return true;
        }
        catch (exception a)
        {
            gt::log_to_console("`4Critical Error: `2Override detected");

        }
    }
    if (packet.find("buttonClicked|hotkeys1") != -1) {
        try
        {
            std::string number = packet.substr(packet.find("message_9|") + 10, packet.size());
            std::string myString = std::string(1, globalhotkey1);
            hotkeys1 = number;
            std::string customvariant = "`9Configrued Custom Hotkey `0: `5" + myString + " `9Command `0: `5 " + hotkeys1;
            variantlist_t hotkeynoti{ "OnTextOverlay" };
            hotkeynoti[1] = customvariant;
            hotkeynoti[4] = 0;
            g_server->send(true, hotkeynoti);
        }
        catch (exception a)
        {
            gt::log_to_console("`4Critical Error: `2Override detected");

        }
    }
    //---- 1. BUTON SON -----//

    //---- 2. BUTON -----//
    if (packet.find("buttonClicked|selecthotkeys2_xd") != -1) {
        std::this_thread::sleep_for(std::chrono::milliseconds(300));
        std::string convertedstr = std::string(1, globalhotkey2);
        Dialog a;
        a.addLabelWithIcon("`#Select Hotkey", 2724, LABEL_BIG);
        if (globalhotkey2 == 0)
            a.addSmallText("`wCurrent Selected Hotkey: None");
        else
            a.addSmallText("`wCurrent Selected Hotkey: " + convertedstr);
        a.addTextBox("`wPress `#One `wOr `# Multiple `wKeys On Your Keyboard");
        a.addButton("selecthotkeys2", "`wSelect Hotkey!");
        a.addButton("clearhotkeys2", "`wClear Hotkey!");
        a.addQuickExit();
        a.endDialog("end", "Okey", "Cancel");
        variantlist_t liste{ "OnDialogRequest" };
        liste[1] = a.finishDialog();
        g_server->send(true, liste);
        return true;
    }
    if (packet.find("buttonClicked|clearhotkeys2") != -1) {
        std::this_thread::sleep_for(std::chrono::milliseconds(300));
        globalhotkey2 = 0;
        hotkeys2 = "None";

        return true;
    }
    if (packet.find("buttonClicked|selecthotkeys2") != -1) {
        try
        {

            // Klavye girişi için bir hook oluşturun
            gt::log_to_console("`2Go Select Hotkey!");
            //std::cout << "[DEV-LOG] Waiting To Prees Key" << std::endl;

            // Klavye girişi için bir hook oluşturun
            keyboardHook = SetWindowsHookEx(WH_KEYBOARD_LL, LowLevelKeyboardProcFor2, GetModuleHandle(nullptr), 0);

            // Message döngüsü
            MSG msg;
            while (GetMessage(&msg, nullptr, 0, 0)) {
                TranslateMessage(&msg);
                DispatchMessage(&msg);

                // Eğer bir tuşa basıldıysa, programı sonlandırmayı iptal etmek için döngüden çıkın
                if (keyPressed) {
                    break;
                }
            }
            std::string myString = std::string(1, globalhotkey2);
            keyPressed = false;
            // Klavye hook'u kaldırıldıktan sonra geri kalan kodlara devam edebilirsiniz
            //std::cout << "[DEV-LOG] Application Contiunening" << std::endl;
            gt::log_to_console("`2Pressed Key : " + myString);
            hotkey2 = globalhotkey2;

            std::this_thread::sleep_for(std::chrono::milliseconds(50));
            Dialog a;
            a.addLabelWithIcon("`#Select Hotkey", 2724, LABEL_BIG);
            a.addSmallText("`wCurrent Selected Hotkey: " + myString);
            a.addInputBox("message_9", "`#Command Shortcut `0:", "", 20);
            a.addQuickExit();
            a.addButton("hotkeys2", "`wSave");
            variantlist_t liste{ "OnDialogRequest" };
            liste[1] = a.finishDialog();
            g_server->send(true, liste);
            return true;
        }
        catch (exception a)
        {
            gt::log_to_console("`4Critical Error: `2Override detected");

        }
    }
    if (packet.find("buttonClicked|hotkeys2") != -1) {
        try
        {
            std::string number = packet.substr(packet.find("message_9|") + 10, packet.size());
            std::string myString = std::string(1, globalhotkey2);
            hotkeys2 = number;
            std::string customvariant = "`9Configrued Custom Hotkey `0: `5" + myString + " `9Command `0: `5 " + hotkeys2;
            variantlist_t hotkeynoti{ "OnTextOverlay" };
            hotkeynoti[1] = customvariant;
            hotkeynoti[4] = 0;
            g_server->send(true, hotkeynoti);
            return true;
        }
        catch (exception a)
        {
            gt::log_to_console("`4Critical Error: `2Override detected");

        }
    }
    //---- 2. BUTON SON -----//
    //---- 3. BUTON  -----//
    if (packet.find("buttonClicked|selecthotkeys3_xd") != -1) {
        std::this_thread::sleep_for(std::chrono::milliseconds(300));
        std::string convertedstr = std::string(1, globalhotkey3);
        Dialog a;
        a.addLabelWithIcon("`#Select Hotkey", 2724, LABEL_BIG);
        if (globalhotkey3 == 0)
            a.addSmallText("`wCurrent Selected Hotkey: None");
        else
            a.addSmallText("`wCurrent Selected Hotkey: " + convertedstr);
        a.addTextBox("`wPress `#One `wOr `# Multiple `wKeys On Your Keyboard");
        a.addButton("selecthotkeys3", "`wSelect Hotkey!");
        a.addButton("clearhotkeys3", "`wClear Hotkey!");
        a.addQuickExit();
        a.endDialog("end", "Okey", "Cancel");
        variantlist_t liste{ "OnDialogRequest" };
        liste[1] = a.finishDialog();
        g_server->send(true, liste);
        return true;
    }
    if (packet.find("buttonClicked|clearhotkeys3") != -1) {
        std::this_thread::sleep_for(std::chrono::milliseconds(300));
        globalhotkey3 = 0;
        hotkeys3 = "None";

        return true;
    }
    if (packet.find("buttonClicked|selecthotkeys3") != -1) {
        try
        {

            // Klavye girişi için bir hook oluşturun
            gt::log_to_console("`2Go Select Hotkey!");
            //std::cout << "[DEV-LOG] Waiting To Prees Key" << std::endl;

            // Klavye girişi için bir hook oluşturun
            keyboardHook = SetWindowsHookEx(WH_KEYBOARD_LL, LowLevelKeyboardProcFor3, GetModuleHandle(nullptr), 0);

            // Message döngüsü
            MSG msg;
            while (GetMessage(&msg, nullptr, 0, 0)) {
                TranslateMessage(&msg);
                DispatchMessage(&msg);

                // Eğer bir tuşa basıldıysa, programı sonlandırmayı iptal etmek için döngüden çıkın
                if (keyPressed) {
                    break;
                }
            }
            std::string myString = std::string(1, globalhotkey3);
            keyPressed = false;
            // Klavye hook'u kaldırıldıktan sonra geri kalan kodlara devam edebilirsiniz
            //std::cout << "[DEV-LOG] Application Contiunening" << std::endl;
            gt::log_to_console("`2Pressed Key : " + myString);
            hotkey3 = globalhotkey3;

            std::this_thread::sleep_for(std::chrono::milliseconds(50));
            Dialog a;
            a.addLabelWithIcon("`#Select Hotkey", 2724, LABEL_BIG);
            a.addSmallText("`wCurrent Selected Hotkey: " + myString);
            a.addInputBox("message_9", "`#Command Shortcut `0:", "", 20);
            a.addQuickExit();
            a.addButton("hotkeys3", "`wSave");
            variantlist_t liste{ "OnDialogRequest" };
            liste[1] = a.finishDialog();
            g_server->send(true, liste);
            return true;
        }
        catch (exception a)
        {
            gt::log_to_console("`4Critical Error: `2Override detected");

        }
    }
    if (packet.find("buttonClicked|hotkeys3") != -1) {
        try
        {
            std::string number = packet.substr(packet.find("message_9|") + 10, packet.size());
            std::string myString = std::string(1, globalhotkey3);
            hotkeys3 = number;
            std::string customvariant = "`9Configrued Custom Hotkey `0: `5" + myString + " `9Command `0: `5 " + hotkeys3;
            variantlist_t hotkeynoti{ "OnTextOverlay" };
            hotkeynoti[1] = customvariant;
            hotkeynoti[4] = 0;
            g_server->send(true, hotkeynoti);
            return true;
        }
        catch (exception a)
        {
            gt::log_to_console("`4Critical Error: `2Override detected");

        }
    }
    //---- 3. BUTON SON -----//
    //---- 4. BUTON -----//
    if (packet.find("buttonClicked|selecthotkeys4_xd") != -1) {
        std::this_thread::sleep_for(std::chrono::milliseconds(300));
        std::string convertedstr = std::string(1, globalhotkey4);
        Dialog a;
        a.addLabelWithIcon("`#Select Hotkey", 2724, LABEL_BIG);
        if (globalhotkey4 == 0)
            a.addSmallText("`wCurrent Selected Hotkey: None");
        else
            a.addSmallText("`wCurrent Selected Hotkey: " + convertedstr);
        a.addTextBox("`wPress `#One `wOr `# Multiple `wKeys On Your Keyboard");
        a.addButton("selecthotkeys4", "`wSelect Hotkey!");
        a.addButton("clearhotkeys4", "`wClear Hotkey!");
        a.addQuickExit();
        a.endDialog("end", "Okey", "Cancel");
        variantlist_t liste{ "OnDialogRequest" };
        liste[1] = a.finishDialog();
        g_server->send(true, liste);
        return true;
    }
    if (packet.find("buttonClicked|clearhotkeys4") != -1) {
        std::this_thread::sleep_for(std::chrono::milliseconds(300));
        globalhotkey4 = 0;
        hotkeys4 = "None";

        return true;
    }
    if (packet.find("buttonClicked|selecthotkeys4") != -1) {
        try
        {

            // Klavye girişi için bir hook oluşturun
            gt::log_to_console("`2Go Select Hotkey!");
            //std::cout << "[DEV-LOG] Waiting To Prees Key" << std::endl;

            // Klavye girişi için bir hook oluşturun
            keyboardHook = SetWindowsHookEx(WH_KEYBOARD_LL, LowLevelKeyboardProcFor4, GetModuleHandle(nullptr), 0);

            // Message döngüsü
            MSG msg;
            while (GetMessage(&msg, nullptr, 0, 0)) {
                TranslateMessage(&msg);
                DispatchMessage(&msg);

                // Eğer bir tuşa basıldıysa, programı sonlandırmayı iptal etmek için döngüden çıkın
                if (keyPressed) {
                    break;
                }
            }
            std::string myString = std::string(1, globalhotkey4);
            keyPressed = false;
            // Klavye hook'u kaldırıldıktan sonra geri kalan kodlara devam edebilirsiniz
            //std::cout << "[DEV-LOG] Application Contiunening" << std::endl;
            gt::log_to_console("`2Pressed Key : " + myString);
            hotkey4 = globalhotkey4;

            std::this_thread::sleep_for(std::chrono::milliseconds(50));
            Dialog a;
            a.addLabelWithIcon("`#Select Hotkey", 2724, LABEL_BIG);
            a.addSmallText("`wCurrent Selected Hotkey: " + myString);
            a.addInputBox("message_9", "`#Command Shortcut `0:", "", 20);
            a.addQuickExit();
            a.addButton("hotkeys4", "`wSave");
            variantlist_t liste{ "OnDialogRequest" };
            liste[1] = a.finishDialog();
            g_server->send(true, liste);
            return true;;
        }
        catch (exception a)
        {
            gt::log_to_console("`4Critical Error: `2Override detected");

        }
    }
    if (packet.find("buttonClicked|hotkeys4") != -1) {
        try
        {
            std::string number = packet.substr(packet.find("message_9|") + 10, packet.size());
            std::string myString = std::string(1, globalhotkey4);
            hotkeys4 = number;
            std::string customvariant = "`9Configrued Custom Hotkey `0: `5" + myString + " `9Command `0: `5 " + hotkeys4;
            variantlist_t hotkeynoti{ "OnTextOverlay" };
            hotkeynoti[1] = customvariant;
            hotkeynoti[4] = 0;
            g_server->send(true, hotkeynoti);
            return true;
        }
        catch (exception a)
        {
            gt::log_to_console("`4Critical Error: `2Override detected");

        }
    }
    //---- 4. BUTON SON -----//
    //---- 5. BUTON -----//
    if (packet.find("buttonClicked|selecthotkeys5_xd") != -1) {
        std::this_thread::sleep_for(std::chrono::milliseconds(300));
        std::string convertedstr = std::string(1, globalhotkey5);
        Dialog a;
        a.addLabelWithIcon("`#Select Hotkey", 2724, LABEL_BIG);
        if (globalhotkey5 == 0)
            a.addSmallText("`wCurrent Selected Hotkey: None");
        else
            a.addSmallText("`wCurrent Selected Hotkey: " + convertedstr);
        a.addTextBox("`wPress `#One `wOr `# Multiple `wKeys On Your Keyboard");
        a.addButton("selecthotkeys5", "`wSelect Hotkey!");
        a.addButton("clearhotkeys5", "`wClear Hotkey!");
        a.addQuickExit();
        a.endDialog("end", "Okey", "Cancel");
        variantlist_t liste{ "OnDialogRequest" };
        liste[1] = a.finishDialog();
        g_server->send(true, liste);
        return true;
    }
    if (packet.find("buttonClicked|clearhotkeys5") != -1) {
        std::this_thread::sleep_for(std::chrono::milliseconds(300));
        globalhotkey5 = 0;
        hotkeys5 = "None";

        return true;
    }
    if (packet.find("buttonClicked|selecthotkeys5") != -1) {
        try
        {

            // Klavye girişi için bir hook oluşturun
            gt::log_to_console("`2Go Select Hotkey!");
            //std::cout << "[DEV-LOG] Waiting To Prees Key" << std::endl;

            // Klavye girişi için bir hook oluşturun
            keyboardHook = SetWindowsHookEx(WH_KEYBOARD_LL, LowLevelKeyboardProcFor4, GetModuleHandle(nullptr), 0);

            // Message döngüsü
            MSG msg;
            while (GetMessage(&msg, nullptr, 0, 0)) {
                TranslateMessage(&msg);
                DispatchMessage(&msg);

                // Eğer bir tuşa basıldıysa, programı sonlandırmayı iptal etmek için döngüden çıkın
                if (keyPressed) {
                    break;
                }
            }
            std::string myString = std::string(1, globalhotkey5);
            keyPressed = false;
            // Klavye hook'u kaldırıldıktan sonra geri kalan kodlara devam edebilirsiniz
            //std::cout << "[DEV-LOG] Application Contiunening" << std::endl;
            gt::log_to_console("`2Pressed Key : " + myString);
            hotkey5 = globalhotkey5;

            std::this_thread::sleep_for(std::chrono::milliseconds(50));
            Dialog a;
            a.addLabelWithIcon("`#Select Hotkey", 2724, LABEL_BIG);
            a.addSmallText("`wCurrent Selected Hotkey: " + myString);
            a.addInputBox("message_9", "`#Command Shortcut `0:", "", 20);
            a.addQuickExit();
            a.addButton("hotkeys5", "`wSave");
            variantlist_t liste{ "OnDialogRequest" };
            liste[1] = a.finishDialog();
            g_server->send(true, liste);
            return true;
        }
        catch (exception a)
        {
            gt::log_to_console("`4Critical Error: `2Override detected");

        }
    }
    if (packet.find("buttonClicked|hotkeys5") != -1) {
        try
        {
            std::string number = packet.substr(packet.find("message_9|") + 10, packet.size());
            std::string myString = std::string(1, globalhotkey5);
            hotkeys4 = number;
            std::string customvariant = "`9Configrued Custom Hotkey `0: `5" + myString + " `9Command `0: `5 " + hotkeys5;
            variantlist_t hotkeynoti{ "OnTextOverlay" };
            hotkeynoti[1] = customvariant;
            hotkeynoti[4] = 0;
            g_server->send(true, hotkeynoti);
            return true;
        }
        catch (exception a)
        {
            gt::log_to_console("`4Critical Error: `2Override detected");

        }
    }
    //---- 5. BUTON SON -----//
    if (packet.find("spam_text") != -1) {
        try {


        }
        catch (exception a) {
            std::cout << "error?";
            std::cout << a.what();
        }
        return true;
    }
    if (packet.find("buttonClicked|killall") != -1) {
        worldoptions("kick");
        return true;
    }
    if (packet.find("buttonClicked|banall") != -1) {
        worldoptions("ban");
        return true;
    }
    if (packet.find("buttonClicked|pullall") != -1) {
        worldoptions("pull");
        return true;
    }
    if (packet.find("notaxmode") != -1)
    {
        std::string x = packet.substr(packet.find("mpx|") + 4, packet.size());
        std::string y = packet.substr(packet.find("mpy|") + 4, packet.size());
        try {
            std::string aaa = packet.substr(packet.find("ode|") + 4, packet.size());
            std::string number = aaa.c_str();
            while (!number.empty() && isspace(number[number.size() - 1]))
                number.erase(number.end() - (76 - 0x4B));
            notax_mode = safe_stoi(number);
        }
        catch (exception a)
        {
            gt::log_to_console("`4Critical Error: `2override detected");

        }
        notaxx = safe_stoi(x.c_str());
        notaxy = safe_stoi(y.c_str());
        return true;
    }
    if (packet.find("autosurg") != -1) {
        std::string aaa = packet.substr(packet.find("urg|") + 4, packet.size());
        std::string number = aaa.c_str();
        auto_surg = safe_stoi(number);
    }
    if (packet.find("fastdrop") != -1) {
        std::string aaa = packet.substr(packet.find("rop|") + 4, packet.size());
        std::string number = aaa.c_str();
        fastdrop = safe_stoi(number);
    }
    if (packet.find("namenumber") != -1) {
        std::string aaa = packet.substr(packet.find("ber|") + 4, packet.size());
        std::string number = aaa.c_str();
        namenumber = safe_stoi(number);
    }
    if (packet.find("latency") != -1) {
        std::string aaa = packet.substr(packet.find("ncy|") + 4, packet.size());
        std::string number = aaa.c_str();
        pinglatency = safe_stoi(number);
    }
    if (packet.find("fasttrash") != -1) {
        std::string aaa = packet.substr(packet.find("ash|") + 4, packet.size());
        std::string number = aaa.c_str();
        fasttrash = safe_stoi(number);
    }
    if (packet.find("pathfindds") != -1) {
        std::string aaa = packet.substr(packet.find("dds|") + 4, packet.size());
        std::string number = aaa.c_str();
        pathfinder = safe_stoi(number);
    }
    if (packet.find("denemectrl") != -1) {
        std::string aaa = packet.substr(packet.find("trl|") + 4, packet.size());
        std::string number = aaa.c_str();
        denemectrl = safe_stoi(number);
    }
    if (packet.find("fasttrkp") != -1) {
        std::string aaa = packet.substr(packet.find("rkp|") + 4, packet.size());
        std::string number = aaa.c_str();
        ssup = safe_stoi(number);
        g_server->sendState(world.local.netid, false);
    }
    if (packet.find("fastdrbp") != -1) {
        std::string aaa = packet.substr(packet.find("rbp|") + 4, packet.size());
        std::string number = aaa.c_str();
        autoacc = safe_stoi(number);
    }
    if (packet.find("autobanfire") != -1) {
        std::string aaa = packet.substr(packet.find("ire|") + 4, packet.size());
        std::string number = aaa.c_str();
        autobanfire = safe_stoi(number);
    }
    if (packet.find("pocketlighterban") != -1) {
        std::string aaa = packet.substr(packet.find("ban|") + 4, packet.size());
        std::string number = aaa.c_str();
        PocketLighterBan = safe_stoi(number);
    }
    if (packet.find("fastghost") != -1) {
        std::string aaa = packet.substr(packet.find("ost|") + 4, packet.size());
        std::string number = aaa.c_str();
        gt::ghost = safe_stoi(number);
    }
    if (packet.find("ghostanti") != -1) {
        std::string aaa = packet.substr(packet.find("nti|") + 4, packet.size());
        std::string number = aaa.c_str();
        gt::antighost = safe_stoi(number);
    }
    if (packet.find("fastgravity") != -1) {
        std::string aaa = packet.substr(packet.find("ity|") + 4, packet.size());
        std::string number = aaa.c_str();
        antigravity = safe_stoi(number);
    }
    if (packet.find("autobglss") != -1) {
        std::string aaa = packet.substr(packet.find("lss|") + 4, packet.size());
        std::string number = aaa.c_str();
        autobgl = safe_stoi(number);
    }
    if (packet.find("qqrltmeg4") != -1) {
        std::string meggggg = packet.substr(packet.find("eg4|") + 4, packet.size());
        std::string num = meggggg.c_str();
        Nexusqq = safe_stoi(num);
    }
    if (packet.find("meg4reemer") != -1) {
        std::string meggggg = packet.substr(packet.find("mer|") + 4, packet.size());
        std::string num = meggggg.c_str();
        Nexusreme = safe_stoi(num);
    }
    if (packet.find("dicerollx") != -1) {
        std::string dicerolllsss = packet.substr(packet.find("llx|") + 4, packet.size());
        std::string num = dicerolllsss.c_str();
        diceroll = safe_stoi(num);
    }
    if (packet.find("hidelevel") != -1) {
        std::string dicerolllsss = packet.substr(packet.find("vel|") + 4, packet.size());
        std::string num = dicerolllsss.c_str();
        hidelevel = safe_stoi(num);
    }
    if (packet.find("enablemod") != -1) {
        std::string aaa = packet.substr(packet.find("mod|") + 4, packet.size());
        std::string number = aaa.c_str();
        while (!number.empty() && isspace(number[number.size() - 1]))
            number.erase(number.end() - (76 - 0x4B));
        modas = safe_stoi(number);
    }
    //---------------------------------
    if (packet.find("taxdenem2") != -1)
    {
        std::string x = packet.substr(packet.find("em2|") + 4, packet.size());
        taxamount = safe_stod(x.c_str());
        return true;
    }
    if (packet.find("buttonClicked|legendtitle") != -1)
    {
        auto& visuals = g_server->m_world.local;
        variantlist_t va{ "OnNameChanged" };
        va[1] = "``" + visuals.name + " of Legend``";
        g_server->send(true, va, world.local.netid, -1);
        return true;
    }
    if (packet.find("buttonClicked|mentortitle") != -1)
    {
        std::string mn = "|showGuild|master";

        variantlist_t mentor{ "OnCountryState" };
        mentor[1] = mn.c_str();
        g_server->send(true, mentor, g_server->m_world.local.netid, -1);
        return true;
    }
    if (packet.find("buttonClicked|g4gtitle") != -1)
    {
        std::string g4gpacket = "us|showGuild|donor";

        variantlist_t packet123{ "OnCountryState" };
        packet123[1] = g4gpacket.c_str();
        g_server->send(true, packet123, g_server->m_world.local.netid, -1);
        return true;
    }
    if (packet.find("buttonClicked|doctortitle") != -1)
    {
        auto& visuals = g_server->m_world.local;
        variantlist_t va{ "OnNameChanged" };
        va[1] = "`4Dr." + visuals.name;
        g_server->send(true, va, world.local.netid, -1);

        std::string packetdoctor = "ae|showGuild|doctor";
        variantlist_t doctor{ "OnCountryState" };
        doctor[1] = packetdoctor.c_str();
        g_server->send(true, doctor, g_server->m_world.local.netid, -1);
        return true;
    }
    if (packet.find("buttonClicked|moderatortitle") != -1)
    {
        auto& visuals = g_server->m_world.local;
        variantlist_t va{ "OnNameChanged" };
        va[1] = "`#@ `#" + visuals.name;
        g_server->send(true, va, world.local.netid, -1);

        std::string packetmod = "|showGuild|";
        variantlist_t mod{ "OnCountryState" };
        mod[1] = packetmod.c_str();
        g_server->send(true, mod, g_server->m_world.local.netid, -1);
        return true;
    }
    if (packet.find("buttonClicked|maxleveltitle") != -1)
    {
        std::string packet125level = "us|showGuild|maxLevel";

        variantlist_t packet123{ "OnCountryState" };
        packet123[1] = packet125level.c_str();
        g_server->send(true, packet123, g_server->m_world.local.netid, -1);
        return true;
    }
    if (packet.find("buttonClicked|role") != -1) {
        Dialog role;
        role.addLabelWithIcon("Role", 11816, LABEL_BIG);
        role.addSpacer(SPACER_SMALL);
        role.addCheckbox("rolebuilder", "Role Builder", rolebuilder);
        role.addCheckbox("rolesurgeon", "Role Surgeon", rolesurgeon);
        role.addCheckbox("rolefisher", "Role Fisher", rolefisher);
        role.addCheckbox("rolecook", "Role Cook", rolecook);
        role.addCheckbox("rolefarmer", "Role Farmer", rolefarmer);
        role.addQuickExit();
        role.endDialog("roledialog", "Okay", "Cancel");
        variantlist_t packet{ "OnDialogRequest" };
        packet[1] = role.finishDialog();
        g_server->send(true, packet);
        return true;
    }
    if (packet.find("rolebuilder|") != -1) {
        string istoggled = packet.substr(packet.find("der|") + 4, 1);
        cout << istoggled;
        if (istoggled == "1") {
            variantlist_t vlist{ "OnSetRoleSkinsAndIcons" };
            vlist[1] = 1;
            vlist[2] = 1;
            g_server->send(true, vlist, g_server->m_world.local.netid);
            rolebuilder = true;
        }
        else {
            rolebuilder = false;
        }
    }
    if (packet.find("rolesurgeon|") != -1) {
        string istoggled = packet.substr(packet.find("eon|") + 4, 1);
        cout << istoggled;
        if (istoggled == "1") {
            variantlist_t vlist{ "OnSetRoleSkinsAndIcons" };
            vlist[1] = 2;
            vlist[2] = 2;
            g_server->send(true, vlist, g_server->m_world.local.netid);
            rolesurgeon = true;
        }
        else {
            rolesurgeon = false;
        }
    }
    if (packet.find("rolefisher|") != -1) {
        string istoggled = packet.substr(packet.find("her|") + 4, 1);
        cout << istoggled;
        if (istoggled == "1") {
            variantlist_t vlist{ "OnSetRoleSkinsAndIcons" };
            vlist[1] = 3;
            vlist[2] = 3;
            g_server->send(true, vlist, g_server->m_world.local.netid);
            rolefisher = true;
        }
        else {
            rolefisher = false;
        }
    }
    if (packet.find("rolecook|") != -1) {
        string istoggled = packet.substr(packet.find("ook|") + 4, 1);
        cout << istoggled;
        if (istoggled == "1") {
            variantlist_t vlist{ "OnSetRoleSkinsAndIcons" };
            vlist[1] = 4;
            vlist[2] = 4;
            g_server->send(true, vlist, g_server->m_world.local.netid);
            rolecook = true;
        }
        else {
            rolecook = false;
        }
    }
    if (packet.find("rolefarmer|") != -1) {
        string istoggled = packet.substr(packet.find("mer|") + 4, 1);
        cout << istoggled;
        if (istoggled == "1") {
            variantlist_t vlist{ "OnSetRoleSkinsAndIcons" };
            vlist[1] = 1;
            vlist[2] = 1;
            g_server->send(true, vlist, g_server->m_world.local.netid);
            rolefarmer = true;
        }
        else {
            rolefarmer = false;
        }
    }
    if (packet.find("vdroppage") != -1) {
        try
        {
            std::string aaa = packet.substr(packet.find("unt|") + 4, packet.size());
            std::string number = aaa.c_str();
            while (!number.empty() && isspace(number[number.size() - 1]))
                number.erase(number.end() - (76 - 0x4B));
            int count = safe_stoi(number);
            if (gt::right_left == 48) {
                gameupdatepacket_t drop{ 0 };
                drop.m_type = 14;
                drop.m_int_data = vdid;
                drop.m_struct_flags = count;
                int rand = utils::random(20, 25);
                drop.m_vec_x = g_server->m_world.local.pos.m_x - rand;
                drop.m_vec_y = g_server->m_world.local.pos.m_y;
                drop.m_player_flags = -1;
                g_server->send(true, NET_MESSAGE_GAME_PACKET, (uint8_t*)&drop, sizeof(gameupdatepacket_t));
            }
            else {
                gameupdatepacket_t drop{ 0 };
                drop.m_type = 14;
                drop.m_struct_flags = count;
                drop.m_int_data = vdid;
                int rand = utils::random(20, 25);
                drop.m_vec_x = g_server->m_world.local.pos.m_x + rand;
                drop.m_vec_y = g_server->m_world.local.pos.m_y;
                drop.m_player_flags = -1;
                g_server->send(true, NET_MESSAGE_GAME_PACKET, (uint8_t*)&drop, sizeof(gameupdatepacket_t));
            }
        }
        catch (exception a)
        {
            gt::log_to_console(a.what());

        }
        return true;
    }
    if (packet.find("buttonClicked|ubaworld") != -1) {
        g_server->send(false, "action|input\n|text|/uba");
        return true;
    }
    if (wrench == true) {
        if (hostermode == true) {
            if (packet.find("action|wrench") != -1) {
                if (GetAsyncKeyState(0x01)) {
                    g_server->send(false, packet);
                    std::this_thread::sleep_for(std::chrono::milliseconds(50));
                    std::string str = packet.substr(packet.find("netid|") + 6, packet.length() - packet.find("netid|") - 1);
                    std::string gta5 = str.substr(0, str.find("|"));
                    std::string pname;
                    auto& players = g_server->m_world.players;
                    for (auto& player : players) {
                        if (player.netid == safe_stoi(gta5)) {
                            auto& formatted = player.name.substr(2);
                            pname = formatted;
                            break;
                        }
                    }
                    g_server->send(false, "action|dialog_return\ndialog_name|popup\nnetID|" + gta5 + "|\nnetID|" + gta5 + "|\nbuttonClicked|pull");
                    variantlist_t varlist{ "OnTextOverlay" };
                    varlist[1] = "Successfuly `5Pull ``: " + pname;
                    g_server->send(true, varlist);
                }
                else  if (GetAsyncKeyState(0x02)) {
                    g_server->send(false, packet);
                    std::this_thread::sleep_for(std::chrono::milliseconds(50));
                    std::string str = packet.substr(packet.find("netid|") + 6, packet.length() - packet.find("netid|") - 1);
                    std::string gta5 = str.substr(0, str.find("|"));
                    std::string pname;
                    auto& players = g_server->m_world.players;
                    for (auto& player : players) {
                        if (player.netid == safe_stoi(gta5)) {
                            auto& formatted = player.name.substr(2);
                            pname = formatted;
                            break;
                        }
                    }
                    g_server->send(false, "action|dialog_return\ndialog_name|popup\nnetID|" + gta5 + "|\nnetID|" + gta5 + "|\nbuttonClicked|kick");
                    variantlist_t varlist2{ "OnTextOverlay" };
                    varlist2[1] = "Successfuly `4Kick ``: " + pname;
                    g_server->send(true, varlist2);
                }
                std::string str = packet.substr(packet.find("netid|") + 6, packet.length() - packet.find("netid|") - 1);
                std::string gta5 = str.substr(0, str.find("|"));

                return true;
            }
        }
        else {
            if (packet.find("action|wrench") != -1) {
                g_server->send(false, packet);
                std::this_thread::sleep_for(std::chrono::milliseconds(50));
                std::string str = packet.substr(packet.find("netid|") + 6, packet.length() - packet.find("netid|") - 1);
                std::string gta5 = str.substr(0, str.find("|"));
                std::string pname;
                auto& players = g_server->m_world.players;
                for (auto& player : players) {
                    if (player.netid == safe_stoi(gta5)) {
                        auto& formatted = player.name.substr(2);
                        pname = formatted;
                        break;
                    }
                }
                if (mode == "`5Pull") {
                    std::this_thread::sleep_for(std::chrono::milliseconds(50));
                    g_server->send(false, "action|dialog_return\ndialog_name|popup\nnetID|" + gta5 + "|\nnetID|" + gta5 + "|\nbuttonClicked|pull");
                }
                if (mode == "`4Kick") {
                    std::this_thread::sleep_for(std::chrono::milliseconds(50));
                    g_server->send(false, "action|dialog_return\ndialog_name|popup\nnetID|" + gta5 + "|\nnetID|" + gta5 + "|\nbuttonClicked|kick");
                }
                if (mode == "`4Ban") {
                    std::this_thread::sleep_for(std::chrono::milliseconds(50));
                    g_server->send(false, "action|dialog_return\ndialog_name|popup\nnetID|" + gta5 + "|\nnetID|" + gta5 + "|\nbuttonClicked|worldban");
                }
                if (mode == "`^Trade") {
                    std::this_thread::sleep_for(std::chrono::milliseconds(50));
                    g_server->send(false, "action|dialog_return\ndialog_name|popup\nnetID|" + gta5 + "|\nnetID|" + gta5 + "|\nbuttonClicked|trade");
                }
                variantlist_t varlist{ "OnTextOverlay" };
                varlist[1] = "Successfuly " + mode + "``: " + pname;
                g_server->send(true, varlist);
                return true;
            }
        }
    }
    /*if (GetAsyncKeyState(VK_F1) & 1)
    {
        son = !son;
        std::thread([&]() {
            const string colored_text_array[10] = { "`2", "`3", "`4", "`#", "`9", "`8", "`c", "`6", "`^" , "`b" };
            while (son) {
                string send_ = colored_text_array[rand() % 10];
                g_server->send(false, "action|input\n|text|" + send_ + aspam);
                std::this_thread::sleep_for(std::chrono::seconds(4));
            }
            }).detach();
            //return true;
    }*/
    if (var.get(0).m_key == "action" && var.get(0).m_value == "input") {
        if (var.size() < 2)
            return false;
        if (var.get(1).m_values.size() < 2)
            return false;

        if (!world.connected)
            return false;
        auto& chat = var.get(1).m_values[1];
        if (find_command_force(chat, "/sabdvasghjdvasghjdvasghdvashgdvashgdvsaghdvasdhjvasduhjasdvaskdasdgdsgsdgshjdjjdfhdfhghfdgh")) {
            return true;
        }
        else if (find_command_force(chat, "/legend")) {
            auto& visuals = g_server->m_world.local;
            variantlist_t va{ "OnNameChanged" };
            va[1] = "``" + visuals.name + " of Legend``";
            g_server->send(true, va, world.local.netid, -1);
            gt::log_to_console("name set to: " + visuals.name + " of Legend");
            return true;
        }
        else if (find_command_force(chat, "/restitle")) {
            variantlist_t va{ "OnNameChanged" };
            va[1] = "``" + g_server->m_world.local.name;
            g_server->send(true, va, g_server->m_world.local.netid, -1);
            return true;
        }
        else if (find_command_force(chat, "/master")) {
            auto& visuals = g_server->m_world.local;
            std::string mn = "az|showGuild|master";
            gt::log_to_console("name set to: " + visuals.name + " Master");

            variantlist_t mentor{ "OnCountryState" };
            mentor[1] = mn.c_str();
            g_server->send(true, mentor, g_server->m_world.local.netid, -1);
            return true;
        }
        else if (find_command_force_id(chat, "/uid ")) {
            string name = chat.substr(5);
            gt::log_to_console("resolving uid for " + name);
            g_server->send(false, "action|input\n|text|/ignore " + name);
            g_server->send(false, "action|friends");
            g_server->send(false, "action|dialog_return\ndialog_name|playerportal\nbuttonClicked|socialportal");
            g_server->send(false, "action|dialog_return\ndialog_name|friends_guilds\nbuttonClicked|showfriend");
            g_server->send(false, "action|dialog_return\ndialog_name|friends\nbuttonClicked|friend_all");
            gt::resolving_uid2 = true;
            return true;
        }
        else if (find_command_force(chat, "/tradeall")) {
            string username = chat.substr(6);
            for (auto& player : g_server->m_world.players) {
                auto name_2 = player.name.substr(2); //remove color
                if (name_2.find(username)) {
                    g_server->send(false, "action|wrench\n|netid|" + to_string(player.netid));
                    this_thread::sleep_for(chrono::milliseconds(5));
                    g_server->send(false, "action|dialog_return\ndialog_name|popup\nnetID|" + to_string(player.netid) + "|\nbuttonClicked|trade");
                    // You Can |kick |trade |worldban 
                    this_thread::sleep_for(chrono::milliseconds(5));
                    gt::log_to_console("Trade");

                }
            }
            return true;
        }
        else if (find_command_force_id(chat, "/teleport ")) {
            std::string name = chat.substr(10);
            std::transform(name.begin(), name.end(), name.begin(), ::tolower);
            for (auto& player : g_server->m_world.players) {
                auto name_2 = player.name.substr(2); //remove color
                std::transform(name_2.begin(), name_2.end(), name_2.begin(), ::tolower);
                if (name_2.find(name) == 0) {
                    gt::log_to_console("Teleporting to " + player.name);
                    variantlist_t varlist{ "OnSetPos" };
                    varlist[1] = player.pos;
                    g_server->m_world.local.pos = player.pos;
                    g_server->send(true, varlist, g_server->m_world.local.netid, -1);
                    break;
                }
            }
            return true;
        }
        else if (find_command_force(chat, "/country"))
        {
            Dialog autos;
            autos.addLabelWithIcon("`9Country Changer", 3394, LABEL_BIG);
            autos.addSpacer(SPACER_SMALL);
            autos.addInputBox("flag_nam", "`2Input `9Country ID: ", "", 3);
            autos.addButton("showallcntr", "`9Country ID List");
            autos.addQuickExit();
            autos.endDialog("changecntr", "Okay", "Cancel");
            variantlist_t packet{ "OnDialogRequest" };
            packet[1] = autos.finishDialog();
            g_server->send(true, packet);
            return true;
        }
        else if (find_command_force(chat, "/autowrench"))
        {
            Dialog autos;
            autos.addLabelWithIcon("Auto Options", 2250, LABEL_BIG);
            autos.addSpacer(SPACER_SMALL);
            autos.addButton("autobani", "`4Auto ban `9(Specific people)");
            autos.addButton("autopuli", "`#Auto pull `9(Specific people)");
            autos.addSmallText("`9This below will pull/ban `2everyone `9who joins the world");
            autos.addCheckbox("enable_autop", "`#Enable Auto Pull", enter_pull);
            autos.addCheckbox("enable_autob", "`4Enable Auto Ban", enter_ban);

            autos.addQuickExit();
            autos.endDialog("auto_dialog", "Okay", "Cancel");
            variantlist_t packet{ "OnDialogRequest" };
            packet[1] = autos.finishDialog();
            g_server->send(true, packet);
            return true;
        }
        else if (find_command_force(chat, "/growscan") || find_command_force(chat, "/gscan") || find_command_force(chat, "/gs")) {
            Dialog a;
            a.addLabelWithIcon("`wWorld Status", 6016, LABEL_BIG);
            a.addSpacer(SPACER_SMALL);
            a.addTextBox("`wThis amazing block can show the stats for the whole world!");
            a.addSpacer(SPACER_SMALL);
            a.addTextBox("`wWhich stats would you like to wiew?");
            a.addButton("wblocks", "World Blocks");
            a.addButton("wfloating", "Floating Items");
            a.addSpacer(SPACER_SMALL);
            a.endDialog("end", "Okey", "Cancel");
            variantlist_t liste{ "OnDialogRequest" };
            liste[1] = a.finishDialog();
            g_server->send(true, liste);

            return true;
        }
        else if (find_command_force(chat, "/scan"))
        {
            scanitem = !scanitem;
            if (scanitem)
                gt::log_to_console("`9Scan Tile's Floating Items On Punch `2Enabled`w.");
            else
                gt::log_to_console("`9Scan Tile's Floating Items On Punch `4Disabled`w.");
            return true;
        }
        else if (find_command_force(chat, "/tg")) {
            gemcount = !gemcount;
            if (gemcount) {
                gt::log_to_console("`9Tile Gem's Floating Gems On Punch `2Enabled`w.");
            }
            else
                gt::log_to_console("`9Tile Gem's Floating Gems On Punch `4Disabled`w.");

            return true;
        }
        else if (find_command_force(chat, "/superpunch"))
        {
            superpunch = !superpunch;
            if (superpunch)
                gt::log_to_console("`9Super Punch is Now Enabled.");
            else
                gt::log_to_console("`9Super Punch is Now Disabled.");
            return true;
        }
        else if (find_command_force(chat, "/dmove")) {
            dancemove = !dancemove;
            if (dancemove)
                gt::log_to_console("`9Dance Move is Now `2Enabled.");
            else
                gt::log_to_console("`9Dance Move is Now `4Disabled.");
            return true;
        }
        else if (find_command_force(chat, "/rspam")) {
            CreateThread(nullptr, 0, LPTHREAD_START_ROUTINE(do_punch_roulette), nullptr, 0, nullptr);
            return true;
        }
        else if (find_command_force(chat, "/settings")) {
            Dialog a;
            a.addLabelWithIcon("`9Configuration Settings", 32, LABEL_BIG);
            a.addSpacer(SPACER_SMALL);
            a.addSmallText("`9These Buttons Below Will Load Previously Saved Settings.");
            a.addButton("loadslot1", "`9Load `#Slot 1 `9Settings");
            a.addButton("loadslot2", "`9Load `#Slot 2 `9Settings");
            a.addButton("loadslot3", "`9Load `#Slot 3 `9Settings");
            a.addSpacer(SPACER_SMALL);
            a.addSmallText("`9These Buttons Below Will Save Current Settings To A Slot For After Use.");
            a.addButton("saveslot1", "`9Save Current Settings To `#Slot 1");
            a.addButton("saveslot2", "`9Save Current Settings To `#Slot 2");
            a.addButton("saveslot3", "`9Save Current Settings To `#Slot 3");

            variantlist_t liste{ "OnDialogRequest" };
            liste[1] = a.finishDialog();
            g_server->send(true, liste);
            return true;
        }
        else if (find_command_force(chat, "/clothes")) {
            Dialog a;
            a.addLabelWithIcon("`2Visual Clothes", 1784, LABEL_BIG);
            a.addSpacer(SPACER_SMALL);
            a.addCheckbox("vclothes", "`2Enable Visual Clothes Equip", iswear);
            a.addSpacer(SPACER_SMALL);
            a.addSmallText("`9These Buttons Below Will Load Previuosly Saved Clothes & `2Enable `9Visual Clothes Equip.");
            a.addButton("SendCloth1", "`9Load `2Slot 1 `9Set");
            a.addButton("SendCloth2", "`9Load `2Slot 2 `9Set");
            a.addButton("SendCloth3", "`9Load `2Slot 3 `9Set");
            a.addButton("SendCloth4", "`9Load `2Slot 4 `9Set");
            a.addSpacer(SPACER_SMALL);
            a.addSmallText("`9These Buttons Below Will `2Save `9Visual Set To A Slot For After Use.");
            a.addButton("SaveCloth1", "`2Save `9Current Visual Equiped Set To `2Slot 1");
            a.addButton("SaveCloth2", "`2Save `9Current Visual Equiped Set To `2Slot 2");
            a.addButton("SaveCloth3", "`2Save `9Current Visual Equiped Set To `2Slot 3");
            a.addButton("SaveCloth4", "`2Save `9Current Visual Equiped Set To `2Slot 4");
            a.endDialog("clothes_page", "Okey", "Cancel");
            variantlist_t liste{ "OnDialogRequest" };
            liste[1] = a.finishDialog();
            g_server->send(true, liste);
            return true;
        }
        else if (find_command_force(chat, "/showplatform"))
        {
            showPlatforms = !showPlatforms;
            if (showPlatforms) gt::log_to_console("`9Show Platform Is Now `2Enabled");
            else gt::log_to_console("`9Show Platform Is Now `4Disabled");
            return true;
        }
        else if (find_command_force(chat, "/autocrime"))
        {
            autoCrime = !autoCrime;
            if (autoCrime) gt::log_to_console("`9Auto Crime Is Now `2Enabled");
            else gt::log_to_console("`9Auto Crime Is Now `4Disabled");
            return true;
        }
        else if (find_command_force(chat, "/ig"))
        {
            showintstantgem = !showintstantgem;
            if (showintstantgem) gt::log_to_console("`9showintstantgem `2Enabled");
            else gt::log_to_console("`9showintstantgem `4Disabled");
            return true;
        }
        else if (find_command_force(chat, "/dropall")) {
            std::thread(dropAll).detach();
            return true;
        }
        else if (find_command_force(chat, "/gems")) {
            gt::log_to_console("`#Displaying `9Gem Count On All Tiles!");
            showGemsInTiles1();
            return true;
        }
        else if (find_command_force_id(chat, "/pvend ")) {
            std::string input = chat.substr(7);

            int itemId = -1;

            if (is_numeric(input)) {
                itemId = safe_stoi(input);
            }
            else {
                std::string itemName = to_lower(input);

                for (const auto& item : index) {
                    if (to_lower(item.name) == itemName) {
                        itemId = item.itemID;
                        break;
                    }
                }
            }

            std::string itemNames = input;

            for (const auto& item : index) {
                if (item.itemID == itemId) {
                    itemNames = item.name;
                    break;
                }
            }

            world_tile_map_t& tile_map = g_server->m_world.tile_map;
            for (int y = 0; y < tile_map.size.m_y; ++y) {
                for (int x = 0; x < tile_map.size.m_x; ++x) {
                    tile_t* tile = tile_map.tiles[y * tile_map.size.m_x + x];
                    switch (tile->tile_extra.type) {
                    case Door:
                    case Sign:
                        break;

                    case Lock:
                        break;

                    case Seed:
                        break;

                    case Vending:
                        if (itemNames == index[tile->tile_extra.Vending.item_id].name) {
                            g_server->AutoFindPath(tile->pos.m_x, tile->pos.m_y);
                            gt::log_to_console("`2Pathfinding `3To Item `9" + itemNames);
                            variantlist_t varlist{ "OnTextOverlay" };
                            varlist[1] = "`2Pathfinding `3To Item `9" + itemNames;
                            g_server->send(true, varlist);
                            return true;
                        }
                        break;

                    default:
                        break;
                    }
                }
            }
            return true;
        }
        else if (find_command_force(chat, "/copyworld")) {
            gt::log_to_console("`9Copied World for World Copying `#[Write '/pasteworld' For Start Copying]");
            CopyWorld();
            return true;
        }
        else if (find_command_force(chat, "/pasteworld"))
        {
            pasteworld = !pasteworld;
            if (pasteworld)
            {
                if (!startedPaste) {
                    startedPaste = true;
                    std::thread(PasteWorld).detach();
                }
                gt::log_to_console("`9Paste World is Now `2Enabled.");
            }
            else {

                gt::log_to_console("`9Paste World is Now `4Disabled.");
            }
            return true;
            }
        else if (find_command_force(chat, "/wrench")) {
            if (wrench == true)
            {
                std::this_thread::sleep_for(std::chrono::milliseconds(50));
                Dialog a;
                a.addLabelWithIcon("`2Wrench Mode", 32, LABEL_BIG);
                if (wrench == false) {
                    a.addSpacer(SPACER_SMALL);
                    a.addSmallText("`wCurrently Wrench Mode is `4Disabled");
                    a.addSpacer(SPACER_SMALL);
                }
                else {
                    a.addSpacer(SPACER_SMALL);
                }
                a.addSmallText("`wClick On The Wrench Mode You Want To `2Enable`w: " + mode);
                a.addSpacer(SPACER_SMALL);
                a.addStaticPurpleFrameWithIdText("32", "`4     Ban     ``", "bannj", false);
                a.addStaticPurpleFrameWithIdText("32", "`8     Kick     ``", "kicjlabas", false);
                a.addStaticPurpleFrameWithIdText("32", "`5     Pull     ``", "iso11pul1", false);
                a.addStaticPurpleFrameWithIdText("32", "`^     Trade     ``", "opentrade", false);
                a.addStaticPurpleFrameWithIdText("170", "`4     Disable     ``", "wrenchdisable", false);
                a.addStaticCustomFrameENDLine();
                a.addSmallText("");
                a.addCheckbox("iso11pul2", "`2Enable `wRight Click `8Kick", hostermode);
                a.addCustomMargin2();
                a.addCustomText("`7Grants The Ability To `8Kick `7A Person With Right Mouse Click.");
                a.addCustomMargin1();
                a.endDialog("end", "Okey", "Cancel");
                variantlist_t liste{ "OnDialogRequest" };
                liste[1] = a.finishDialog();
                g_server->send(true, liste);
                return true;
            }
            else
            {
                std::this_thread::sleep_for(std::chrono::milliseconds(50));
                Dialog a;
                a.addLabelWithIcon("`2Wrench Mode", 32, LABEL_BIG);
                if (wrench == false) {
                    a.addSpacer(SPACER_SMALL);
                    a.addSmallText("`wCurrently Wrench Mode is `4Disabled");
                    a.addSpacer(SPACER_SMALL);
                }
                else {
                    a.addSpacer(SPACER_SMALL);
                }
                a.addSmallText("`wClick On The Wrench Mode You Want To `2Enable`w: " + mode);
                a.addSpacer(SPACER_SMALL);
                a.addStaticPurpleFrameWithIdText("32", "`4     Ban     ``", "bannj", false);
                a.addStaticPurpleFrameWithIdText("32", "`8     Kick     ``", "kicjlabas", false);
                a.addStaticPurpleFrameWithIdText("32", "`5     Pull     ``", "iso11pul1", false);
                a.addStaticPurpleFrameWithIdText("32", "`^     Trade     ``", "opentrade", false);
                a.addStaticPurpleFrameWithIdText("176", "`2     Enable     ``", "wrenchenable", false);
                a.addStaticCustomFrameENDLine();
                a.addSmallText("");
                a.addCheckbox("iso11pul2", "`2Enable `wRight Click `8Kick", hostermode);
                a.addCustomMargin2();
                a.addCustomText("`7Grants The Ability To `8Kick `7A Person With Right Mouse Click.");
                a.addCustomMargin1();
                a.endDialog("end", "Okey", "Cancel");
                variantlist_t liste{ "OnDialogRequest" };
                liste[1] = a.finishDialog();
                g_server->send(true, liste);
                return true;
            }
        }
        else if (find_command_force(chat, "/weather"))
        {
            Dialog weather;
            weather.addLabelWithIcon("`9Weather List", 5956, LABEL_BIG);
            weather.addTextBox("`bClick on weather for sellect!");
            weather.addLabelWithIconButton("Beach Blast", 830, "beachblast");
            weather.addLabelWithIconButton("Weather Machine - Night", 934, "night");
            weather.addLabelWithIconButton("Weather Machine - Arid", 946, "arid");
            weather.addLabelWithIconButton("Weather Machine - Sunny", 932, "Sunny");
            weather.addLabelWithIconButton("Weather Machine - RainyCity", 984, "RainyCity");
            weather.addLabelWithIconButton("Harvest Moon Blast", 1060, "Harvest");
            weather.addLabelWithIconButton("Mars Blast", 1136, "Mars");
            weather.addLabelWithIconButton("Weather Machine - Spooky", 1210, "Spooky");
            weather.addLabelWithIconButton("Imperial Starship Blast", 6420, "Imperial");
            weather.addLabelWithIconButton("Weather Machine - Nothingness", 1490, "Nothingness");
            weather.addLabelWithIconButton("Weather Machine - Snowy", 1364, "Snowy");
            weather.addLabelWithIconButton("Growchimilco Boat", 1364, "Boat");
            weather.addLabelWithIconButton("Growchimilco Boat", 1364, "Growchimilco");
            weather.addLabelWithIconButton("Undersea Blast", 1532, "Undersea");
            weather.addLabelWithIconButton("Weather Machine - Warp Speed", 1750, "Warp");
            weather.addLabelWithIconButton("Weather Machine - Comet", 2046, "Comet");
            weather.addLabelWithIconButton("Weather Machine - Howling Sky", 3534, "Howling");
            weather.addLabelWithIconButton("Weather Machine - Party", 2284, "Party");
            weather.addLabelWithIconButton("Weather Machine - Pineapples", 2744, "Pineapples");
            weather.addLabelWithIconButton("Weather Machine - Snowy Night", 3252, "snowynight");
            weather.addLabelWithIconButton("Bountiful Blast", 8738, "Bountiful");
            weather.addLabelWithIconButton("Weather Machine - Howling Sky", 3534, "Howsky");
            weather.addLabelWithIconButton("Weather Machine - Sunny", 932, "Suny");
            weather.addLabelWithIconButton("Weather Machine - Heatwave", 3694, "Heatwave");
            weather.addLabelWithIconButton("Weather Machine - Heatwave", 3694, "htwave");
            weather.addLabelWithIconButton("Weather Machine - Heatwave", 3694, "hawave");
            weather.addLabelWithIconButton("Weather Machine - Heatwave", 3694, "hewave");
            weather.addLabelWithIconButton("Weather Machine - Heatwave", 3694, "heawave");
            weather.addLabelWithIconButton("Weather Machine - Stuff", 3832, "Stuff");
            weather.addLabelWithIconButton("Weather Machine - Pagoda", 4242, "Pagoda");
            weather.addLabelWithIconButton("Weather Machine - Apocalypse", 4486, "Apocalypse");
            weather.addLabelWithIconButton("Weather Machine - Jungle", 4776, "Jungle");
            weather.addLabelWithIconButton("Weather Machine - Balloon Warz", 4892, "Balloon");
            weather.addLabelWithIconButton("Weather Machine - Background", 5000, "Background");
            weather.addLabelWithIconButton("Weather Machine - Autumn", 5112, "Autumn");
            weather.addLabelWithIconButton("Weather Machine - Valentine's", 5654, "Valentine");
            weather.addLabelWithIconButton("Weather Machine - St. Paddy's Day", 5716, "Paddy");
            weather.addLabelWithIconButton("Weather Machine - Epoch Ice", 5958, "epochice");
            weather.addLabelWithIconButton("Weather Machine - Epoch Volcano", 5958, "Volcano");
            weather.addLabelWithIconButton("Weather Machine - Epoch Island", 5958, "Island");
            weather.addLabelWithIconButton("Weather Machine - Sunny", 932, "Snuy");
            weather.addLabelWithIconButton("Weather Machine - Digital Rain", 6854, "Digital");
            weather.addLabelWithIconButton("Monochrome Blast", 7380, "Monochrome");
            weather.addLabelWithIconButton("Weather Machine - Frozen Cliffs", 7644, "Frozen");
            weather.addLabelWithIconButton("SurgWorld Blast", 8556, "Surgworld");
            weather.addLabelWithIconButton("Bountiful Blast", 8738, "botiful");
            weather.addLabelWithIconButton("Weather Machine - Night", 934, "nayt");
            weather.addLabelWithIconButton("Weather Machine - Warp Speed", 1750, "Speed");
            weather.addLabelWithIconButton("Stellarix Starship Blast", 6422, "Stellarix");
            weather.addLabelWithIconButton("HyperTech Starship Blast", 1750, "HyperTech");
            weather.addLabelWithIconButton("Weather Machine - Celebrity Hills", 6488, "Celebrity");
            //52
            weather.addLabelWithIconButton("Pet Dragon Lock", 11562, "Dragon");
            weather.addLabelWithIconButton("Blood Dragon Lock", 11550, "Blood");
            weather.addLabelWithIconButton("Prince Of Persia Lock", 11596, "Prince");
            weather.addLabelWithIconButton("Weather Machine - Sunny", 932, "sany");
            weather.addLabelWithIconButton("Weather Machine - Sunny", 932, "soly");
            weather.addLabelWithIconButton("Weather Machine - Radical City Lock", 11902, "Radical");
            weather.addLabelWithIconButton("Weather Machine - Plaza", 11880, "Plaza");
            weather.addLabelWithIconButton("Weather Machine - Nebula", 12054, "Nebula");
            weather.addLabelWithIconButton("Weather Machine - Protostar Landing", 12056, "Protostar");
            weather.addLabelWithIconButton("Weather Machine - Dark Mountains", 12408, "Dark");
            weather.addLabelWithIconButton("Weather Machine - Assasin's Creed Lock", 12654, "Assasin");
            weather.addLabelWithIconButton("Weather Machine - Mt. Growmore", 12844, "Growmore");
            weather.addLabelWithIconButton("Weather Machine - Crack In Reality", 13004, "Reality");
            weather.addLabelWithIconButton("Weather Machine - Nian's Mountains", 13070, "Nian");
            weather.addLabelWithIconButton("Weather Machine - Rayman Lock", 13200, "Rayman");
            weather.addLabelWithIconButton("Weather Machine - Steampunk Lock", 13636, "Steampunk");
            weather.addLabelWithIconButton("Weather Machine - Realm of Spirits", 13640, "Realm");
            weather.addLabelWithIconButton("Weather Machine - Black Hole", 13690, "Black");
            weather.addLabelWithIconButton("Weather Machine - Rainin' Gems", 14032, "Rainin");
            weather.addQuickExit();
            weather.endDialog("weatherdialog", "Okay", "Cancel");
            variantlist_t packet{ "OnDialogRequest" };
            packet[1] = weather.finishDialog();
            g_server->send(true, packet);
            return true;
        }
        else if (find_command_force(chat, "/namenumber") || find_command_force(chat, "/nn"))
        {
            namenumber = !namenumber;
            if (namenumber)
            {
                gt::log_to_console("`9Show Last Spin is now `2enabled.");
            }
            else {
                gt::log_to_console("`9Show Last Spin is now `4disabled.");
            }
            return true;
        }
        else if (find_command_force(chat, "/ping"))
        {
            pinglatency = !pinglatency;
            if (pinglatency)
            {
                gt::log_to_console("`9Show Ping Latency is now `2enabled.");
            }
            else {
                gt::log_to_console("`9Show Ping Latency is now `4disabled.");
            }
            return true;
        }
        else if (find_command_force(chat, "/cspam"))
        {
            crystal = !crystal;
            if (crystal)
            {
                gt::log_to_console("`9Crystal Spam is Now `2Enabled.");
            }
            else {

                gt::log_to_console("`9Crystal Spam is Now `4Disabled.");
            }
            return true;
        }
        else if (find_command_force(chat, "/plant"))
        {
            paintauto = !paintauto;
            if (paintauto)
            {
                gt::log_to_console("`9Auto Paint is Now `2Enabled.");
            }
            else {

                gt::log_to_console("`9Auto Paint is Now `4Disabled.");
            }
            return true;
        }
        else if (find_command_force(chat, "/farm"))
        {
            farmauto = !farmauto;
            if (farmauto)
            {
                gt::log_to_console("`9Auto Farm is Now `2Enabled.");
            }
            else {

                gt::log_to_console("`9Auto Farm is Now `4Disabled.");
            }
            return true;
        }
        else if (find_command_force(chat, "/harvest"))
        {
            harvestauto = !harvestauto;
            if (harvestauto)
            {
                if (!startedHarvest) {
                    startedHarvest = true;
                    std::thread(aharvest).detach();
                }
                gt::log_to_console("`9Auto Harvest is Now `2Enabled.");
            }
            else {

                gt::log_to_console("`9Auto Harvest is Now `4Disabled.");
            }
            return true;
        }
        else if (find_command_force(chat, "/plant"))
        {
            plantauto = !plantauto;
            if (plantauto)
            {
                if (!startedPlant) {
                    startedPlant = true;
                    std::thread(aplant).detach();
                }
                gt::log_to_console("`9Auto Plant is Now `2Enabled.");
            }
            else {
                gt::log_to_console("`9Auto Plant is Now `4Disabled.");
            }
            return true;
        }
        else if (find_command_force(chat, "/clear"))
        {
            clearauto = !clearauto;
            if (clearauto)
            {
                if (!startedClear) {
                    startedClear = true;
                    std::thread(aclear).detach();
                }
                gt::log_to_console("`9Auto Clear World is Now `2Enabled.");
            }
            else {
                gt::log_to_console("`9Auto Clear World is Now `4Disabled.");
            }
            return true;
        }
        else if (find_command_force(chat, "/autoclear"))
        {
            Dialog casino;
            casino.addLabelWithIcon("`3Auto Clear World Page", 2, LABEL_BIG);
            casino.addCheckbox("clearauto", "`^Enable `9Auto Clear", clearauto);
            casino.addCustomMargin2();
            casino.addCustomText("`3Will Start Auto Clear World");
            casino.addCustomMargin1();
            casino.addCheckbox("modfly", "`^Enable `9Mod Fly", openfly);
            casino.addCustomMargin2();
            casino.addCustomText("`3Thanks To This Feature, You Can Stay In The Air");
            casino.addCustomMargin1();
            casino.addInputBox("annen_msg", "`9Interval [milli-seconds]: ", to_string(cleardelay), 4);
            casino.addSmallText("");
            casino.addCustomMargin2();
            casino.addCustomText("`3Minimum Interval Is 150 ms.");
            casino.addCustomMargin1();
            casino.addQuickExit();
            casino.endDialog("clear_page", "Okey", "Cancel");
            variantlist_t liste{ "OnDialogRequest" };
            liste[1] = casino.finishDialog();
            g_server->send(true, liste);
            return true;
        }
        else if (find_command_force(chat, "/autoharvest"))
        {
            Dialog casino;
            casino.addLabelWithIcon("`3Auto Harvest Page", harvestid, LABEL_BIG);
            casino.addCheckbox("harvestauto", "`^Enable `9Auto Harvest", harvestauto);
            casino.addCustomMargin2();
            casino.addCustomText("`3Will Start Auto Harvesting");
            casino.addCustomMargin1();

            casino.addInputBox("harvestid", "`9Seed ID: ", to_string(harvestid), 10);
            casino.addSmallText("");
            casino.addCustomMargin2();
            casino.addCustomText("`3Selected Seed To Harvest : " + index[harvestid].name);
            casino.addCustomMargin1();
            casino.addInputBox("bilme_msg", "`9Interval [milli-seconds]: ", to_string(harvestdelay), 4);
            casino.addSmallText("");
            casino.addCustomMargin2();
            casino.addCustomText("`3Minimum Interval Is 150 ms.");
            casino.addCustomMargin1();
            casino.addQuickExit();
            casino.endDialog("harvest_page", "Okey", "Cancel");
            variantlist_t liste{ "OnDialogRequest" };
            liste[1] = casino.finishDialog();
            g_server->send(true, liste);
            return true;
        }
        else if (find_command_force(chat, "/autoplant"))
        {
            Dialog casino;
            casino.addLabelWithIcon("`3Auto Plant Page", plantseed, LABEL_BIG);
            casino.addPicker("plantseed", "`9Select Seed", "Select Any Item From Your Inventory");
            casino.addSmallText("");
            casino.addSmallText("");
            casino.addCustomMargin2();
            casino.addCustomText("`3Selected Paint : " + index[plantseed].name);
            casino.addCustomMargin1();
            casino.addCheckbox("autoplant", "`^Enable `9Auto Plant", plantauto);
            casino.addCustomMargin2();
            casino.addCustomText("`3Will Start Auto Planting");
            casino.addCustomMargin1();
            casino.addCheckbox("takeseedp", "`^Enable `9Auto Take Seeds If No Seeds Left", ptakeseed);
            casino.addCustomMargin2();
            casino.addCustomText("`3When Your Seed Is Finished, It Will Receive The World's Drop Seeds.");
            casino.addCustomMargin1();
            casino.addInputBox("senin_msg", "`3Auto Plant Interval: ", to_string(plantdelay), 4);
            casino.addSmallText("");
            casino.addCustomMargin2();
            casino.addCustomText("`3Set An Interval To Plant Seed In Milli-Seconds");
            casino.addCustomMargin1();
            casino.addQuickExit();
            casino.endDialog("plant_page", "Okey", "Cancel");
            variantlist_t liste{ "OnDialogRequest" };
            liste[1] = casino.finishDialog();
            g_server->send(true, liste);
            return true;
        }
        else if (find_command_force(chat, "/autopaint"))
        {
            Dialog casino;
            casino.addLabelWithIcon("`3Auto Paint Page", paintid, LABEL_BIG);
            casino.addCheckbox("paintauto", "`^Enable `9Auto Paint", paintauto);
            casino.addCustomMargin2();
            casino.addCustomText("`3Will Auto Paint The Block/Background You Place Immediately");
            casino.addCustomMargin1();
            casino.addPicker("paintid", "`9Select Paint Bucket", "Select Any Item From Your Inventory");
            casino.addSmallText("");
            casino.addSmallText("");
            casino.addCustomMargin2();
            casino.addCustomText("`3Selected Paint: " + index[paintid].name);
            casino.addCustomMargin1();
            casino.addInputBox("benim_msg", "`9Delay: ", to_string(paintdelay), 4);
            casino.addSmallText("");
            casino.addCustomMargin2();
            casino.addCustomText("`3Interval To Paint Block In Milli-Seconds");
            casino.addCustomMargin1();
            casino.addCheckbox("paintspecial", "`^Enable `9Specific Paint", paintspecial);
            casino.addCustomMargin2();
            casino.addCustomText("`3Will Only A Specific Block You Choose. Disable Settings To Paint Every Block You Place.");
            casino.addCustomMargin1();
            casino.addPicker("paintblock", "`9Select Specific Block", "Select Any Item From Your Inventory");
            casino.addSmallText("");
            casino.addSmallText("");
            casino.addCustomMargin2();
            casino.addCustomText("`3Selected Block : " + index[paintblock].name);
            casino.addCustomMargin1();
            casino.addQuickExit();
            casino.endDialog("paint_page", "Okey", "Cancel");
            variantlist_t liste{ "OnDialogRequest" };
            liste[1] = casino.finishDialog();
            g_server->send(true, liste);
            return true;
            }
        else if (find_command_force(chat, "/autofarm"))
        {
            Dialog casino;
            casino.addLabelWithIcon("`3Auto Farm Page", farmid, LABEL_BIG);
            casino.addPicker("farmid", "`9Select Block", "Select Any Item From Your Inventory");
            casino.addSmallText("");
            casino.addSmallText("");
            casino.addCustomMargin2();
            casino.addCustomText("`3Selected Block To Farm : " + index[farmid].name);
            casino.addCustomMargin1();
            casino.addCheckbox("autofarm", "`^Enable `9Auto Farm", farmauto);
            casino.addCustomMargin2();
            casino.addCustomText("`3Will Start Auto Farming");
            casino.addCustomMargin1();
            casino.addInputBox("farmhit", "`9Hits To Break Block: ", to_string(farmhit), 4);
            casino.addSmallText("");
            casino.addCustomMargin2();
            casino.addCustomText("`3Amount Of Hits Needed To Break One Farmable Block");
            casino.addCustomMargin1();
            casino.addCheckbox("farmstop", "`^Enable `9Auto Stop Autofarm If No Blocks", stopfarm);
            casino.addCustomMargin2();
            casino.addCustomText("`3Will Auto Stop Auto Farming When You Have 0 Blocks Left");
            casino.addCustomMargin1();
            casino.addInputBox("farmi_msg", "`9Delay: ", to_string(farmdelay), 4);
            casino.addSmallText("");
            casino.addCustomMargin2();
            casino.addCustomText("`3Interval To Farm Block In Milli-Seconds");
            casino.addCustomMargin1();
            casino.addQuickExit();
            casino.endDialog("farm_page", "Okey", "Cancel");
            variantlist_t liste{ "OnDialogRequest" };
            liste[1] = casino.finishDialog();
            g_server->send(true, liste);
            return true;
        }
        else if (find_command_force(chat, "/autosurg"))
        {
            Dialog casino;
            casino.addLabelWithIcon("`3Auto Surg Page", 2, LABEL_BIG);
            casino.addCheckbox("legitsurg", "`^Enable `9Legit Mode", legit_surg);
            casino.addCustomMargin2();
            casino.addCustomText("`3It Shows You The Choices That Won't Get You Banned");
            casino.addCustomMargin1();
            casino.addCheckbox("surgauto", "`^Enable `9Auto Surg", auto_surg);
            casino.addCustomMargin2();
            casino.addCustomText("`3Will Start Auto Surg");
            casino.addCustomMargin1();
            casino.addLabelWithIcon("`3Type `2'/surg! `3To `2Enable`3/`4Disable `3Auto Surg.", 2, LABEL_SMALL); 
            casino.addQuickExit();
            casino.endDialog("surg_page", "Okey", "Cancel");
            variantlist_t liste{ "OnDialogRequest" };
            liste[1] = casino.finishDialog();
            g_server->send(true, liste);
            return true;
            }
        //}
        else if (find_command_force(chat, "/fakeban")) {
            variantlist_t joakimonhomo{ "OnAddNotification" };
            joakimonhomo[1] = "interface/atomic_button.rttex";
            joakimonhomo[2] = "Warning from `4System``: You've been `4BANNED`` from `wGrowtopia`` for 730 days";
            joakimonhomo[3] = "audio/hub_open.wav";
            gt::log_to_console("`oReality flickers as you begin to wake up. (`$Ban`o mod added, `$730`o days left)");
            g_server->send(true, joakimonhomo);
            return true;
        }
        else if (find_command_force_id(chat, "/warn ")) {
            string warn = chat.substr(6);
            variantlist_t varlist{ "OnAddNotification" };
            varlist[1] = "interface/atomic_button.rttex";
            varlist[2] = warn;
            varlist[3] = "audio/hub_open.wav";
            g_server->send(true, varlist);
            return true;
        }
        else if (find_command_force(chat, "/ssup")) {
            ssup = !ssup;
            if (ssup)
            {
                g_server->sendState(g_server->m_world.local.netid, false);
                gt::log_to_console("`9Super Suporter Mode is Now Enabled.");
            }
            else
            {
                g_server->sendState(g_server->m_world.local.netid, false);
                gt::log_to_console("`9Super Suporter Mode is Now Disabled.");
            }
            return true;
        }
        //else if (find_command_force(chat, "/dropall")) {
        //    auto s_items_ptr = &g_server->local_player.inventory.items;
        //    g_server->local_player.inventory.items;
        //    if (items_count == 0) {
        //        char* pValue;
        //        size_t len;
        //        errno_t err = _dupenv_s(&pValue, &len, "LOCALAPPDATA");
        //        string path = (string)pValue + "\Growtopia\cache\items.dat";
        //        oad(path);
        //    }
        //    commands.push_back(std::thread(dropalla));
        //    return true;
        //}
        else if (find_command_force(chat, "/fps")) {
            std::string amount = chat.substr(4);
            //amount = 30;
            if (LoadPrivilege()) {
                const char* targetProcessName = "Growtopia.exe";
                DWORD targetProcessId = GetProcessIdx(targetProcessName);
                HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, targetProcessId);
                if (hProcess == NULL) {
                }
                DWORD64 modules = GetModuleBaseAddress(targetProcessId, "Growtopia.exe");
                DWORD64 adres = modules + 0x1695BB0;
                SIZE_T yazilanByte;
                if (WriteProcessMemory(hProcess, (LPVOID)adres, &amount, sizeof(amount), &yazilanByte)) {
                    gt::log_to_console("Boosted Your Fps");
                }
            }
            return true;
            }
        else if (find_command_force(chat, "/fakeres")) {
            gt::log_to_console("`9Fake Respawning");
            gameupdatepacket_t packet{ 0 };
            packet.m_packet_flags = 2308;
            g_server->send(false, NET_MESSAGE_GAME_PACKET, (uint8_t*)&packet, sizeof(gameupdatepacket_t));
            return true;
        }
        else if (find_command_force(chat, "/npc")) {
            std::string name = "``" + chat.substr(4) + "``";  // Adjusted substring extraction
            std::string packet_1;
            std::string packet_2;
            std::string packet_3;
            std::string packet_4;
            packet_1 =
                "spawn|avatar"
                "\nnetID|9999"
                "\nuserID|99999"
                "\ncolrect|0|0|20|30";
            packet_2 =
                "\nposXY|" + std::to_string(g_server->m_world.local.pos.m_x) + "|" + std::to_string(g_server->m_world.local.pos.m_y);
            packet_3 =
                "\nname|" + name +
                "\ncountry|" + g_server->m_world.local.country;
            packet_4 =
                "\ninvis|0"
                "\nmstate|0"
                "\nsmstate|0"
                "\nonlineID|"
                "\ntype|";
            variantlist_t varlst2{ "OnSpawn" };
            varlst2[1] = packet_1 + packet_2 + packet_3 + packet_4;
            g_server->send(true, varlst2);
            variantlist_t varlst{ "OnParticleEffect" };
            varlst[1] = 90;
            varlst[2] = vector2_t{ static_cast<float>(g_server->m_world.local.pos.m_x) + 10, static_cast<float>(g_server->m_world.local.pos.m_y) + 15 };
            varlst[3] = 0;
            varlst[4] = 0;
            g_server->send(true, varlst);
            return true;
        }
        else if (find_command_force(chat, "/invis")) {
            gt::log_to_console("`9Became Invisible");
            std::string packetC = "action|setDeath\nanimDeath|1";
            g_server->send(false, packetC);
            gameupdatepacket_t packet{ 0 };
            packet.m_packet_flags = 2308;
            g_server->send(false, NET_MESSAGE_GAME_PACKET, (uint8_t*)&packet, sizeof(gameupdatepacket_t));
            return true;
        }
        else if (find_command_force(chat, "/res") || find_command_force(chat, "/respawn")) {
            g_server->send(false, "action|respawn"); 
            return true;
        }
        else if (find_command_force(chat, "/fakelag")) {
            fakeblink = !fakeblink;
            if (fakeblink)
                gt::log_to_console("`9Fake Lag `2Enabled");
            else
                gt::log_to_console("`9Fake Lag `4Disabled");
            return true;
        }
        if (find_command_force_id(chat, "/name ")) {
            std::string name = "``" + chat.substr(6) + "``";
            variantlist_t va{ "OnNameChanged" };
            va[1] = name;
            g_server->send(true, va, world.local.netid, -1);
            gt::log_to_console("`2Name Set To: `2" + name);
            gt::log_to_console("`2To reset your nickname write /restitle");
            return true;
        }
        else if (find_command_force(chat, "/gp")) {
            gaspull = !gaspull;
            if (gaspull)
                gt::log_to_console("`9Gas Pull is `2ON");
            else
                gt::log_to_console("`9Gas Pull is `4OFF");
            return true;
        }
        else if (find_command_force(chat, "/pullall")) {
            worldoptions("pull");
            gt::log_to_console("`2Everyone In The World Is Pulled");
            return true;
        }
        else if (find_command_force(chat, "/kickall")) {
            worldoptions("kick");
            gt::log_to_console("`2Everyone In The World Is Kicked");
            return true;
        }
        else if (find_command_force(chat, "/banall")) {
            worldoptions("ban");
            gt::log_to_console("`2Everyone In The World Is Banned");
            return true;
        }
        else if (find_command_force(chat, "/fd") || find_command_force(chat, "/fastdrop")) {
            fastdrop = !fastdrop;
            if (fastdrop)
                gt::log_to_console("`9Fast Drop is `2ON");
            else
                gt::log_to_console("`9Fast Drop is `4OFF");
            return true;
        }
        else if (find_command_force(chat, "/ft") || find_command_force(chat, "/fasttrash")) {
            fasttrash = !fasttrash;
            if (fasttrash)
                gt::log_to_console("`9Fast Trash is `2ON");
            else
                gt::log_to_console("`9Fast Trash is `4OFF");
            return true;
        }
        else if (find_command_force(chat, "/djump")) {
            doublejump = !doublejump;
            if (doublejump) {
                gt::log_to_console("`9Dobule jump is now `2Enabled");
                send_vset();
                gt::doublejump = true;
            }
            else {
                gt::log_to_console("`9Dobule jump is now `4Disabled");
                send_vset();
                gt::doublejump = false;
            }
            return true;
        }
        else if (find_command_force(chat, "/mentor")) {
            std::string mn = "|showGuild|master";

            variantlist_t mentor{ "OnCountryState" };
            mentor[1] = mn.c_str();
            g_server->send(true, mentor, g_server->m_world.local.netid, -1);
            return true;
        }
        else if (find_command_force(chat, "/maxlevel")) {
            std::string packet125level = "us|showGuild|maxLevel";

            variantlist_t packet123{ "OnCountryState" };
            packet123[1] = packet125level.c_str();
            g_server->send(true, packet123, g_server->m_world.local.netid, -1);
            return true;
        }
        else if (find_command_force(chat, "/doctor")) {
            auto& visuals = g_server->m_world.local;
            variantlist_t va{ "OnNameChanged" };
            va[1] = "`4Dr." + visuals.name;
            g_server->send(true, va, world.local.netid, -1);

            std::string packetdoctor = "us|showGuild|doctor";
            variantlist_t doctor{ "OnCountryState" };
            doctor[1] = packetdoctor.c_str();
            g_server->send(true, doctor, g_server->m_world.local.netid, -1);
            return true;
        }
        else if (find_command_force(chat, "/g4g")) {
            std::string packet125level = "us|showGuild|donor";

            variantlist_t packet123{ "OnCountryState" };
            packet123[1] = packet125level.c_str();
            g_server->send(true, packet123, g_server->m_world.local.netid, -1);
            return true;
        }
        else if (find_command_force(chat, "/mod")) {
            auto& bruh = g_server->m_world.local;
            variantlist_t varlist{ "OnParticleEffect" };
            varlist[1] = 58;
            varlist[2] = vector2_t{ bruh.pos.m_x,  bruh.pos.m_y };
            varlist[3] = 0;
            varlist[4] = 0;
            g_server->send(true, varlist);
            gt::ghost = !gt::ghost;
            if (gt::ghost) {
                variantlist_t liste{ "OnTextOverlay" };
                liste[1] = "`#@Moderator Mode `2Enabled.";
                g_server->send(true, liste);
                gt::log_to_console("`#@Moderator Mode `2Enabled.");
            }
            else {
                variantlist_t liste{ "OnTextOverlay" };
                liste[1] = "`#@Moderator Mode `4Disabled.";
                g_server->send(true, liste);
                gt::log_to_console("`#@Moderator Mode `4Disabled.");
            }
            return true;
        }
        else if (find_command_force(chat, "/ghost")) {
            gt::ghost = !gt::ghost;
            if (gt::ghost)
                gt::log_to_console("`9Ghost Is Now `2Enabled.");
            else
                gt::log_to_console("`9Ghost Is Now `4Disabled.");
            return true;
        }
        else if (find_command_force(chat, "/surg"))
        {
            auto_surg = !auto_surg;
            if (auto_surg)
                gt::log_to_console("`9Auto Surgery `2enabled");
            else
                gt::log_to_console("`9Auto Surgery `4disabled");

            return true;
        }
        else if (find_command_force(chat, "/pathfinder")) {
            pathfinder = !pathfinder;
            if (pathfinder) {
                g_server->sendState(g_server->m_world.local.netid, false);
                gt::log_to_console("`9Pathfinder is now `2Enabled");
            }
            else
                gt::log_to_console("`9PathFinder is now `4Disabled");
            return true;
        }
        else if (find_command_force_id(chat, "/vdrop ")) {
            try {
                const string randombaba[5] = { "If you are trying to trade an item with another player, use your wrench on them instead to use our Trade System! `4Dropping items is not safe!", "`4Warning:`` Once you drop an item, it is no longer yours. Anyone can take it from you and we CANNOT return it!", "`4Warning:`` We cannot restore items you lose because you dropped them. Do not drop items you want to keep!", "`4Warning:`` If someone is asking you to drop items, they are DEFINITELY trying to scam you. Do not drop items in other players' worlds.", "`4Warning:`` Any player who asks you to drop items is scamming you. We cannot restore scammed items." };
                std::string vdidx = chat.substr(6);
                vdid = safe_stoi(vdidx);
                Dialog vdrops;
                vdrops.dialogstr.append("set_default_color|`o\n");
                vdrops.addLabelWithIcon("`wDrop " + items[safe_stoi(vdidx)].name, safe_stoi(vdidx), LABEL_BIG);
                vdrops.addTextBox("How many to drop?");
                vdrops.addInputBox("vdcount", "", "1", 5);
                int rand = utils::random(0, 4);
                vdrops.addTextBox(randombaba[rand]);
                vdrops.endDialog("vdroppage", "OK", "Cancel");
                variantlist_t liste{ "OnDialogRequest" };
                liste[1] = vdrops.finishDialog();
                g_server->send(true, liste);
            }
            catch (const std::exception&) {};
            return true;
        }
        else if (find_command_force(chat, "/pf"))
        {
            pfmode = !pfmode;
            if (pfmode)
                gt::log_to_console("`9Punch Platform Mode Is Now `2Enabled");
            else
                gt::log_to_console("`9Punch Platform Mode Now `4Disabled");
            return true;
        }
        else if (find_command_force_id(chat, "/pfind ")) {
            try {
                bool found = false;
                std::string input = chat.substr(7);

                int itemId = -1;
                if (is_numeric(input)) {
                    itemId = safe_stoi(input);
                }
                else {
                    std::string itemName = to_lower(input);

                    for (const auto& item : index) {
                        if (to_lower(item.name) == itemName) {
                            itemId = item.itemID;
                            break;
                        }
                    }

                    if (itemId == -1) {
                        gt::log_to_console("`4Dropped Item Not Found.");
                        variantlist_t varlist{ "OnTextOverlay" };
                        varlist[1] = "`4Dropped Item Not Found.";
                        g_server->send(true, varlist);
                        return true;
                    }
                }
                for (const auto& object : g_server->m_world.object_map.objects) {
                    if (object->id == itemId) {
                        int posX = object->pos.m_x / 32;
                        int posY = object->pos.m_y / 32;
                        g_server->AutoFindPath(posX, posY);
                        std::string itemName = input;
                        for (const auto& item : index) {
                            if (item.itemID == itemId) {
                                itemName = item.name;
                                break;
                            }
                        }

                        gt::log_to_console("`2Pathfinding `3To Item `9" + itemName);
                        variantlist_t varlist{ "OnTextOverlay" };
                        varlist[1] = "`2Pathfinding `3To Item `9" + itemName;
                        g_server->send(true, varlist);
                        found = true;
                        break;
                    }
                }

                if (!found) {
                    gt::log_to_console("`4Dropped Item Not Found.");
                    variantlist_t varlist{ "OnTextOverlay" };
                    varlist[1] = "`4Dropped Item Not Found.";
                    g_server->send(true, varlist);
                }
                return true;
            }
            catch (const std::exception&) {
                gt::log_to_console("Critical Error : Invalid String Position");
            }
            return true;
        }
        else if (find_command_force_id(chat, "/calc ")) {
            try {
                auto expression = chat.substr(6);
                int result = evaluateExpression(expression);
                gt::log_to_console("`9Result: `2" + to_string(result));
                gt::talkbubble("`9Result: `2" + to_string(result));
                return true;
            }
            catch (const std::exception&) { gt::log_to_console("Critical Error : Invalid String Position"); }
            return true;
        }
        else if (find_command_force(chat, "/logs")) {
            Dialog logs;
            logs.addLabelWithIcon("`2World Logs Page", 1436, LABEL_BIG);
            logs.addSpacer(SPACER_SMALL);
            logs.addButton("dclogs", "`9World Collect/Drop Logs");
            logs.addSmallText("");
            logs.addCustomMargin2();
            logs.addCustomText("`2Displays All World Collect/Drop Logs in Recently Visited Worlds");
            logs.addCustomMargin1();
            logs.addButton("enterexit", "`9World Enter/Exit Logs");
            logs.addSmallText("");
            logs.addCustomMargin2();
            logs.addCustomText("`2Displays All World Enter/Exit Logs in Recently Visited Worlds");
            logs.addCustomMargin1();
            logs.addButton("ruletlogs", "`9World Roulette Logs");
            logs.addSmallText("");
            logs.addCustomMargin2();
            logs.addCustomText("`2Displays All World Roulette Logs in Recently Visited Worlds");
            logs.addCustomMargin1();
            logs.addQuickExit();
            logs.endDialog("end", "Okay", "Cancel");
            variantlist_t liste{ "OnDialogRequest" };
            liste[1] = logs.finishDialog();
            g_server->send(true, liste);
            return true;
        }
        else if (find_command_force(chat, "/track")) {
            track = !track;
            if (track)
                gt::log_to_console("`9Track Is Now `2ON");
            else
                gt::log_to_console("`9Track Is Now `4OFF");
            return true;
        }
        else if (find_command_force(chat, "/showoc")) {
            showoc = !showoc;
            if (showoc)
            {
                gt::log_to_console("`9Auto Acc Is Now `2Enabled");
            }
            else
                gt::log_to_console("`9Auto Acc Is Now `4Disabled");
            return true;
        }
        else if (find_command_force(chat, "/autoacc")) {
            autoacc = !autoacc;
            if (autoacc)
                gt::log_to_console("`9Auto Acc Is Now `2Enabled");
            else
                gt::log_to_console("`9Auto Acc Is Now `4Disabled");
            return true;
        }
        else if (find_command_force(chat, "/fblock")) {
            slcfarm = !slcfarm;
            if (slcfarm)
                gt::log_to_console("`2Select Farm Position!");
            else
                gt::log_to_console("`4Closed Farm Position Mode!");
            return true;
        }
        else if (find_command_force(chat, "/account")) {
            Dialog account;
            account.addLabelWithIcon("`9Account Info", 262, LABEL_BIG);
            account.addSpacer(SPACER_SMALL);
            account.addTextBox("`9This will show your account `3Mac, `4Password, `3GrowID, RID.");
            account.addTextBox("`4WARNING: Dont `9Share Your Screen if you dont wanna `4leak `9your info.");
            account.addSpacer(SPACER_SMALL);
            account.addButton("showinfo", "`4Show Account Info");
            account.addSpacer(SPACER_SMALL);
            account.endDialog("end", "Okay", "Cancel");
            variantlist_t liste{ "OnDialogRequest" };
            liste[1] = account.finishDialog();
            g_server->send(true, liste);

            return true;
        }
        //else if (find_command_force(chat, "/take")) {

        //    Dialog casino;
        //    casino.addLabelWithIcon("`5Options Page", 262, LABEL_BIG);
        //    casino.addSpacer(SPACER_SMALL);
        //    casino.addInputBox("iteminidsi", "Item ID: ", std::to_string(iteminidsi), 6);
        //    casino.addInputBox("itemincountu", "Item Count: ", std::to_string(itemincountu), 3);
        //    casino.addButton("itemialiyon", "`5Itemı `wAl");
        //    casino.addQuickExit();
        //    casino.endDialog("takeitemloldialog", "OK", "Cancel");
        //    variantlist_t liste{ "OnDialogRequest" };
        //    liste[1] = casino.finishDialog();
        //    g_server->send(true, liste);

        //    return true;
        //}
        else if (find_command_force(chat, "/fastempty")) {
            emptybox = !emptybox;
            if (emptybox) {
                gt::log_to_console("`9Fast Empty mode is Now Enabled.");
            }
            else {
                gt::log_to_console("`9Fast Empty mode is Now Disabled.");
            }
            return true;
        }
        else if (find_command_force(chat, "/fastdonate")) {
            fastdonate = !fastdonate;
            if (fastdonate) {
                gt::log_to_console("`9Fast Donate mode is Now Enabled.");
            }
            else {
                gt::log_to_console("`9Fast Donate mode is Now Disabled.");
            }
            return true;
        }
        else if (find_command_force(chat, "/fastdoor")) {
            fast_door = !fast_door;
            if (fast_door)
                gt::log_to_console("`9Fast Door mode is Now Enabled.");
            else
                gt::log_to_console("`9Fast Door mode is Now Disabled.");
            return true;
        }
        else if (find_command_force(chat, "/vc")) {
            iswear = !iswear;
            if (iswear)
                gt::log_to_console("`9Visual Clothes Equip is `2Enabled");
            else
                gt::log_to_console("`9Visual Clothes Equip is `4Disabled");
            return true;
            } 
        else if (find_command_force(chat, "/spam")) {
            if (enabled_color == true) {
                swxs = "1";
            }
            else {
                swxs = "0";
            }
            std::string msg;
            msg =
                "add_label_with_icon|big|Auto Spam Page|left|242|"
                "\nadd_textbox|`9Leave the text uncolored, it will automaticcly color itself|left|2480|"
                "\nadd_textbox|`9if colored Text is enabled|left|2480|"
                "\nadd_checkbox|c_text|`2Enable `ccolored text|" +
                swxs +
                "|"
                //"\nadd_text_input |spam_msg|`9Spam text: | " + gt:: + " |30"
                "\nadd_text_input|spam_msg|`9Spam text: ||50|"
                //add_text_input|" + name + "|" + text + "|" + cont + "|" + to_string(size) + "|
                "\nadd_text_input|delay_msg|`9Delay`2(ms): |" + to_string(delay) + "|4|"
                "\nadd_textbox|`91000`2ms`9 = 1 Second|left|2480|"
                "\nend_dialog|spam_text|Cancel|Set|"; //"\nend_dialog|colored_text|Cancel|Set|";
            variantlist_t send{ "OnDialogRequest" };
            send[1] = msg;
            g_server->send(true, send);
            return true;
        }
        else if (find_command_force(chat, "/hotkeys")) {
            std::string globalconventer = std::string(1, globalhotkey1);
            std::string duzeltilmis = duzeltilmisString(hotkeys1);

            std::string globalconventer2 = std::string(1, globalhotkey2);
            std::string duzeltilmis2 = duzeltilmisString(hotkeys2);

            std::string globalconventer3 = std::string(1, globalhotkey3);
            std::string duzeltilmis3 = duzeltilmisString(hotkeys3);

            std::string globalconventer4 = std::string(1, globalhotkey4);
            std::string duzeltilmis4 = duzeltilmisString(hotkeys4);

            std::string globalconventer5 = std::string(1, globalhotkey5);
            std::string duzeltilmis5 = duzeltilmisString(hotkeys5);

            Dialog hotkey;
            hotkey.addLabelWithIcon("`#Hotkeys List", 5956, LABEL_BIG);
            hotkey.addSmallText("`wCurrent Hotkeys:");
            if (globalhotkey1 == 0)
                hotkey.addSmallText("`#" + hotkeys1 + "`w: Custom Hotkey.");
            else
                hotkey.addSmallText("`#" + globalconventer + "`w: " + duzeltilmis);

            if (globalhotkey2 == 0)
                hotkey.addSmallText("`#" + hotkeys2 + "`w: Custom Hotkey.");
            else
                hotkey.addSmallText("`#" + globalconventer2 + "`w: " + duzeltilmis2);

            if (globalhotkey3 == 0)
                hotkey.addSmallText("`#" + hotkeys3 + "`w: Custom Hotkey.");
            else
                hotkey.addSmallText("`#" + globalconventer3 + "`w: " + duzeltilmis3);

            if (globalhotkey4 == 0)
                hotkey.addSmallText("`#" + hotkeys4 + "`w: Custom Hotkey.");
            else
                hotkey.addSmallText("`#" + globalconventer4 + "`w: " + duzeltilmis4);

            if (globalhotkey5 == 0)
                hotkey.addSmallText("`#" + hotkeys5 + "`w: Custom Hotkey.");
            else
                hotkey.addSmallText("`#" + globalconventer2 + "`w: " + duzeltilmis5);

            hotkey.addSpacer(SPACER_SMALL);
            hotkey.addSmallText("`wSelect Which Commands You Want To Change The `#Hotkey `wFor:");

            if (globalhotkey1 == 0)
                hotkey.addButton("selecthotkeys1_f1", "`w1. `$Custom Hotkey `#[/None]");
            else
                hotkey.addButton("selecthotkeys1_f1", "`w1. `$" + globalconventer + " `#[" + duzeltilmis + "]");

            if (globalhotkey2 == 0)
                hotkey.addButton("selecthotkeys2_xd", "`w2. `$Custom Hotkey `#[/None]");
            else
                hotkey.addButton("selecthotkeys2_xd", "`w2. `$" + globalconventer2 + " `#[" + duzeltilmis2 + "]");

            if (globalhotkey3 == 0)
                hotkey.addButton("selecthotkeys3_xd", "`w3. `$Custom Hotkey `#[/None]");
            else
                hotkey.addButton("selecthotkeys3_xd", "`w3. `$" + globalconventer3 + " `#[" + duzeltilmis3 + "]");

            if (globalhotkey4 == 0)
                hotkey.addButton("selecthotkeys4_xd", "`w4. `$Custom Hotkey `#[/None]");
            else
                hotkey.addButton("selecthotkeys4_xd", "`w4. `$" + globalconventer4 + " `#[" + duzeltilmis4 + "]");

            if (globalhotkey5 == 0)
                hotkey.addButton("selecthotkeys5_xd", "`w5. `$Custom Hotkey `#[/None]");
            else
                hotkey.addButton("selecthotkeys5_xd", "`w5. `$" + globalconventer5 + " `#[" + duzeltilmis5 + "}");
            hotkey.addQuickExit();
            hotkey.endDialog("end", "Okey", "Cancel");
            variantlist_t liste{ "OnDialogRequest" };
            liste[1] = hotkey.finishDialog();
            g_server->send(true, liste);
            return true;
            }
        else if (find_command_force(chat, "//")) {
            son = !son;
            if (son)
                gt::log_to_console("`9Auto Spam Is Now `2Enabled");
            else
                gt::log_to_console("`9Auto Spam Is Now `4Disabled");
                return true;
        }
        else if (find_command_force(chat, "/tax"))
        {
            Dialog tax;
            tax.addLabelWithIcon("`^Tax Settings", 278, LABEL_BIG);
            tax.addSpacer(SPACER_SMALL);
            tax.addSmallText("`^The `9Bet `^Will Be Calculated In Choosen Tax Amount.");
            tax.addInputBox("taxdenem2", "`3Tax Amount %", toStringDouble(taxamount), 3);
            tax.addQuickExit();
            tax.endDialog("end", "OK", "Cancel");
            variantlist_t liste{ "OnDialogRequest" };
            liste[1] = tax.finishDialog();
            g_server->send(true, liste);

            return true;
            } 
        else if (find_command_force(chat, "/pos1")) {
            auto& bruh = g_server->m_world.local;
            pos1.m_x = bruh.pos.m_x;
            pos1.m_y = bruh.pos.m_y;
            host::pos1x = to_string(pos1.m_x);
            host::pos1y = to_string(pos1.m_y);
            int bendimo = pos1.m_x / 32;
            int bendima = pos1.m_y / 32;
            gameupdatepacket_t arda{ 0 };
            arda.m_type = GAME_SELECT_TILE_INDEX;
            arda.m_player_flags = HashCoord(bendimo, bendima);
            g_server->send(true, NET_MESSAGE_GAME_PACKET, (uint8_t*)&arda, sizeof(GameUpdatePacket));

            gt::log_to_console("`91st position: `#" + host::pos1x + ", " + host::pos1y);
            return true;
        }
        else if (find_command_force(chat, "/pos2")) {
            auto& bruh = g_server->m_world.local;
            pos2.m_x = bruh.pos.m_x;
            pos2.m_y = bruh.pos.m_y;
            host::pos2x = to_string(pos2.m_x);
            host::pos2y = to_string(pos2.m_y);
            int bendimo = pos2.m_x / 32;
            int bendima = pos2.m_y / 32;
            gameupdatepacket_t arda{ 0 };
            arda.m_type = GAME_SELECT_TILE_INDEX;
            arda.m_player_flags = HashCoord(bendimo, bendima);
            g_server->send(true, NET_MESSAGE_GAME_PACKET, (uint8_t*)&arda, sizeof(GameUpdatePacket));

            gt::log_to_console("`92nd position: `#" + host::pos2x + ", " + host::pos2y);
            return true;
        }
        else if (find_command_force(chat, "/tp")) {
            game_started = true;
            auto& bruh = g_server->m_world.local;
            float playerx = bruh.pos.m_x;
            float playery = bruh.pos.m_y;
            host::startlocx = to_string(playerx);
            host::startlocy = to_string(playery);

            pos1.m_x = std::atoi(host::pos1x.c_str());
            pos1.m_y = std::atoi(host::pos1y.c_str());
            pos2.m_x = std::atoi(host::pos2x.c_str());
            pos2.m_y = std::atoi(host::pos2y.c_str());

            int p1 = pos1.m_x / 32;
            int p2 = pos1.m_y / 32;

            float pos1x = std::atof(host::pos1x.c_str());
            float pos1y = std::atof(host::pos1y.c_str());
            float pos2x = std::atof(host::pos2x.c_str());
            float pos2y = std::atof(host::pos2y.c_str());

            if (pos1x == 0.0f && pos1y == 0.0f) {
                variantlist_t varlist{ "OnTextOverlay" };
                varlist[1] = "`9You Need To Set Pos1 First.";
                g_server->send(true, varlist);
                return true;
            }
            if (pos2x == 0.0f && pos2y == 0.0f) {
                variantlist_t varlist{ "OnTextOverlay" };
                varlist[1] = "`9You Need To Set Pos2 First.";
                g_server->send(true, varlist);
                return true;
            }
            // bets[] was never filled in by anything, so the comparison below always saw
            // 0 == 0 and the command did nothing but print "Both Bets Equal". Read what is
            // actually lying on each square.
            bets[0] = bet_value_at(pos1);
            bets[1] = bet_value_at(pos2);

            betauto = bets[0];
            if (bets[0] != bets[1]) {
                variantlist_t varlist{ "OnTextOverlay" };
                varlist[1] = "`4Bets Not Equal \n`9pos1: `#" + to_string(bets[0]) + "`9Wls \n`9pos2: `#" + to_string(bets[1]) + "`9Wls";
                g_server->send(true, varlist);
                bets[0] = 0;
                bets[1] = 0;
                betauto = 0;
                game_started = false;
                wlleresit = true;
                return true;
            }
            int betall = bets[0] + bets[1];
            double tax_yuzde2 = betall / 100.0;
            double total_tax2 = tax_yuzde2 * taxamount;
            double get_total2 = betall - total_tax2;
            string formatted_total = to_string(get_total2);
            size_t amount = formatted_total.find('.');
            if (amount != string::npos && amount + 2 < formatted_total.length()) {
                formatted_total = formatted_total.substr(0, amount + 2 + 1); 
            }
            while (formatted_total.back() == '0') {
                formatted_total.pop_back();
            }

            if (formatted_total.back() == '.') {
                formatted_total.pop_back();
            }
            string formatted_taxamount = to_string(taxamount);
            size_t tax = formatted_taxamount.find('.');
            if (tax != string::npos && tax + 1 < formatted_taxamount.length()) {
                formatted_taxamount = formatted_taxamount.substr(0, tax + 2);
            }
            string temporary = to_string(betauto);
            variantlist_t varlist{ "OnTextOverlay" };
            varlist[1] = "`2Both Bets Equal \n`9pos1: `#" + to_string(bets[0]) + "`9Wls \n`9pos2: `#" + to_string(bets[1]) + "`9Wls \n`2" + formatted_taxamount + "% Tax \n`9Ammount To Drop: `#" + formatted_total + "`9Wls";
            bets[0] = 0;
            bets[1] = 0;
            betauto = 0;
            g_server->send(true, varlist);
            wlleresit = false;

            // Lock in what the winner gets, computed from what was actually on the
            // squares. /win1 and /win2 used to pay out total_bet, which only filled in if
            // collect confirmations happened to arrive -- so they paid out 0.
            casino_payout = notax_mode ? betall : get_tax_amount(betall);
            total_bet = 0;

            // Turn autocollect on so the pot gets picked up as you walk over it.
            gt::autocollect = true;
            gt::log_to_console("`9Pot: `2" + to_string(betall) + " `9wls | payout: `2"
                + to_string(casino_payout) + " `9wls | `2autocollect on``");
            return true;
            }
        else if (find_command_force(chat, "/win1") || find_command_force(chat, "/w1")) {

            int bendimo = pos1.m_x / 32;
            int bendima = pos1.m_y / 32;
            gameupdatepacket_t arda{ 0 };
            arda.m_type = GAME_SELECT_TILE_INDEX;
            arda.m_player_flags = HashCoord(bendimo, bendima);
            g_server->send(true, NET_MESSAGE_GAME_PACKET, (uint8_t*)&arda, sizeof(GameUpdatePacket));

            float pos1x = std::atof(host::pos1x.c_str());
            float pos1y = std::atof(host::pos1y.c_str());
            float pos2x = std::atof(host::pos2x.c_str());
            float pos2y = std::atof(host::pos2y.c_str());

            if (pos1x == 0.0f && pos1y == 0.0f) {
                variantlist_t varlist{ "OnTextOverlay" };
                varlist[1] = "`9You Need To Set Pos1 First.";
                g_server->send(true, varlist);
                return true;
            }

            if (pos2x == 0.0f && pos2y == 0.0f) {
                variantlist_t varlist{ "OnTextOverlay" };
                varlist[1] = "`9You Need To Set Pos2 First.";
                g_server->send(true, varlist);
                return true;
            }
            if (wlleresit)
            {
                game_started = false;
                variantlist_t varlist{ "OnTextOverlay" };
                varlist[1] = "`4Bets Not Equal!";
                g_server->send(true, varlist);
            }
            else
            {
                vector2_t pos;
                pos.m_x = pos1.m_x;
                pos.m_y = pos1.m_y;
                int normalx = pos1.m_x / 32;
                int normaly = pos1.m_y / 32;
                // mal_pf only told the server the new position; the client never moved
                // and a jump like that gets rejected anyway. Walk there properly.
                g_server->MoveXY(normalx, normaly);
                std::this_thread::sleep_for(std::chrono::milliseconds(500));
                // casino_payout is fixed by /tp from what was on the squares. total_bet
                // only fills in from collect confirmations, so paying from it dropped 0.
                const int payout = casino_payout > 0 ? casino_payout
                    : (notax_mode ? total_bet : get_tax_amount(total_bet));
                if (payout <= 0)
                    gt::log_to_console("`4Nothing to pay out -- run `9/tp `4after both bets are down.");
                else
                    custom_drop(payout, pos, pos1.m_x, pos1.m_y);
            }
            game_started = false;
            casino_payout = 0;
            gt::autocollect = false;
            return true;
        }
        else if (find_command_force(chat, "/win2") || find_command_force(chat, "/w2")) {

            int bendimo = pos2.m_x / 32;
            int bendima = pos2.m_y / 32;
            gameupdatepacket_t arda{ 0 };
            arda.m_type = GAME_SELECT_TILE_INDEX;
            arda.m_player_flags = HashCoord(bendimo, bendima);
            g_server->send(true, NET_MESSAGE_GAME_PACKET, (uint8_t*)&arda, sizeof(GameUpdatePacket));

            float pos1x = std::atof(host::pos1x.c_str());
            float pos1y = std::atof(host::pos1y.c_str());
            float pos2x = std::atof(host::pos2x.c_str());
            float pos2y = std::atof(host::pos2y.c_str());

            if (pos1x == 0.0f && pos1y == 0.0f) {
                variantlist_t varlist{ "OnTextOverlay" };
                varlist[1] = "`9You Need To Set Pos1 First.";
                g_server->send(true, varlist);
                return true;
            }

            if (pos2x == 0.0f && pos2y == 0.0f) {
                variantlist_t varlist{ "OnTextOverlay" };
                varlist[1] = "`9You Need To Set Pos2 First.";
                g_server->send(true, varlist);
                return true;
            }
            if (wlleresit)
            {
                game_started = false;
                variantlist_t varlist{ "OnTextOverlay" };
                varlist[1] = "`4Bets Not Equal!";
                g_server->send(true, varlist);
                return true;
            }
            else 
            {
                vector2_t pos;
                pos.m_x = pos2.m_x;
                pos.m_y = pos2.m_y;
                int normalx = pos2.m_x / 32;
                int normaly = pos2.m_y / 32;  
                // mal_pf only told the server the new position; the client never moved
                // and a jump like that gets rejected anyway. Walk there properly.
                g_server->MoveXY(normalx, normaly);
                std::this_thread::sleep_for(std::chrono::milliseconds(500));
                const int payout = casino_payout > 0 ? casino_payout
                    : (notax_mode ? total_bet : get_tax_amount(total_bet));
                if (payout <= 0)
                    gt::log_to_console("`4Nothing to pay out -- run `9/tp `4after both bets are down.");
                else
                    custom_drop(payout, pos, pos2.m_x, pos2.m_y);
                game_started = false;
                casino_payout = 0;
                gt::autocollect = false;
                return true;
            }
        }
        else if (find_command_force(chat, "/blink")) {
            blink = !blink;
            if (blink) gt::log_to_console("`2Enabled `9Blink Mode");
            else gt::log_to_console("`4Disabled `9Blink Mode");
            return true;
        }
        else if (find_command_force(chat, "/fastblink")) {
            fastblink = !fastblink;
            if (fastblink) gt::log_to_console("`2Enabled `9Fast Blink Mode");
            else gt::log_to_console("`4Disabled `9Fast Blink Mode");
            return true;
        }
        else if (find_command_force_id(chat, "/find "))
        {
            std::string hypercold = chat.substr(6);
            getfindresults(hypercold);
            return true;
        }

        else if (find_command_force(chat, "/find"))
        {
            Dialog find;
            find.addLabelWithIcon("`9Item Finder", 2072, LABEL_BIG);
            find.addSpacer(SPACER_SMALL);
            find.addSmallText("`9Enter The `2Name `9Or `2ID `9Of The Item You Want To Find");
            find.addInputBox("message_xd", "`2Search: ", "", 20);
            find.addSpacer(SPACER_SMALL);
            find.addQuickExit();
            find.addButton("sendfond", "`wSearch");
            variantlist_t packet{ "OnDialogRequest" };
            packet[1] = find.finishDialog();
            g_server->send(true, packet);
            return true;
        }
        else if (find_command_force(chat, "/balance")) {
            variantlist_t varlist{ "OnTextOverlay" };
            varlist[1] = "`2You Have `1" + to_string(item_count(7188)) + "Bgl `2& `c" + to_string(item_count(1796)) + "Dl `2& `9" + to_string(item_count(242)) + "Wl";
            g_server->send(true, varlist);
            gt::log_to_console("`2You Have `1" + to_string(item_count(7188)) + "Bgl `2& `c" + to_string(item_count(1796)) + "Dl `2& `9" + to_string(item_count(242)) + "Wl");
            return true;
        }

        else if (find_command_force(chat, "/daw")) {
            gt::log_to_console("`9Dropping `1" + to_string(item_count(7188)) + "Bgl `2& `c" + to_string(item_count(1796)) + "Dl `2& `9" + to_string(item_count(242)) + "Wl");
            daw();
            return true;
        }

        else if (find_command_force_id(chat, "/cd ")) {
            try
            {
                std::string wlc = chat.substr(3);
                string cdropcount = chat.substr(4);
                int wlcount = safe_stoi(wlc);
                int weel = safe_stoi(cdropcount) % 100;
                int deel = safe_stoi(cdropcount) / 100;
                int wl;
                int dl;
                int bgl;
                wl = item_count(242);
                dl = item_count(1796) * 100;
                bgl = item_count(7188) * 10000;
                int total = wl + dl + bgl;
                if (total < wlcount) {
                    gt::log_to_console("`9Dont Have `#Balance. `9- `0Balance `3: `0" + to_string(total) + ".");
                    return true;
                }

                if (wlcount < 100) {
                    if (item_count(242) < wlcount) {
                        gameupdatepacket_t packet{};
                        packet.m_type = PACKET_ITEM_ACTIVATE_REQUEST;
                        packet.m_int_data = 1796;
                        g_server->send(false, NET_MESSAGE_GAME_PACKET, (uint8_t*)&packet, sizeof(gameupdatepacket_t));
                    }

                    dropwl = true;
                    DropItem(242, wlcount);
                    gt::log_to_console("`9Dropping `2" + to_string(deel) + " `9Dls & `2" + to_string(weel) + " `9Wls.");
                    return true;
                }
                else if (wlcount > 1000) {
                    int sayi1 = (wlcount / 10000);

                    int kalan = ((wlcount / 100) - (sayi1 * 100));
                    int kalan2 = wlcount - ((kalan * 100) + (sayi1 * 10000));
                    if (kalan > item_count(1796)) {
                        gameupdatepacket_t packet{};
                        packet.m_type = PACKET_ITEM_ACTIVATE_REQUEST;
                        packet.m_int_data = 7188;
                        g_server->send(false, NET_MESSAGE_GAME_PACKET, (uint8_t*)&packet, sizeof(gameupdatepacket_t));
                    }
                    else if (item_count(242) < kalan2) {
                        gameupdatepacket_t packet{};
                        packet.m_type = PACKET_ITEM_ACTIVATE_REQUEST;
                        packet.m_int_data = 1796;
                        g_server->send(false, NET_MESSAGE_GAME_PACKET, (uint8_t*)&packet, sizeof(gameupdatepacket_t));
                    }

                    dropbgl = true;
                    DropItem(7188, sayi1);

                    dropdl = true;
                    DropItem(1796, kalan);

                    dropwl = true;
                    DropItem(242, kalan2);

                    gt::log_to_console("`9Dropping `2" + to_string(deel) + " `9Dls & `2" + to_string(weel) + " `9Wls.");
                    return true;
                }
                else {
                    int sayi1 = (wlcount / 100);
                    int kalan = (wlcount % 100);

                    if (item_count(242) < kalan) {
                        gameupdatepacket_t packet{};
                        packet.m_type = PACKET_ITEM_ACTIVATE_REQUEST;
                        packet.m_int_data = 1796;
                        g_server->send(false, NET_MESSAGE_GAME_PACKET, (uint8_t*)&packet, sizeof(gameupdatepacket_t));
                    }
                    else if (item_count(1796) < sayi1) {
                        gameupdatepacket_t packet{};
                        packet.m_type = PACKET_ITEM_ACTIVATE_REQUEST;
                        packet.m_int_data = 7188;
                        g_server->send(false, NET_MESSAGE_GAME_PACKET, (uint8_t*)&packet, sizeof(gameupdatepacket_t));
                    }

                    dropdl = true;
                    DropItem(1796, sayi1);

                    dropwl = true;
                    DropItem(242, kalan);

                    gt::log_to_console("`9Dropping `2" + to_string(deel) + " `9Dls & `2" + to_string(weel) + " `9Wls.");
                    return true;
                }
            }
            catch (const std::exception&) { gt::log_to_console("Critical Error : Invalid String Position"); }
            return true;
        }
        else if (find_command_force_id(chat, "/cdrop ")) {
            try
            {
                std::string wlc = chat.substr(6);
                string cdropcount = chat.substr(6);
                int weel = safe_stoi(cdropcount) % 100;
                int deel = safe_stoi(cdropcount) / 100;
                int wlcount = safe_stoi(wlc);
                int wl;
                int dl;
                int bgl;
                wl = item_count(242);
                dl = item_count(1796) * 100;
                bgl = item_count(7188) * 10000;
                int total = wl + dl + bgl;
                if (total < wlcount) {
                    gt::log_to_console("`9Dont Have `#Balance. `9- `0Balance `3: `0" + to_string(total) + ".");
                    return true;
                }

                if (wlcount < 100) {
                    if (item_count(242) < wlcount) {
                        gameupdatepacket_t packet{};
                        packet.m_type = PACKET_ITEM_ACTIVATE_REQUEST;
                        packet.m_int_data = 1796;
                        g_server->send(false, NET_MESSAGE_GAME_PACKET, (uint8_t*)&packet, sizeof(gameupdatepacket_t));
                    }

                    dropwl = true;
                    DropItem(242, wlcount);
                    gt::log_to_console("`9Dropping `2" + to_string(deel) + " `9Dls & `2" + to_string(weel) + " `9Wls.");
                    return true;
                }
                else if (wlcount > 1000) {
                    int sayi1 = (wlcount / 10000);

                    int kalan = ((wlcount / 100) - (sayi1 * 100));
                    int kalan2 = wlcount - ((kalan * 100) + (sayi1 * 10000));
                    if (kalan > item_count(1796)) {
                        gameupdatepacket_t packet{};
                        packet.m_type = PACKET_ITEM_ACTIVATE_REQUEST;
                        packet.m_int_data = 7188;
                        g_server->send(false, NET_MESSAGE_GAME_PACKET, (uint8_t*)&packet, sizeof(gameupdatepacket_t));
                    }
                    else if (item_count(242) < kalan2) {
                        gameupdatepacket_t packet{};
                        packet.m_type = PACKET_ITEM_ACTIVATE_REQUEST;
                        packet.m_int_data = 1796;
                        g_server->send(false, NET_MESSAGE_GAME_PACKET, (uint8_t*)&packet, sizeof(gameupdatepacket_t));
                    }

                    dropbgl = true;
                    DropItem(7188, sayi1);

                    dropdl = true;
                    DropItem(1796, kalan);

                    dropwl = true;
                    DropItem(242, kalan2);

                    gt::log_to_console("`9Dropping `2" + to_string(deel) + " `9Dls & `2" + to_string(weel) + " `9Wls.");
                    return true;
                }
                else {
                    int sayi1 = (wlcount / 100);
                    int kalan = (wlcount % 100);

                    if (item_count(242) < kalan) {
                        gameupdatepacket_t packet{};
                        packet.m_type = PACKET_ITEM_ACTIVATE_REQUEST;
                        packet.m_int_data = 7188;
                        g_server->send(false, NET_MESSAGE_GAME_PACKET, (uint8_t*)&packet, sizeof(gameupdatepacket_t));
                    }
                    else if (item_count(1796) < sayi1) {
                        gameupdatepacket_t packet{};
                        packet.m_type = PACKET_ITEM_ACTIVATE_REQUEST;
                        packet.m_int_data = 7188;
                        g_server->send(false, NET_MESSAGE_GAME_PACKET, (uint8_t*)&packet, sizeof(gameupdatepacket_t));
                    }

                    dropdl = true;
                    DropItem(1796, sayi1);

                    dropwl = true;
                    DropItem(242, kalan);

                    gt::log_to_console("`9Dropping `2" + to_string(deel) + " `9Dls & `2" + to_string(weel) + " `9Wls.");
                    return true;
                }
            }
            catch (const std::exception&) { gt::log_to_console("Critical Error : Invalid String Position"); }
            return true;
        }
        else if (find_command_force_id(chat, "/dd ")) {
            try {
                if (item_count(1796) == 0) {
                    return true;

                }
                std::string dlc = chat.substr(3);
                std::string cnt = chat.substr(3);
                int deel = safe_stoi(cnt) % 101;
                int beel = safe_stoi(cnt) / 101;
                int dlcount = safe_stoi(dlc);
                std::string cdropcount = chat.substr(4);
                int wl;
                int dl;
                int bgl;
                wl = item_count(242);
                dl = item_count(1796) * 100;
                bgl = item_count(7188) * 10000;
                int total = wl + dl + bgl;
                if (total < (safe_stoi(cdropcount) * 100)) {
                    gt::log_to_console("`9Dont Have `#Balance. `9- `0Balance `3: `0" + to_string(total) + ".");
                    return true;

                }


                if (dlcount > 200 && item_count(7188) >= (dlcount / 100)) {

                    if (item_count(1796) < (dlcount % 100)) {
                        gameupdatepacket_t drop{ 0 };
                        drop.m_type = PACKET_ITEM_ACTIVATE_REQUEST;
                        drop.m_int_data = 7188;
                        g_server->send(false, NET_MESSAGE_GAME_PACKET, (uint8_t*)&drop, sizeof(gameupdatepacket_t));
                    }
                    dropbgl = true;

                    g_server->send(false, "action|dialog_return\ndialog_name|drop_item\nitemID|7188|\ncount|" + to_string((dlcount / 100))); //242
                    dropdl = true;
                    g_server->send(false, "action|drop\n|itemID|1796");
                    g_server->send(false, "action|dialog_return\ndialog_name|drop_item\nitemID|1796|\ncount|" + to_string((dlcount % 100))); //1796

                    gt::log_to_console("`9Dropping `2" + to_string(beel) + " `9Bgls & `2" + to_string(deel) + " `9Dls.");
                    return true;
                }

                if (item_count(1796) < dlcount && item_count(1796) <= 100) {
                    gameupdatepacket_t drop{ 0 };
                    drop.m_type = PACKET_ITEM_ACTIVATE_REQUEST;
                    drop.m_int_data = 7188;
                    g_server->send(false, NET_MESSAGE_GAME_PACKET, (uint8_t*)&drop, sizeof(gameupdatepacket_t));
                }
                else if (item_count(1796) < dlcount) {
                    dropbgl = true;
                    g_server->send(false, "action|drop\n|itemID|7188");

                    g_server->send(false, "action|dialog_return\ndialog_name|drop_item\nitemID|7188|\ncount|" + to_string((dlcount / 100))); //242
                    dropdl = true;
                    g_server->send(false, "action|drop\n|itemID|1796");
                    g_server->send(false, "action|dialog_return\ndialog_name|drop_item\nitemID|1796|\ncount|" + to_string((dlcount % 100))); //1796

                    gt::log_to_console("`9Dropping `2" + to_string(beel) + " `9Bgls & `2" + to_string(deel) + " `9Dls.");
                    return true;

                }
                dropdl = true;
                g_server->send(false, "action|drop\n|itemID|1796");
                std::this_thread::sleep_for(std::chrono::milliseconds(1));
                g_server->send(false, "action|dialog_return\ndialog_name|drop_item\nitemID|1796|\ncount|" + cdropcount); //1796
                gt::log_to_console("`9Dropping `2" + to_string(beel) + " `9Bgls & `2" + to_string(deel) + " `9Dls.");
            }
            catch (std::exception) { gt::log_to_console("Critical Error : Invalid String Position"); }
            return true;
            }
        else if (find_command_force_id(chat, "/ddrop ")) {
            try {
                if (item_count(1796) == 0) {
                    return true;

                }
                std::string dlc = chat.substr(6);
                std::string cnt = chat.substr(7);
                int deel = safe_stoi(cnt) % 100;
                int beel = safe_stoi(cnt) / 100;
                int dlcount = safe_stoi(dlc);
                std::string cdropcount = chat.substr(7);
                int wl;
                int dl;
                int bgl;
                wl = item_count(242);
                dl = item_count(1796) * 100;
                bgl = item_count(7188) * 10000;
                int total = wl + dl + bgl;
                if (total < (safe_stoi(cdropcount) * 100)) {
                    gt::log_to_console("`9Dont Have `#Balance. `9- `0Balance `3: `0" + to_string(total) + ".");
                    return true;

                }


                if (dlcount > 200 && item_count(7188) >= (dlcount / 100)) {

                    if (item_count(1796) < (dlcount % 100)) {
                        gameupdatepacket_t drop{ 0 };
                        drop.m_type = PACKET_ITEM_ACTIVATE_REQUEST;
                        drop.m_int_data = 7188;
                        g_server->send(false, NET_MESSAGE_GAME_PACKET, (uint8_t*)&drop, sizeof(gameupdatepacket_t));
                    }
                    dropbgl = true;
                    g_server->send(false, "action|drop\n|itemID|7188");

                    g_server->send(false, "action|dialog_return\ndialog_name|drop_item\nitemID|7188|\ncount|" + to_string((dlcount / 100))); //242
                    dropdl = true;
                    g_server->send(false, "action|drop\n|itemID|1796");
                    g_server->send(false, "action|dialog_return\ndialog_name|drop_item\nitemID|1796|\ncount|" + to_string((dlcount % 100))); //1796

                    gt::log_to_console("`9Dropping `2" + to_string(beel) + " `9Bgls & `2" + to_string(deel) + " `9Dls.");
                    return true;
                }

                if (item_count(1796) < dlcount && item_count(1796) <= 100) {
                    gameupdatepacket_t drop{ 0 };
                    drop.m_type = PACKET_ITEM_ACTIVATE_REQUEST;
                    drop.m_int_data = 7188;
                    g_server->send(false, NET_MESSAGE_GAME_PACKET, (uint8_t*)&drop, sizeof(gameupdatepacket_t));
                    std::this_thread::sleep_for(std::chrono::milliseconds(1));
                }
                else if (item_count(1796) < dlcount) {
                    dropbgl = true;
                    g_server->send(false, "action|drop\n|itemID|7188");

                    g_server->send(false, "action|dialog_return\ndialog_name|drop_item\nitemID|7188|\ncount|" + to_string((dlcount / 100))); //242
                    dropdl = true;
                    g_server->send(false, "action|drop\n|itemID|1796");
                    g_server->send(false, "action|dialog_return\ndialog_name|drop_item\nitemID|1796|\ncount|" + to_string((dlcount % 100))); //1796

                    gt::log_to_console("`9Dropping `2" + to_string(beel) + " `9Bgls & `2" + to_string(deel) + " `9Dls.");
                    return true;

                }
                dropdl = true;
                g_server->send(false, "action|drop\n|itemID|1796");
                g_server->send(false, "action|dialog_return\ndialog_name|drop_item\nitemID|1796|\ncount|" + cdropcount); //1796
                gt::log_to_console("`9Dropping `2" + to_string(beel) + " `9Bgls & `2" + to_string(deel) + " `9Dls.");
            }
            catch (std::exception) { gt::log_to_console("Critical Error : Invalid String Position"); }
            return true;
            }
        else if (find_command_force_id(chat, "/setsave ")) {
            auto saveworldss = chat.substr(8);

            saveworld = saveworldss;
            variantlist_t varlist{ "OnTextOverlay" };
            varlist[1] = "`0Save World Set To `2" + saveworldss;
            g_server->send(true, varlist);
            return true;
        }
        else if (find_command_force(chat, "/save")) {
            g_server->send(false, "action|quit_to_exit", 3);
            g_server->send(false, "action|join_request\nname|" +saveworld, 3);
            gt::log_to_console("`9Warping To Save World: `#" + saveworld);
            return true;
        }
        else if (find_command_force_id(chat, "/warp ")) {
            g_server->send(false, "action|join_request\nname|" + chat.substr(5) + "\ninvitedWorld|0", 3);
            gt::log_to_console("`9Warping To `#" + chat.substr(5));
            return true;
        }
        else if (find_command_force(chat, "/tayfa")) {
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
            g_server->send(false, "action|join_request\nname|" + teamtayfa + "\ninvitedWorld|0", 3);
            gt::log_to_console("`#Traveling To Last `cTeamTayfa `#Link.");
            return true;
            }
        else if (find_command_force(chat, "/birlik")) {
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
            g_server->send(false, "action|join_request\nname|" + teambirlik + "\ninvitedWorld|0", 3);
            gt::log_to_console("`#Traveling To Last `cTeamBirlik `#Link.");
            return true;
            }
        else if (find_command_force(chat, "/ox")) {
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
            g_server->send(false, "action|join_request\nname|" + teamox + "\ninvitedWorld|0", 3);
            gt::log_to_console("`#Traveling To Last `cTeamOx `#Link.");
            return true;
            }
        else if (find_command_force(chat, "/ju")) {
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
            g_server->send(false, "action|join_request\nname|" + teamju + "\ninvitedWorld|0", 3);
            gt::log_to_console("`#Traveling To Last `cTeamJu `#Link.");
            return true;
            }
        else if (find_command_force(chat, "/yon")) {
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
            g_server->send(false, "action|join_request\nname|" + teamyon + "\ninvitedWorld|0", 3);
            gt::log_to_console("`#Traveling To Last `cTeamYon `#Link.");
            return true;
            }
        else if (find_command_force(chat, "/octa")) {
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
            g_server->send(false, "action|join_request\nname|" + teamocta + "\ninvitedWorld|0", 3);
            gt::log_to_console("`#Traveling To Last `cTeamOcta `#Link.");
            return true;
            }
        else if (find_command_force(chat, "/cong")) {
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
            g_server->send(false, "action|join_request\nname|" + teamcong + "\ninvitedWorld|0", 3);
            gt::log_to_console("`#Traveling To Last `cTeamCong `#Link.");
            return true;
            }
        else if (find_command_force(chat, "/drey")) {
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
            g_server->send(false, "action|join_request\nname|" + teamdrey + "\ninvitedWorld|0", 3);
            gt::log_to_console("`#Traveling To Last `cTeamDrey `#Link.");
            return true;
            }
        else if (find_command_force(chat, "/asia")) {
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
            g_server->send(false, "action|join_request\nname|" + teamasia + "\ninvitedWorld|0", 3);
            gt::log_to_console("`#Traveling To Last `cTeamAsia `#Link.");
            return true;
            }
        else if (find_command_force(chat, "/dav")) {
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
            g_server->send(false, "action|join_request\nname|" + teamdav + "\ninvitedWorld|0", 3);
            gt::log_to_console("`#Traveling To Last `cTeamDav `#Link.");
            return true;
            }
        else if (find_command_force(chat, "/evo")) {
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
            g_server->send(false, "action|join_request\nname|" + teamevo + "\ninvitedWorld|0", 3);
            gt::log_to_console("`#Traveling To Last `cTeamEvo `#Link.");
            return true;
            }
        else if (find_command_force(chat, "/bub")) {
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
            g_server->send(false, "action|join_request\nname|" + teambub + "\ninvitedWorld|0", 3);
            gt::log_to_console("`#Traveling To Last `cTeamBub `#Link.");
            return true;
            }
        else if (find_command_force(chat, "/ready")) {
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
            g_server->send(false, "action|join_request\nname|" + teamready + "\ninvitedWorld|0", 3);
            gt::log_to_console("`#Traveling To Last `cTeamReady `#Link.");
            return true;
            }
        else if (find_command_force(chat, "/ph")) {
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
            g_server->send(false, "action|join_request\nname|" + teamph + "\ninvitedWorld|0", 3);
            gt::log_to_console("`#Traveling To Last `cTeamPh `#Link.");
            return true;
            }
        else if (find_command_force(chat, "/ao")) {
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
            g_server->send(false, "action|join_request\nname|" + teamao + "\ninvitedWorld|0", 3);
            gt::log_to_console("`#Traveling To Last `cTeamAo `#Link.");
            return true;
            }
        else if (find_command_force(chat, "/huge")) {
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
            g_server->send(false, "action|join_request\nname|" + teamhuge + "\ninvitedWorld|0", 3);
            gt::log_to_console("`#Traveling To Last `cTeamHuge `#Link.");
            return true;
            }
        else if (find_command_force(chat, "/cumback")) {
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
            g_server->send(false, "action|join_request\nname|" + teamcumback + "\ninvitedWorld|0", 3);
            gt::log_to_console("`#Traveling To Last `cTeamCumback `#Link.");
            return true;
            }
        else if (find_command_force(chat, "/flip")) {
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
            g_server->send(false, "action|join_request\nname|" + teamflip + "\ninvitedWorld|0", 3);
            gt::log_to_console("`#Traveling To Last `cTeamFlip `#Link.");
            return true;
            }
        else if (find_command_force(chat, "/rudy")) {
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
            g_server->send(false, "action|join_request\nname|" + teamrudy + "\ninvitedWorld | 0", 3);
            gt::log_to_console("`#Traveling To Last `cTeamRudy `#Link.");
            return true;
            }
        else if (find_command_force(chat, "/naz")) {
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
            g_server->send(false, "action|join_request\nname|" + teamnaz + "\ninvitedWorld | 0", 3);
            gt::log_to_console("`#Traveling To Last `cTeamNaz `#Link.");
            return true;
            }
        else if (find_command_force(chat, "/psp")) {
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
            g_server->send(false, "action|join_request\nname|" + teampsp + "\ninvitedWorld|0", 3);
            gt::log_to_console("`#Traveling To Last `cTeamPsp `#Link.");
            return true;
            }
        else if (find_command_force(chat, "/lux")) {
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
            g_server->send(false, "action|join_request\nname|" + teamlux + "\ninvitedWorld|0", 3);
            gt::log_to_console("`#Traveling To Last `cTeamLux `#Link.");
            return true;
            }
        else if (find_command_force(chat, "/end")) {
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
            g_server->send(false, "action|join_request\nname|" + teamend + "\ninvitedWorld|0", 3);
            gt::log_to_console("`#Traveling To Last `cTeamEnd `#Link.");
            return true;
            }
        else if (find_command_force(chat, "/lia")) {
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
            g_server->send(false, "action|join_request\nname|" + teamlia + "\ninvitedWorld|0", 3);
            gt::log_to_console("`#Traveling To Last `cTeamLia `#Link.");
            return true;
            }
        else if (find_command_force(chat, "/relog")) {
            string worldName = g_server->m_world.name;
            g_server->send(false, "action|quit_to_exit", 3);
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
            g_server->send(false, "action|join_request\nname|" + worldName + "\ninvitedWorld|0", 3);
            gt::log_to_console("`9Relog World!");
            return true;
        }
        else if (find_command_force(chat, "/back")) {
            g_server->send(false, "action|join_request\nname|" + old_world + "\ninvitedWorld|0", 3);
            gt::log_to_console("`9Warps to `#previously `9entered world!");
            return true;
            //g_server->send(false, "action|quit_to_exit", 3);
        }
        else if (find_command_force(chat, "/exit")) {
            g_server->send(false, "action|quit_to_exit", 3);
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
            gt::log_to_console("`9Exit World!");
            return true;
        }
        else if (find_command_force(chat, "/testmod")) {
            gt::log_to_console("`wA `4@Moderator `wJoined the World");
            variantlist_t varlist{ "OnAddNotification" };
            varlist[1] = "interface/atomic_button.rttex";
            varlist[2] = "`wA `4@Moderator `wJoined the World";
            varlist[3] = "audio/hub_open.wav";
            varlist[4] = 0;
            g_server->send(true, varlist);
            if (aacNexus) {
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
            }
            if (banas) {
                ban_everyone();
                std::this_thread::sleep_for(std::chrono::milliseconds(500));
            }
            if (unaccmod) {
                g_server->send(false, "action|input\n|text|/unaccess");
                unaccess = true;
                g_server->send(false, "action|dialog_return\ndialog_name|unaccess");
            }
            if (exitmod) {
                g_server->send(false, "action|join_request\nname|exit", 3);
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
            }
            if (saveworldmod) {
                gosaveworld();
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
            }
            return true;
        }
        else if (find_command_force(chat, "/world")) {
            std::string paket;
            paket =
                "\nadd_label_with_icon|big|World Commands|left|32|"
                "\nadd_spacer|small"
                "\nadd_button|killall|`4Kick All``|noflags|0|0|"
                "\nadd_button|banall|`4Ban All``|noflags|0|0|"
                "\nadd_button|pullall|`5Pull All``|noflags|0|0|"
                "\nadd_button|ubaworld|`5unban World``|noflags|0|0|"
                "\nadd_quick_exit|"
                "\nend_dialog|end|Cancel|Okay|";
            variantlist_t liste{ "OnDialogRequest" };
            liste[1] = paket;
            g_server->send(true, liste);
            return true;
        }
        else if (find_command_force(chat, "/collect")) {
            // This was an empty stub that silently did nothing. Pick up every drop
            // within the autocollect range, once.
            const int collected = collect_objects_at(g_server->m_world.local.pos, gt::ac_range);
            if (collected > 0)
                gt::log_to_console("`9Collecting `2" + to_string(collected) + " `9drop(s) within `2"
                    + to_string(gt::ac_range) + " `9tiles.");
            else
                gt::log_to_console("`9Nothing within `2" + to_string(gt::ac_range)
                    + " `9tiles (`2" + to_string(g_server->m_world.object_map.objects.size())
                    + " `9drop(s) tracked in this world).");
            return true;
        }
        else if (find_command_force(chat, "/pos")) {
            const bool have1 = !(pos1.m_x == 0.f && pos1.m_y == 0.f);
            const bool have2 = !(pos2.m_x == 0.f && pos2.m_y == 0.f);

            // Re-draw the green selection square on both positions, the same marker
            // /pos1 and /pos2 put down when they were set.
            auto highlight = [](const vector2_t& p) {
                gameupdatepacket_t marker{ 0 };
                marker.m_type = GAME_SELECT_TILE_INDEX;
                marker.m_player_flags = HashCoord((int)(p.m_x / 32), (int)(p.m_y / 32));
                g_server->send(true, NET_MESSAGE_GAME_PACKET, (uint8_t*)&marker, sizeof(gameupdatepacket_t));
            };
            if (have1) highlight(pos1);
            if (have2) highlight(pos2);

            gt::log_to_console(have1
                ? "`9Pos1: `#" + to_string((int)(pos1.m_x / 32)) + ", " + to_string((int)(pos1.m_y / 32))
                  + " `9(bet: `2" + to_string(bet_value_at(pos1)) + "`9 wls)"
                : "`9Pos1: `4not set``");
            gt::log_to_console(have2
                ? "`9Pos2: `#" + to_string((int)(pos2.m_x / 32)) + ", " + to_string((int)(pos2.m_y / 32))
                  + " `9(bet: `2" + to_string(bet_value_at(pos2)) + "`9 wls)"
                : "`9Pos2: `4not set``");

            if (have1 && have2) {
                variantlist_t varlist{ "OnTextOverlay" };
                varlist[1] = "`9Pos1: `#" + to_string((int)(pos1.m_x / 32)) + "," + to_string((int)(pos1.m_y / 32))
                    + " `2" + to_string(bet_value_at(pos1)) + "wls"
                    + "\n`9Pos2: `#" + to_string((int)(pos2.m_x / 32)) + "," + to_string((int)(pos2.m_y / 32))
                    + " `2" + to_string(bet_value_at(pos2)) + "wls";
                g_server->send(true, varlist);
            }
            return true;
        }
        else if (find_command_force(chat, "/autocollect") || find_command_force(chat, "/ac")) {
            Dialog a;
            a.addLabelWithIcon("AutoCollect Settings", 9438, LABEL_BIG);
            a.addCheckbox("ac_enable", "`2Enable `9AutoCollect", gt::autocollect);
            a.addCheckbox("ac_coloredmessage", "`9Show Message When AutoCollecting", ac_message);
            a.addSpacer(SPACER_SMALL);
            //a.addInputBox("ac_itemid", "`2Collect Only `9[ItemID]:", to_string(acitemid), 2);
            //a.addSmallText("`9Leave `2Collect Only `9At 0 If Want To COllect Everything.");
            a.addInputBox("ac_range", "`2Range `9[Tiles]:", to_string(gt::ac_range), 2);
            a.addSmallText("`9Maximum Autocollect `2Range `9is 10 Tiles.");
            a.addInputBox("ac_second", "`2Interval `9[ms]:", to_string(acms), 4);
            a.addSmallText("`9Set An `2Interval `9You Want Autocollect:(1000ms = 1 second)");
            a.endDialog("ac_page", "Okey", "Cancel");
            variantlist_t liste{ "OnDialogRequest" };
            liste[1] = a.finishDialog();
            g_server->send(true, liste);

            return true;
            }
        else if (find_command_force(chat, "/speed")) {
            Dialog a;
            a.addLabelWithIcon("Speed Settings", 2324, LABEL_BIG);
            a.addInputBox("speed_x", "`9Speed:", to_string(speed_x), 7);
            a.addInputBox("speed_y", "`9Gravity:", to_string(speed_y), 7);
            a.endDialog("speed_page", "Okey", "Cancel");
            variantlist_t liste{ "OnDialogRequest" };
            liste[1] = a.finishDialog();
            g_server->send(true, liste);

            return true;
        }
        else if (find_command_force(chat, "/options"))
        {
            Dialog casino;
            casino.addLabelWithIcon("`3Options Page", 262, LABEL_BIG);
            casino.addSpacer(SPACER_SMALL);
            casino.addCheckbox("autosurg", "`^Enable `9Auto Surg", auto_surg);
            casino.addCustomMargin2();
            casino.addCustomText("`3Grants You The Abilit To Auto Surg.");
            casino.addCustomMargin1();
            casino.addCheckbox("fastdrbp", "`^Enable `9Auto Acces", autoacc);
            casino.addCustomMargin2();
            casino.addCustomText("`3It Allows You To Automaticaly Accept Any Access From Any Given Lock.");
            casino.addCustomMargin1();
            casino.addCheckbox("autobglss", "`^Enable `9Fast Change BGL", autobgl);
            casino.addCustomMargin2();
            casino.addCustomText("`3Enables You To Fast Change BGL With Wrench Only.");
            casino.addCustomMargin1();
            casino.addCheckbox("pathfindds", "`^Enable `9Pathfinding Toggle", pathfinder);
            casino.addCustomMargin2();
            casino.addCustomText("`3You Can Teleport To Any Position With `2Shift + Click.");
            casino.addCustomMargin1();
            casino.addCheckbox("denemectrl", "`^Enable `9Ctrl Tp", denemectrl);
            casino.addCustomMargin2();
            casino.addCustomText("`3You Can Use It For Tele.");
            casino.addCustomMargin1();
            casino.addCheckbox("fastdrop", "`^Enable `9Fast Drop", fastdrop);
            casino.addCustomMargin2();
            casino.addCustomText("`3Enables Fast Drop Without Additional Dialogs.");
            casino.addCustomMargin1();
            casino.addCheckbox("fasttrash", "`^Enable `9Fast Trash", fasttrash);
            casino.addCustomMargin2();
            casino.addCustomText("`3Enables Fast TrashWithout Additional Dialogs.");
            casino.addCustomMargin1();
            casino.addCheckbox("fastghost", "`^Enable `9Ghost Mode", gt::ghost);
            casino.addCustomMargin2();
            casino.addCustomText("`3Enables To Moderator Mode Can Go Throught Blocks.");
            casino.addCustomMargin1();
            casino.addCheckbox("ghostanti", "`^Enable `9Anti Ghost", gt::antighost);
            casino.addCustomMargin2();
            casino.addCustomText("`3It Prevents Ghosts From Appearing.");
            casino.addCustomMargin1();
            casino.addCheckbox("fastgravity", "`^Enable `9AntiGravity", antigravity);
            casino.addCustomMargin2();
            casino.addCustomText("`3Grants You Ability To Unlimited Jumps.");
            casino.addCustomMargin1();
            casino.addCheckbox("enablemod", "`^Enable `9Auto Mod Detect", modas);
            casino.addCustomMargin2();
            casino.addCustomText("`3Enables Automatic `#@Mod `3Detector & Acts Upon Mod Detect Settings.");
            casino.addCustomMargin1();
            casino.addButton("kadaryt", "`9Mod Detect Settings");
            casino.addCheckbox("pocketlighterban", "`^Enable `9Pocket Lighter Ban", PocketLighterBan);
            casino.addCustomMargin2();
            casino.addCustomText("`3Will Automatically Ban Someone Put Pocket Lighter.");
            casino.addCustomMargin1();
            casino.addCheckbox("autobanfire", "`^Enable `9Eldritch Flame Fire", autobanfire);
            casino.addCustomMargin2();
            casino.addCustomText("`3Will Automatically Ban Someone Put Eldritch Flame.");
            casino.addCustomMargin1();
            casino.addCheckbox("namenumber", "`^Enable `9Show Last Roulette Spin", namenumber);
            casino.addCustomMargin2();
            casino.addCustomText("`3Will Display Every Player's Last Roulette Wheel Spin Next To Their Name.");
            casino.addCustomMargin1();
            casino.addCheckbox("latency", "`^Enable `9Display Ping Latency", pinglatency);
            casino.addCustomMargin2();
            casino.addCustomText("`3Will Display Your Ping Latency Next To Your Name.");
            casino.addCustomMargin1();
            casino.addCheckbox("meg4reemer", "`^Enable `9Roulette Reme Mode", Nexusreme);
            casino.addCustomMargin2();
            casino.addCustomText("`3Will Show You Reme Numbers Next To Orginal Spin.");
            casino.addCustomMargin1();
            casino.addCheckbox("qqrltmeg4", "`^Enable `9Roulette QQ Mode", Nexusqq);
            casino.addCustomMargin2();
            casino.addCustomText("`3Will Show You QQ Numbers Next To Original Spin.");
            casino.addCustomMargin1();
            casino.addCheckbox("dicerollx", "`^Enable `9Dice Roll", diceroll);
            casino.addCustomMargin2();
            casino.addCustomText("`3Show Dice-Roll Amount Fastly.");
            casino.addCustomMargin1();
            casino.addCheckbox("hidelevel", "`^Enable `9Hide You Need To Be Level Message", hidelevel);
            casino.addCustomMargin2();
            casino.addCustomText("`3Will Hide The Message 'You Need To Be Level'");
            casino.addCustomMargin1();
            casino.addQuickExit();
            casino.endDialog("end", "OK", "Cancel");
            variantlist_t liste{ "OnDialogRequest" };
            liste[1] = casino.finishDialog();
            g_server->send(true, liste);
            return true;
        }
        else if (find_command_force(chat, "/title")) {
            Dialog a;
            a.addLabelWithIcon("`@Title Page", 5956, LABEL_BIG);
            a.addSmallText(" ");
            a.addButton("legendtitle", "`9Legend `wTitle");
            a.addButton("mentortitle", "`8Mentor `wTitle");
            a.addButton("g4gtitle", "`2G4G `wTTitle");
            a.addButton("doctortitle", "`4Doctor `wTitle");
            a.addButton("moderatortitle", "`5Moderator `wTitle");
            a.addButton("maxleveltitle", "`eMaxLevel `wTitle");
            variantlist_t liste{ "OnDialogRequest" };
            liste[1] = a.finishDialog();
            g_server->send(true, liste);

            return true;
            }
        else if (find_command_force(chat, "/role"))
        {
            Dialog role;
            role.addLabelWithIcon("Role", 11816, LABEL_BIG);
            role.addSpacer(SPACER_SMALL);
            role.addCheckbox("rolebuilder", "Role Builder", rolebuilder);
            role.addCheckbox("rolesurgeon", "Role Surgeon", rolesurgeon);
            role.addCheckbox("rolefisher", "Role Fisher", rolefisher);
            role.addCheckbox("rolecook", "Role Cook", rolecook);
            role.addCheckbox("rolefarmer", "Role Farmer", rolefarmer);
            role.addQuickExit();
            role.endDialog("roledialog", "Okay", "Cancel");
            variantlist_t packet{ "OnDialogRequest" };
            packet[1] = role.finishDialog();
            g_server->send(true, packet);
            return true;
        }
        else if (find_command_force(chat, "/vend")) {
            Dialog vend;
            vend.addLabelWithIcon("`9Vending Page", 2978, LABEL_BIG);
            vend.addSpacer(SPACER_SMALL);
            vend.addCheckbox("stockadd", "`^Enable `9Fast Vend Stock", addstock);
            vend.addCustomMargin2();
            vend.addCustomText("`3Will Restock Items Without Any Additional Dialogs.");
            vend.addCustomMargin1();
            vend.addCheckbox("stockpull", "`^Enable `9Fast Vend Withdraw", pullstock);
            vend.addCustomMargin2();
            vend.addCustomText("`3Will Withdraw Items Without Any Additional Dialogs.");
            vend.addCustomMargin1();
            vend.addCheckbox("fastvend", "`^Enable `9Fast Vend Buy", fastvend);
            vend.addCustomMargin2();
            vend.addCustomText("`3Will Automatically Purchase 'Fast Vend Buy' Amount Of Items Without Any Additional Dialogs.");
            vend.addCustomMargin1();
            vend.addInputBox("vend_count", "`9Fast Vend Buy Amount: ", to_string(vendcount), 3);
            vend.addQuickExit();
            vend.endDialog("vend_page", "OK", "Cancel");
            variantlist_t liste{ "OnDialogRequest" };
            liste[1] = vend.finishDialog();
            g_server->send(true, liste);
            return true;

        }
        else if (find_command_force_id(chat, "/count ")) {
            dcount = chat.substr(7);
            if (check_number(dcount)) {
                gt::log_to_console("`9Successfully set count to `2" + dcount);
            }
            else {
                gt::log_to_console("`9PLease use only digits");
            }
            return true;
        }
        else if (find_command_force_id(chat, "/dcount ")) {
            fastcount = chat.substr(8);
            if (check_number(fastcount)) {
                gt::log_to_console("`9Successfully set count to `2" + fastcount);
            }
            else {
                gt::log_to_console("`9PLease use only digits");
            }
            return true;
        }
        else if (find_command_force(chat, "/ntax")) {
            Dialog casino;
            casino.addLabelWithIcon("Sellect Game Mode", 758, LABEL_BIG);
            casino.addSpacer(SPACER_SMALL);
            casino.addTextBox("`4Note! ``If you open NoTax mode pos1 is notax, pos2 is host!");
            casino.addCheckbox("notaxmode", "Enable Notax Mode", notax_mode);
            casino.addInputBox("lampx", "Notax X:", std::to_string(notaxx), 2);
            casino.addInputBox("lampy", "Notax Y:", std::to_string(notaxy), 2);
            casino.addQuickExit();
            casino.endDialog("end", "OK", "Cancel");
            variantlist_t liste{ "OnDialogRequest" };
            liste[1] = casino.finishDialog();
            g_server->send(true, liste);
            return true;
            }
        else if (find_command_force_id(chat, "/split ")) {
            vector2_t pos;
            pos.m_x = pos1.m_x;
            pos.m_y = pos1.m_y;
            std::string splittax = chat.substr(7);
            int taxxd = safe_stoi(splittax);
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
            int wl;
            int dl;
            int bgl;
            wl = item_count(242);
            dl = item_count(1796) * 100;
            bgl = item_count(7188) * 10000;
            int total = wl + dl + bgl;
            int totalwl = total;
            int calculate = totalwl * taxxd / 100;
            gt::log_to_console("`9" + std::to_string(taxxd) + "% Of `2" + std::to_string(totalwl) + " `9is `2" + std::to_string(calculate) + " `9Wls");
            bool aga = custom_drop(calculate, pos, pos1.m_x, pos1.m_y);
            return true;
        }
        else if (find_command_force(chat, "/gd")) {
            vector2_t pos;
            pos.m_x = pos2.m_x;
            pos.m_y = pos2.m_y;
            int normalx = pos2.m_x / 32;
            int normaly = pos2.m_y / 32;

            gt::log_to_console("`2You're trying to drop `9" + to_string(gdrop) + "Wls `2But you don't have that many");

            bool aga = custom_drop(gdrop, pos, pos2.m_x, pos2.m_y);
            return true;
        }
        else if (find_command_force_id(chat, "/game ")) {
            std::string totalbet = chat.substr(6);

            int player_basina_dusen_bet = (safe_stoi(totalbet) / 2);

            gt::log_to_console("`9This bet `b" + std::to_string(player_basina_dusen_bet) + "Wls `9v `b" + std::to_string(player_basina_dusen_bet) + "Wls");

            int total_tax = get_tax_amount(safe_stoi(totalbet));

            gt::log_to_console("`9Amount to drop: `b" + std::to_string(total_tax) + "Wls");

            int kac_tax_aldim = safe_stoi(totalbet) - total_tax;

            gt::log_to_console("`9Tax Taken: `b" + std::to_string(kac_tax_aldim) + "Wls");
            gdrop = total_tax;
            return true;
        }

        else if (find_command_force(chat, "/showxy")) {
            gt::showxy = !gt::showxy;
            if (gt::showxy)
                gt::log_to_console("`9Show X,Y Position `2Enabled");
            else
                gt::log_to_console("`9Show X,Y Position `4Disabled");
            return true;
        }
        else if (find_command_force(chat, "/debug"))
        {
            debug = !debug;
            if (debug)
                gt::log_to_console("`9Packet Debugger Mode is `2ON");
            else
                gt::log_to_console("`9Packet Debugger Mode is `4OFF");
            return true;
        }
        else if (find_command_force(chat, "/antighost"))
        {
            gt::antighost = !gt::antighost;
            if (gt::antighost)
                gt::log_to_console("`2Anti Ghost Enabled!");
            else
                gt::log_to_console("`4Anti Ghost Disabled!");
            return true;
        }
        else if (find_command_force(chat, "/antizombie"))
        {
            antizombie = !antizombie;
            if (antizombie)
                gt::log_to_console("`2Anti Zombie Enabled!");
            else
                gt::log_to_console("`4Anti Zombie Disabled!");
            return true;
        }
        else if (find_command_force(chat, "/dev")) {
            devmode = !devmode;
            if (devmode)
            {
                variantlist_t liste{ "OnTextOverlay" };
                liste[1] = "`#@Developer Mode `2Enabled.";
                g_server->send(true, liste);
                gt::log_to_console("`#@Developer Mode `2Enabled.");
                return true;
            }
            else
            {
                variantlist_t liste{ "OnTextOverlay" };
                liste[1] = "`#@Developer Mode `4Disabled.";
                g_server->send(true, liste);
                gt::log_to_console("`#@Developer Mode `4Disabled.");
                return true;
            }

        }
        else if (find_command_force(chat, "/chest"))
        {
            for (auto& tile : g_server->m_world.tile_map.tiles)
            {
                if (tile->foreground == 596 || tile->foreground == 3398)
                {
                    GameUpdatePacket packet{ 0 };
                    packet.type = PACKET_TILE_CHANGE_REQUEST;
                    packet.item_id = 2;
                    packet.int_x = tile->pos.m_x;
                    packet.int_y = tile->pos.m_y;
                    g_server->send(true, NET_MESSAGE_GAME_PACKET, (uint8_t*)&packet, sizeof(GameUpdatePacket));   
                }
            }
            gt::log_to_console("`9Revealing Items In Chests!");
            return true;
        }
        else if (find_command_force(chat, "/replace"))
        {
            for (int xx = 0; xx < world.tile_map.size.m_x; xx++) {
                for (int yy = 0; yy < world.tile_map.size.m_y; yy++) {

                    GameUpdatePacket packet{ 0 };
                    packet.type = PACKET_TILE_CHANGE_REQUEST;
                    packet.item_id = 56;
                    packet.int_x = xx;
                    packet.int_y = yy;
                    g_server->send(true, NET_MESSAGE_GAME_PACKET, (uint8_t*)&packet, sizeof(GameUpdatePacket));
                }
            }
            gt::log_to_console("`9Replacing chest with glass!");
            return true;

        }
        else if (find_command_force(chat, "/antigravity") || find_command_force(chat, "/ag")) {
            antigravity = !antigravity;
            if (antigravity)
            {
                gt::log_to_console("`2Antigravity Enabled!");
                GameUpdatePacket packet{ 0 };
                packet.type = PACKET_TILE_CHANGE_REQUEST;
                packet.item_id = 4992;
                packet.int_x = 99;
                packet.int_y = 59;
                g_server->send(true, NET_MESSAGE_GAME_PACKET, (uint8_t*)&packet, sizeof(GameUpdatePacket));
                std::this_thread::sleep_for(std::chrono::milliseconds(500));

                auto pos = g_server->m_world.local.pos;

                g_server->enterDoor(true, 99, 59);


                GameUpdatePacket legitpacket{ 0 };
                legitpacket.type = PACKET_STATE;
                legitpacket.item_id = 18;
                legitpacket.int_x = 99;
                legitpacket.int_y = 59;
                legitpacket.vec_x = pos.m_x;
                legitpacket.vec_y = pos.m_y;
                legitpacket.flags = 2592;
                g_server->send(true, NET_MESSAGE_GAME_PACKET, (uint8_t*)&legitpacket, sizeof(GameUpdatePacket));
                antigravity = true;
                return true;
            }
            else
            {
                gt::log_to_console("`4Antigravity Disabled!");
                return true;
            }

        }
        else if (find_command_force(chat, "/autopull"))
        {
            enter_pull = !enter_pull;
            if (enter_pull)
                gt::log_to_console("`9Auto Pull Is Now `2Enabled");
            else
                gt::log_to_console("`9Auto Pull Is Now `4Disabled");
            return true;
        }
        else if (find_command_force(chat, "/autoban"))
        {
            enter_ban = !enter_ban;
            if (enter_ban)
                gt::log_to_console("`9Auto Ban Is Now `2Enabled");
            else
                gt::log_to_console("`9Auto Ban Is Now `4Disabled");
            return true;
        }
        else if (find_command_force(chat, "/realspin"))
        {
            realfake = !realfake;
            if (realfake)
                gt::log_to_console("`2Enable `9Real Spin Mode.");
            else
                gt::log_to_console("`4Disable `9Real Spin Mode.");
            return true;
            }
        else if (find_command_force(chat, "/spinqq"))
        {
            Nexusqq = !Nexusqq;
            if (Nexusqq)
                gt::log_to_console("`2Enable `9Roulette QQ Mode.");
            else
                gt::log_to_console("`4Disable `9Roulette QQ Mode.");
            return true;
        }
        else if (find_command_force(chat, "/spinreme"))
        {
            Nexusreme = !Nexusreme;
            if (Nexusreme)
                gt::log_to_console("`2Enable `9Roulette Reme Mode.");
            else
                gt::log_to_console("`4Disable `9Roulette Reme Mode.");
            return true;
        }
        else if (find_command_force(chat, "/fdice"))
        {
            diceroll = !diceroll;
            if (diceroll)
                gt::log_to_console("`2Enable `9Fast Dice Mode.");
            else
                gt::log_to_console("`4Disable `9Fast Dice Mode.");
            return true;
            }
        else if (find_command_force(chat, "/proxy")) {
            Dialog a;
            a.addLabelWithIcon("`2Proxy Commands", 1790, LABEL_SMALL);
            a.addSpacer(SPACER_SMALL);
            a.addTextBox("`2Information");
            a.addSmallText("`2/proxy `9(Opens Proxy Commands Dialog)");
            a.addSmallText("`2/discord `9(Opens Nexus Premium Proxy Discord Link)");
            a.addSmallText("`2/news `9(Opens Proxy's News Gazette)");
            a.addSpacer(SPACER_SMALL);
            a.addTextBox("`2Useful For Casino Hosting/Playing");
            a.addSmallText("`2/pos `9(Show Both Positions And Bets)");
            a.addSmallText("`2/pos1 `9(Set Display Box Pos 1)");
            a.addSmallText("`2/pos2 `9(Set Display Box Pos 2)");
            a.addSmallText("`2/tp `9(Take WLS/DLS/BGLS From Positions)");
            a.addSmallText("`2/win1 & /w1 `9(Drop Winnings To Player 1)");
            a.addSmallText("`2/win2 & /w2 `9(Drop Winnings To Player 2)");
            a.addSmallText("`2/reset `9(Reset All Positions)");
            a.addSmallText("`2/gems `9(Displays Gem Count In All Tiles)");
            a.addSmallText("`2/track `9(Enables Dropping/Collecting Tracker)");
            a.addSmallText("`2/logs `9(Opens Logs Page For Item Collecting/Dropping/Roulette)");
            a.addSmallText("`2/instantgem & /ig `9(Enable Show Instant Gem Count In Tile)");
            a.addSmallText("`2/numbername & /nn `9(Show Last Spinned Number Next To Player Name)");
            a.addSmallText("`2/fastdoor `9(Enables Fast Open/Close Doors With Wrench)");
            a.addSmallText("`2/spam `9(Opens Auto Spam Page Dialog For Auto Spamming)");
            a.addSmallText("`2// `9(Open/Close Auto Spam Shortcut)");
            a.addSmallText("`2/realspin `9(Enables Show REAL/FAKE Spin)");
            a.addSmallText("`2/spinqq `9(Enable QQ Spins)");
            a.addSmallText("`2/spinreme `9(Enable Reme Spins)");
            a.addSmallText("`2/scan `9(Enables Free Extract-O-Snap)");
            a.addSmallText("`2/tax `9(Set Game Tax)");
            a.addSmallText("`2/game `9[Bet Amount] (Calculate Tax From Bet)");
            a.addSmallText("`2/gdrop & /gd `9(Drop Last Played Game Winnings)");
            a.addSpacer(SPACER_SMALL);
            a.addTextBox("`2Useful Features");
            a.addSmallText("`2/antigravity `9(Enables Anti Gravity)");
            a.addSmallText("`2/antighost `9(Enables Anti Ghost)");
            a.addSmallText("`2/antizombie `9(Enables Anti Zombie)");
            a.addSmallText("`2/emptybox `9(Enables Fast Put To Donate Box)");
            a.addSmallText("`2/dcount [count] `9(Set Fast Donate Count)");
            a.addSmallText("`2/fastempty `9(Enables Fast Empty To Donate Box)");
            a.addSmallText("`2/settings `9(Opens Dialog For Proxy Settings Page)");
            a.addSmallText("`2/vend `9(Opens Fast Vend Page)");
            a.addSmallText("`2/fdice `9(Enables Show Instant Dice Roll In Tile)");
            a.addSmallText("`2/name [name] `9(Change Your GrowID Visually)");
            a.addSmallText("`2/hotkeys `9(Opens Hotkeys Page Dialog)");
            a.addSmallText("`2/setsave [world] `9(Set Save World)");
            a.addSmallText("`2/save `9(Warps To Save World) (/setsave [world])");
            a.addSmallText("`2/split [%] `9(Split Your WLS/DLS/BGLS With Percent)");
            a.addSmallText("`2/gscan & /growscan `9(Free GrowScan With Proxy)");
            a.addSpacer(SPACER_SMALL);
            a.addTextBox("`2Cool Features");
            a.addSmallText("`2/weather `9(Opens Visual Weather Changer Page)");
            a.addSmallText("`2/vdrop [Item ID] `9(Drop Custom Item Visually)");
            a.addSmallText("`2/pf `9(Enables PathFinder) [Shift + Punch]");
            a.addSmallText("`2/speed `9(Opens A Speed Changer Dialog)");
            a.addSmallText("`2/title `9(Opens A Dialog Of Visual Titles)");
            a.addSmallText("`2/find `9(Opens A Item Finder Dialog)");
            a.addSmallText("`2/clothes `9(Visual Clothes Page)");
            a.addSmallText("`2/vc `9(Enables Visual Clothes Equip)");
            a.addSmallText("`2/mod `9(Enables Moderator Mode)");
            a.addSmallText("`2/blink `9(Automatic Skin Changer)");
            a.addSmallText("`2/rspam `9(Spam All Roulettes In World)");
            a.addSpacer(SPACER_SMALL);
            a.addTextBox("`2Drop Faster Features");
            a.addSmallText("`2/fd `9(Enables Fast Drop)");
            a.addSmallText("`2/ft `9(Enables Fast Trash)");
            a.addSmallText("`2/daw `9(Drop All WLS/DLS/BGLS)");
            a.addSmallText("`2/dropall `9(Drop All Items From Inventory)");
            a.addSmallText("`2/cd & /cdrop [Amount] `9(Drop WLS/DLS By Amount)");
            a.addSmallText("`2/dd & /ddrop [Amount] `9(Drop DLS/BGLS By Amount)");
            a.addSpacer(SPACER_SMALL);
            a.addTextBox("`2Automatic Features");
            a.addSmallText("`2/collect `9(Auto Collect All Items In 10 Far)");
            a.addSmallText("`2/ac `9(Enables Auto Collect)");
            a.addSmallText("`2/autocollect `9(Enables Collect Settings Page)");
            a.addSmallText("`2/autosurg `9(Enables Auto Surg)");
            a.addSmallText("`2/autocrime `9(Enables Auto Crime)");
            a.addSmallText("`2/autofarm `9(Opens Auto Farm Page For Changing Item, Delay, Etc..)");
            a.addSmallText("`2/farm `9(Enables Auto Farm)");
            a.addSmallText("`2/automsg `9(Enables Auto Msg)");
            a.addSmallText("`2/setmsg [Text] `9(Set Auto Msg Text)");
            a.addSmallText("`2/autowrench `9(Opens Page For Auto Pull & Auto Ban)");
            a.addSmallText("`2/autopull `9(Enables Auto Pull When Join)");
            a.addSmallText("`2/autoban `9(Enables Auto Ban When Join)");
            a.addSmallText("`2/fireban `9(Enables Auto Ban Players That Use Pocket Lighter)");
            a.addSmallText("`2/autoacc `9(Enables Auto Take Access)");
            a.addSmallText("`2/gp `9(Enables Gass Pull) (If Someone Say Gass You Will Pull The Player)");
            a.addSmallText("`2/testmod `9(Mod Detect Test)");
            a.addSpacer(SPACER_SMALL);
            a.addTextBox("`2Other Features");
            a.addSmallText("`2/fdonate `9(Opens A Dialog For Settings Fake Donation)");
            a.addSmallText("`2/donate `9(Execute The Fake Donation Message)");
            a.addSmallText("`2/fps `9(Increases Growtopia's Smoothness)");
            a.addSmallText("`2/teleport [Name] `9(Pathfinds To Specific Player's Position)");
            a.addSmallText("`2/account `9(Opens A Dialog Of `4Your Account `9Info)");
            a.addSmallText("`2/wrench `9(Open Wrench Change Dialog)");
            a.addSmallText("`2/back `9(Warps To Back To Previous World)");
            a.addSmallText("`2/inventory `9(Opens A Dialog Of Your Items In Inventory)");
            a.addSmallText("`2/relog `9(Fast Leave And Enter To World)");
            a.addSmallText("`2/balance `9(Show Current Lock Balance)");
            a.addSmallText("`2/ghost `9(Enables Ghost Mode)");
            a.addSmallText("`2/options `9(Opens Proxy Options Page)");
            a.addSmallText("`2/fakeban `9(You Will Get Visually `4Banned `9With This)");
            a.addSmallText("`2/res `9(Respawn Shortcut)");
            a.addSmallText("`2/flag [ID] `9(Change Flag To Item ID)");
            a.addSmallText("`2/warp [World] `9(Warp To Other World)");
            a.addSmallText("`2/pullall `9(Pull Everyone Same World)");
            a.addSmallText("`2/kickall `9(Kick Everyone Same World)");
            a.addSmallText("`2/banall `9(Ban Everyone Same World)");
            a.addSpacer(SPACER_SMALL);
            a.addQuickExit();
            a.endDialog("Thank You", "Okey", "Cancel");
            variantlist_t liste{ "OnDialogRequest" };
            liste[1] = a.finishDialog();
            g_server->send(true, liste);
            return true;
            }
            return false;
    }
    if (packet.find("game_version|") != -1) {
        rtvar var = rtvar::parse(packet);
        // This used to re-serialise the login untouched despite the message below.
        // Overwrite the version the client reports so the proxy presents gt::version.
        if (rtvar_t* game_version = var.find("game_version")) {
            game_version->m_value = gt::version;
            if (!game_version->m_values.empty())
                game_version->m_values[0] = gt::version;
        }
        packet = var.serialize();
        gt::in_game = false;
        PRINTS("Spoofing login info (game_version -> %s)\n", gt::version.c_str());
        g_server->send(false, packet);
        return true;
    } // şu anlık böyle giriyo mu diye bak 

    return false;
}

bool events::out::gamemessage(std::string packet) {
    PRINTS("Game Message: %s\n", packet.c_str());
    if (packet == "action|quit") {
        g_server->quit();
        return true;
    }
    return false;
}

bool events::out::state(gameupdatepacket_t* packet, GameUpdatePacket* packets) {
    if (!g_server->m_world.connected)
        return false;
    g_server->m_world.local.pos = vector2_t{ packet->m_vec_x, packet->m_vec_y };
    PRINTS("Local Pos: %.0f %.0f\n", packets->vec_x, packets->vec_y);

    if (slcfarm && (packet->m_packet_flags == UPDATE_PACKET_PUNCH_TILE_LEFT || packet->m_packet_flags == UPDATE_PACKET_PUNCH_TILE_RIGHT)) {
        //gt::log_to_console(std::to_string(packet->m_state1) + " " + std::to_string(packet->m_state2));

        std::ofstream outfile("C:\\Users\\Public\\Nexus_Stuff\\selectfarm.txt", std::ios_base::app);
        if (outfile.is_open()) {
            outfile << "x:" << packet->m_state1 << " | y:" << packet->m_state2 << std::endl;
            outfile.close();
        }
    }
    if (gt::showxy == true)
    {
        int playerx = g_server->m_world.local.pos.m_x / 32;
        int playery = g_server->m_world.local.pos.m_y / 32;
        variantlist_t va{ "OnNameChanged" };
        va[1] = g_server->m_world.local.name + " `4[" + std::to_string(playerx) + "," + std::to_string(playery) + "]" + " `4[" + std::to_string(g_server->m_world.local.netid) + "]``" + " `#[" + to_string(g_server->m_world.local.userid) + "]``";
        g_server->send(true, va, g_server->m_world.local.netid, -1);
    }
    if (packets->type == 0 && (packets->flags == 32 || packets->flags == 48)) {
        gt::right_left = packets->flags;
    }

    bool ctrlPressed = ((GetKeyState(VK_CONTROL) & 0x8000) != 0) ? true : false;
    if (denemectrl && ctrlPressed && packet->m_state1 != 4294967295 && packet->m_state2 != 4294967295) {
        gt::denemectrl(packet->m_state1, packet->m_state2);
        return true;
    }

    bool key = ((GetKeyState(VK_SHIFT) & 0x8000) != 0) ? true : false;

    // Pathfinding is driven by the punch packet, and the client only sends one for tiles
    // inside its punch range -- click further away and nothing arrives, so nothing moves.
    // Telling the client it has unlimited reach while shift is held makes it report the
    // distant tile too. The punch itself is swallowed below, so the server never sees it.
    //
    // This used to flip serverlongpunch on every state packet, so the extended reach only
    // reached the client half the time and the normal-reach reset was spammed constantly.
    // Send it once when the state changes instead.
    static bool long_punch_active = false;
    const bool want_long_punch = pathfinder && key;

    if (want_long_punch != long_punch_active) {
        long_punch_active = want_long_punch;
        serverlongpunch = want_long_punch;
        g_server->sendState(g_server->m_world.local.netid, want_long_punch);
    }


    static std::chrono::steady_clock::time_point lastExecutionTime = std::chrono::steady_clock::now();

    if (pathfinder && key && packet->m_state1 != 4294967295 && packet->m_state2 != 4294967295) {
        std::chrono::steady_clock::time_point currentTime = std::chrono::steady_clock::now();
        std::chrono::duration<double> elapsedTime = std::chrono::duration_cast<std::chrono::duration<double>>(currentTime - lastExecutionTime);

        if (elapsedTime >= std::chrono::milliseconds(300)) {
            g_server->MoveXY(packet->m_state1, packet->m_state2);
            lastExecutionTime = std::chrono::steady_clock::now();

            return true;
        }
        else {
            variantlist_t notif{ "OnTextOverlay" };
            notif[1] = "`4Slow down -- `9Pathfinding `4again in a moment.";
            g_server->send(true, notif, -1, -1);
            // Swallow it. While shift is held the client is running on a faked punch
            // range, so forwarding this would show the server a punch from well out of
            // reach -- exactly the kind of thing it disconnects you for.
            return true;
        }
    }
    if (pfmode && (packet->m_packet_flags == UPDATE_PACKET_PUNCH_TILE_LEFT || packet->m_packet_flags == UPDATE_PACKET_PUNCH_TILE_RIGHT)) {
        GameUpdatePacket packets{ 0 };
        packets.type = PACKET_TILE_CHANGE_REQUEST;
        packets.item_id = 102;
        packets.int_x = packet->m_state1;
        packets.int_y = packet->m_state2;
        g_server->send(true, NET_MESSAGE_GAME_PACKET, (uint8_t*)&packets, sizeof(GameUpdatePacket));
    }

    if (devmode && (packet->m_packet_flags == UPDATE_PACKET_PUNCH_TILE_LEFT || packet->m_packet_flags == UPDATE_PACKET_PUNCH_TILE_RIGHT)) {
        GameUpdatePacket packets{ 0 };
        packets.type = PACKET_TILE_CHANGE_REQUEST;
        packets.item_id = 0;
        packets.int_x = packet->m_state1;
        packets.int_y = packet->m_state2;
        g_server->send(true, NET_MESSAGE_GAME_PACKET, (uint8_t*)&packets, sizeof(GameUpdatePacket));
    }
    if (devmode && (packet->m_packet_flags == UPDATE_PACKET_PLACE_BLOCK_LEFT || packet->m_packet_flags == UPDATE_PACKET_PLACE_BLOCK_RIGHT)) {
        GameUpdatePacket packetlol{ 0 };
        packetlol.type = PACKET_TILE_CHANGE_REQUEST;
        packetlol.item_id = packet->m_int_data;
        packetlol.int_x = packet->m_state1;
        packetlol.int_y = packet->m_state2;
        g_server->send(true, NET_MESSAGE_GAME_PACKET, (uint8_t*)&packetlol, sizeof(GameUpdatePacket));
    }

    if (paintauto && (packet->m_packet_flags == UPDATE_PACKET_PLACE_BLOCK_LEFT || packet->m_packet_flags == UPDATE_PACKET_PLACE_BLOCK_RIGHT))
    {
        if (paintspecial)
        {
            if (paintblock == packet->m_int_data)
            {
                Sleep(paintdelay);
                gameupdatepacket_t packetknk{};
                packetknk.m_type = 3;
                packetknk.m_int_data = paintid;
                packetknk.m_vec_x = g_server->m_world.local.pos.m_x;
                packetknk.m_vec_y = g_server->m_world.local.pos.m_y;
                packetknk.m_state1 = packet->m_state1;
                packetknk.m_state2 = packet->m_state2;
                g_server->send(false, NET_MESSAGE_GAME_PACKET, (uint8_t*)&packetknk, sizeof(gameupdatepacket_t));
                Sleep(paintdelay);
                gt::log_to_console("`2[" + std::to_string(packet->m_state1) + "," + std::to_string(packet->m_state2) + "] Sent Place!");
            }
            if (paintblock == 0)
            {
                Sleep(paintdelay);
                gameupdatepacket_t packetknk{};
                packetknk.m_type = 3;
                packetknk.m_int_data = paintid;
                packetknk.m_vec_x = g_server->m_world.local.pos.m_x;
                packetknk.m_vec_y = g_server->m_world.local.pos.m_y;
                packetknk.m_state1 = packet->m_state1;
                packetknk.m_state2 = packet->m_state2;
                g_server->send(false, NET_MESSAGE_GAME_PACKET, (uint8_t*)&packetknk, sizeof(gameupdatepacket_t));
                Sleep(paintdelay);
                gt::log_to_console("`2[" + std::to_string(packet->m_state1) + "," + std::to_string(packet->m_state2) + "] Sent Place!");
            }
        }
        else
        {
            Sleep(paintdelay);
            gameupdatepacket_t packetknk{};
            packetknk.m_type = 3;
            packetknk.m_int_data = paintid;
            packetknk.m_vec_x = g_server->m_world.local.pos.m_x;
            packetknk.m_vec_y = g_server->m_world.local.pos.m_y;
            packetknk.m_state1 = packet->m_state1;
            packetknk.m_state2 = packet->m_state2;
            g_server->send(false, NET_MESSAGE_GAME_PACKET, (uint8_t*)&packetknk, sizeof(gameupdatepacket_t));
            Sleep(paintdelay);
            gt::log_to_console("`2[" + std::to_string(packet->m_state1) + "," + std::to_string(packet->m_state2) + "] Sent Place!");
        }
    }


    if (scanitem && (packet->m_packet_flags == UPDATE_PACKET_PUNCH_TILE_LEFT || packet->m_packet_flags == UPDATE_PACKET_PUNCH_TILE_RIGHT)) {
        thread(itemScanDialog, packet->m_state1, packet->m_state2).detach();
    }

    if (pathfinder && key || gemcount)
    {
        serverlongpunch = !serverlongpunch;
        if (serverlongpunch) {
            g_server->sendState(g_server->m_world.local.netid, true);

        }
    }
    else
    {
        serverlongpunch = false;
        g_server->sendState(g_server->m_world.local.netid, false);
    }

    if (gemcount && packet->m_state1 != 4294967295 && packet->m_state2 != 4294967295) {
        thread(GemTake, packet->m_state1, packet->m_state2).detach();
    }
    if (dancemove) {
        packet->m_jump_amount = 0;
        packet->m_packet_flags = 16 | 8;
        packet->m_vec2_x = 0;
        packet->m_vec2_y = 0;
    }
    if (superpunch) {
        packet->m_packet_flags = 8390688;
        packet->m_state1 = packet->m_state1;
        packet->m_state2 = packet->m_state2;
        packet->m_vec_x = packet->m_vec_x;
        packet->m_vec_y = packet->m_vec_y;
    }

    eff_flags1 = packets->effect_flags;
    eff_flags2 = packets->flags;

    if (gt::antighost) {
        packets->flags &= ~(1 << 11);
        eff_flags2 &= ~(1 << 11);
    }
    if (fakeblink) {
        std::this_thread::sleep_for(std::chrono::milliseconds(5));
        variantlist_t varlist{ "OnSetPos" };
        varlist[1] = g_server->m_world.local.pos;
        g_server->m_world.local.pos = g_server->m_world.local.pos;
        g_server->send(true, varlist, g_server->m_world.local.netid, -1);
        return true;
    }
    if (gt::ghost)
        return true;
    return false;
}

vector<string> split(const string& str, const string& delim)
{
    vector<string> tokens;
    size_t prev = 0, pos = 0;
    do
    {
        pos = str.find(delim, prev);
        if (pos == string::npos) pos = str.length();
        string token = str.substr(prev, pos - prev);
        if (!token.empty()) tokens.push_back(token);
        prev = pos + delim.length();
    } while (pos < str.length() && prev < str.length());
    return tokens;
}

bool events::in::variantlist(gameupdatepacket_t* packet) {
    variantlist_t varlist{};
    auto extended = utils::get_extended(packet);
    extended += 4;
    varlist.serialize_from_mem(extended);
    auto func = varlist[0].get_string();
    //probably subject to change, so not including in switch statement.
        PRINTC("varlist: %s\n", varlist.print().c_str());
    if (func.find("OnSuperMainStartAcceptLogon") != -1)
        gt::in_game = true;
    switch (hs::hash32(func.c_str())) {
    case fnv32("OnSetRoleSkinsAndIcons"): {
        skinas = varlist[5].get_string();
    } break;
    case fnv32("OnRequestWorldSelectMenu"): {
        old_world = g_server->m_world.name;
        auto& world = g_server->m_world;
        world.players.clear();
        world.local = {};
        world.connected = false;
        world.name = "EXIT";
    } break;
    case fnv32("OnSetClothing"): {

        // check if enough time has passed since last useü
        if (g_server->m_world.connected) {
            if (iswear) {
                int anan = (int)packet->m_player_flags;
                if (anan == g_server->m_world.local.netid) {

                    send_vset();

                    return true;
                }
            }
            else {
                int anan = (int)packet->m_player_flags;
                if (anan == g_server->m_world.local.netid) {
                    hair = varlist.get(3).get_vector3().m_y;
                    neck = varlist.get(3).get_vector3().m_z;
                    face = varlist.get(2).get_vector3().m_y;
                    hand = varlist.get(2).get_vector3().m_z;
                    pants = varlist.get(1).get_vector3().m_z;
                    shoe = varlist.get(2).get_vector3().m_x;
                    shirt = varlist.get(1).get_vector3().m_y;
                    back = varlist.get(3).get_vector3().m_x;
                    mask = varlist.get(1).get_vector3().m_x;
                    send_effect_real();//puncheffect
                }
            }

        }
    } break;
    case fnv32("OnGuildDataChanged"): {
        int Guild_ID = varlist[1].get_int32();
        if (Guild_ID == 15519) {
            if (modas) {
                gt::log_to_console("`wA `4@Moderator `wJoined the World");
                variantlist_t varlist{ "OnAddNotification" };
                varlist[1] = "interface/atomic_button.rttex";
                varlist[2] = "`wA `4@Moderator `wJoined the World";
                varlist[3] = "audio/hub_open.wav";
                varlist[4] = 0;
                g_server->send(true, varlist);
                if (aacNexus) {
          
                    std::this_thread::sleep_for(std::chrono::milliseconds(50));
                }
                if (banas) {
                    ban_everyone();
                    std::this_thread::sleep_for(std::chrono::milliseconds(500));
                }
                if (unaccmod) {
                    g_server->send(false, "action|input\n|text|/unaccess");
                    unaccess = true;
                    g_server->send(false, "action|dialog_return\ndialog_name|unaccess");
                }
                if (exitmod) {
                    g_server->send(false, "action|join_request\nname|exit", 3);
                    std::this_thread::sleep_for(std::chrono::milliseconds(50));
                }
                if (saveworldmod) {
                    gosaveworld();
                    std::this_thread::sleep_for(std::chrono::milliseconds(50));
                }
            }
        }
    }break;
    case fnv32("OnSendToServer"):
    {
        g_server->redirect_server(varlist);
        return true;
    }
    case fnv32("OnConsoleMessage"): {
        auto& wry = varlist[1].get_string();
        if (showPlatforms) {
            std::smatch matches;
            if (std::regex_search(wry, matches, std::regex{ "PL:(\\d+)_" })) {
                std::size_t pos;
                if ((pos = wry.find("```5) in ")) != std::string::npos) {
                    int platformId = safe_stoi(matches[1]);
                    std::string fstr = " `4[`b" + std::string(magic_enum::enum_name(static_cast<platform_id>(platformId))) + "`4]``";
                    wry.insert(pos, fstr);
                    varlist[1].set(wry);
                }
                else if ((pos = wry.find("``>`` `$")) != std::string::npos) {
                    int platformId = safe_stoi(matches[1]);
                    std::string fstr = " `4[`b" + std::string(magic_enum::enum_name(static_cast<platform_id>(platformId))) + "`4]``";
                    wry.insert(pos, fstr);
                    varlist[1].set(wry);
                }
            }
        }
        if (gaspull) {
            if (wry.find("gas") != -1) {
                if (wry.find("`w") != -1) {
                    std::string nasmasma = wry.substr(wry.find("[W]_ `6<`w") + 10, wry.length() - wry.find("[W]_ `6<`w") - 1);
                    nasmasma.erase(nasmasma.begin() + nasmasma.find("``>``"), nasmasma.end());
                    g_server->send(false, "action|input\n|text|/pull " + nasmasma);
                }
            }
        }
        if (wry.find("One moment, updating item data...") != -1)
        {
            gt::log_to_console("`9Skipping ``One moment, updating item data...");
            return true;
        }
  
        if (autoacc) {
            if (wry.find("Wrench yourself to accept.") != -1) {
                nopulldialog = true;
                g_server->send(false, "action|wrench\n|netid|" + std::to_string(g_server->m_world.local.netid));
                g_server->send(false, "action|dialog_return\ndialog_name|popup\nnetID|" + std::to_string(g_server->m_world.local.netid) + "|\nbuttonClicked|acceptlock");
                autoaccc = true;
                g_server->send(false, "action|dialog_return\ndialog_name|acceptaccess");
                variantlist_t varlist{ "OnTextOverlay" };
                varlist[1] = "`2ACCESS GRANTED!";
                thread(autoacca).detach();
                g_server->send(true, varlist);
            }
        }
        if (son && wry.find("`6>>`4Spam detected! ``Please wait a bit before typing anything else.  Please note, any form of bot/macro/auto-paste will get all your accounts banned, so don't do it!") != -1) {
            variantlist_t varlist{ "OnTextOverlay" };
            varlist[1] = "`4Disabled `9Auto Spam Due To Spam Detect.";
            g_server->send(true, varlist);
            son = false;
        }
        varlist[1] = "`$[`^Nexus`$] `$" + varlist[1].get_string();
        g_server->send(true, varlist);
        return true;
    }break;
    case fnv32("OnTalkBubble"): {
        auto wry = varlist[2].get_string();

        auto str = varlist[2].get_string();
        auto netid = varlist[1].get_int32();
        auto& bruh = g_server->m_world.local;
        auto netidspin = varlist[1].get_int32();

        if (netid == g_server->m_world.local.netid)
        {
            //gt::log_to_console("OnTalkBubble Detected on local player!");
            if (str.find("spun the wheel and got") != -1)
            {
                if (PocketLighterBan == true)
                {
                    if (wry.find("`7[```4MWAHAHAHA!! FIRE FIRE FIRE") != -1) {
                        g_server->send(false, "action|wrench\n|netid|" + std::to_string(netid));
                        std::chrono::milliseconds(50);
                        g_server->send(false, "action|dialog_return\ndialog_name|popup\nnetID|" + std::to_string(netid) + "|\nnetID|" + std::to_string(netid) + "|\nbuttonClicked|worldban");
                        return true;
                    }
                }
            }
        }
        if (autobanfire == true) {
            if (wry.find("`7[```4MWAHAHAHA!! FIRE FIRE FIRE") != -1) {
                g_server->send(false, "action|wrench\n|netid|" + std::to_string(netid));
                g_server->send(false, "action|dialog_return\ndialog_name|popup\nnetID|" + std::to_string(netid) + "|\nnetID|" + std::to_string(netid) + "|\nbuttonClicked|worldban");
                return true;
            }
        }
        if (hidelevel)
        {
            if (wry.find("You need to be Level") != -1)
            {
                return true;
            }
        }
        if (wry.find("the wheel and got") != -1)
        {
            std::string spyxde = wry.substr(wry.find("spun the wheel and got ") + 25, wry.length());
            int toplam = 0;
            std::string qqspins = "";
            std::string remespins = "";
            std::string vurulansayi = "";
            int the_num = std::atoi(spyxde.substr(0, spyxde.find(" ")).c_str());

            if (Nexusnumber) {
                switch (the_num) {
                case 0:
                    vurulansayi = "`20";
                    break;
                case 1:
                    vurulansayi = "`41";
                    break;
                case 2:
                    vurulansayi = "`b2";
                    break;
                case 3:
                    vurulansayi = "`43";
                    break;
                case 4:
                    vurulansayi = "`b4";
                    break;
                case 5:
                    vurulansayi = "`45";
                    break;
                case 6:
                    vurulansayi = "`b6";
                    break;
                case 7:
                    vurulansayi = "`47";
                    break;
                case 8:
                    vurulansayi = "`b8";
                    break;
                case 9:
                    vurulansayi = "`49";
                    break;
                case 10:
                    vurulansayi = "`b10";
                    break;
                case 11:
                    vurulansayi = "`b11";
                    break;
                case 12:
                    vurulansayi = "`412";
                    break;
                case 13:
                    vurulansayi = "`b13";
                    break;
                case 14:
                    vurulansayi = "`414";
                    break;
                case 15:
                    vurulansayi = "`b15";
                    break;
                case 16:
                    vurulansayi = "`416";
                    break;
                case 17:
                    vurulansayi = "`b17";
                    break;
                case 18:
                    vurulansayi = "`418";
                    break;
                case 19:
                    vurulansayi = "`419";
                    break;
                case 20:
                    vurulansayi = "`b20";
                    break;
                case 21:
                    vurulansayi = "`421";
                    break;
                case 22:
                    vurulansayi = "`b22";
                    break;
                case 23:
                    vurulansayi = "`423";
                    break;
                case 24:
                    vurulansayi = "`b24";
                    break;
                case 25:
                    vurulansayi = "`425";
                    break;
                case 26:
                    vurulansayi = "`b26";
                    break;
                case 27:
                    vurulansayi = "`427";
                    break;
                case 28:
                    vurulansayi = "`b28";
                    break;
                case 29:
                    vurulansayi = "`b29";
                    break;
                case 30:
                    vurulansayi = "`430";
                    break;
                case 31:
                    vurulansayi = "`b31";
                    break;
                case 32:
                    vurulansayi = "`432";
                    break;
                case 33:
                    vurulansayi = "`b33";
                    break;
                case 34:
                    vurulansayi = "`434";
                    break;
                case 35:
                    vurulansayi = "`b35";
                    break;
                case 36:
                    vurulansayi = "`436";
                    break;
                }
            }
            if (Nexusreme) {
                switch (the_num) {
                case 0:
                    remespins = "`20";
                    break;
                case 1:
                    remespins = "`41";
                    break;
                case 2:
                    remespins = "`42";
                    break;
                case 3:
                    remespins = "`83";
                    break;
                case 4:
                    remespins = "`84";
                    break;
                case 5:
                    remespins = "`65";
                    break;
                case 6:
                    remespins = "`96";
                    break;
                case 7:
                    remespins = "`97";
                    break;
                case 8:
                    remespins = "`98";
                    break;
                case 9:
                    remespins = "`29";
                    break;
                case 10:
                    remespins = "`41";
                    break;
                case 11:
                    remespins = "`42";
                    break;
                case 12:
                    remespins = "`83";
                    break;
                case 13:
                    remespins = "`84";
                    break;
                case 14:
                    remespins = "`65";
                    break;
                case 15:
                    remespins = "`96";
                    break;
                case 16:
                    remespins = "`97";
                    break;
                case 17:
                    remespins = "`98";
                    break;
                case 18:
                    remespins = "`29";
                    break;
                case 19:
                    remespins = "`20";
                    break;
                case 20:
                    remespins = "`42";
                    break;
                case 21:
                    remespins = "`83";
                    break;
                case 22:
                    remespins = "`84";
                    break;
                case 23:
                    remespins = "`65";
                    break;
                case 24:
                    remespins = "`96";
                    break;
                case 25:
                    remespins = "`97";
                    break;
                case 26:
                    remespins = "`98";
                    break;
                case 27:
                    remespins = "`29";
                    break;
                case 28:
                    remespins = "`20";
                    break;
                case 29:
                    remespins = "`41";
                    break;
                case 30:
                    remespins = "`83";
                    break;
                case 31:
                    remespins = "`84";
                    break;
                case 32:
                    remespins = "`65";
                    break;
                case 33:
                    remespins = "`96";
                    break;
                case 34:
                    remespins = "`97";
                    break;
                case 35:
                    remespins = "`98";
                    break;
                case 36:
                    remespins = "`29";
                    break;
                }
            }
            if (Nexusqq) {
                switch (the_num) {
                case 0:
                    qqspins = "`20";
                    break;
                case 1:
                    qqspins = "`41";
                    break;
                case 2:
                    qqspins = "`42";
                    break;
                case 3:
                    qqspins = "`83";
                    break;
                case 4:
                    qqspins = "`84";
                    break;
                case 5:
                    qqspins = "`65";
                    break;
                case 6:
                    qqspins = "`96";
                    break;
                case 7:
                    qqspins = "`97";
                    break;
                case 8:
                    qqspins = "`98";
                    break;
                case 9:
                    qqspins = "`29";
                    break;
                case 10:
                    qqspins = "`20";
                    break;
                case 11:
                    qqspins = "`41";
                    break;
                case 12:
                    qqspins = "`42";
                    break;
                case 13:
                    qqspins = "`83";
                    break;
                case 14:
                    qqspins = "`84";
                    break;
                case 15:
                    qqspins = "`65";
                    break;
                case 16:
                    qqspins = "`96";
                    break;
                case 17:
                    qqspins = "`97";
                    break;
                case 18:
                    qqspins = "`98";
                    break;
                case 19:
                    qqspins = "`29";
                    break;
                case 20:
                    qqspins = "`20";
                    break;
                case 21:
                    qqspins = "`41";
                    break;
                case 22:
                    qqspins = "`42";
                    break;
                case 23:
                    qqspins = "`83";
                    break;
                case 24:
                    qqspins = "`84";
                    break;
                case 25:
                    qqspins = "`65";
                    break;
                case 26:
                    qqspins = "`96";
                    break;
                case 27:
                    qqspins = "`97";
                    break;
                case 28:
                    qqspins = "`98";
                    break;
                case 29:
                    qqspins = "`29";
                    break;
                case 30:
                    qqspins = "`20";
                    break;
                case 31:
                    qqspins = "`41";
                    break;
                case 32:
                    qqspins = "`42";
                    break;
                case 33:
                    qqspins = "`83";
                    break;
                case 34:
                    qqspins = "`84";
                    break;
                case 35:
                    qqspins = "`65";
                    break;
                case 36:
                    qqspins = "`96";
                    break;
                }
            }
            if (packet->m_int_data == 1800)
            {
                std::string directoryPath = "C:\\Users\\Public\\Nexus_Stuff\\Logs";
                std::string fileName = "Nexus_RouletteLogs.txt";
                std::string filePath = directoryPath + "\\" + fileName;
                std::ofstream file(filePath, std::ios::app);
                if (file.is_open()) {
                    std::string currentTime = getCurrentTime();
                    file << "`9[`2" << currentTime << "`9] `$" << wry + " `9In World: `2" + g_server->m_world.name << std::endl;
                    file.close();
                }
                else {
                    return 1;
                }
                if (namenumber)
                {
                    for (auto& player : g_server->m_world.players) {
                        if (netidspin == player.netid) {
                            if (wry.find(player.name.substr(2).substr(0, player.name.length() - 4)) != -1) {
                                std::string numb = wry.substr(wry.find("spun the wheel and got ") + 23, wry.length());
                                string sw = numb.substr(0, numb.find("!"));
                                if (player.name == g_server->m_world.local.name)
                                {
                                    if (pinglatency == false)
                                    {
                                        variantlist_t va{ "OnNameChanged" };
                                        va[1] = player.name + " `b[``" + sw + "`b]";
                                        g_server->send(true, va, player.netid);
                                    }
                                    else
                                    {
                                        locallastspin = " `b[``" + sw + "`b]";
                                    }
                                }
                                else
                                {
                                    variantlist_t va{ "OnNameChanged" };
                                    va[1] = player.name + " `b[``" + sw + "`b]";
                                    g_server->send(true, va, player.netid);
                                }
                            }
                        }
                    }
                }
                if (realfake)
                {
                    varlist[2] = "`w[`2REAL`0]`w " + wry;
                    if (Nexusreme)
                        varlist[2] = "`w[`2REAL`0]`w " + wry + " `^REME " + remespins;
                    if (Nexusqq)
                        varlist[2] = "`w[`2REAL`0]`w " + wry + " `3QQ " + qqspins;
                    if (Nexusreme && Nexusqq)
                        varlist[2] = "`w[`2REAL`0]`w " + wry + " `^REME " + remespins + " `3QQ " + qqspins;

                }
                g_server->send(true, varlist);
                return true; 
            }
            else
            {
                varlist[2] = varlist[2].get_string() + " `w[`4FAKE`w]`0`w ";
                g_server->send(true, varlist);
                return true;
            }
        }
        g_server->send(true, varlist);
        return true;
    } break;
    case fnv32("OnDialogRequest"): {
        auto content = varlist[1].get_string();
        if (dAll)
        {
            return true;
        }
        if (dropwl == true) {
            if (content.find("Drop") != -1) {
                dropwl = false;
                return true;
            }
        }
        else if (dropdl == true) {
            if (content.find("Drop") != -1) {
                dropdl = false;
                return true;
            }
        }
        else if (dropbgl == true) {
            if (content.find("Drop") != -1) {
                dropbgl = false;
                return true;
            }
        }
        if (content.find("add_label_with_icon|big|`wThe Growtopia Gazette``|left|5016|") != -1)
        {
            std::string paket = "\nadd_label_with_icon|big|`#Nexus Proxy V1.1 Commands|left|262|"
                "\nadd_spacer|small"
                "\nadd_textbox|`#Oh, hello. Welcome again Nexus Proxy Premium Commands.|left|"
                "\nadd_url_button||`#Join Nexus Proxy Discord Server``|NOFLAGS|https://discord.gg/UFPxYvpfZc|Open link?|0|0|"
                "\nadd_spacer|small"
                "\nadd_image_button|banner|interface/Nexusco.rttex|bannerlayout|||"
                "\nadd_spacer|small"
                "\nadd_textbox|`9Info:|left|"
                "\nadd_smalltext|`2/proxy `9(Commands)|left|"
                "\nadd_smalltext|`2/discord `9(Opens Nexus Premium Proxy Discord Link)|left|"
                "\nadd_smalltext|`2/news `9(Opens a dialog of Proxy Updates, etc..)|left|"
                "\nadd_spacer|small"
                "\nadd_textbox|`9Auto Hosting:|left|"
                "\nadd_smalltext|`2/pos `9(Show Both Positions And Bets)|left|"
                "\nadd_smalltext|`2/pos1 `9(Set Display Box Pos 1)|left|"
                "\nadd_smalltext|`2/pos2 `9(Set Display Box Pos 2)|left|"
                "\nadd_smalltext|`2/tp `9(Take Wls/Dls/Bgls From Positions)|left|"
                "\nadd_smalltext|`2/win1 `9(Drop Winnings To Player 1)|left|"
                "\nadd_smalltext|`2/win2 `9(Drop Winnings To Player 2)|left|"
                "\nadd_smalltext|`2/rpos `9(Reset All Positions)|left|"
                "\nadd_spacer|small"
                "\nadd_textbox|`9Auto Collecting:|left|"
                "\nadd_smalltext|`2/collect `9(Auto Collect all items in 10 Far)|left|"
                "\nadd_smalltext|`2/ac `9(Enable Auto Collect)|left|"
                "\nadd_smalltext|`2/autocollect `9(Auto Collect Settings Page)|left|"
                "\nadd_spacer|small"
                "\nadd_textbox|`9Useful Commands:|left|"
                "\nadd_smalltext|`2/settings `9(Save/Load Current Settings/Commands)|left|"  
                "\nadd_smalltext|`2/hotkeys `9(Open Hotkeys Page Dialog)|left|"
                "\nadd_smalltext|`2/setsave `9[world] (Set Save World)|left|"
                "\nadd_smalltext|`2/save `9(Warps To Save World)(/setsave [world])|left|"
                "\nadd_smalltext|`2/split `9[%] (Split Your Wls/Dls/Bgls With Percent)|left|"
                "\nadd_smalltext|`2/pf `9(Enable PathFinder)[Shift + Click]|left|"
                "\nadd_smalltext|`2/growscan `9(See All Floating Items In World Wıth Dialog)|left|"
                "\nadd_smalltext|`2/pullall `9(Pull All Players In The World)|left|"
                "\nadd_smalltext|`2/kickall `9(Kick All Players In The World)|left|"
                "\nadd_smalltext|`2/banall `9(Ban All Players In The World)|left|"
                "\nadd_smalltext|`2/tradeall `9(Trade All Players In Current World)|left|"
                "\nadd_spacer|small"
                "\nadd_textbox|`9Cool Commands:|left|"
                "\nadd_smalltext|`2/speed `9(Opens a Speed Changer Dialog)|left|"
                "\nadd_smalltext|`2/title `9(Opens a Dialog of Visual Titles)|left|"
                "\nadd_smalltext|`2/flag `9(Change Flag You Can Check Countrylist)|left|"
                "\nadd_smalltext|`2/clist `9(Show Country List)|left|"
                "\nadd_smalltext|`2/find `9(Opens A Item Finder Dialog)|left|"
                "\nadd_smalltext|`2/clothes `9(Visual Clothes Page)|left|"
                "\nadd_smalltext|`2/antigravity `9(Enable Anti Gravity)|left|"
                "\nadd_smalltext|`2/mod `9(Enable Moderator Mod)|left|"
                "\nadd_smalltext|`2/blink `9(Automatic Skin Changer)|left|"
                "\nadd_smalltext|`2/fd `9(Enable Fast Drop)|left|"
                "\nadd_smalltext|`2/ft `9(Enable Fast Trash)|left|"
                "\nadd_smalltext|`2/exit `9(Exit World)|left|"
                "\nadd_smalltext|`2/relog `9(Fast Relog Of World)|left|"
                "\nadd_smalltext|`2/res `9(Quick Res)|left|"
                "\nadd_smalltext|`2/balance `9(Show Total Locks In Inventory)|left|"
                "\nadd_smalltext|`2/teleport `9[Name](Teleport To A Player In World)|left|"
                "\nadd_smalltext|`2/dropall `9(Drop All Your Inv Items)|left|"
                "\nadd_spacer|small"
                "\nadd_textbox|`9Wrench:|left|"
                "\nadd_smalltext|`2/wrench `9(Open Wrench Mode Dialog)|left|"
                "\nadd_smalltext|`2/wp `9(Turn On Wrench Pull)|left|"
                "\nadd_smalltext|`2/wk `9(Turn On Wrench Kick)|left|"
                "\nadd_smalltext|`2/wp `9(Turn On Wrench Ban)|left|"
                "\nadd_spacer|small"
                "\nadd_textbox|`9Auto Wrench:|left|"
                "\nadd_smalltext|`2/autopull `9(Auto Pull Player When Joining World)|left|"
                "\nadd_smalltext|`2/autoban `9(Auto Ban Player When Joining World)|left|"
                "\nadd_spacer|small"
                "\nadd_textbox|`9Fast Vend Commands:|left|"
                "\nadd_smalltext|`2/vendcount`9 [Amount] (The Number You Will Receive)|left|"
                "\nadd_smalltext|`2/fastvend `9(Buys From The Product Sold)|left|"
                "\nadd_spacer|small"
                "\nadd_textbox|`9Options Commands:|left|"
                "\nadd_smalltext|`2/options `9(Open Features Dialog)|left|"
                "\nadd_smalltext|`2/cd & /cdrop `9[Amount] (Drops Custom Amount Of WLS)|left|"
                "\nadd_smalltext|`2/dd & /ddrop `9[Amount] (Drops Custom Amount Of DLS)|left|"
                "\nadd_smalltext|`2/daw `9(Drop All Your WLS/DLS/BGLS)|left|"
                "\nadd_smalltext|`2/antighost `9(Enable Anti Ghost)|left|"
                "\nadd_smalltext|`2/fakelag `9(Enable Fake Lag)|left|"
                "\nadd_smalltext|`2/autoacc `9(Auto Acc Acceptor)|left|"
                "\nadd_spacer|small"
                "\nadd_textbox|`9Trick Commands:|left|"
                "\nadd_smalltext|`2/trick `9(Open All Tricks Dialog)|left|"
                "\nadd_smalltext|`2/lamp1 `9(Set To Hit Reme Lamp1 For Trick)|left|"
                "\nadd_smalltext|`2/lamp2 `9(Set To Hit Lamp2 For Trick)|left|"
                "\nadd_smalltext|`2/showxy `9(Show X,Y Positions)|left|"
                "\nadd_smalltext|`2/type `9(Opens Type Dialog)|left|"
                "\nadd_smalltext|`2/senntype `9(Send Type Trick Punch And Text)|left|"
                "\nadd_smalltext|`2/fakeban `9(Fake Suspend)|left|"
                "\nadd_smalltext|`2/tele `9(Opens Tele Dialog)|left|"
                "\nadd_smalltext|`2/t1 `9(Wear Goblin + Hood + OneRing)|left|"
                "\nadd_smalltext|`2/t2 `9(Wear No-Face + OneRing)|left|"
                "\nadd_smalltext|`2/t3 `9(Wear Goblin + Hood + OneRing + Rabbit Top Hat + Riding Rabbit)|left|"
                "\nadd_smalltext|`2/t4 `9(Wear Goblin + Hood + OneRing + Rabbit Top Hat + Riding Rabbit + Kimono + Clam)|left|"
                "\nadd_smalltext|`2/fakeres `9(Fake Respawn)|left|"
                "\nadd_smalltext|`2/invis `9(Invisibility)|left|"
                "\nadd_spacer|small"
                "\nadd_smalnadd_textboxltext|`9Auto Spam:|left|"
                "\nadd_smalltext|`2/spam `9(Opens Auto Spam Page Dialog)|left|"
                "\nadd_smalltext|`2// `9(Enable Auto Spam Shortcut)|left|"
                "\nadd_spacer|small"
                "\nadd_textbox|`9Trolling:|left|"
                "\nadd_smalltext|`2/rspam `9(Spam All Roulettes In World)|left|"
                "\nadd_spacer|small"
                "\nadd_textbox|`9Casino Commands:|left|"
                "\nadd_smalltext|`2/spincheck `9(Show REAL/FAKE spin)|left|"
                "\nadd_smalltext|`2/spinqq `9(Enable QQ Spins)|left|"
                "\nadd_smalltext|`2/spinreme `9(Enable Reme Spins)|left|"
                "\nadd_smalltext|`2/gd `9(Drops The Bet Amount Of The Last Played Game)|left|"
                "\nadd_smalltext|`2/tax`9[Amount (%)](Set Game Tax)|left|"
                "\nadd_smalltext|`2/game`9[Total Bet Amount](Calculate Tax With Any Tax)|left|"
                "\nadd_spacer|small"
                "\nadd_textbox|`9Auto Surg:|left|"
                "\nadd_smalltext|`2/surg `9(Enable Auto Surg)|left|"
                "\nadd_spacer|small"
                "\nadd_smalltext|`9Check My Social Media:|left|"
                "\nadd_spacer|small"
                "\nadd_quick_exit|"
                "\nset_survey_enabled|1"
                "\nend_dialog|gazette||OK|";
            varlist[1] = paket;
            g_server->send(true, varlist);
            return true;
        }
        if (content.find("add_label_with_icon|big|`wThe Growtopia Gazette") != std::string::npos) {
            g_server->send(false, "action|dialog_return\ndialog_name|gazette");
            return true;
        }
        if (autobgl) {
            if (content.find("Dial a number to call somebody in Growtopia.") != -1) {
                int x = safe_stoi(content.substr(content.find("embed_data|tilex|") + 17, content.length() - content.find("embed_data|tilex|") - 1));
                int y = safe_stoi(content.substr(content.find("embed_data|tiley|") + 17, content.length() - content.find("embed_data|tiley|") - 1));
                g_server->send(false, "action|dialog_return\ndialog_name|phonecall\ntilex|" + to_string(x) + "|\ntiley|" + to_string(y) + "|\nnum|-2|\ndial|53785");
                return true;
            }
            if (autobgl) {
                if (content.find("embed_data|num|53785") != -1) {
                    int x = safe_stoi(content.substr(content.find("embed_data|tilex|") + 17, content.length() - content.find("embed_data|tilex|") - 1));
                    int y = safe_stoi(content.substr(content.find("embed_data|tiley|") + 17, content.length() - content.find("embed_data|tiley|") - 1));
                    g_server->send(false, "action|dialog_return\ndialog_name|phonecall\ntilex|" + to_string(x) + "|\ntiley|" + to_string(y) + "|\nnum|53785|\nbuttonClicked|chc5");
                    return true;
                }
            }
        }
        if (autobgl) {
            if (content.find("Excellent! I'm happy to sell you a Blue Gem Lock in exchange for 100 Diamond Lock") != -1) {
                int x = safe_stoi(content.substr(content.find("embed_data|tilex|") + 17, content.length() - content.find("embed_data|tilex|") - 1));
                int y = safe_stoi(content.substr(content.find("embed_data|tiley|") + 17, content.length() - content.find("embed_data|tiley|") - 1));
                g_server->send(false, "action|dialog_return\ndialog_name|phonecall\ntilex|" + to_string(x) + "|\ntiley|" + to_string(y) + "|\nnum|-34|\nbuttonClicked|chc0");
                return true;
            }
        }
        if (content.find("add_label_with_icon|big|`wDonation Box``|left|1452|") != -1)
        {
            if (emptybox) {
                if (content.find("You have ") != -1) {
                    int x = safe_stoi(content.substr(content.find("embed_data|tilex|") + 17, content.length() - content.find("embed_data|tilex|") - 1));
                    int y = safe_stoi(content.substr(content.find("embed_data|tiley|") + 17, content.length() - content.find("embed_data|tiley|") - 1));
                    g_server->send(false, "action|dialog_return\ndialog_name|donation_box_edit\ntilex|" + std::to_string(x) + "|\ntiley|" + std::to_string(y) + "|\nbuttonClicked|clear");
                    return true;
                }
            }
        }
        if (fastdonate)
        {
            if (fastcount == "0")
            {
                if (content.find("How many to put in the box as a gift?") != -1) {
                    int x = safe_stoi(content.substr(content.find("embed_data|tilex|") + 17, content.length() - content.find("embed_data|tilex|") - 1));
                    int y = safe_stoi(content.substr(content.find("embed_data|tiley|") + 17, content.length() - content.find("embed_data|tiley|") - 1));
                    int itemid = safe_stoi(content.substr(content.find("embed_data|itemID|") + 18, content.length() - content.find("embed_data|itemID|") - 1));

                    g_server->send(false, "action|dialog_return\ndialog_name|give_item\nitemID|" + std::to_string(itemid) + "|\ntilex|" + std::to_string(x) + "|\ntiley|" + std::to_string(y) + "|\nbuttonClicked|give\n\ncount|" + std::to_string(item_count(itemid)) + "\nsign_text|\n");
                    return true;
                }
            }
            else
            {
                if (content.find("How many to put in the box as a gift?") != -1) {
                    int x = safe_stoi(content.substr(content.find("embed_data|tilex|") + 17, content.length() - content.find("embed_data|tilex|") - 1));
                    int y = safe_stoi(content.substr(content.find("embed_data|tiley|") + 17, content.length() - content.find("embed_data|tiley|") - 1));
                    int itemid = safe_stoi(content.substr(content.find("embed_data|itemID|") + 18, content.length() - content.find("embed_data|itemID|") - 1));

                    g_server->send(false, "action|dialog_return\ndialog_name|give_item\nitemID|" + std::to_string(itemid) + "|\ntilex|" + std::to_string(x) + "|\ntiley|" + std::to_string(y) + "|\nbuttonClicked|give\n\ncount|" + fastcount + "\nsign_text|\n");
                    return true;
                }
            }
        }
        if (fast_door) {
            if (content.find("gateway_edit") != -1) {
                wrenchx = atoi(content.substr(content.find("embed_data|tilex|") + 17, content.length() - content.find("embed_data|tilex|") - 1).c_str());
                wrenchy = atoi(content.substr(content.find("embed_data|tiley|") + 17, content.length() - content.find("embed_data|tiley|") - 1).c_str());
                int kral = atoi(content.substr(content.find("Is open to public|") + 18, content.length() - content.find("Is open to public|") - 1).c_str());

                if (kral == 0) kral = 1;
                else kral = 0;

                g_server->send(false, "action|dialog_return\ndialog_name|gateway_edit\ntilex|" + to_string(wrenchx) + "|\ntiley|" + to_string(wrenchy) + "|\ncheckbox_public|" + to_string(kral) + "n");
                if (kral == 0) gt::log_to_console("`4Successufly Closed Door!");
                else gt::log_to_console("`2Successufly Opened Door!");
                return true;
            }
        }
        if (autoCrime) {
            if (content.find("Crime") != std::string::npos) {
                if (content.find("`8You have defeated") != std::string::npos) return true;
                if (content.find("Devil Ham``'s `2Crush`` card `4melted") != std::string::npos) {
                    liq = "yes";
                }
                if (content.find("Ms. Terry") != std::string::npos && content.find("Ban Hammer") != std::string::npos) {
                    ban = "yes";
                }

                if (content.find("|Battle!|") != std::string::npos) {
                    x = safe_stoi(content.substr(content.find("embed_data|tilex|") + 17, content.length() - content.find("embed_data|tilex|") - 1));
                    y = safe_stoi(content.substr(content.find("embed_data|tiley|") + 17, content.length() - content.find("embed_data|tiley|") - 1));


                    if (content.find("Crime in Progress") != std::string::npos) {
                        c1 = 2298;
                        c2 = 2308;
                        c3 = 2320;
                        c4 = 2324;
                        c5 = 2332;
                    }
                    if (content.find("Jimmy Snow") != std::string::npos) {
                        use1 = c5;
                        use2 = c4;
                        use3 = c3;
                    }
                    else if (content.find("Big Bertha") != std::string::npos) {
                        use1 = c1;
                        use2 = c4;
                        use3 = c3;
                    }
                    else if (content.find("Shockinator") != std::string::npos) {
                        use1 = c4;
                        use2 = c3;
                        use3 = c1;
                    }
                    else if (content.find("The Firebug") != std::string::npos) {
                        use1 = c2;
                        use2 = c1;
                        use3 = c5;
                    }
                    else if (content.find("Generic Thug") != std::string::npos || content.find("Kat 5") != std::string::npos) {
                        use1 = c4;
                        use2 = c3;
                        use3 = c2;
                    }
                    else if (content.find("Professor Pummel") != std::string::npos) {
                        c1 = 2296;
                        c2 = 2298;
                        c3 = 2300;
                        c4 = 2320;
                        c5 = 2324;
                        use1 = c3;
                        use2 = c2;
                        use3 = c1;
                    }
                    else if (content.find("Z. Everett Koop") != std::string::npos) {
                        c1 = 2296;
                        c2 = 2298;
                        c3 = 2300;
                        c4 = 2320;
                        c5 = 2324;
                        use1 = c3;
                        use2 = c1;
                        use3 = c2;


                    }
                    else if (content.find("Dr. Destructo") != std::string::npos) {
                        c1 = 2298;
                        c2 = 2308;
                        c3 = 2310;
                        c4 = 2314;
                        c5 = 2316;
                        step = "0";
                        use1 = c1;
                        use2 = c3;
                        use3 = c5;
                    }
                    else if (content.find("Dragon Hand") != std::string::npos || content.find("Almighty Seth") != std::string::npos) {
                        c1 = 2308;
                        c2 = 2312;
                        c3 = 2314;
                        c4 = 2326;
                        c5 = 2328;
                        use1 = c5;
                        use2 = c4;
                        use3 = 6969;
                    }
                    else if (content.find("Devil Ham") != std::string::npos) {
                        c1 = 2298;
                        c2 = 2332;
                        c3 = 2334;
                        c4 = 2336;
                        c5 = 2338;
                        use1 = c1;
                        use2 = c4;
                        use3 = c5;
                        use4 = c3;
                        use5 = c2;

                        liq = "no";
                        step = "1";
                    }
                    else if (content.find("Ms. Terry") != std::string::npos) {
                        c1 = 2294;
                        c2 = 2316;
                        c3 = 2322;
                        c4 = 2328;
                        c5 = 2338;
                        use1 = c1;
                        use2 = c2;
                        use3 = c3;
                        use4 = c4;
                        use5 = c5;
                        ban = "no";
                    }

                    g_server->send(false, "action|dialog_return\ndialog_name|crime_edit\ntilex|" + std::to_string(x) + "|\ntiley|" + std::to_string(y) + "|\nstate|0||\nbuttonClicked|button_ok\n\nc" + std::to_string(c1) + "|1\nc" + std::to_string(c2) + "|1\nc" + std::to_string(c3) + "|1\nc" + std::to_string(c4) + "|1\nc" + std::to_string(c5) + "|1\n");
                    return true;
                }
                else if (content.find("Fighting Crime") != std::string::npos && content.find("Devil Ham") != std::string::npos) {
                    if (liq == "no") {
                        if (content.find("c" + std::to_string(use1)) != std::string::npos) {
                            use = std::to_string(use1);
                        }
                        else {
                            use = "passturn";
                        }
                    }
                    else if (content.find("c" + std::to_string(use2)) != std::string::npos && step == "0") {
                        step = "1";
                        use = std::to_string(use2);
                    }
                    else if (content.find("c" + std::to_string(use3)) != std::string::npos) {
                        use = std::to_string(use3);
                    }
                    else if (content.find("c" + std::to_string(use4)) != std::string::npos) {
                        use = std::to_string(use4);
                    }
                    else if (content.find("c" + std::to_string(use5)) != std::string::npos) {
                        use = std::to_string(use5);
                    }
                    else {
                        use = "passturn";
                    }
                    if (use != "passturn") {
                        variantlist_t notif{ "OnTextOverlay" };
                        notif[1] = "`9Using `2" + items[safe_stoi(use)].name;
                        g_server->send(true, notif, -1, -1);
                        g_server->send(false, "action|dialog_return\ndialog_name|crime_edit\ntilex|" + std::to_string(x) + "|\ntiley|" + std::to_string(y) + "|\nstate|1||\nbuttonClicked|c" + use);
                    }
                    return true;
                }
                else if (content.find("Fighting Crime") != std::string::npos && content.find("Ms. Terry") != std::string::npos) {
                    if (content.find("is going to play `d") != std::string::npos) {
                        if (content.find("is going to play `2") != std::string::npos) {
                            use = std::to_string(use1);
                        }
                        else if (content.find("is going to play `4") != std::string::npos) {
                            use = std::to_string(use2);
                        }
                        else if (content.find("is going to play `9") != std::string::npos) {
                            use = std::to_string(use4);
                        }
                        else if (content.find("is going to play `1") != std::string::npos) {
                            use = std::to_string(use5);
                        }
                    }
                    else if (content.find("c" + std::to_string(use3)) != std::string::npos) {
                        use = std::to_string(use3);
                    }
                    else if (content.find("c" + std::to_string(use5)) != std::string::npos && ban == "yes") {
                        use = std::to_string(use5);
                    }
                    else if (content.find("c" + std::to_string(use1)) != std::string::npos) {
                        use = std::to_string(use1);
                    }
                    else if (content.find("c" + std::to_string(use4)) != std::string::npos) {
                        use = std::to_string(use4);
                    }
                    else if (content.find("c" + std::to_string(use5)) != std::string::npos) {
                        use = std::to_string(use5);
                    }
                    else if (content.find("c" + std::to_string(use2)) != std::string::npos) {
                        use = std::to_string(use2);
                    }
                    else {
                        use = "passturn";
                    }
                    if (use != "passturn") {
                        variantlist_t notif{ "OnTextOverlay" };
                        notif[1] = "`9Using `2" + items[safe_stoi(use)].name;
                        g_server->send(true, notif, -1, -1);
                        g_server->send(false, "action|dialog_return\ndialog_name|crime_edit\ntilex|" + std::to_string(x) + "|\ntiley|" + std::to_string(y) + "|\nstate|1||\nbuttonClicked|c" + use);
                    }
                    return true;
                }
                else if (content.find("Fighting Crime") != std::string::npos && content.find("Dr. Destructo") != std::string::npos) {
                    if (content.find("c" + std::to_string(use1)) != std::string::npos && step == "0") {
                        use = std::to_string(use1);
                        step = "1";
                    }
                    else if (content.find("c" + std::to_string(use2)) != std::string::npos) {
                        use = std::to_string(use2);
                    }
                    else if (content.find("c" + std::to_string(use3)) != std::string::npos) {
                        use = std::to_string(use3);
                    }
                    else {
                        use = "passturn";
                    }
                    if (use != "passturn") {
                        variantlist_t notif{ "OnTextOverlay" };
                        notif[1] = "`9Using `2" + items[safe_stoi(use)].name;
                        g_server->send(true, notif, -1, -1);
                        g_server->send(false, "action|dialog_return\ndialog_name|crime_edit\ntilex|" + std::to_string(x) + "|\ntiley|" + std::to_string(y) + "|\nstate|1||\nbuttonClicked|c" + use);
                    }
                    return true;
                }
                else if (content.find("Fighting Crime") != std::string::npos) {
                    if (content.find("c" + std::to_string(use1)) != std::string::npos) {
                        use = std::to_string(use1);
                    }
                    else if (content.find("c" + std::to_string(use2)) != std::string::npos) {
                        use = std::to_string(use2);
                    }
                    else if (content.find("c" + std::to_string(use3)) != std::string::npos) {
                        use = std::to_string(use3);
                    }
                    else {
                        use = "passturn";
                    }
                    if (use != "passturn") {
                        variantlist_t notif{ "OnTextOverlay" };
                        notif[1] = "`9Using `2" + items[safe_stoi(use)].name;
                        g_server->send(true, notif, -1, -1);
                        g_server->send(false, "action|dialog_return\ndialog_name|crime_edit\ntilex|" + std::to_string(x) + "|\ntiley|" + std::to_string(y) + "|\nstate|1||\nbuttonClicked|c" + use);
                    }
                    return true;
                }
            }
            return true;
        }
        if (auto_surg) {
            if (content.find("end_dialog|surgery||") != -1)
            {
                // maybe use switch statement is better but idc
                if (content.find("tool4312") != -1)
                { // defibrilator
                    // use defibrilator
                    g_server->send(false, "action|dialog_return\ndialog_name|surgery\nbuttonClicked|tool4312");
                    PRINTC("DEFIBRILATOR");
                    variantlist_t varlist{ "OnTextOverlay" };
                    varlist[1] = "`3DEFIBRILATOR!";
                    g_server->send(true, varlist);
                }
                else if
                    (content.find("|tool1260|") == std::string::npos // scalpel
                    && content.find("|tool1262|") == std::string::npos // anes
                    && content.find("|tool1264|") == std::string::npos // antisept
                    && content.find("|tool1266|") == std::string::npos // antibio
                    && content.find("|tool1268|") == std::string::npos // splint
                    && content.find("|tool1270|") == std::string::npos // stit
                    && content.find("|tool4308|") == std::string::npos // pins
                    && content.find("|tool4310|") == std::string::npos // transfusion
                    && content.find("|tool4312|") == std::string::npos // defibrilator
                    && content.find("|tool4314|") == std::string::npos // clamp
                    && content.find("|tool4316|") == std::string::npos // ultrasound
                    && content.find("|tool4318|") == std::string::npos // lab kit
                    )
                { 
                    g_server->send(false, "action|dialog_return\ndialog_name|surgery\nbuttonClicked|tool1258");
                    PRINTC("SPONGE");
                    variantlist_t varlist{ "OnTextOverlay" };
                    varlist[1] = "`3SPONGE!";
                    g_server->send(true, varlist);
                }
                else if (content.find("Patient is `6losing blood") != -1 || content.find("Patient is losing blood") != -1) {
                    if (content.find("Fix It!") != -1) {
                        g_server->send(false, "action|dialog_return\ndialog_name|surgery\nbuttonClicked|tool1270");
                        PRINTC("STITCH");
                        variantlist_t varlist{ "OnTextOverlay" };
                        varlist[1] = "`3STITCH!";
                        g_server->send(true, varlist);
                    }
                    else if (content.find("tool4314") != -1) {
                        g_server->send(false, "action|dialog_return\ndialog_name|surgery\nbuttonClicked|tool4314");
                        PRINTC("CLAMP");
                        variantlist_t varlist{ "OnTextOverlay" };
                        varlist[1] = "`3CLAMP!";
                        g_server->send(true, varlist);
                    }
                    else {
                        g_server->send(false, "action|dialog_return\ndialog_name|surgery\nbuttonClicked|tool1270");
                        PRINTC("STITCH");
                        variantlist_t varlist{ "OnTextOverlay" };
                        varlist[1] = "`3STITCH!";
                        g_server->send(true, varlist);
                    }
                }
                else if (content.find("Patient's fever is ") != -1) {
                    if (content.find("tool4318") != -1) {
                        g_server->send(false, "action|dialog_return\ndialog_name|surgery\nbuttonClicked|tool4318");
                        PRINTC("LAB KIT");
                        variantlist_t varlist{ "OnTextOverlay" };
                        varlist[1] = "`3LAB KIT!";
                        g_server->send(true, varlist);
                    }
                    else {
                        g_server->send(false, "action|dialog_return\ndialog_name|surgery\nbuttonClicked|tool1266");
                        PRINTC("ANTIBIOTICS");
                        variantlist_t varlist{ "OnTextOverlay" };
                        varlist[1] = "`3ANTIBIOTICS!";
                        g_server->send(true, varlist);
                    }
                }
                else if (content.find("Status: `6Coming to``|left|") != -1) {
                    g_server->send(false, "action|dialog_return\ndialog_name|surgery\nbuttonClicked|tool1262");
                    PRINTC("ANESTHETIC");
                    variantlist_t varlist{ "OnTextOverlay" };
                    varlist[1] = "`3ANESTHETIC!";
                    g_server->send(true, varlist);
                }
                else if (content.find("add_smalltext|Pulse: `4Extremely Weak``") != -1) {
                    g_server->send(false, "action|dialog_return\ndialog_name|surgery\nbuttonClicked|tool4310");
                    PRINTC("TRANSFUSION");
                    variantlist_t varlist{ "OnTextOverlay" };
                    varlist[1] = "`3TRANSFUSION!";
                    g_server->send(true, varlist);
                }
                else if (content.find(" shattered``") != -1 && content.substr(content.find("Incisions: `") + 13, 1) != "0") {
                    // use pins
                    g_server->send(false, "action|dialog_return\ndialog_name|surgery\nbuttonClicked|tool4308");
                    PRINTC("PINS");
                    variantlist_t varlist{ "OnTextOverlay" };
                    varlist[1] = "`3PINS!";
                    g_server->send(true, varlist);
                }
                else if (content.find(" broken``") != -1) {
                    g_server->send(false, "action|dialog_return\ndialog_name|surgery\nbuttonClicked|tool1268");
                    PRINTC("SPLINT");
                    variantlist_t varlist{ "OnTextOverlay" };
                    varlist[1] = "`3SPLINT!";
                    g_server->send(true, varlist);
                }
                else if (content.find("Fix It!") != -1) {
                    if (content.substr(content.find("Incisions: `") + 13, 1) != "0") {
                        g_server->send(false, "action|dialog_return\ndialog_name|surgery\nbuttonClicked|tool1296"); // might be wrong
                        PRINTC("FIX IT!");
                        variantlist_t varlist{ "OnTextOverlay" };
                        varlist[1] = "`3FIX IT!";
                        g_server->send(true, varlist);
                        gloves = true;


                    }
                    else {
                        g_server->send(false, "action|dialog_return\ndialog_name|surgery\nbuttonClicked|tool1270");
                        PRINTC("STITCH");
                        variantlist_t varlist{ "OnTextOverlay" };
                        varlist[1] = "`3STITCH!";
                        g_server->send(true, varlist);
                    }
                }
                else if (content.find("Temp: `6") != -1 || content.find("Temp: `4") != -1 || content.find("Temp: `3") != -1) {
                    if (content.find("tool4318") != -1) {
                        g_server->send(false, "action|dialog_return\ndialog_name|surgery\nbuttonClicked|tool4318");
                        PRINTC("LAB KIT");
                        variantlist_t varlist{ "OnTextOverlay" };
                        varlist[1] = "`3LAB KIT!";
                        g_server->send(true, varlist);
                    }
                    else {
                        g_server->send(false, "action|dialog_return\ndialog_name|surgery\nbuttonClicked|tool1266");
                        PRINTC("ANTIBIOTIC");
                        variantlist_t varlist{ "OnTextOverlay" };
                        varlist[1] = "`3ANTIBIOTIC!";
                        g_server->send(true, varlist);
                    }
                }
                else if (content.find("tool4316") == std::string::npos) {
                    if (content.find("Status: `4Awake``|") == std::string::npos && content.find("Status: `3Awake``|") == std::string::npos) {

                        if (gloves)
                        {
                            g_server->send(false, "action|dialog_return\ndialog_name|surgery\nbuttonClicked|tool1270");
                            PRINTC("STITCH");
                            variantlist_t varlist{ "OnTextOverlay" };
                            varlist[1] = "`3STITCH!";
                            g_server->send(true, varlist);
                        }
                        else
                        {
                            g_server->send(false, "action|dialog_return\ndialog_name|surgery\nbuttonClicked|tool1260");
                            PRINTC("SCALPEL");
                            variantlist_t varlist{ "OnTextOverlay" };
                            varlist[1] = "`3SCALPEL!";
                            g_server->send(true, varlist);
                        }

                    }
                    else {
                        g_server->send(false, "action|dialog_return\ndialog_name|surgery\nbuttonClicked|tool1262");
                        PRINTC("ANESTHETIC");
                        variantlist_t varlist{ "OnTextOverlay" };
                        varlist[1] = "`3ANESTHETIC!";
                        g_server->send(true, varlist);
                    }
                }
                else if (content.find("tool4316") != -1) {
                    g_server->send(false, "action|dialog_return\ndialog_name|surgery\nbuttonClicked|tool4316");
                    PRINTC("ULTRASOUND");
                    variantlist_t varlist{ "OnTextOverlay" };
                    varlist[1] = "`3ULTRASOUND!";
                    g_server->send(true, varlist);
                }
                else if (content.find("Temp: `2") != -1) {
                    g_server->send(false, "action|dialog_return\ndialog_name|surgery\nbuttonClicked|tool1266");
                    PRINTC("ANTIBIOTIC");
                    variantlist_t varlist{ "OnTextOverlay" };
                    varlist[1] = "`3ANTIBIOTIC!";
                    g_server->send(true, varlist);
                }
                return true;
            }
        }

        if (legit_surg) {
            if (content.find("end_dialog|surgery||") != -1)
            {
                if (content.find("tool4312") != -1) { 
                    SendWarn("DEFIBRILATOR!");
                }
                else if 
                    (content.find("|tool1260|") == std::string::npos // scalpel
                    && content.find("|tool1262|") == std::string::npos // anes
                    && content.find("|tool1264|") == std::string::npos // antisept
                    && content.find("|tool1266|") == std::string::npos // antibio
                    && content.find("|tool1268|") == std::string::npos // splint
                    && content.find("|tool1270|") == std::string::npos // stit
                    && content.find("|tool4308|") == std::string::npos // pins
                    && content.find("|tool4310|") == std::string::npos // transfusion
                    && content.find("|tool4312|") == std::string::npos // defibrilator
                    && content.find("|tool4314|") == std::string::npos // clamp
                    && content.find("|tool4316|") == std::string::npos // ultrasound
                    && content.find("|tool4318|") == std::string::npos // lab kit
                    )
                {
                    SendWarn("SPONGE!");
                }
                else if (content.find("Patient is `6losing blood") != -1 || content.find("Patient is losing blood") != -1) {
                    if (content.find("Fix It!") != -1) {
                        SendWarn("STITCH!");
                    }
                    else if (content.find("tool4314") != -1) {
                        SendWarn("CLAMP!");
                    }
                    else {
                        SendWarn("STITCH!");
                    }
                }
                else if (content.find("Patient's fever is ") != -1) {
                    if (content.find("tool4318") != -1) {
                        SendWarn("LAB KIT!");
                    }
                    else {
                        SendWarn("ANTIBIOTICS!");
                    }
                }
                else if (content.find("Status: `6Coming to``|left|") != -1) {
                    SendWarn("ANESTHETIC!");
                }
                else if (content.find("add_smalltext|Pulse: `4Extremely Weak``") != -1) {
                    SendWarn("TRANSFUSION!");
                }
                else if (content.find(" shattered``") != -1 && content.substr(content.find("Incisions: `") + 13, 1) != "0") {
                    SendWarn("PINS!");
                }
                else if (content.find(" broken``") != -1) {
                    SendWarn("SPLINT!");
                }
                else if (content.find("Fix It!") != -1) {
                    if (content.substr(content.find("Incisions: `") + 13, 1) != "0") {
                        SendWarn("FIX IT!");
                        gloves = true;


                    }
                    else {
                        SendWarn("STITCH!");
                    }
                }
                else if (content.find("Temp: `6") != -1 || content.find("Temp: `4") != -1 || content.find("Temp: `3") != -1) {
                    if (content.find("tool4318") != -1) {
                        SendWarn("LAB KIT!");
                    }
                    else {
                        SendWarn("ANTIBIOTIC!");
                    }
                }
                else if (content.find("tool4316") == std::string::npos) {
                    if (content.find("Status: `4Awake``|") == std::string::npos && content.find("Status: `3Awake``|") == std::string::npos) {

                        if (gloves)
                        {
                            SendWarn("STITCH!");
                        }
                        else
                        {
                            SendWarn("SCAPEL!");
                        }

                    }
                    else {
                        SendWarn("ANESTHETIC!");
                    }
                }
                else if (content.find("tool4316") != -1) {
                    SendWarn("ULTRASOUND!");
                }
                else if (content.find("Temp: `2") != -1) {
                    SendWarn("ANTIBIOTIC!");
                }
                return true;
            }
        }

        if (wrench) {
            std::string bruhmomento = content.substr(content.find("embed_data|netID") + 17, content.length() - content.find("embed_data|netID") - 1);
            if (content.find("set_default_color|`o") != -1) {
                if (content.find("embed_data|netID") != -1) {
                    return true;
                }
            }
        }
        if (content.find("add_label_with_icon|big|`wVending Machine``|left|2978") != -1)
        {
            if (fastvend) {
                if (content.find("How many would you like to buy?") != -1) {
                    int x = safe_stoi(content.substr(content.find("embed_data|tilex|") + 17, content.length() - content.find("embed_data|tilex|") - 1));
                    int y = safe_stoi(content.substr(content.find("embed_data|tiley|") + 17, content.length() - content.find("embed_data|tiley|") - 1));
                    int itemid = safe_stoi(content.substr(content.find("embed_data|expectitem|") + 22, content.length() - content.find("embed_data|expectitem|") - 1));
                    int expectprice = safe_stoi(content.substr(content.find("embed_data|expectprice|") + 23, content.length() - content.find("embed_data|expectprice|") - 1));
                    int yourwl = safe_stoi(content.substr(content.find("add_textbox|You have") + 20, content.length() - content.find("add_textbox|You have") - 1));
                    if (yourwl - expectprice < 0) {
                    }
                    else {
                        g_server->send(false, "action|dialog_return\ndialog_name|vending\ntilex|" + std::to_string(x) + "|\ntiley|" + std::to_string(y) + "|\nverify|1|\nbuycount|" + to_string(vendcount) + "|\nexpectprice|" + std::to_string(expectprice) + "|\nexpectitem|" + std::to_string(itemid) + "|");
                        return true;
                    }
                }


            }

        }
        if (pullstock) {
            if (content.find("You have earned ") != -1) {
                int x = safe_stoi(content.substr(content.find("embed_data|tilex|") + 17, content.length() - content.find("embed_data|tilex|") - 1));
                int y = safe_stoi(content.substr(content.find("embed_data|tiley|") + 17, content.length() - content.find("embed_data|tiley|") - 1));

                g_server->send(false, "action|dialog_return\ndialog_name|vending\ntilex|" + std::to_string(x) + "|\ntiley|" + std::to_string(y) + "|\nbuttonClicked|withdraw");

                return true;
            }
        }
        if (addstock) {
            if (content.find("Update") != -1) {
                int x = safe_stoi(content.substr(content.find("embed_data|tilex|") + 17, content.length() - content.find("embed_data|tilex|") - 1));
                int y = safe_stoi(content.substr(content.find("embed_data|tiley|") + 17, content.length() - content.find("embed_data|tiley|") - 1));

                g_server->send(false, "action|dialog_return\ndialog_name|vending\ntilex|" + std::to_string(x) + "|\ntiley|" + std::to_string(y) + "|\nbuttonClicked|addstock");

                return true;
            }
        }
        if (fastdrop) {
            if (dcount == "0") {
                std::string itemid = content.substr(content.find("embed_data|itemID|") + 18, content.length() - content.find("embed_data|itemID|") - 1);
                std::string count = content.substr(content.find("count||") + 7, content.length() - content.find("count||") - 1);
                if (content.find("embed_data|itemID|") != -1) {
                    if (content.find("Drop") != -1) {
                        std::this_thread::sleep_for(std::chrono::milliseconds(300));
                        g_server->send(false, "action|dialog_return\ndialog_name|drop_item\nitemID|" + itemid + "|\ncount|" + count);
                        return true;
                    }
                }
            }
            else {
                std::string itemid = content.substr(content.find("embed_data|itemID|") + 18, content.length() - content.find("embed_data|itemID|") - 1);
                g_server->send(false, "action|dialog_return\ndialog_name|drop_item\nitemID|" + itemid + "|\ncount|" + dcount);
                return true;
            }
        }
        if (fasttrash) {
            std::string itemid = content.substr(content.find("embed_data|itemID|") + 18, content.length() - content.find("embed_data|itemID|") - 1);
            std::string count = content.substr(content.find("you have ") + 9, content.length() - content.find("you have ") - 1);
            std::string delimiter = ")";
            std::string token = count.substr(0, count.find(delimiter));
            if (content.find("embed_data|itemID|") != -1) {
                if (content.find("Trash") != -1) {
                    g_server->send(false, "action|dialog_return\ndialog_name|trash_item\nitemID|" + itemid + "|\ncount|" + token);
                    return true;
                }
            }
        }
    }break;

    case fnv32("OnRemove"): {
        auto text = varlist.get(1).get_string();
        if (text.find("netID|") == 0) {
            auto netid = atoi(text.substr(6).c_str());

            if (netid == g_server->m_world.local.netid) {
                g_server->m_world.local = {};
                g_server->m_world.object_map.objects.clear();


            }
            auto& players = g_server->m_world.players;
            for (size_t i = 0; i < players.size(); i++) {
                auto& player = players[i];
                if (player.netid == netid) {
                    players.erase(std::remove(players.begin(), players.end(), player), players.end());
                    break;
                    auto overlay = varlist.get(1).get_string();
                }
            }
        }
    } break;
    case fnv32("OnSpawn"): {

        std::string meme = varlist.get(1).get_string();
        rtvar var = rtvar::parse(meme);
        auto name = var.find("name");
        auto netid = var.find("netID");
        auto onlineid = var.find("onlineID");
        if (name && netid && onlineid) {
            player ply{};
            ply.mod = false;
            ply.invis = false;
            ply.name = name->m_value;
            ply.country = var.get("country");
            name->m_values[0] += "";

            std::string playerName = ply.name.substr(2).substr(0, ply.name.length() - 4);
            std::set<std::string> autoPullNames = loadAutoPullNames("C:\\Users\\Public\\Nexus_Stuff\\autopull.txt");
            if (autoPullNames.find(playerName) != autoPullNames.end()) {
                if (specficpull) {
                    g_server->send(false, "action|input\n|text|/pull " + playerName);
                }
            }
            if (playerName != g_server->m_world.local.name) {
                adamlar.push_back(playerName);
            }

            std::string playerNames = ply.name.substr(2).substr(0, ply.name.length() - 4);
            std::set<std::string> autoPullNamesn = loadAutoPullNames("C:\\Users\\Public\\Nexus_Stuff\\autoban.txt");
            if (autoPullNamesn.find(playerNames) != autoPullNamesn.end()) {
                if (specficban) {
                    g_server->send(false, "action|input\n|text|/ban " + playerNames);
                }
            }
            if (playerNames != g_server->m_world.local.name) {
                adamlar.push_back(playerNames);
            }


            if (enter_pull) {
                g_server->send(false, "action|input\n|text|/pull " + ply.name.substr(2).substr(0, ply.name.length() - 4));
            }
            if (enter_ban) {
                g_server->send(false, "action|input\n|text|/ban " + ply.name.substr(2).substr(0, ply.name.length() - 4));
            }
            if (ply.name.substr(2).substr(0, ply.name.length() - 4) != g_server->m_world.local.name) {
                adamlar.push_back(ply.name.substr(2).substr(0, ply.name.length() - 4));
            }

            auto pos = var.find("posXY");
            if (pos && pos->m_values.size() >= 2) {
                auto x = atoi(pos->m_values[0].c_str());
                auto y = atoi(pos->m_values[1].c_str());
                ply.pos = vector2_t{ float(x), float(y) };
            }
            ply.userid = var.get_int("userID");
            ply.netid = var.get_int("netID");
            if (meme.find("type|local") != -1) {
                //set mod state to 1 (allows infinite zooming, this doesnt ban cuz its only the zoom not the actual long punch)
                var.find("mstate")->m_values[0] = "1";
                g_server->m_world.local = ply;
                if (antigravity) anti_gravity();

                blockfirst = true;
            }
            g_server->m_world.players.push_back(ply);
            auto str = var.serialize();
            utils::replace(str, "onlineID", "onlineID|");
            varlist[1] = str;
            //PRINTC("new: %s\n", varlist.print().c_str());
            g_server->send(true, varlist, -1, -1);
            return true;
        }
    } break;
    case fnv32("OnSetPos"): {
        if (gt::noclip)
            return true;
        return false;
    }
    
    }
    return false;
}
bool events::in::generictext(std::string packet) {
    if (iswear) send_vset();
    return false;
}

bool events::in::gamemessage(std::string packet) {
    PRINTC("Game Message: %s\n", packet.c_str());
    if (gt::resolving_uid2) {
        if (packet.find("PERSON IGNORED") != -1) {
            g_server->send(false, "action|dialog_return\ndialog_name|friends_guilds\nbuttonClicked|showfriend");
            g_server->send(false, "action|dialog_return\ndialog_name|friends\nbuttonClicked|friend_all");
        }
        else if (packet.find("Nobody is currently online with the name") != -1) {
            gt::resolving_uid2 = false;
            gt::log_to_console("Target is offline, cant find uid.");
        }
        else if (packet.find("Clever perhaps") != -1) {
            gt::resolving_uid2 = false;
            gt::log_to_console("Target is a moderator, can't ignore them.");
        }
    }
    return false;
}

bool events::in::sendmapdata(ENetEvent event, gameupdatepacket_t* packet) {
    g_server->m_world = {};
    g_server->m_world.connected = true;

    // The end was computed as ptr + data_size + 40, but the payload actually runs to
    // ptr + 56 + data_size, so it cut the buffer 16 bytes short and the parse failed on
    // the trailing drop table. Use the packet's own length, which cannot be wrong.
    uint8_t* begin = utils::get_extended(packet);
    uint8_t* end = event.packet->data + event.packet->dataLength;

    if (!g_server->m_world.serialize(begin, end)) {
        PRINTS("World parse FAILED -- /scan, /growscan and /pathfinder will not work.\n");
    }
    else {
        auto& w = g_server->m_world;
        PRINTS("World '%s' parsed: %dx%d, %zu tiles, %zu drops.\n",
            w.name.c_str(), w.tile_map.size.m_x, w.tile_map.size.m_y,
            w.tile_map.tiles.size(), w.object_map.objects.size());
    }
    return false;
}

bool events::in::state(gameupdatepacket_t* packet) {
    if (!g_server->m_world.connected)
        return false;
    if (packet->m_player_flags == -1)
        return false;

    auto& players = g_server->m_world.players;

    for (auto& player : players) {
        if (player.netid == packet->m_player_flags) {
            player.pos = vector2_t{ packet->m_vec_x, packet->m_vec_y };
            std::PRINTC("Player %s Position Is %.0f %.0f\n", player.name.c_str(), player.pos.m_x, player.pos.m_y);
            break;
        }
    }

    return false;
}



bool events::in::tracking(std::string packet)
{
    PRINTC("Tracking packet: %s\n", packet.c_str());
    rtvar var = rtvar::parse(packet);
    std::string event = var.get("eventName");

    if (packet.find("Authentication_error|10") != -1)
    {
        gt::log_to_console("`4OOPS:`` Too many people logging in at once. Please press `5CANCEL`` and try again in a few seconds.");
        gt::log_to_console("`3Reconnecting...");
        g_server->reconnect();

    }

    if (packet.find("Authentication_error|2") != -1)
    {
        gt::log_to_console("`4OOPS:`` Too many people logging in at once. Please press `5CANCEL`` and try again in a few seconds.");
        gt::log_to_console("`3Reconnecting...");
    }

    if (packet.find("Authentication_error|17") != -1)
    {
        gt::log_to_console("`4OOPS:`` Too many people logging in at once. Please press `5CANCEL`` and try again in a few seconds.");
        gt::log_to_console("`3Reconnecting...");
        g_server->reconnect();

    }

    if (packet.find("Authentication_error|25") != -1)
    {
        gt::log_to_console("`3Generating `5Mac `9Address.");
        std::string denemes = "Wi-Fi";
        std::string newMAC = generateRandomMAC();
        newMAC.erase(std::remove(newMAC.begin(), newMAC.end(), ':'), newMAC.end());
        changeMACAddress(denemes, newMAC);
        gt::log_to_console("`2Successfully `9Generated Mac Address.");
        gt::log_to_console("`3Cleaning `5Regedit `9File.");
        std::string RegistryRndmRegistery = generateRandomString(10);
        SpoofRegistery();
        updateRegistry(RegistryRndmRegistery);
        gt::log_to_console("`2Successfully `9Cleaned Regedit File.");
        gt::log_to_console("`4Note: ``You can close and reopen Nexus Proxy and Growtopia and log in to the game!");
    }

    if (packet.find("eventName|305_DROP") != -1) {
        if (packet.find("Item_id|^^7188") != -1) {
            std::string amount = packet.substr(packet.find("Amount_dropped|") + 15, packet.length() - packet.find("Amount_dropped|") - 1);
            gt::log_to_console("`9Dropped `2" + amount + "`9Blue Gem Lock");
            return true;
        }
    }
    if (packet.find("eventName|305_DROP") != -1) {
        if (packet.find("Item_id|^^1796") != -1) {
            std::string amount = packet.substr(packet.find("Amount_dropped|") + 15, packet.length() - packet.find("Amount_dropped|") - 1);
            gt::log_to_console("`9Dropped `2" + amount + "`9Diamond Lock");
            return true;
        }
    }
    if (packet.find("eventName|305_DROP") != -1) {
        if (packet.find("Item_id|^^242") != -1) {
            std::string amount = packet.substr(packet.find("Amount_dropped|") + 15, packet.length() - packet.find("Amount_dropped|") - 1);
            gt::log_to_console("`9Dropped `2" + amount + "`9World Lock");
            return true;
        }
    }

    if (event == "102_PLAYER.AUTHENTICATION") {
        string wlbalance = packet.substr(packet.find("Worldlock_balance|") + 18, packet.length() - packet.find("Worldlock_balance|") - 1);

        if (wlbalance.find("PLAYER.") != -1)
        {
            if (LoginNotification == 2)
            {
                gt::log_to_console("`9World Lock Balance: `#0");
            }
        }
        else
        {
            if (LoginNotification == 2)
            {
                gt::log_to_console("`9World Lock Balance: `#" + wlbalance);
            }
        }

        if (packet.find("Authenticated|1") != -1)
        {
            if (LoginNotification == 2)
            {
                gt::log_to_console("`9Player Authentication `2Successfuly.");
            }
        }
        else
        {
            if (LoginNotification == 2)
            {
                gt::log_to_console("`9Player Authentication `4Failed.");
            }
        }
    }

    if (var.validate_int("Worldlock_balance")) {
        g_server->local_player.wl_balance = var.get_int("Worldlock_balance");
    }

    if (var.validate_int("Gems_balance")) {
        g_server->local_player.gems_balance = var.get_int("Gems_balance");
        gt::log_to_console("`9");
    }

    if (var.validate_int("Level")) {
        g_server->local_player.level = var.get_int("Level");
    }

    if (var.validate_int("Awesomeness")) {
        g_server->local_player.awesomeness = var.get_int("Awesomeness");
    }

    if (packet.find("eventName|100_MOBILE.START") != -1)
    {
        gems = packet.substr(packet.find("Gems_balance|") + 13, packet.length() - packet.find("Gems_balance|") - 1);
        level = packet.substr(packet.find("Level|") + 6, packet.length() - packet.find("Level|") - 1);
        uid = packet.substr(packet.find("GrowId|") + 7, packet.length() - packet.find("GrowId|") - 1);
        //gt::log_to_console("`9Gems Balance: `#" + gems);
        //gt::log_to_console("`9Account Level: `#" + level);
        gt::log_to_console("`9Your Current UID: `#" + uid);
    }
    if (packet.find("eventName|300_WORLD_VISIT") != -1)
    {
        if (packet.find("Locked|0") != -1)
        {
            gt::log_to_console("`4This world is not locked by a world lock.");
        }
        else
        {
            gt::log_to_console("`2This world is locked by a world lock.");

            if (packet.find("World_owner|") != -1)
            {
                string uidd = packet.substr(packet.find("World_owner|") + 12, packet.length() - packet.find("World_owner|") - 1);
                //gt::log_to_console("`9World Owner UID: `#" + uidd);

            }
        }
    }

    return true;
}