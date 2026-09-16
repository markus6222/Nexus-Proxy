#pragma once
#include <fstream>
#include <string>
#include <iostream>
#ifndef __linux__
#include <conio.h>
#endif
#include <iomanip>
#include "print.h"

void color(int y) {
	HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);

	SetConsoleTextAttribute(h, y);
}
using namespace std;
string to_lower(string s) {
	for (char& c : s)
		c = tolower(c);
	return s;
}
string to_upper(string s) {
	for (char& c : s)
		c = toupper(c);
	return s; 
}
inline string getStrLower(string txt)
{
	string ret;
	for (char c : txt) ret += tolower(c);
	return ret;
}

struct types {
	float hair, shirt, pants = 0.f; //cloth_hair & cloth_shirt & cloth_pants
	float shoe, face, hand = 0.f; //cloth_feet & cloth_face & cloth_hand
	float back, mask, neck = 0.f;//cloth_back cloth_mask cloth_necklace
	float ances = 0; //cloth_ances
	float hat = 0.f;
};

struct item {
	int mods = 0;
	string description = "No info.";
	int newField = 0;
	//from items.dat
	int itemID = 0;
	char itemProps1 = 0;
	char itemProps2 = 0;
	unsigned char itemCategory = 0;
	char hitSoundType = 0;
	string name = "";
	string texture = "";
	int textureHash = 0;
	char itemKind = 0;
	int val1;
	char textureX = 0;
	char textureY = 0;
	char spreadType = 0;
	char isStripeyWallpaper = 0;
	char collisionType = 0;
	unsigned char breakHits = 0;
	int restoreTime = 0;
	char clothingType = 0;
	int16_t rarity = 0;
	unsigned char maxAmount = 0;
	string extraFile = "";
	int extraFileHash = 0;
	int IOSextraFileHash = 0;
	int audioVolume = 0;
	string petName = "";
	string petPrefix = "";
	string petSuffix = "";
	string petAbility = "";
	char seedBase = 0;
	char seedOverlay = 0;
	char treeBase = 0;
	char treeLeaves = 0;
	int16_t ing1 = 0;
	int16_t ing2 = 0;
	int seedColor = 0;
	int seedOverlayColor = 0;
	int growTime = 0;
	short val2;
	short isRayman = 0;
	string extraOptions = "";
	string texture2 = "";
	string extraOptions2 = "";
	string punchOptions = "";
	string extraFieldUnk_4 = "";
	short value = 0;
	short value2 = 0;
	short unkValueShort1 = 0;
	short unkValueShort2 = 0;
	int newValue = 0;
	char newValue1 = 0;
	char newValue2 = 0;
	char newValue3 = 0;
	char newValue4 = 0;
	char newValue5 = 0;
	char newValue6 = 0;
	char newValue7 = 0;
	char newValue8 = 0;
	char newValue9 = 0;
	int newInt1 = 0;
	int newInt2 = 0;
};
item* items = NULL;
int itemCount = 0;
vector<item> index;
int theid = 0;

// Highest items.dat version whose complete per-item field layout this decoder models.
// Files newer than this are handled by the resync path below.
#define ITEMS_DAT_FULL_PARSE_VERSION 18

// Item ids are sequential 4-byte ints at the head of each record, so a record boundary
// can be recognised. Used to resynchronise when the field walk does not land exactly on
// the next record, which a few items in newer files trigger by carrying extra fields.
static bool looks_like_item_record(const char* data, int size, int pos, int expected_id) {
	if (pos < 0 || pos + 12 > size)
		return false;
	int32_t id = 0;
	memcpy(&id, data + pos, 4);
	if (id != expected_id)
		return false;

	int16_t name_len = 0;
	memcpy(&name_len, data + pos + 8, 2);
	if (name_len < 0 || name_len > 256)
		return false;

	const int texture_at = pos + 10 + name_len;
	if (texture_at + 2 > size)
		return false;
	int16_t texture_len = 0;
	memcpy(&texture_len, data + texture_at, 2);
	return texture_len >= 0 && texture_len <= 256;
}

static int find_next_item_record(const char* data, int size, int from, int expected_id) {
	for (int pos = from; pos + 12 <= size; pos++)
		if (looks_like_item_record(data, size, pos, expected_id))
			return pos;
	return -1;
}

