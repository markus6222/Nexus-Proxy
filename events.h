#pragma once
#include "enet/include/enet.h"
#include <string>
#include <conio.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <time.h>
#include <thread>
#include "proton/vector.hpp"
#include "proton/variant.hpp"
#include <mutex>
#include <queue>
#include "world.h"
#include <regex>

//return value: true - dont send original packet, false - send original packet

// A Growtopia game packet is 56 bytes on the wire; sizeof(GameUpdatePacket) is 60.
constexpr int GAME_PACKET_SIZE = 56;
void build_collect_packet(uint32_t oid, float x, float y, uint8_t* out);

namespace events {

    namespace out {
        bool worldoptions(std::string option);
        bool variantlist(gameupdatepacket_t* packet);
        bool pingreply(gameupdatepacket_t* packet);
        bool generictext(std::string packet);
        bool gamemessage(std::string packet);
        bool state(gameupdatepacket_t* packet, GameUpdatePacket* packets);
        int get_punch_id(const int id_);

    }; // namespace out
    namespace in {
        bool variantlist(gameupdatepacket_t* packet);
        bool generictext(std::string packet);
        bool gamemessage(std::string packet);
        bool sendmapdata(ENetEvent event, gameupdatepacket_t* packet);
        bool state(gameupdatepacket_t* packet);
        bool tracking(std::string packet);
    }; // namespace in
};     // namespace events
bool iswear = false;
/*float itemidx1 = 0, itemidx2 = 0, itemidx3 = 0, itemidx5 = 0,
itemidy1 = 0, itemidy2 = 0, itemidy3 = 0, itemidy5 = 0,
itemidz1 = 0, itemidz2 = 0, itemidz3 = 0, itemidz5 = 0;*/
int skincolor = 1685231359;
bool bruhmoment = false;
std::vector<std::thread> sel;
float hair, shirt, pants = 0.f; //cloth_hair & cloth_shirt & cloth_pants
float shoe, face, hand = 0.f; //cloth_feet & cloth_face & cloth_hand
float back, mask, neck = 0.f;//cloth_back cloth_mask cloth_necklace nasý ossurdu lan
float ances = 0; //cloth_ances
float hat = 0.f;
vector <std::string> collect_logs{};
vector <std::string> drop_logs{};
vector <std::string> banned{};
//-----------------
bool gemcount = false;

std::string MacAdrName = "";
std::string gaccname = "";
int LoginNotification = 0;

//-----------------
enum class platform_id {
    Windows = 0,
    iPhone = 1,
    Mac = 2,
    Android = 4
};
bool showPlatforms = false;
//----------------
bool track = false;
float m_x = 0.0;
float m_y = 0.0;
bool debug = false;

std::vector<std::thread> commands;
std::vector<uint32_t> block_list;
//std::unordered_map<uint32_t, Clothing> clothes_list;
uint32_t item_id;
int wrenchx = 0;
int wrenchy = 0;
int puncheffect = 0;
bool doublejump = false;
int w_id = 0;
int yuzde = 10;
bool ssup = false;
std::string saveworld;
std::string spinsglobal;
bool pulas = false;
bool kickas = false;
bool banas = false;
bool autobgl = false;
bool gaspull = false;
bool ac_message = false;
float speed_x = 250.0f;
bool Nexusreme = false;
bool Nexusqq = false;
bool pathfinder = false;
bool pathfinding = false;
bool denemectrl = false;
bool rolefarmer = false;
bool rolebuilder = false;
bool rolesurgeon = false;
bool rolefisher = false;
bool rolecook = false;
bool aacNexus = false;
bool unaccmod = false;
bool exitmod = false;
bool fast_door = false;
bool unaccess = false;
bool Nexusnumber = true;
bool realfake = true;
//-------------------------
int notaxx = 0;
int notaxy = 0;
float speed_y = 1000.0f;
int bruh;
int bruh2;
int bruh3;
int bruh4;
bool enabled_color = false;
int delay = 4000;
bool logs_no_limited = false;
//--------------------
std::string teamtayfa = "";
std::string teambirlik = "";
std::string teamox = "";
std::string teamju = "";
std::string teamyon = "";
std::string teamhuge = "";
std::string teamrudy = "";
std::string teamnaz = "";
std::string teamocta = "";
std::string teamcong = "";
std::string teamdrey = "";
std::string teamasia = "";
std::string teamdav = "";
std::string teamevo = "";
std::string teambub = "";
std::string teamready = "";
std::string teamph = "";
std::string teamao = "";
std::string teamcumback = "";
std::string teamflip = "";
std::string teampsp = "";
std::string teamlux = "";
std::string teamend = "";
std::string teamlia = "";
//--------------------
std::string isim = "";
std::string gisim = "";
std::string sifre = "";
std::string macaddress = "";
std::string yourrid = "";
std::string yourklv = "";
std::string yourhash = "";
//--------------------
std::vector<std::string> adamlar = {};
std::string bc = "0";
std::string mode = "`5Pull";
std::string skinas = "";
std::string dcount = "0";
std::string aspam = "I Use Nexus Premium Proxy V1.0!";
std::string growid = "";
std::string password = "";
std::string level = "";
std::string uid = "";
std::string gems = "";
std::string backworld = "";
std::string swx = "0";
std::string swxs = "0";
std::string c_text = "";
std::string old_world = "EXIT";
std::string pname = "0";
std::string bname = "0";
//-------------------
bool saveworldmod = false;
bool notax_mode = false;
int eff_flags1 = 0;
int eff_flags2 = 0;
bool used_sponge = false;
bool blink = false;
bool fastblink = false;

