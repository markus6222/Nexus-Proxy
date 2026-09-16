#pragma once
#define CPPHTTPLIB_OPENSSL_SUPPORT
#define _CRT_SECURE_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <ctime>
#include <iomanip>
#include <sstream>
#include <thread>
#include "enet/include/enet.h"
#include "httplib.h"
#include "server.h"
#include "proton/rtparam.hpp"
#include "skStr.h"
#include "HTTPRequest.hpp"
#include <fstream>
#include <string>
#include "print.h"
#include <iostream>
#include <Lmcons.h>
#include <windows.h>
#ifndef __linux__
#include <conio.h>
#include "json.hpp"
#endif
#include <iomanip>
#include "items_dat_decode.h"
#include "events.h"
#include "discord_rpc.h"
#include <Shlobj.h>
#include <urlmon.h>
#include "gt.hpp"
#include "unban.h"
#include <filesystem>
#include <map>
#include "proxy_sidecar.h"

static int64_t eptime = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count();
using namespace std;
#pragma comment( lib, "Advapi32.lib" )
#pragma comment( lib, "User32.lib" )
#pragma comment(lib, "urlmon.lib")
#pragma comment(lib, "winmm.lib") // enet timeGetTime/timeBeginPeriod (pragma used to live in the removed http.h)



bool edited = false;
using json = nlohmann::json;


std::string decodeBase64(const std::string& base64Text)
{
    const char* ALPHABET = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    const uint8_t DECODED_ALPHBET[128] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,62,0,0,0,63,52,53,54,55,56,57,58,59,60,61,0,0,0,0,0,0,0,0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,0,0,0,0,0,0,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,0,0,0,0,0 };

    if (base64Text.empty())
        return "";

    assert((base64Text.size() & 3) == 0 && "The base64 text to be decoded must have a length devisible by 4!");

    uint32_t numPadding = (*std::prev(base64Text.end(), 1) == '=') + (*std::prev(base64Text.end(), 2) == '=');

    std::string decoded((base64Text.size() * 3 >> 2) - numPadding, '.');

    union
    {
        uint32_t temp;
        char tempBytes[4];
    };
    const uint8_t* bytes = reinterpret_cast<const uint8_t*>(base64Text.data());

    std::string::iterator currDecoding = decoded.begin();

    for (uint32_t i = 0, lim = (base64Text.size() >> 2) - (numPadding != 0); i < lim; ++i, bytes += 4)
    {
        temp = DECODED_ALPHBET[bytes[0]] << 18 | DECODED_ALPHBET[bytes[1]] << 12 | DECODED_ALPHBET[bytes[2]] << 6 | DECODED_ALPHBET[bytes[3]];
        (*currDecoding++) = tempBytes[2];
        (*currDecoding++) = tempBytes[1];
        (*currDecoding++) = tempBytes[0];
    }

    switch (numPadding)
    {
    case 2:
        temp = DECODED_ALPHBET[bytes[0]] << 18 | DECODED_ALPHBET[bytes[1]] << 12;
        (*currDecoding++) = tempBytes[2];
        break;

    case 1:
        temp = DECODED_ALPHBET[bytes[0]] << 18 | DECODED_ALPHBET[bytes[1]] << 12 | DECODED_ALPHBET[bytes[2]] << 6;
        (*currDecoding++) = tempBytes[2];
        (*currDecoding++) = tempBytes[1];
        break;
    }

    return decoded;
}

