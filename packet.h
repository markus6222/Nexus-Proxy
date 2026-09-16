#pragma once
#include <stdint.h>
#include <iostream>
#pragma pack(push, 1)

struct GameUpdatePacket {
	uint8_t type{}; //0
	union {
		uint8_t objtype{}; //1
		uint8_t punchid;
		uint8_t npctype;
	};
	union {
		uint8_t count1{}; //2
		uint8_t jump_count;
		uint8_t build_range;
		uint8_t npc_id;
		uint8_t lost_item_count;
	};
	union {
		uint8_t count2{}; //3
		uint8_t animation_type;
		uint8_t punch_range;
		uint8_t npc_action;
		uint8_t particle_id;
		uint8_t gained_item_count;
		uint8_t dice_result;
		uint8_t fruit_count;
	};
	union {
		int32_t netid{}; //4
		int32_t effect_flags_check;
		int32_t object_change_type;
		int32_t particle_emitter_id;
	};
	union {
		int32_t item{}; //8
		int32_t ping_hash;
		int32_t item_netid;
		int32_t pupil_color;
		int32_t tiles_length;
	};
	int32_t flags{}; //12 - 0xC
	union {
		float float_var{}; //16 - 0x10
		float water_speed;
		float obj_alt_count;
	};
	union {
		int32_t int_data{}; //20 - 0x14
		int32_t ping_item;
		int32_t elapsed_ms;
		int32_t delay;
		int32_t tile_damage;
		int32_t item_id;
		int32_t item_speed;
		int32_t effect_flags;
		int32_t object_id;
		int32_t hash;
		int32_t verify_pos;
		int32_t client_hack_type;
	};
	union {
		float vec_x{}; //24 - 0x18
		float pos_x;
		float accel;
		float punch_range_in;
	};
	union {
		float vec_y{}; //28 - 0x1C
		float pos_y;
		float build_range_in;
		float punch_strength;
	};
	union {
		float vec2_x{}; //32 - 0x20
		float dest_x;
		float gravity_in;
		float speed_out;
		float velocity_x;
		float particle_variable;
		float pos2_x;
		int hack_type;
	};
	union {
		float vec2_y{}; //36 - 0x24
		float dest_y;
		float speed_in;
		float gravity_out;
		float velocity_y;
		float particle_alt_id;
		float pos2_y;
		int hack_type2;
	};
	union {
		float particle_rotation{}; //40 - 0x28
		float npc_variable;
	};
	union {
		uint32_t int_x{}; //44 - 0x2C
		uint32_t item_id_alt;
		uint32_t eye_shade_color;
	};
	union {
		uint32_t int_y{}; //48 - 0x30
		uint32_t item_count;
		uint32_t eyecolor;
		uint32_t npc_speed;
		uint32_t particle_size_alt;
	};
	uint32_t data_size;
	uint32_t data; //not officially a part of gameupdatepacket, thus sizeof(GameUpdatePacket) is 60 and not 56
	void debug_print(const char* name) {
		printf("---- start debug print ----\n");
		printf("\tplayer: %s\n", name);
		printf("type: %d\tobjtype: %d\tcount1: %d\tcount2: %d\n", type, objtype, count1, count2);
		printf("netid: %d\titem: %d\tflags: %d\tfloat1: %0.2f\n", netid, item, flags, float_var);
		printf("idata: %d\tvecx: %0.2f\tvecy: %0.2f\n", int_data, vec_x, vec_y);
		printf("vec2x: %0.2f\tvec2y: %0.2f\tpart: %0.2f\n", vec2_x, vec2_y, particle_rotation);
		printf("int_x: %d\tint_y: %d\tdata_size: %d\n", int_x, int_y, data_size);
		if (fabsf(0.f - float_var) < 0.01f && float_var != 0.f)
			printf("float var (small): %f (%d)\n", float_var, *(int*)&float_var);
		if (fabsf(0.f - vec_x) < 0.01f && vec_x != 0.f)
			printf("vecx (small): %f (%d)\n", vec_x, *(int*)&vec_x);
		if (fabsf(0.f - vec_y) < 0.01f && vec_y != 0.f)
			printf("vecy (small): %f (%d)\n", vec_y, *(int*)&vec_y);
		if (fabsf(0.f - vec2_x) < 0.01f && vec2_x != 0.f)
			printf("vec2x (small): %f (%d)\n", vec2_x, *(int*)&vec2_x);
		if (fabsf(0.f - vec2_y) < 0.01f && vec2_y != 0.f)
			printf("vec2y (small): %f (%d)\n", vec2_y, *(int*)&vec2_y);
		if (fabsf(0.f - particle_rotation) < 0.01f && particle_rotation != 0.f)
			printf("part (small): %f (%d)\n", particle_rotation, *(int*)&particle_rotation);
		printf("---- end debug print ----\n");
	}
};


