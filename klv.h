#pragma once
#include <openssl/evp.h>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <vector>
#include <array>
namespace hashx {
    std::string sha256(const std::string& text) {
        unsigned char hash[EVP_MAX_MD_SIZE];
        unsigned int lengthOfHash = 0;

        EVP_MD_CTX* context = EVP_MD_CTX_new();
        if (context != nullptr) {
            if (EVP_DigestInit_ex(context, EVP_sha256(), nullptr)) {
                if (EVP_DigestUpdate(context, text.c_str(), text.size())) {
                    if (EVP_DigestFinal_ex(context, hash, &lengthOfHash)) {
                        // Succeed
                    }
                }
            }
            EVP_MD_CTX_free(context);
        }

        std::stringstream ss;
        for (unsigned int i = 0; i < lengthOfHash; ++i) {
            ss << std::hex << std::setw(2) << std::setfill('0') << (int)hash[i];
        }
        return ss.str();
    }
    std::string md5(const std::string& input) {
        unsigned char digest[EVP_MAX_MD_SIZE];
        unsigned int digest_len;
        EVP_MD_CTX* ctx = EVP_MD_CTX_new();
        EVP_DigestInit_ex(ctx, EVP_md5(), nullptr);         
        EVP_DigestUpdate(ctx, input.c_str(), input.length());
        EVP_DigestFinal_ex(ctx, digest, &digest_len);
        EVP_MD_CTX_free(ctx);

        char md5string[33];
        for (int i = 0; i < 16; i++) {
            sprintf(&md5string[i * 2], "%02x", (unsigned int)digest[i]);
        }
        md5string[32] = '\0';

        std::string hash = std::string(md5string);
        std::transform(hash.begin(), hash.end(), hash.begin(), ::toupper);
        return hash;
    }
}


std::string generate_klv(const int protocol, const std::string& version, const std::string& rid, std::vector<std::string> salts) {
    //Bekle geliom ok
    static std::array constant_values = {
        hashx::sha256(hashx::md5(hashx::sha256(std::to_string(protocol)))),
        hashx::sha256(hashx::sha256(version)),
        hashx::sha256(hashx::sha256(std::to_string(protocol)) + salts[3])
    };

    return hashx::sha256(constant_values[0]
        + salts[0]
        + constant_values[1]
        + salts[1]
        + hashx::sha256(hashx::md5(hashx::sha256(rid)))
        + salts[2]
        + constant_values[2]
    );
}