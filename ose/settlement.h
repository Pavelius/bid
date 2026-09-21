#pragma once

enum arean : unsigned char;
enum areafn : unsigned char;

const int settlement_maximum = 32;

enum kindomn : unsigned char {
	MiddleKindom, NothernKindom, SouthernKindom, WesternKindom, EasternKindom,
	ElvishLand, DwarvenMountains, FrozenNorth,
};
enum directionn : unsigned char {
	Center, North, East, South, West,
};

extern const char* area_names[];
extern const char* direction_names[West + 1];
extern const char* kindom_names[FrozenNorth + 1];
extern const char* settlement_names[(FrozenNorth + 1) * 8];

struct areafc {
	unsigned	flags;
	bool is(areafn v) const { return (flags & (1 << v)) != 0; }
	void set(areafn v) { flags |= (1 << v); }
};

struct buildingc {
	unsigned	buildings;
	bool is(arean v) const { return (buildings & (1 << v)) != 0; }
	void set(arean v) { buildings |= (1 << v); }
};

struct settlement : areafc, buildingc {
	arean		type, landscape;
	kindomn		kindom;
	directionn	side;
	unsigned char name_id;
	constexpr explicit operator bool() const { return type != (arean)0; }
	const char* name() const { return settlement_names[name_id]; }
	int index() const;
	bool is(arean v) const { return buildingc::is(v); }
	bool is(areafn v) const { return areafc::is(v); }
	void set(arean v) { buildingc::set(v); }
	void set(areafn v) { areafc::set(v); }
};
extern settlement settlements[settlement_maximum]; // All world settlements
extern settlement* last_settlement;
extern settlement* next_settlement; // If none path is not choose

struct kindomi {
	unsigned char capital_id;
	settlement* capital() const { return settlements + capital_id; }
};
extern kindomi kindoms[FrozenNorth + 1];

struct sitei : areafc {
	arean			type;
	unsigned char	settlement_id; // Site located near this settlement
	unsigned char	miles; // Distance from settlement to site. Usually 0-40 miles. 0 - site in settlement.
	unsigned char	name_part[2]; // Use for unique naming.
	constexpr explicit operator bool() const { return settlement_id != 0xFF; }
	settlement*	parent() const { return settlements + settlement_id; }
	const char* name() const { return area_names[type]; }
	const char* namefull() const;
	void clear();
};
extern sitei sites[128];
extern sitei* last_site;

struct stagei : areafc {
	unsigned char	site_id, level;
	sitei* parent() const { return sites + site_id; }
	void clear();
};
extern stagei stages[4096];

settlement* find_settlement(kindomn kindom, arean type);
settlement* find_settlement(kindomn kindom, arean type, settlement* start);

sitei* find_site(settlement* target, arean type);

void create_site(arean type);

void adventure_move(int miles);
void settlement_move();