void startHTTPS()
{
    using namespace httplib;
    std::string temp = getenv("TEMP");
    std::string CertPem = decodeBase64("LS0tLS1CRUdJTiBDRVJUSUZJQ0FURS0tLS0tCk1JSUR6akNDQXJhZ0F3SUJBZ0lKQUtjT0JBRlcrT0JWTUEwR0NTcUdTSWIzRFFFQkN3VUFNSE14Q3pBSkJnTlYKQkFZVEFsUlNNUXN3Q1FZRFZRUUlFd0pZUkRFU01CQUdBMVVFQnhNSlNHVjVVM1Z5Wm1WeU1SSXdFQVlEVlFRSwpFd2xJWlhsVGRYSm1aWEl4RWpBUUJnTlZCQXNUQ1VobGVWTjFjbVpsY2pFYk1Ca0dBMVVFQXhNU2QzZDNMbWR5CmIzZDBiM0JwWVRFdVkyOXRNQjRYRFRJeU1EWXhOekUwTURNd00xb1hEVEkwTURZeE5qRTBNRE13TTFvd2N6RUwKTUFrR0ExVUVCaE1DVkZJeEN6QUpCZ05WQkFnVEFsaEVNUkl3RUFZRFZRUUhFd2xJWlhsVGRYSm1aWEl4RWpBUQpCZ05WQkFvVENVaGxlVk4xY21abGNqRVNNQkFHQTFVRUN4TUpTR1Y1VTNWeVptVnlNUnN3R1FZRFZRUURFeEozCmQzY3VaM0p2ZDNSdmNHbGhNUzVqYjIwd2dnRWlNQTBHQ1NxR1NJYjNEUUVCQVFVQUE0SUJEd0F3Z2dFS0FvSUIKQVFEYWI5YTFSUDV1ZW5iaitNV3B0UHRWMHVhRlU1Vjc5WFJoYUl5akhyd2hhTUUxM242bHVvdWd0djNJTFhqSwp1UTJlM3ZoR0R2RTROVVBlU0JhRkw3ZFVVcnViWGZ0ZEFCQjJvdVV0Tis0SnBZNE11QnlJTWNHcjQxQVFVakVhCjl6SlJDQlFJSGhpOGxQUS9MMS8zTXFwY1ZxQmpUNTFPRC9qYUI2UU1iSGVzaEN1cjlIVGo5RXE4ckhZOHRFTDIKTUJMY29JeWpXM1VkTGJIVEI5NUk2aVUyTmlha0F0VG9weXpmaXV4bEJjRE9yM2l0SDBuek9qcjY0RWoraUpidgo5MG5zRXRRYTNFYkVrMEtMY1RNYS9xdFZGY1BpeHpJUklVSVJZY1lXeGErN3RkcUUrOThRUERWYmoxbDY5NDIwCkd2YUFqRUdJMlYzUXp2MFBxU2dZbnMwTkFnTUJBQUdqWlRCak1BNEdBMVVkRHdFQi93UUVBd0lEaURBVEJnTlYKSFNVRUREQUtCZ2dyQmdFRkJRY0RBVEE4QmdOVkhSRUVOVEF6Z2hKM2QzY3VaM0p2ZDNSdmNHbGhNUzVqYjIyQwpFbmQzZHk1bmNtOTNkRzl3YVdFeUxtTnZiWUlKTVRJM0xqQXVNQzR4TUEwR0NTcUdTSWIzRFFFQkN3VUFBNElCCkFRQkJnTDZkZExJUVZpRElGTzIzNEZCWE8vZjRKOFNkVEN0YXh2OEpnM2dvVGtWbXJPamphcXB4K2FvTitRdEkKMXJqVllQTUNOVVNoYjcrZFEzTmVsTjNvRFRmS2tuNWtoNkpSdlhOOFVxeGI5eXpsMmNFbnhSNmpKajB4OWdGdgo5N21lZWVoN1ErTko4MkY3b28wYW5RVWk1MWVYWHdHUDlBS1RDK3NiUStqWWVLSlpYWDdkOTJ3dVM3MnhUY0V4Ci8yT0RyT01aYWszUkJlUUtDaHR2eUIwNGphWEFGdzdURENMbTRwRktSaUhMdUhNY1lVQ2IrcVNDMnJPSUh6VGMKS0dLZzBjUGgxVUhkdThoUVpPWTVicmR5V2RBTFpzK3cxekZ0UFhrMWZ5Y2xJdHV6MkZlcVdvM2FYN0pIRG1zeAppM3VreEF1Q3l5bVA2eU5qQm9PVmVhYVUKLS0tLS1FTkQgQ0VSVElGSUNBVEUtLS0tLQ==");
    std::string KeyPem = decodeBase64("LS0tLS1CRUdJTiBSU0EgUFJJVkFURSBLRVktLS0tLQpNSUlFb2dJQkFBS0NBUUVBMm0vV3RVVCtibnAyNC9qRnFiVDdWZExtaFZPVmUvVjBZV2lNb3g2OElXakJOZDUrCnBicUxvTGI5eUMxNHlya05udDc0Umc3eE9EVkQza2dXaFMrM1ZGSzdtMTM3WFFBUWRxTGxMVGZ1Q2FXT0RMZ2MKaURIQnErTlFFRkl4R3ZjeVVRZ1VDQjRZdkpUMFB5OWY5ektxWEZhZ1kwK2RUZy80Mmdla0RHeDNySVFycS9SMAo0L1JLdkt4MlBMUkM5akFTM0tDTW8xdDFIUzJ4MHdmZVNPb2xOalltcEFMVTZLY3MzNHJzWlFYQXpxOTRyUjlKCjh6bzYrdUJJL29pVzcvZEo3QkxVR3R4R3hKTkNpM0V6R3Y2clZSWEQ0c2N5RVNGQ0VXSEdGc1d2dTdYYWhQdmYKRUR3MVc0OVpldmVOdEJyMmdJeEJpTmxkME03OUQ2a29HSjdORFFJREFRQUJBb0lCQUI4bllrVDZNUnVLcGRnLwp5OWszY2IwODFobmY0T3NNQit3NG9BNUh2T2M2N0l1RlR5VE41VW9ucnl4VXAreXAxZko1dElreGFsL3M0T0FjCkFmSSs2dlBBMVBjRXdXdnpMV1h1TjZkcVdhM1FpZUR3aFVrN1ozYmZkYlRPTkNpM1p0cTl2eldsTFR3QU5wR28KYlJSeGluQ2Uva01Md05DNFlIS2dNbHAvUWRZOXhBTUxoOHRjeDN6N0FsUjIyeEpUY3VaeVpNZXQvanZ6bXhTQQpHb3FtYm94SW5tUnZNSjdaRHc4R25zTm5LZ1VBOG1xWnViZ2FOTnNDZHE4c3RyRE5QdER2TmRTRXlrMmJnWjFOCmZuR0VKMmY5MW03cWR6RXFLZGY1YnBwRWEya2d0Y0oyRHRlWlVxd0NUbmgwNndCUUtZcWV1Y0RhLy9RdDhJVngKYmtCYXo0RUNnWUVBOTJRczFwVDdpYkU0MmxqdElyMzQxMjgwWm1pOHE0aEkxaE5WT3haWm4yRThWZjBwK2p1RAordU9VQlMwL3IxN2M2azJuQ3JYYTVYdkFDTVNjSFBLVFJDY1VBNWN3MFdEcTRmS2FmVm8yNnhSU0FHSG9Gd1BCClNoUXk4enFrMnIvTVc5ZjdPcU4rck5QZnF1cnB6TnBBZVpaL3UvWXM0VllJQm52Z1UwL0xnK0VDZ1lFQTRnbTYKNHh4Nm9tQlFtUlZLaFdNNmtUMEUzSGhISlkyNCt6QVlWR2hiTVNlVExBU3AvYlNEaGVaeUtEWEd2Y0tzYTM0UApDZ2RXUlFDNEx0OHp4YnR3NTlRNC9uckhabENTdS85Z3owQmQ1WTFXRDYxdmNKKzZwdFZNT1J2NFhGS2w1elovClkwYnBrUW5yUkJ3M25pUzZvNTdBTGcwR1dtQkVvVEpyZU9ic2JxMENnWUJ3UExXUEFQYUJ5TEtYZFVMWXdVRjEKVkJGODZNVzRPTk42dERpMTN2VDROeUF0anZjTmZSVHFyWGRKUmZjZnREVWI0L0VHRGUxcXNkTTA1eVpBaDlsQwpVVXhtT0tEQVRXMGk1M01wcmRVK24vQjRGZk03QmN3YXRNRk0wbTFhaFN2TSsxY1Npbng5SW43V1IwK2RUZU4wCmhsQWJVWnVZKy9RV0pQdG9NTXFQWVFLQmdDc1dxYjZUZGprdjNRMWhocVFveDBoYWRtdkVyZU5Wd2RaNFU1cjcKamE2d0daa0JocG9yYUFzRlkrdVFYTU5kc2RxSDNEd1FLL3paWjBMZ0g1Rm82dHYyazZySEl1MjVIRStrSGdORQpCT0kyY0JwcStGeGl4b1Q1RWgrczJrcFhJdk1SYTNVMFZsL2tvU21KcTN5RkNlTVk1dytnUWY3R2JTN0JXc1ZnClY5KzlBb0dBSWQ3Z2Q4UTJmcEl1TWIzRkZkMFhOWUtmK0RaY0dWUXFEcFRtek4ydjY1STg1V3RKZGV3cHhqQnIKWFhHSDBZY3Z1SWRMRFNhZWlOc1VoY3lENmdLbGxNbE5nNnhXU2RpcUUzblI1Z3ErLys4dnNETXh3UmdwTnIrTApxeXRaZGsrRVZ1SUJSUFRzMmpjL2ZJY3VXSlVRanUraEZqV2VhQi9pVWhoRll4Y0tyN1E9Ci0tLS0tRU5EIFJTQSBQUklWQVRFIEtFWS0tLS0t");
    std::ofstream CertPemAppend;
    CertPemAppend.open(temp + ("//cert.pem"), std::ios_base::trunc);
    CertPemAppend << CertPem;
    CertPemAppend.close();
    std::ofstream KeyPemAppend;
    KeyPemAppend.open(temp + ("//key.pem"), std::ios_base::trunc);
    KeyPemAppend << KeyPem;// tamam �imdi bi girmeyi dene 
    KeyPemAppend.close();
    SSLServer svr(std::string{ temp + ("//cert.pem") }.c_str(), std::string{ temp + ("//key.pem") }.c_str());                                 // bekle metayi removla sendekini at direk buraya tamam bekle 
    svr.Post("/growtopia/server_data.php", [](const Request& req, Response& res) {
        // Point the client at us, but pass through the real meta fetched by
        // setgtserver(). This used to send a hardcoded "meta|defined", which the
        // game rejects.
        std::string body =
            "server|127.0.0.1\n"
            "port|" + std::to_string(g_server->m_proxyport) + "\n"
            "type|1\n"
            "#maint|Under maintenance.\n"
            "beta_server|127.0.0.1\n"
            "beta_port|1945\n"
            "beta_type|1\n"
            "meta|" + g_server->meta + "\n"
            "RTENDMARKERBS1001\nunknown\nloginurl|login.growtopiagame.com";
        printf("[HTTP]: server_data.php -> 127.0.0.1:%d meta=%s\n",
            g_server->m_proxyport, g_server->meta.c_str());
        res.set_content(body, "text/html");
        });
    svr.set_logger([](const Request& req, const Response& res) {
        printf("[HTTP]: %s %s -> %d\n", req.method.c_str(), req.path.c_str(), res.status);
        });
    remove(std::string{ temp + ("//cert.pem") }.c_str());
    remove(std::string{ temp + ("//key.pem") }.c_str());

    svr.listen("127.0.0.1", 443);
    //cout << "Done HTTPS!" << endl;
}

