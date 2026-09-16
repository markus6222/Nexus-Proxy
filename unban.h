#pragma once
#include <iostream>
#include <Windows.h>
#ifndef MAX_KEY_LENGTH
#define MAX_KEY_LENGTH 255
#endif
#include <filesystem>
#include <atlbase.h>
#include <iphlpapi.h>

#pragma comment(lib, "iphlpapi.lib")

std::string generateRandomString(size_t length) {
    const char charset[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    std::default_random_engine rng(std::random_device{}());
    std::uniform_int_distribution<> dist(0, sizeof(charset) - 2);
    std::string result;
    result.reserve(length);

    for (size_t i = 0; i < length; ++i) {
        result += charset[dist(rng)];
    }

    return result;
}

void updateRegistry(const std::string& value) {
    HKEY hKey;
    LONG lResult;
    lResult = RegOpenKeyEx(HKEY_LOCAL_MACHINE, "SYSTEM\\ControlSet001\\Services\\Tcpip\\Parameters", 0, KEY_SET_VALUE, &hKey);

    if (lResult == ERROR_SUCCESS) {
        RegSetValueEx(hKey, "Hostname", 0, REG_SZ, (const BYTE*)value.c_str(), value.size() + 1);
        RegSetValueEx(hKey, "NV Hostname", 0, REG_SZ, (const BYTE*)value.c_str(), value.size() + 1);
        RegCloseKey(hKey);
    }

    lResult = RegOpenKeyEx(HKEY_LOCAL_MACHINE, "SOFTWARE\\Microsoft\\Cryptography", 0, KEY_SET_VALUE, &hKey);
    if (lResult == ERROR_SUCCESS) {
        RegDeleteValue(hKey, "MachineGuid");
        RegCloseKey(hKey);
    }
}



bool IsFullyNumeric(const std::string& str) {
    return std::regex_match(str, std::regex("[0-9]+"));
}

void DeleteNumericSubKeys(HKEY hKey) {
    DWORD subKeyCount = 0;
    DWORD maxSubKeyLen = 0;
    DWORD retCode;

    retCode = RegQueryInfoKey(
        hKey, NULL, NULL, NULL, &subKeyCount, &maxSubKeyLen,
        NULL, NULL, NULL, NULL, NULL, NULL);

    if (retCode != ERROR_SUCCESS) {
        return;
    }

    std::vector<char> subKeyNameBuffer(maxSubKeyLen + 1);
    for (DWORD i = 0; i < subKeyCount; ++i) {
        DWORD subKeyNameLen = maxSubKeyLen + 1;
        retCode = RegEnumKeyEx(
            hKey, i, subKeyNameBuffer.data(), &subKeyNameLen,
            NULL, NULL, NULL, NULL);

        if (retCode != ERROR_SUCCESS) {
            continue;
        }

        std::string subKeyName(subKeyNameBuffer.data(), subKeyNameLen);
        if (IsFullyNumeric(subKeyName)) {
            retCode = RegDeleteTree(hKey, subKeyName.c_str());
            if (retCode == ERROR_SUCCESS) {
                i = -1;
                subKeyCount--;
            }
        }
    }
}

void SpoofRegistery() {
    HKEY hKey;

    if (RegOpenKeyEx(HKEY_CURRENT_USER, NULL, 0, KEY_READ | KEY_WRITE, &hKey) == ERROR_SUCCESS) {
        DeleteNumericSubKeys(hKey);
        RegCloseKey(hKey);
    }

    LPCSTR subKey = "SOFTWARE\\Microsoft";

    if (RegOpenKeyEx(HKEY_CURRENT_USER, subKey, 0, KEY_READ | KEY_WRITE, &hKey) == ERROR_SUCCESS) {
        DeleteNumericSubKeys(hKey);
        RegCloseKey(hKey);
    }
}
