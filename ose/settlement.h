/*
	Copyright 2026 by Pavel Chistyakov

	Licensed under the Apache License, Version 2.0 (the "License");
	you may not use this file except in compliance with the License.
	You may obtain a copy of the License at

	http://www.apache.org/licenses/LICENSE-2.0

	Unless required by applicable law or agreed to in writing, software
	distributed under the License is distributed on an "AS IS" BASIS,
	WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
	See the License for the specific language governing permissions and
	limitations under the License.77
*/

#pragma once

const int settlement_maximum = 32;

enum kindomn : unsigned char {
	MiddleKindom, NothernKindom, SouthernKindom, WesternKindom, EasternKindom,
	ElvishLand, DwarvenMountains, FrozenNorth,
};
enum directionn : unsigned char {
	Center, North, East, South, West,
};
enum areafn : unsigned char {
	Visited, Known,
	Dusked, Darkened, Fired, Icy,
	Locked, Looted, Opened, Hidden,
	Far, Near,
};
enum arean : unsigned char {
	Plains, Sands, Wastes, Swamps, Hills, Mountains, Jungle, Forest,
	Hamlet, Village, SmallTown, LargeTown,
	Cave, Dungeon, Ruins,
	Market, Garden, Temple, Inn, Tavern, Palace,
	LastArea = Palace,
};

extern const char* area_look[LastArea + 1];
extern const char* area_visit[LastArea + 1];
extern const char* area_names[LastArea + 1];
extern const char* area_names_of[LastArea + 1];
extern const char* area_names_to[LastArea + 1];
extern const char* direction_names[West + 1];
extern const char* kindom_names[FrozenNorth + 1];
extern const char* settlement_names[(FrozenNorth + 1) * 8];

extern const char* LocationFemaleFirstName[];
extern const char* LocationMaleFirstName[];
extern const char* DayDistance[9];

extern arean area;

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

const char* get_name(arean type, int p1, int p2);
const char* get_range_name(int miles);

void create_site(arean type);
void adventure_move(int miles);
bool is_outdoor(unsigned char v);
bool is_settlement(unsigned char v);
void settlement_move();
void update_area_items();