server* g_server = new server();
#ifdef _WIN32
BOOL WINAPI exit_handler(DWORD dwCtrlType) {
    try {
        std::ofstream clearhost("C:\\Windows\\System32\\drivers\\etc\\hosts");

        switch (dwCtrlType) {
        case CTRL_BREAK_EVENT || CTRL_CLOSE_EVENT || CTRL_C_EVENT:
            if (clearhost.is_open()) {
                clearhost << "";
                clearhost.close();
            }
            return TRUE;

        default: return FALSE;
        }

        return TRUE;
    }
    catch (int e) {}
}
#endif

bool isInside(int circle_x, int circle_y, int rad, int x, int y) {
    // Compare radius of circle with distance
    // of its center from given point
    if ((x - circle_x) * (x - circle_x) + (y - circle_y) * (y - circle_y) <= rad * rad)
        return true;
    else
        return false;
}

void setgtserver() {
    // Clear our own redirect before asking the real server anything. A previous run that
    // crashed leaves "127.0.0.1 growtopia1.com" behind, and then this request resolves to
    // us instead of Ubisoft -- the fetch fails, and the proxy silently runs on the stale
    // built-in address and a placeholder meta.
    {
        std::ofstream clearhost("C:\\Windows\\System32\\drivers\\etc\\hosts", std::ios::trunc);
    }
    system("ipconfig /flushdns >nul 2>&1");

    try
    {
        using namespace httplib;
        Headers Header;
        Header.insert(make_pair("User-Agent", "UbiServices_SDK_2022.Release.9_PC64_ansi_static"));
        Header.insert(make_pair("Host", "www.growtopia1.com"));
        Client cli("https://www.growtopia1.com");
        cli.set_default_headers(Header);
        cli.enable_server_certificate_verification(false);
        cli.set_connection_timeout(2, 0);
        auto res = cli.Post("/growtopia/server_data.php");
        if (res.error() == Error::Success)
        {
            rtvar var = rtvar::parse({ res->body });
            g_server->m_server = (var.find("server") ? var.get("server") : g_server->m_server);
            g_server->m_port = (var.find("port") ? var.get_int("port") : g_server->m_port);
            g_server->meta = (var.find("meta") ? var.get("meta") : g_server->meta);
            // Remember the main server so a later reset can return to it.
            g_server->serverz = g_server->m_server;
            g_server->portz = g_server->m_port;
            printf("[HTTP]: upstream server_data -> %s:%d meta=%s\n",
                g_server->m_server.c_str(), g_server->m_port, g_server->meta.c_str());
        }
        else {
            print::set_text("[ERROR] Could not reach growtopia1.com for server_data.\n", Red);
            print::set_text("        The proxy has no real server address or meta, so the\n", Red);
            print::set_text("        game will fail to connect. Check your internet, then\n", Red);
            print::set_text("        restart the proxy.\n", Red);
            printf("[HTTP]: upstream server_data request FAILED (error %d); falling back to %s:%d\n",
                (int)res.error(), g_server->m_server.c_str(), g_server->m_port);
        }
    }
    catch (const std::exception& e)
    {
        std::cerr << "Request failed, error: " << e.what() << '\n';
    }
    std::ofstream sethost("C:\\Windows\\System32\\drivers\\etc\\hosts", std::ios::trunc);
    sethost << "127.0.0.1 www.growtopia1.com\n127.0.0.1 www.growtopia2.com";
    sethost.close();
    system("ipconfig /flushdns >nul 2>&1");
    edited = true;
}
void reset_host() {
    std::ofstream sethost("C:\\Windows\\System32\\drivers\\etc\\hosts");

    if (sethost.is_open()) {
        sethost << "";
        sethost.close();


    }
}

std::wstring GetUserProfilePath() {
    PWSTR userProfile;
    if (SUCCEEDED(SHGetKnownFolderPath(FOLDERID_Profile, 0, NULL, &userProfile))) {
        std::wstring userProfilePath = userProfile;
        CoTaskMemFree(userProfile);
        return userProfilePath;
    }
    else {
        return L"";
    }
}