void decitem() {
	color(2);
	//print::set_text("[ITEMS] Decoding Items...\n", LightGreen);
	char* pValue;
	size_t len;
	errno_t err = _dupenv_s(&pValue, &len, "LOCALAPPDATA");
	std::string path = (std::string)pValue + "\\Growtopia\\cache\\items.dat";
	std::ifstream file(path, std::ios::binary | std::ios::ate);
	int size = file.tellg();
	if (size == -1) {
		print::set_text("[ERROR] Didn't find items.dat. Is it at right place?\n", Red);
		exit(-1);
	}
	char* data = new char[size];
	file.seekg(0, std::ios::beg);
	if (file.read((char*)(data), size));
	else {
		print::set_text("[ERROR] Something gone wrong while decoding .dat file!\n", Red);
	}
	string secret = "PBG892FXX982ABC*";
	int memPos = 0;
	int itemsdatVer = 0;
	memcpy(&itemsdatVer, data + memPos, 2);
	memPos += 2;
	memcpy(&itemCount, data + memPos, 4);
	memPos += 4;
	items = new item[itemCount];
	if (itemsdatVer > ITEMS_DAT_FULL_PARSE_VERSION) {
		std::string message = "[ITEMS] items.dat version " + std::to_string(itemsdatVer)
			+ ": reading the common fields and resyncing per record.\n";
		print::set_text(message.c_str(), LightCyan);
	}
	for (int i = 0; i < itemCount; i++) {
		item item;
		const int itemStart = memPos;
		{
			memcpy(&item.itemID, data + memPos, 4);
			memPos += 4;
		}
		{
			item.itemProps1 = data[memPos];
			memPos += 1;
		}
		{
			item.itemProps2 = data[memPos];
			memPos += 1;
		}
		{
			item.itemCategory = data[memPos];
			memPos += 1;
		}
		{
			item.hitSoundType = data[memPos];
			memPos += 1;
		}
		{
			int16_t strLen = *(int16_t*)&data[memPos];
			memPos += 2;
			for (int j = 0; j < strLen; j++) {
				item.name += data[memPos] ^ (secret[(j + item.itemID) % secret.length()]);
				memPos++;
			}
		}
		{
			int16_t strLen = *(int16_t*)&data[memPos];
			memPos += 2;
			for (int j = 0; j < strLen; j++) {
				item.texture += data[memPos];
				memPos++;
			}
		}
		memcpy(&item.textureHash, data + memPos, 4);
		memPos += 4;
		item.itemKind = data[memPos];
		memPos += 1;
		memcpy(&item.val1, data + memPos, 4);
		memPos += 4;
		item.textureX = data[memPos];
		memPos += 1;
		item.textureY = data[memPos];
		memPos += 1;
		item.spreadType = data[memPos];
		memPos += 1;
		item.isStripeyWallpaper = data[memPos];
		memPos += 1;
		item.collisionType = data[memPos];
		memPos += 1;
		item.breakHits = data[memPos];
		memPos += 1;
		memcpy(&item.restoreTime, data + memPos, 4);
		memPos += 4;
		item.clothingType = data[memPos];
		memPos += 1;
		memcpy(&item.rarity, data + memPos, 2);
		memPos += 2;
		item.maxAmount = data[memPos];
		memPos += 1;
		{
			int16_t strLen = *(int16_t*)&data[memPos];
			memPos += 2;
			for (int j = 0; j < strLen; j++) {
				item.extraFile += data[memPos];
				memPos++;
			}
		}
		memcpy(&item.extraFileHash, data + memPos, 4);
		memPos += 4;
		memcpy(&item.audioVolume, data + memPos, 4);
		memPos += 4;
		{
			int16_t strLen = *(int16_t*)&data[memPos];
			memPos += 2;
			for (int j = 0; j < strLen; j++) {
				item.petName += data[memPos];
				memPos++;
			}
		}
		{
			int16_t strLen = *(int16_t*)&data[memPos];
			memPos += 2;
			for (int j = 0; j < strLen; j++) {
				item.petPrefix += data[memPos];
				memPos++;
			}
		}
		{
			int16_t strLen = *(int16_t*)&data[memPos];
			memPos += 2;
			for (int j = 0; j < strLen; j++) {
				item.petSuffix += data[memPos];
				memPos++;
			}
		}
		{
			int16_t strLen = *(int16_t*)&data[memPos];
			memPos += 2;
			for (int j = 0; j < strLen; j++) {
				item.petAbility += data[memPos];
				memPos++;
			}
		}
		{
			item.seedBase = data[memPos];
			memPos += 1;
		}
		{
			item.seedOverlay = data[memPos];
			memPos += 1;
		}
		{
			item.treeBase = data[memPos];
			memPos += 1;
		}
		{
			item.treeLeaves = data[memPos];
			memPos += 1;
		}
		{
			memcpy(&item.seedColor, data + memPos, 4);
			memPos += 4;
		}
		{
			memcpy(&item.seedOverlayColor, data + memPos, 4);
			memPos += 4;
		}
		memPos += 4; // deleted ingredients
		{
			memcpy(&item.growTime, data + memPos, 4);
			memPos += 4;
		}
		memcpy(&item.val2, data + memPos, 2);
		memPos += 2;
		memcpy(&item.isRayman, data + memPos, 2);
		memPos += 2;
		{
			int16_t strLen = *(int16_t*)&data[memPos];
			memPos += 2;
			for (int j = 0; j < strLen; j++) {
				item.extraOptions += data[memPos];
				memPos++;
			}
		}
		{
			int16_t strLen = *(int16_t*)&data[memPos];
			memPos += 2;
			for (int j = 0; j < strLen; j++) {
				item.texture2 += data[memPos];
				memPos++;
			}
		}
		{
			int16_t strLen = *(int16_t*)&data[memPos];
			memPos += 2;
			for (int j = 0; j < strLen; j++) {
				item.extraOptions2 += data[memPos];
				memPos++;
			}
		}
		//memPos += 80;
		{
			int16_t extraField4Length = 0;
			memcpy(&extraField4Length, data + memPos, 2);
			memPos += 2;
			for (int j = 0; j < extraField4Length; j++) {
				item.extraFieldUnk_4 += data[memPos];
				memPos++;
			}
			memPos += 4;
			memcpy(&item.value, data + memPos, 2);
			memPos += 2;
			memcpy(&item.value2, data + memPos, 2);
			memPos += 2;
			memcpy(&item.unkValueShort1, data + memPos, 2);
			//if (unkValueShort1 & 4) cout << itemID << ":Transmute" << endl;
			memPos += 2 + (16 - item.value);
			memcpy(&item.unkValueShort2, data + memPos, 2);
			memPos += 2;
		}
		memPos += 50;
		if (itemsdatVer >= 11) {
			{
				int16_t strLen = *(int16_t*)&data[memPos];
				memPos += 2;
				for (int j = 0; j < strLen; j++) {
					item.punchOptions += data[memPos];
					memPos++;
				}
			}
		}
		if (itemsdatVer >= 12) {
			memcpy(&item.newValue, data + memPos, 4);
			memPos += 4;
			item.newValue1 = data[memPos];
			memPos++;
			item.newValue2 = data[memPos];
			memPos++;
			item.newValue3 = data[memPos];
			memPos++;
			item.newValue4 = data[memPos];
			memPos++;
			item.newValue5 = data[memPos];
			memPos++;
			item.newValue6 = data[memPos];
			memPos++;
			item.newValue7 = data[memPos];
			memPos++;
			item.newValue8 = data[memPos];
			memPos++;
			item.newValue9 = data[memPos];
			memPos++;
		}
		if (itemsdatVer >= 13) {
			memcpy(&item.newInt1, data + memPos, 4);
			memPos += 4;
		}
		if (itemsdatVer >= 14) {
			memcpy(&item.newInt2, data + memPos, 4);
			memPos += 4;
		}
		if (itemsdatVer >= 15) memPos += *(uint16_t*)&data[memPos + 25] + 27;
		if (i != item.itemID) {
		}
		if (itemsdatVer >= 16) {
			int16_t newFieldLength = *(int16_t*)&data[memPos];
			memPos += 2;
			for (int j = 0; j < newFieldLength; j++) {
				item.newField += data[memPos];
				memPos++;
			}
		}
		if (i != item.itemID) {
			print::set_text("[ERROR] Unordered item! Something gone wrong?\n", Red);
			exit(-1);
		}
		if (itemsdatVer >= 17) {
			memPos += 4;
		}
		if (itemsdatVer >= 18) {
			memPos += 4;
		}
		// Versions 19-26 append a fixed 11-byte block, the item description as a
		// length-prefixed string, then a further 12 bytes. Verified against a v26
		// items.dat: this walks 16385 of 16386 records exactly (see test_items_dat.cpp).
		if (itemsdatVer > ITEMS_DAT_FULL_PARSE_VERSION) {
			memPos += 11;
			int16_t descLen = *(int16_t*)&data[memPos];
			memPos += 2;
			std::string description;
			for (int j = 0; j < descLen; j++) {
				description += data[memPos];
				memPos++;
			}
			if (!description.empty())
				item.description = description;	// field defaults to "No info."
			memPos += 12;
		}
		//ItemDataContainer::arr.push_back(item.name);
		//ItemDataContainer::itd.push_back(std::to_string(item.itemID));
		//ItemDataContainer::itr.push_back(std::to_string(item.rarity));
		//ItemDataContainer::itt.push_back(item.texture);
		//ItemDataContainer::itx.push_back(std::to_string(item.textureX));
		//ItemDataContainer::ity.push_back(std::to_string(item.textureY));
		//ItemDataContainer::ita.push_back(std::to_string(item.itemCategory));
		//ItemDataContainer::ith.push_back(std::to_string(item.growTime));
		index.push_back(item); // build
		items[i] = item;

		// A handful of items carry extra fields this walk does not model (an FX string
		// on some newer items, for example). Rather than desync the whole rest of the
		// file, confirm we landed on the next record and resynchronise if we did not.
		if (itemsdatVer > ITEMS_DAT_FULL_PARSE_VERSION && i + 1 < itemCount
			&& !looks_like_item_record(data, size, memPos, i + 1)) {
			int next = find_next_item_record(data, size, memPos, i + 1);
			if (next < 0)	// overshot the record; retry from just inside this one
				next = find_next_item_record(data, size, itemStart + 12, i + 1);
			if (next < 0) {
				print::set_text("[ERROR] Lost track of items.dat records while resyncing.\n", Red);
				exit(-1);
			}
			memPos = next;
		}
		theid = item.itemID;
	}
	//print::set_text("[ITEMS] Decoded Items!\n", LightGreen);
}