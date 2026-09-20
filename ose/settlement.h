#pragma once

enum arean : unsigned char;
enum areafn : unsigned char;

const int settlement_maximum = 32;

enum kindomn : unsigned char {
	MiddleKindom, NothernKindom, SouthernKindom, WesternKindom, EasternKindom,
	ElvishLand, DwarvenMountains, FrozenNorth,
};

extern const char* kindom_names[FrozenNorth + 1];
extern const char* settlement_names[(FrozenNorth + 1) * 8];

struct settlement {
	arean		type;
	kindomn		kindom;
	unsigned	buildings;
	unsigned	flags;
	unsigned char name_id;
	constexpr explicit operator bool() const { return type != (arean)0; }
	const char* name() const { return settlement_names[name_id]; }
	bool is(arean v) const { return (buildings & (1 << v)) != 0; }
	bool is(areafn v) const { return (flags & (1 << v)) != 0; }
	void set(arean v) { buildings |= (1 << v); }
	void set(areafn v) { flags |= (1 << v); }
};
extern settlement settlements[settlement_maximum]; // All world settlements
extern settlement* last_settlement;
extern settlement* next_settlement; // If none path is not choose

settlement* find_settlement(kindomn kindom, arean type);
settlement* find_settlement(kindomn kindom, arean type, settlement* start);

void adventure_move(int miles);
void kindom_adventure_move();
void settlement_move();