void load_settings() {
    std::wstring dosyaYolu = L"C:\\Users\\Public\\Nexus_Stuff\\save\\config.json";
    std::ifstream ifs(dosyaYolu);
    if (!ifs.is_open()) {
        json j2;
        j2["auto_surg"] = auto_surg;
        j2["fastdrop"] = fastdrop;
        j2["fasttrash"] = fasttrash;
        j2["rqqmode"] = Nexusqq;
        j2["rremeemode"] = Nexusreme;
        j2["namenumber"] = namenumber;
        j2["pinglatency"] = pinglatency;
        j2["denemectrl"] = denemectrl;
        j2["autobgl"] = autobgl;
        j2["autoacc"] = autoacc;
        j2["pathfinder"] = pathfinder;
        j2["antigravity"] = antigravity;
        j2["antighost"] = gt::antighost;
        j2["ghost"] = gt::ghost;
        j2["autobanfire"] = autobanfire;
        j2["PocketLighterBan"] = PocketLighterBan;
        j2["modas"] = modas;
        std::ofstream f_(dosyaYolu);
        f_ << j2;
        f_.close();
    }
    else {
        json j;
        ifs >> j;
        auto_surg = j["auto_surg"];
        fastdrop = j["fastdrop"];
        fasttrash = j["fasttrash"];
        Nexusqq = j["rqqmode"];
        Nexusreme = j["rremeemode"];
        namenumber = j["namenumber"];
        pinglatency = j["pinglatency"];
        denemectrl = j["denemectrl"];
        autobgl = j["autobgl"];
        autoacc = j["autoacc"];
        pathfinder = j["pathfinder"];
        antigravity = j["antigravity"];
        gt::antighost = j["antighost"];
        gt::ghost = j["ghost"];
        autobanfire = j["autobanfire"];
        PocketLighterBan = j["PocketLighterBan"];
        modas = j["modas"];
    }
}
void load_settings2() {
    std::wstring dosyaYolu = L"C:\\Users\\Public\\Nexus_Stuff\\save\\config2.json";
    std::ifstream ifs(dosyaYolu);
    if (!ifs.is_open()) {
        json j2;
        j2["auto_surg"] = auto_surg;
        j2["fastdrop"] = fastdrop;
        j2["fasttrash"] = fasttrash;
        j2["rqqmode"] = Nexusqq;
        j2["rremeemode"] = Nexusreme;
        j2["namenumber"] = namenumber;
        j2["pinglatency"] = pinglatency;
        j2["denemectrl"] = denemectrl;
        j2["autobgl"] = autobgl;
        j2["autoacc"] = autoacc;
        j2["pathfinder"] = pathfinder;
        j2["antigravity"] = antigravity;
        j2["antighost"] = gt::antighost;
        j2["ghost"] = gt::ghost;
        j2["autobanfire"] = autobanfire;
        j2["PocketLighterBan"] = PocketLighterBan;
        j2["modas"] = modas;
        std::ofstream f_(dosyaYolu);
        f_ << j2;
        f_.close();
    }
    else {
        json j;
        ifs >> j;
        auto_surg = j["auto_surg"];
        fastdrop = j["fastdrop"];
        fasttrash = j["fasttrash"];
        Nexusqq = j["rqqmode"];
        Nexusreme = j["rremeemode"];
        namenumber = j["namenumber"];
        pinglatency = j["pinglatency"];
        denemectrl = j["denemectrl"];
        autobgl = j["autobgl"];
        autoacc = j["autoacc"];
        pathfinder = j["pathfinder"];
        antigravity = j["antigravity"];
        gt::antighost = j["antighost"];
        gt::ghost = j["ghost"];
        autobanfire = j["autobanfire"];
        PocketLighterBan = j["PocketLighterBan"];
        modas = j["modas"];
    }
}

void load_settings3() {
    std::wstring dosyaYolu = L"C:\\Users\\Public\\Nexus_Stuff\\save\\config3.json";
    std::ifstream ifs(dosyaYolu);
    if (!ifs.is_open()) {
        json j2;
        j2["auto_surg"] = auto_surg;
        j2["fastdrop"] = fastdrop;
        j2["fasttrash"] = fasttrash;
        j2["rqqmode"] = Nexusqq;
        j2["rremeemode"] = Nexusreme;
        j2["namenumber"] = namenumber;
        j2["pinglatency"] = pinglatency;
        j2["denemectrl"] = denemectrl;
        j2["autobgl"] = autobgl;
        j2["autoacc"] = autoacc;
        j2["pathfinder"] = pathfinder;
        j2["antigravity"] = antigravity;
        j2["antighost"] = gt::antighost;
        j2["ghost"] = gt::ghost;
        j2["autobanfire"] = autobanfire;
        j2["PocketLighterBan"] = PocketLighterBan;
        j2["modas"] = modas;
        std::ofstream f_(dosyaYolu);
        f_ << j2;
        f_.close();
    }
    else {
        json j;
        ifs >> j;
        auto_surg = j["auto_surg"];
        fastdrop = j["fastdrop"];
        fasttrash = j["fasttrash"];
        Nexusqq = j["rqqmode"];
        Nexusreme = j["rremeemode"];
        namenumber = j["namenumber"];
        pinglatency = j["pinglatency"];
        denemectrl = j["denemectrl"];
        autobgl = j["autobgl"];
        autoacc = j["autoacc"];
        pathfinder = j["pathfinder"];
        antigravity = j["antigravity"];
        gt::antighost = j["antighost"];
        gt::ghost = j["ghost"];
        autobanfire = j["autobanfire"];
        PocketLighterBan = j["PocketLighterBan"];
        modas = j["modas"];
    }
}


void hotkeys() {
    while (true) {
        if (GetAsyncKeyState(globalhotkey1) & 0x8000)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(200));
            g_server->queue_command("action|input\n|text|" + hotkeys1);
        }
        else  if (GetAsyncKeyState(globalhotkey2) & 0x8000)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(200));
            g_server->queue_command("action|input\n|text|" + hotkeys2);
        }
        else  if (GetAsyncKeyState(globalhotkey3) & 0x8000)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(200));
            g_server->queue_command("action|input\n|text|" + hotkeys3);
        }
        else  if (GetAsyncKeyState(globalhotkey4) & 0x8000)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(200));
            g_server->queue_command("action|input\n|text|" + hotkeys4);
        }
        else  if (GetAsyncKeyState(globalhotkey5) & 0x8000)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(200));
            g_server->queue_command("action|input\n|text|" + hotkeys5);
        }
    }
}

