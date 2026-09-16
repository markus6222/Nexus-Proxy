#pragma once
#include <stdint.h>
#include <string>
#include <vector>
#include "enet/include/enet.h"
#include "packet.h"
#include "proton/variant.hpp"
#include <chrono>
#include <mutex>

using namespace std::chrono;

#define PRINTS(msg, ...) printf("[SERVER]: " msg, ##__VA_ARGS__);
#define PRINTC(msg, ...) printf("[CLIENT]: " msg, ##__VA_ARGS__);
#define MALLOC(type, ...) (type*)(malloc(sizeof(type) __VA_ARGS__))
#define get_packet_type(packet) (packet->dataLength > 3 ? *packet->data : 0)
#define DO_ONCE            \
    ([]() {                \
        static char o = 0; \
        return !o && ++o;  \
    }())
#ifdef _WIN32
#define INLINE __forceinline
#else //for gcc/clang
#define INLINE inline
#endif

namespace utils {
    char* get_text(ENetPacket* packet);
    gameupdatepacket_t* get_struct(ENetPacket* packet);
    GameUpdatePacket* get_structGUP(ENetPacket* packet);

    int random(int min, int max);
    std::string generate_rid();
    uint32_t hash(uint8_t* str, uint32_t len);
    std::string generate_mac(const std::string& prefix = "02");
    std::string hex_str(unsigned char data);
    std::string random(uint32_t length);
    std::string trim(const std::string& str);
    INLINE uint8_t* get_extended(gameupdatepacket_t* packet) {
        return reinterpret_cast<uint8_t*>(&packet->m_data_size);
    }
    bool replace(std::string& str, const std::string& from, const std::string& to);
    bool is_number(const std::string& s);
    bool runAtInterval(time_point<system_clock>& timer, double interval);
} // namespace utils
