#pragma once
#include <string>
#include <unordered_map>
#include <memory>
#include <ctime>
#include <functional>
#include <iostream>
#include "enet/include/enet.h"
#include "proton/variant.hpp"
#include "proton/rtparam.hpp"
#include "utils.h"
#include "packet.h"

/*struct AutoFarm {
	int x;
	int y;
	bool active;
};*/

struct TileHeader {
	uint16_t foreground;
	uint16_t background;
	uint16_t data;
	uint8_t flags_1;
	uint8_t flags_2;
	bool openeds = false;
	bool enableds = false;
};

struct PlayerMovings {
	int packetType;
	int netID;
	float x;
	float y;
	int characterState;
	int plantingTree;
	float XSpeed;
	float YSpeed;
	int punchX;
	int punchY;
	int SecondaryNetID;
};

struct DroppedItem {
	uint16_t itemID;
	vector2_t pos;
	uint8_t count;
	uint8_t flags;
	uint32_t uid;
};

struct Tile {
	TileHeader header;
	vector2i_t pos;
};

struct ItemData {
	TileHeader header;
	int itemID = 0;
	char editableType = 0;
	char itemCategory = 0;
	char actionType = 0;
	char hitSoundType = 0;
	const std::string& name;
	const std::string& texture;
	int textureHash = 0;
	char itemKind = 0;
	int val1;
	char itemProps2 = 0;
	char textureX = 0;
	char textureY = 0;
	char spreadType = 0;
	char isStripeyWallpaper = 0;
	char collisionType = 0;
	char aa = 0;
	char breakHits = 0;
	int dropChance = 0;
	char clothingType = 0;
	int16_t rarity = 0;
	unsigned char maxAmount = 0;
	const std::string& extraFile;
	int extraFileHash = 0;
	int audioVolume = 0;
	const std::string& petName;
	const std::string& petPrefix;
	const std::string& petSuffix;
	const std::string& petAbility;
	char seedBase = 0;
	char seedOverlay = 0;
	char treeBase = 0;
	char treeLeaves = 0;
	int seedColor = 0;
	int seedOverlayColor = 0;
	int growTime = 0;
	short val2;
	short isRayman = 0;
	bool toggleable = false;
	bool entrance = false;
	const std::string& extraOptions;
	const std::string& texture2;
	const std::string& extraOptions2;
	const std::string& punchOptions;
};

struct Item {
	uint16_t id;
	uint8_t count;
	uint8_t type;
};

struct PlayerInventory {
	uint32_t slotCount;
	uint16_t itemCount;
	std::unordered_map<uint32_t, Item> items;

	bool isItemEquipped(uint32_t itemID) {
		return doesItemExistUnsafe(itemID) ? items[itemID].type == 1 : false;
	}

	bool doesItemExist(uint32_t itemID) {
		return doesItemExistUnsafe(itemID);
	}

	bool doesItemExistUnsafe(uint32_t itemID) {
		return items.find(itemID) != items.end();
	}

	int getItemCount(uint32_t itemID) {
		return getItemCountUnsafe(itemID);
	}

	int getItemCountUnsafe(uint32_t itemID) {
		return doesItemExistUnsafe(itemID) ? items.find(itemID)->second.count : 0;
	}

	int getObjectAmountToPickUp(DroppedItem obj) {
		return getObjectAmountToPickUpUnsafe(obj);
	}

	int getObjectAmountToPickUpUnsafe(DroppedItem obj) {
		int count = getItemCountUnsafe(obj.itemID);
		return count ? (count < 200 ? (200 - count < obj.count ? 200 - count : obj.count) : 0) : (slotCount > items.size() ? obj.count : 0);
	}
};

struct LocalPlayer {
	PlayerInventory inventory;
	uint32_t gems_balance = -1;
	uint32_t wl_balance = -1;
	uint32_t level = -1;
	uint32_t awesomeness = -1;
};