void AutoCollectThread()
{
    while (true)
    {
        if (g_server->m_world.connected)
        {
            // The "Auto Collecting..." overlay used to be sent from here. That is the
            // same unsafe cross-thread send as the collect packets, and its 750ms sleep
            // stalled collection for three quarters of a second every cycle.
            if (gt::autocollect)
            {
                // Snapshot first: the network thread frees these objects as they are
                // collected, and this loop used to hold pointers into that list.
                const auto drops = g_server->m_world.object_map.snapshot();
                const auto me = g_server->m_world.local.pos;

                // Don't re-ask for the same drop every 20ms while the server gets around
                // to it; that floods the send queue for no benefit.
                static std::map<uint32_t, std::chrono::steady_clock::time_point> last_try;
                const auto now = std::chrono::steady_clock::now();

                for (const auto& object : drops)
                {
                    if (!isInside((int)object.pos.m_x, (int)object.pos.m_y,
                        gt::ac_range * 32, (int)me.m_x, (int)me.m_y))
                        continue;

                    const auto seen = last_try.find(object.oid);
                    if (seen != last_try.end() &&
                        std::chrono::duration_cast<std::chrono::milliseconds>(now - seen->second).count() < 1500)
                        continue;
                    last_try[object.oid] = now;

                    uint8_t packet[GAME_PACKET_SIZE];
                    build_collect_packet(object.oid, object.pos.m_x, object.pos.m_y, packet);
                    // Queued, not sent: this is a background thread and the ENet host
                    // belongs to the polling thread.
                    g_server->queue_to_server(packet, GAME_PACKET_SIZE);
                }
            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(20ms));

    }
}

void UpdateMap()
{
    while (true)
    {
        try {
            if (g_server->m_world.connected)
            {
                std::this_thread::sleep_for(std::chrono::milliseconds(20ms));
                for (auto& tile : g_server->m_world.tile_map.tiles) {
                    if (tile->foreground != 0) {
                        if (tile->foreground != 6 && tile->foreground != 410 && tile->foreground != 20 && tile->foreground != 26 && tile->foreground != 608 &&
                            tile->foreground != 780 && tile->foreground != 102 && tile->foreground != 1222 && tile->foreground != 4422 && tile->foreground != 2978 &&
                            tile->foreground != 3524 && tile->foreground != 762 && tile->foreground != 4632 && tile->foreground != 1308 && tile->foreground != 2862 &&
                            tile->foreground != 4798 && tile->foreground != 9740 && tile->foreground != 4240 && tile->foreground != 846 && tile->foreground != 5616 &&
                            tile->foreground != 9198 && tile->foreground != 1528 && tile->foreground != 3002 && tile->foreground != 430 && tile->foreground != 5036 &&
                            tile->foreground != 7444 && tile->foreground != 482 && tile->foreground != 12 && tile->foreground != 22 && tile->foreground != 16 &&
                            tile->foreground != 954 && tile->foreground != 3898 && tile->foreground != 1042 && tile->foreground != 6854 && tile->foreground != 8260 &&
                            tile->foreground != 5666 && tile->foreground != 340 && tile->foreground != 4722 && tile->foreground != 598 && tile->foreground != 2810 &&
                            tile->foreground != 554 && tile->foreground != 60 && tile->foreground != 1450 && tile->foreground != 8682 && tile->foreground != 5054 &&
                            tile->foreground != 4202 && tile->foreground != 3804 && tile->foreground != 4482 && tile->foreground != 1104 && tile->foreground != 4782 &&
                            tile->foreground != 3806 && tile->foreground != 2948 && tile->foreground != 8676 && tile->foreground != 858 && tile->foreground != 4740 &&
                            tile->foreground != 3796 && tile->foreground != 1700 && tile->foreground != 3232 && tile->foreground != 3572 && tile->foreground != 3566 &&
                            tile->foreground != 484 && tile->foreground != 5034 && tile->foreground != 546 && tile->foreground != 5032 && tile->foreground != 1446 &&
                            tile->foreground != 1604 && tile->foreground != 7164 && tile->foreground != 5040 && tile->foreground != 1684 && tile->foreground != 1702 &&
                            tile->foreground != 4704 && tile->foreground != 4706 && tile->foreground != 2072 && tile->foreground != 1162 && tile->foreground != 1240 &&
                            tile->foreground != 1770 && tile->foreground != 1420 && tile->foreground != 2586 && tile->foreground != 1422 && tile->foreground != 3522 &&
                            tile->foreground != 658 && tile->foreground != 428 && tile->foreground != 224 && tile->foreground != 4720 && tile->foreground != 998 &&
                            tile->foreground != 1326 && tile->foreground != 1752 && tile->foreground != 1324 && tile->foreground != 1682 && tile->foreground != 30 &&
                            tile->foreground != 4794 && tile->foreground != 4546 && tile->foreground != 3808 && tile->foreground != 10076 && tile->foreground != 10078 &&
                            tile->foreground != 2252 && tile->foreground != 2274 && tile->foreground != 62 && tile->foreground != 480 && tile->foreground != 24 &&
                            tile->foreground != 5660 && tile->foreground != 8020 && tile->foreground != 2244 && tile->foreground != 2242 && tile->foreground != 2246 &&
                            tile->foreground != 2248 && tile->foreground != 190 && tile->foreground != 192 && tile->foreground != 188 && tile->foreground != 758 &&
                            tile->foreground != 1256 && tile->foreground != 5468 && tile->foreground != 3072 && tile->foreground != 4352 && tile->foreground != 2272 &&
                            tile->foreground != 8994 && tile->foreground != 1482 && tile->foreground != 379 && tile->foreground != 10776 && tile->foreground != 9614 &&
                            tile->foreground != 886 && tile->foreground != 486 && tile->foreground != 1048 && tile->foreground != 3286 && tile->foreground != 3496 &&
                            tile->foreground != 3832 && tile->foreground != 756 && tile->foreground != 1436 && tile->foreground != 9268 && tile->foreground != 8634 &&
                            tile->foreground != 7362 && tile->foreground != 550 && tile->foreground != 286 && tile->foreground != 444 && tile->foreground != 2964 &&
                            tile->foreground != 1288 && tile->foreground != 854 && tile->foreground != 9330 && tile->foreground != 7984 && tile->foreground != 7888 &&
                            tile->foreground != 652 && tile->foreground != 1108 && tile->foreground != 1594 && tile->foreground != 398 && tile->foreground != 1434 &&
                            tile->foreground != 1304 && tile->foreground != 926 && tile->foreground != 656 && tile->foreground != 544 && tile->foreground != 1698 &&
                            tile->foreground != 974 && tile->foreground != 1694 && tile->foreground != 2646 && tile->foreground != 2068 && tile->foreground != 688 &&
                            tile->foreground != 1432 && tile->foreground != 9080 && tile->foreground != 686 && tile->foreground != 3126 && tile->foreground != 4698 &&
                            tile->foreground != 986 && tile->foreground != 4712 && tile->foreground != 5792 && tile->foreground != 6146 && tile->foreground != 5798 &&
                            tile->foreground != 3794 && tile->foreground != 6772 && tile->foreground != 1696 && tile->foreground != 4718 && tile->foreground != 596) {
                            g_server->gtmap[tile->pos.m_y][tile->pos.m_x] = 1;
                        }
                        if (tile->foreground == 596 && tile->flag == 32) { //Chest
                            g_server->gtmap[tile->pos.m_y][tile->pos.m_x] = 1;
                        }
                        if (tile->foreground == 596 && tile->flag == 0) { //Chest
                            g_server->gtmap[tile->pos.m_y][tile->pos.m_x] = 1;
                        }
                        if (tile->foreground == 4712 && tile->flag == 64) { //Gorilla
                            g_server->gtmap[tile->pos.m_y][tile->pos.m_x] = 1;
                        }
                        if (tile->foreground == 3126 && tile->flag == 1) { //Dark Magic Barrier
                            g_server->gtmap[tile->pos.m_y][tile->pos.m_x] = 1;
                        }
                        if (tile->foreground == 3126 && tile->flag == 0) { //Dark Magic Barrier
                            g_server->gtmap[tile->pos.m_y][tile->pos.m_x] = 1;
                        }
                        if (tile->foreground == 3126 && tile->flag == 32) { //Dark Magic Barrier
                            g_server->gtmap[tile->pos.m_y][tile->pos.m_x] = 1;
                        }
                        if (tile->foreground == 686 && tile->flag == 1) { //Jail Door
                            g_server->gtmap[tile->pos.m_y][tile->pos.m_x] = 1;
                        }
                        if (tile->foreground == 686 && tile->flag == 0) { //Jail Door
                            g_server->gtmap[tile->pos.m_y][tile->pos.m_x] = 1;
                        }
                        if (tile->foreground == 686 && tile->flag == 32) { //Jail Door
                            g_server->gtmap[tile->pos.m_y][tile->pos.m_x] = 1;
                        }
                        if (tile->foreground == 3286 && tile->flag == 1024) { //Steam Door
                            g_server->gtmap[tile->pos.m_y][tile->pos.m_x] = 1;
                        }
                        if (tile->foreground == 3286 && tile->flag == 1056) { //Steam Door
                            g_server->gtmap[tile->pos.m_y][tile->pos.m_x] = 1;
                        }
                        if (tile->foreground == 3798 && tile->flag == 1) { //Vip Entrance
                            g_server->gtmap[tile->pos.m_y][tile->pos.m_x] = 1;
                        }
                        if (tile->foreground == 3798 && tile->flag == 0) { //Vip Entrance
                            g_server->gtmap[tile->pos.m_y][tile->pos.m_x] = 1;
                        }
                        if (tile->foreground == 3798 && tile->flag == 32) { //Vip Entrance
                            g_server->gtmap[tile->pos.m_y][tile->pos.m_x] = 1;
                        }
                        if (tile->foreground == 4352 && tile->flag == 0) { //Wolf Gate
                            g_server->gtmap[tile->pos.m_y][tile->pos.m_x] = 1;
                        }
                        if (tile->foreground == 4352 && tile->flag == 32) { //Wolf Gate
                            g_server->gtmap[tile->pos.m_y][tile->pos.m_x] = 1;
                        }
                        if (tile->foreground == 224 && tile->flag == 0) { //House Entrance
                            g_server->gtmap[tile->pos.m_y][tile->pos.m_x] = 1;
                        }
                        if (tile->foreground == 224 && tile->flag == 32) { //House Entrance
                            g_server->gtmap[tile->pos.m_y][tile->pos.m_x] = 1;
                        }
                        if (tile->foreground == 224 && tile->flag == 48) { //House Entrance
                            g_server->gtmap[tile->pos.m_y][tile->pos.m_x] = 1;
                        }
                        if (tile->foreground == 224 && tile->flag == 24) { //House Entrance
                            g_server->gtmap[tile->pos.m_y][tile->pos.m_x] = 1;
                        }
                        if (tile->foreground == 224 && tile->flag == 288) { //House Entrance
                            g_server->gtmap[tile->pos.m_y][tile->pos.m_x] = 1;
                        }
                        if (tile->foreground == 4240 && tile->flag == 0) { //Jade Portcullis
                            g_server->gtmap[tile->pos.m_y][tile->pos.m_x] = 1;
                        }
                        if (tile->foreground == 4240 && tile->flag == 32) { //Jade Portcullis
                            g_server->gtmap[tile->pos.m_y][tile->pos.m_x] = 1;
                        }
                        if (tile->foreground == 4240 && tile->flag == 24) { //Jade Portcullis
                            g_server->gtmap[tile->pos.m_y][tile->pos.m_x] = 1;
                        }
                        if (tile->foreground == 1162 && tile->flag == 0) { //Forcefield
                            g_server->gtmap[tile->pos.m_y][tile->pos.m_x] = 1;
                        }
                        if (tile->foreground == 1162 && tile->flag == 32) { //Forcefield
                            g_server->gtmap[tile->pos.m_y][tile->pos.m_x] = 1;
                        }
                        if (tile->foreground == 1162 && tile->flag == 24) { //Forcefield
                            g_server->gtmap[tile->pos.m_y][tile->pos.m_x] = 1;
                        }
                        if (tile->foreground == 4706 && tile->flag == 0) { //Adventure Barrier
                            g_server->gtmap[tile->pos.m_y][tile->pos.m_x] = 1;
                        }
                        if (tile->foreground == 7164 && tile->flag == 0) { //Red Entrance
                            g_server->gtmap[tile->pos.m_y][tile->pos.m_x] = 1;
                        }
                        if (tile->foreground == 7164 && tile->flag == 32) { //Red Entrance
                            g_server->gtmap[tile->pos.m_y][tile->pos.m_x] = 1;
                        }
                        if (tile->foreground == 7164 && tile->flag == 24) { //Red Entrance
                            g_server->gtmap[tile->pos.m_y][tile->pos.m_x] = 1;
                        }
                        if (tile->foreground == 7164 && tile->flag > 1000) { //Red Entrance
                            g_server->gtmap[tile->pos.m_y][tile->pos.m_x] = 1;
                        }
                        if (tile->foreground == 5032 && tile->flag == 32) { //Dirt Gate
                            g_server->gtmap[tile->pos.m_y][tile->pos.m_x] = 1;
                        }
                        if (tile->foreground == 5032 && tile->flag == 0) { //Dirt Gate
                            g_server->gtmap[tile->pos.m_y][tile->pos.m_x] = 1;
                        }
                        if (tile->foreground == 5032 && tile->flag == 24) { //Dirt Gate
                            g_server->gtmap[tile->pos.m_y][tile->pos.m_x] = 1;
                        }
                        if (tile->foreground == 5032 && tile->flag == 16) { //Dirt Gate
                            g_server->gtmap[tile->pos.m_y][tile->pos.m_x] = 1;
                        }
                        if (tile->foreground == 60 && tile->flag == 0) { //portuculis
                            g_server->gtmap[tile->pos.m_y][tile->pos.m_x] = 1;
                        }
                        if (tile->foreground == 60 && tile->flag == 32) { //portuculis
                            g_server->gtmap[tile->pos.m_y][tile->pos.m_x] = 1;
                        }
                        if (tile->foreground == 60 && tile->flag == 288) { //portuculis
                            g_server->gtmap[tile->pos.m_y][tile->pos.m_x] = 1;
                        }
                        if (tile->foreground == 2810 && tile->flag == 0) { //air vent
                            g_server->gtmap[tile->pos.m_y][tile->pos.m_x] = 1;
                        }
                        if (tile->foreground == 2810 && tile->flag == 32) { //air vent
                            g_server->gtmap[tile->pos.m_y][tile->pos.m_x] = 1;
                        }
                        if (tile->foreground == 2810 && tile->flag == 24) { //air vent
                            g_server->gtmap[tile->pos.m_y][tile->pos.m_x] = 1;
                        }
                        if (tile->foreground == 598 && tile->flag == 0) { //dragon gate
                            g_server->gtmap[tile->pos.m_y][tile->pos.m_x] = 1;
                        }
                        if (tile->foreground == 598 && tile->flag == 32) { //dragon gate
                            g_server->gtmap[tile->pos.m_y][tile->pos.m_x] = 1;
                        }
                        if (tile->foreground == 598 && tile->flag == 16) { //dragon gate
                            g_server->gtmap[tile->pos.m_y][tile->pos.m_x] = 1;
                        }
                        if (tile->foreground == 598 && tile->flag == 128) { //dragon gate
                            g_server->gtmap[tile->pos.m_y][tile->pos.m_x] = 1;
                        }
                        if (tile->foreground == 5036 && tile->flag == 0) { //hidden door 3 VV
                            g_server->gtmap[tile->pos.m_y][tile->pos.m_x] = 1;
                        }
                        if (tile->foreground == 5036 && tile->flag == 32) { //hidden door 3 VV
                            g_server->gtmap[tile->pos.m_y][tile->pos.m_x] = 1;
                        }
                        if (tile->foreground == 5036 && tile->flag == 24) { //hidden door 3 VV
                            g_server->gtmap[tile->pos.m_y][tile->pos.m_x] = 1;
                        }
                        if (tile->foreground == 5036 && tile->flag == 50) { //hidden door 3 VV
                            g_server->gtmap[tile->pos.m_y][tile->pos.m_x] = 1;
                        }
                        if (tile->foreground == 5036 && tile->flag > 1000) { //hidden door 3 VV
                            g_server->gtmap[tile->pos.m_y][tile->pos.m_x] = 1;
                        }

                        if (tile->flag == 25) {
                            g_server->gtmap[tile->pos.m_y][tile->pos.m_x] = 0;
                        }
                        if (tile->flag == 17) {
                            g_server->gtmap[tile->pos.m_y][tile->pos.m_x] = 0;
                        }
                        if (tile->flag == 19) {
                            g_server->gtmap[tile->pos.m_y][tile->pos.m_x] = 0;
                        }
                        if (tile->flag == 27) {
                            g_server->gtmap[tile->pos.m_y][tile->pos.m_x] = 0;
                        }
                        if (tile->flag == 128) {
                            g_server->gtmap[tile->pos.m_y][tile->pos.m_x] = 0;
                        }
                        if (tile->foreground == 1162 && tile->flag == 128) {
                            g_server->gtmap[tile->pos.m_y][tile->pos.m_x] = 0;
                        }
                        if (tile->foreground == 1042 && tile->flag == 1) { //sumthing lol ahaha
                            g_server->gtmap[tile->pos.m_y][tile->pos.m_x] = 0;
                        }
                        if (tile->foreground == 6854 && tile->flag == 1) { //sumthing lol ahaha
                            g_server->gtmap[tile->pos.m_y][tile->pos.m_x] = 0;
                        }
                        if (tile->foreground == 20 && tile->flag == 1) { //sumthing lol ahaha
                            g_server->gtmap[tile->pos.m_y][tile->pos.m_x] = 0;
                        }
                        if (tile->foreground == 428 && tile->flag == 1) { //sumthing lol ahaha
                            g_server->gtmap[tile->pos.m_y][tile->pos.m_x] = 0;
                        }
                        if (tile->foreground == 430 && tile->flag == 1) { //sumthing lol ahaha
                            g_server->gtmap[tile->pos.m_y][tile->pos.m_x] = 0;
                        }
                        if (tile->foreground == 546 && tile->flag == 1) { //sumthing lol ahaha
                            g_server->gtmap[tile->pos.m_y][tile->pos.m_x] = 0;
                        }
                        if (tile->foreground == 544 && tile->flag == 1) { //sumthing lol ahaha
                            g_server->gtmap[tile->pos.m_y][tile->pos.m_x] = 0;
                        }
                        if (tile->foreground == 986 && tile->flag == 1) { //sumthing lol ahaha
                            g_server->gtmap[tile->pos.m_y][tile->pos.m_x] = 0;
                        }
                        if (tile->foreground == 8802 && tile->flag == 16) { //sumthing lol ahaha
                            g_server->gtmap[tile->pos.m_y][tile->pos.m_x] = 0;
                        }
                        if (tile->foreground == 2272 && tile->flag == 0) { //sumthing lol ahaha
                            g_server->gtmap[tile->pos.m_y][tile->pos.m_x] = 0;
                        }
                        if (tile->foreground == 8220) { //sumthing lol ahaha
                            g_server->gtmap[tile->pos.m_y][tile->pos.m_x] = 0;
                        }
                        if (tile->foreground == 6154 && tile->flag == 0) { //sumthing lol ahaha
                            g_server->gtmap[tile->pos.m_y][tile->pos.m_x] = 0;
                        }
                        if (tile->foreground == 6154 && tile->flag == 64) { //sumthing lol ahaha
                            g_server->gtmap[tile->pos.m_y][tile->pos.m_x] = 1;
                        }
                        //986
                    }
                    if (tile->foreground == 0) {
                        g_server->gtmap[tile->pos.m_y][tile->pos.m_x] = 0;
                    }
                }

            }
        }
        catch (...)
        {
        }
    }
}

BOOL WINAPI ConsoleHandler(DWORD dwType)
{
    switch (dwType) {
    case CTRL_LOGOFF_EVENT: case CTRL_SHUTDOWN_EVENT: case CTRL_CLOSE_EVENT:
    {
        reset_host();
        return TRUE;
    }
    default:
    {
        break;
    }
    }
    return FALSE;
}
void Initialize()
{
    DiscordEventHandlers Handle;
    memset(&Handle, 0, sizeof(Handle));
    Discord_Initialize("1202702420166053988", &Handle, 1, NULL);
}
std::time_t CurrentTime = std::time(0);
void Update()
{
    DiscordRichPresence discordPresence;
    memset(&discordPresence, 0, sizeof(discordPresence));
    discordPresence.details = "Waiting Authenticate To Proxy!";
    discordPresence.state = "Licensed Role: Premium Proxy User";
    discordPresence.largeImageKey = "nexus";
    discordPresence.startTimestamp = eptime;
    discordPresence.largeImageText = "Nexus Premium Proxy V1.1";
    discordPresence.smallImageKey = "kirmizitik";
    discordPresence.smallImageText = "Waiting Authenticate"; 
    discordPresence.button1Label = "Nexus Proxy Discord";
    discordPresence.button1Url = "https://discord.gg/UFPxYvpfZc";
    Discord_UpdatePresence(&discordPresence);
}


int main() {
    // printf and std::cout are both used for logging; without this they buffer
    // independently and the console shows lines out of order, which makes tracing a
    // disconnect misleading.
    setvbuf(stdout, nullptr, _IONBF, 0);
    std::ios::sync_with_stdio(true);
    launch_proxy_sidecar();

#ifdef _WIN32
    BOOL ret = SetConsoleCtrlHandler(ConsoleHandler, TRUE);
#endif
    Initialize();
    Update();

    system("Color 0B");
    SetConsoleTitleA("Nexus Premium Proxy V1.1");

    std::string folderPath1 = "C:\\Users\\Public\\Nexus_Stuff";

    try {
        std::filesystem::create_directory(folderPath1);
    }
    catch (const std::filesystem::filesystem_error& e) {
    }

    std::string folderPath2 = "C:\\Users\\Public\\Nexus_Stuff\\save";

    try {
        std::filesystem::create_directory(folderPath2);
    }
    catch (const std::filesystem::filesystem_error& e) {
    }

    std::string folderPath3 = "C:\\Users\\Public\\Nexus_Stuff\\save\\clothes";

    try {
        std::filesystem::create_directory(folderPath3);
    }
    catch (const std::filesystem::filesystem_error& e) {
    }

    std::string folderPathe = "C:\\Users\\Public\\Nexus_Stuff\\Logs";

    try {
        std::filesystem::create_directory(folderPathe);
    }
    catch (const std::filesystem::filesystem_error& e) {
    }


    std::string CachePath = "C:\\Users\\Public\\Nexus_Stuff\\Logs";
    std::string RoulletteName = "Nexus_RouletteLogs.txt";
    std::string RoulettePath = CachePath + "\\" + RoulletteName;

    std::ifstream deneme(RoulettePath);
    if (!deneme.is_open()) {
        std::ofstream newFile(RoulettePath);
        if (newFile.is_open()) {
            newFile.close();
        }
        else {
            return 1;
        }
    }
    else {
        deneme.close();
        std::ofstream recreateFile(RoulettePath);
        if (recreateFile.is_open()) {
            recreateFile.close();
        }
        else {
            return 1;
        }
    }

    std::string CachePath1 = "C:\\Users\\Public\\Nexus_Stuff\\Logs";
    std::string RoulletteName1 = "Nexus_DropCollect.txt";
    std::string RoulettePath1 = CachePath1 + "\\" + RoulletteName1;

    std::ifstream deneme1(RoulettePath1);
    if (!deneme1.is_open()) {
        std::ofstream newFile1(RoulettePath1);
        if (newFile1.is_open()) {
            newFile1.close();
        }
        else {
            return 1;
        }
    }
    else {
        deneme1.close();
        std::ofstream recreateFile1(RoulettePath1);
        if (recreateFile1.is_open()) {
            recreateFile1.close();
        }
        else {
            return 1;
        }
    }

    std::string CachePath2 = "C:\\Users\\Public\\Nexus_Stuff\\Logs";
    std::string RoulletteName2 = "Nexus_EnterExit.txt";
    std::string RoulettePath2 = CachePath2 + "\\" + RoulletteName2;

    std::ifstream deneme2(RoulettePath2);
    if (!deneme2.is_open()) {
        std::ofstream newFile2(RoulettePath2);
        if (newFile2.is_open()) {
            newFile2.close();
        }
        else {
            return 1;
        }
    }
    else {
        deneme2.close();
        std::ofstream recreateFile2(RoulettePath2);
        if (recreateFile2.is_open()) {
            recreateFile2.close();
        }
        else {
            return 1;
        }
    }

   

    system("CLS");
    SetConsoleTitleA("Nexus Premium Proxy V1.1");

    /*
    PWSTR localAppDataPath = nullptr;

    if (SUCCEEDED(SHGetKnownFolderPath(FOLDERID_LocalAppData, 0, NULL, &localAppDataPath))) {
        WCHAR filePath[MAX_PATH];
        wcscpy_s(filePath, MAX_PATH, localAppDataPath);
        wcscat_s(filePath, MAX_PATH, L"\\Growtopia\\Growtopia.exe");

        HINSTANCE result = ShellExecuteW(nullptr, L"open", filePath, nullptr, nullptr, SW_SHOWNORMAL);
    }*/

    //std::cout << "Succesfully Logged in With Username: " << girilenAnahtar << "\n";
    //std::string consoleTitle = "Nexus Premium Proxy V1.1 - Logged In As '" + girilenAnahtar + "'";
    //SetConsoleTitleA(consoleTitle.c_str());

    const int numDots = 3;
    const std::chrono::milliseconds interval(500);

    std::cout << "Please Wait";

    for (int i = 0; i < numDots; ++i) {
        std::this_thread::sleep_for(interval);
        std::cout << ".";
        std::cout.flush();
    }

    std::cout << " Finished!" << std::endl;
    system("Color 0B");

    DiscordRichPresence discordPresence;
    memset(&discordPresence, 0, sizeof(discordPresence));
    discordPresence.details = "Currently Not In Game!";
    discordPresence.state = "Licensed Role: Premium Proxy User";
    discordPresence.largeImageKey = "Nexus";
    discordPresence.startTimestamp = eptime;
    discordPresence.largeImageText = "Nexus Premium Proxy V1.1";
    discordPresence.smallImageKey = "kirmizitik";
    discordPresence.smallImageText = "Not In-Game";
    discordPresence.button1Label = "Nexus Proxy Discord";
    discordPresence.button1Url = "https://discord.gg/UFPxYvpfZc";
    Discord_UpdatePresence(&discordPresence);

    std::string RegistryRndmRegistery = generateRandomString(10);

    setgtserver();
    SpoofRegistery();
    updateRegistry(RegistryRndmRegistery);

    //std::string denemes = "Wi-Fi";
    //std::string newMAC = generateRandomMAC();
    //newMAC.erase(std::remove(newMAC.begin(), newMAC.end(), ':'), newMAC.end());
    //changeMACAddress(denemes, newMAC);

    std::thread httpS(startHTTPS);
    std::thread(&AutoCollectThread).detach();
    std::thread hotkey(hotkeys);
    //std::thread(&UpdateMap).detach();

    load_settings();
    load_settings2();
    load_settings3();

    system("Color 0B");
    print::set_text("Licensed Role: Nexus Premium Proxy User\n", LightCyan);
    print::set_text("Successfully Loaded Items Database!\n", LightCyan);
    if (!edited) {
        print::set_text("Failed To Edit Hosts File.\n", Red);
        Sleep(5000);
        exit(-1);
    }
    else {
        system("Color 0B");
        print::set_text("Successfully Edited Hosts File.\n", LightCyan);
        decitem();
        httpS.detach();
        enet_initialize();
        g_server->unlockThread();
        if (g_server->start()) {
            system("Color 0B");
            print::set_text("[NOTICE] Server & client proxy is running.\n", LightCyan);
            print::set_text(" \n", LightCyan);
            print::set_text("Your Monitor Supports 60 Hz Increased Smoothness To 90 FPS!\n", LightCyan);
            print::set_text("Restart Growtopia If Proxy Doesn't Connect To Growtopia!\n", LightCyan);
            print::set_text(" \n", LightCyan);
            while (true) {
                g_server->poll();
                std::this_thread::sleep_for(std::chrono::milliseconds(5));

            }
        }
        else
            print::set_text("Failed to start server or proxy.\n", Red);
    }
}