bool specficpull = false;
std::string pname1 = "";

bool specficban = false;
std::string bname1 = "";

bool son = false;
bool enter_pull = false;
bool enter_ban = false;
bool exit_world = false;
bool unacc = false;
bool pinglatency = false;
bool scanitem = false;
bool fakeblink = false;
bool mod_detect = false;
bool auto_surg = false;
bool gloves = false;
bool legit_surg = false;

bool wrench = false;
bool fasttrash = false;
bool fastdrop = false;

bool antigravity = false;
bool antizombie = false;
bool pfmode = false;

bool banpro = false;

bool crystal = false;
uint64_t crystal_sleep = 0;

bool devmode = false;
std::string devid = "";

int total_bet = 0;
bool game_started = false;
bool wlleresit = false;
bool noclip = false;
int punch_effect = 8421376;
bool enableac = false;
bool autoacc = false;
bool autoaccc = false;
bool superpunch = false;
bool nopulldialog = false;
bool dropwl = false;
int pathamount = 0;
bool emptybox = false;
bool fastdonate = false;
std::string fastcount = "0";

bool fastvend = false;
bool pullstock = false;
bool addstock = false;

int vendcount = 0;
string lastshop = "";
std::string bos;
bool set1 = false;
bool set2 = false;
bool set3 = false;
bool set4 = false;
std::string locallastspin = "";
bool dropdl = false;
bool diceroll = false;
bool showintstantgem = false;
uint64_t Nexusgem = 0;
int lastgemsamount = 0;
int acms = 100;
int acitemid = 0;
inline std::vector<std::string> salts = { "e9fc40ec08f9ea6393f59c65e37f750aacddf68490c4f92d0d2523a5bc02ea63", "c85df9056ee603b849a93e1ebab5dd5f66e1fb8b2f4a8caef8d13b9f9e013fa4", "3ca373dffbf463bb337e0fd768a2f395b8e417475438916506c721551f32038d", "73eff5914c61a20a71ada81a6fc7780700fb1c0285659b4899bc172a24c14fc1" };
int droppedCount = 0;
std::vector<int> dropVector;
bool dAll = false;
std::string liq = "no";
std::string ban = "no";
std::string step = "0";
int c1, c2, c3, c4, c5;
int use1 = 0, use2 = 0, use3 = 0, use4 = 0, use5 = 0;
int x = 0, y = 0;
std::string use = "";
bool crimeFinished = false;
bool autoCrime = false;
double taxamount = 10.0;
int gdrop = 0;
bool dropbgl = false;
bool done = true;
bool modas = false;
bool netidspin = false;
bool namenumber = false;
bool serverlongpunch = false;
bool autobanfire = false;
bool PocketLighterBan = false;
bool dancemove = false;
bool hidelevel = false;

bool country = false;

bool showoc = false;


bool stopfarm = false;
bool farmauto = false;
bool startedFarm = false;
uint64_t farm_sleep = 0;
int farmdelay = 300;
int farmhit = 4;
int farmid = 2;

int paintdelay = 300;
bool startedPaint = false;
int paintid = 3490;
int paintblock = 2;
bool paintspecial = false;
bool paintauto = false;

bool ptakeseed = false;
int plantdelay = 350;
bool startedPlant = false;
int plantseed = 3;
bool plantauto = false;

int harvestdelay = 300;
bool startedHarvest = false;
int harvestid = 3;
bool harvestauto = false;

bool startedClear = false;
bool clearauto = false;
float modfly = 1000.0f;
bool openfly = false;
int cleardelay = 250;

bool pasteworld = false;
bool startedPaste = false;

//------------------------
bool slcfarm = false;

int vdid = 0;
int vdcount = 0;
//------------------------
std::string hotkeys1 = "None";
std::string hotkeys2 = "None";
std::string hotkeys3 = "None";
std::string hotkeys4 = "None";
std::string hotkeys5 = "None";

std::string hotkey1 = "None";
std::string hotkey2 = "None";
std::string hotkey3 = "None";
std::string hotkey4 = "None";
std::string hotkey5 = "None";
std::string hotkey6 = "None";

char globalhotkey1 = 0;
char globalhotkey2 = 0;
char globalhotkey3 = 0;
char globalhotkey4 = 0;
char globalhotkey5 = 0;
char globalhotkey6 = 0;
//------------------------
vector2_t punchpos;
vector2_t secondpos;
vector2_t pos1;
vector2_t pos2;
bool blockfirst = false;
/*float r_hair, r_shirt, r_pants = 0.f; //cloth_hair & cloth_shirt & cloth_pants
float r_shoe, r_face, r_hand = 0.f; //cloth_feet & cloth_face & cloth_hand
float r_back, r_mask, r_neck = 0.f;//cloth_back cloth_mask cloth_necklace*/


uint8_t build_range = 128;
uint8_t punch_range = 128;

bool gem_message = true;
uint64_t gem_sleep_end = 0;
int last_gems_collect = 0;
uint64_t pinglatencyd = 0;
uint64_t blink_sleep_end = 0;
uint64_t fastblink_sleep_end = 0;
uint64_t spam_sleep_end = 0;
uint64_t automsg_sleep_end = 0;
int spamdelay = 4000;