struct gameupdatepacket_t {
	uint8_t m_type;
	uint8_t m_netid;
	uint8_t m_jump_amount;
	uint8_t m_count;
	int32_t m_player_flags;
	int32_t m_item;
	int32_t m_packet_flags;
	float m_struct_flags;
	int32_t m_int_data;
	float m_vec_x;
	float m_vec_y;
	float m_vec2_x;
	float m_vec2_y;
	float m_particle_time;
	uint32_t m_state1;
	uint32_t m_state2;
	uint32_t m_data_size;
	uint32_t m_data;
};
typedef struct gametankpacket_t {
	int32_t m_type;
	char m_data;
} gametextpacket_t;
#pragma pack(pop)
enum {
	PACKET_STATE = 0,
	PACKET_CALL_FUNCTION = 1,
	PACKET_UPDATE_STATUS = 2,
	PACKET_TILE_CHANGE_REQUEST = 3,
	PACKET_SEND_MAP_DATA = 4,
	PACKET_SEND_TILE_UPDATE_DATA = 5,
	PACKET_SEND_TILE_UPDATE_DATA_MULTIPLE = 6,
	PACKET_TILE_ACTIVATE_REQUEST = 7,
	PACKET_TILE_APPLY_DAMAGE = 8,
	PACKET_SEND_INVENTORY_STATE = 9,
	PACKET_ITEM_ACTIVATE_REQUEST = 10,
	PACKET_ITEM_ACTIVATE_OBJECT_REQUEST = 11,
	PACKET_SEND_TILE_TREE_STATE = 12,
	PACKET_MODIFY_ITEM_INVENTORY = 13,
	PACKET_ITEM_CHANGE_OBJECT = 14,
	PACKET_SEND_LOCK = 15,
	PACKET_SEND_ITEM_DATABASE_DATA = 16,
	PACKET_SEND_PARTICLE_EFFECT = 17,
	PACKET_SET_ICON_STATE = 18,
	PACKET_ITEM_EFFECT = 19,
	PACKET_SET_CHARACTER_STATE = 20,
	PACKET_PING_REPLY = 21,
	PACKET_PING_REQUEST = 22,
	PACKET_GOT_PUNCHED = 23,
	PACKET_APP_CHECK_RESPONSE = 24,
	PACKET_APP_INTEGRITY_FAIL = 25,
	PACKET_DISCONNECT = 26,
	PACKET_BATTLE_JOIN = 27,
	PACKET_BATTLE_EVEN = 28,
	PACKET_USE_DOOR = 29,
	PACKET_SEND_PARENTAL = 30,
	PACKET_GONE_FISHIN = 31,
	PACKET_STEAM = 32,
	PACKET_PET_BATTLE = 33,
	PACKET_NPC = 34,
	PACKET_SPECIAL = 35,
	PACKET_SEND_PARTICLE_EFFECT_V2 = 36,
	GAME_ACTIVE_ARROW_TO_ITEM = 37,
	GAME_SELECT_TILE_INDEX = 38
};
enum {
	NET_MESSAGE_UNKNOWN = 0,
	NET_MESSAGE_SERVER_HELLO = 1,
	NET_MESSAGE_GENERIC_TEXT = 2,
	NET_MESSAGE_GAME_MESSAGE = 3,
	NET_MESSAGE_GAME_PACKET = 4,
	NET_MESSAGE_ERROR = 5,
	NET_MESSAGE_TRACK = 6,
	NET_MESSAGE_CLIENT_LOG_REQUEST = 7,
	NET_MESSAGE_CLIENT_LOG_RESPONSE = 8
};

enum {
	UPDATE_PACKET_FALLING_DOWN = 0,
	UPDATE_PACKET_TILE_CHANGE_REQUEST_RIGHT = 0,
	UPDATE_PACKET_TILE_CHANGE_REQUEST_LEFT = 16,
	UPDATE_PACKET_PLAYER_MOVING_RIGHT = 32, //higher tile pos
	UPDATE_PACKET_PLAYER_MOVING_LEFT = 48,  //lower tile pos
	UPDATE_PACKET_PLAYER_JUMPING_RIGHT = 128,  //higher tile pos
	UPDATE_PACKET_PLAYER_JUMPING_LEFT = 144,  //lower tile pos
	UPDATE_PACKET_PLACE_BLOCK_RIGHT = 3104,
	UPDATE_PACKET_PLACE_BLOCK_LEFT = 3120,
	UPDATE_PACKET_PUNCH_TILE_RIGHT = 2592,
	UPDATE_PACKET_PUNCH_TILE_LEFT = 2608,
	UPDATE_PACKET_PICK_UP_OBJECT_RIGHT = 16416,
	UPDATE_PACKET_PICK_UP_OBJECT_LEFT = 16432,
};