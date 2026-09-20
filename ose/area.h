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

#include "flagable.h"

enum actionn : unsigned char;
enum picturen : unsigned char;

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
enum kindomn : unsigned char {
	MiddleKindom, NothernKindom, SouthernKindom, WesternKindom, EasternKindom,
	ElvishLand, DwarvenMountains, FrozenNorth,
};

extern const char* area_look[LastArea + 1];
extern const char* area_names[LastArea + 1];
extern const char* area_visit[LastArea + 1];
extern const char* kindom_names[FrozenNorth + 1];

extern const char* LocationFemaleFirstName[];
extern const char* LocationMaleFirstName[];

typedef flagable<2, unsigned> actiona;
typedef flagable<1, unsigned> areana;
typedef flagable<1, unsigned> areafna;

struct areai {
	actionn			leave;
	actiona			actions;
	picturen		picture;
};
extern areai areasa[LastArea + 1];

struct arealink {
	short unsigned	from, to;
};

struct area {
	arean			type; // Area main type
	unsigned char	names[2]; // Custom name
	unsigned short	parent_id; // Parent area id
	unsigned		timestamp; // Creation time in turns (10 minutes each) from zero year.
	flag32			flags;
	constexpr explicit operator bool() const { return timestamp != 0; }
	const char* name() const { return area_names[type]; }
	const char* namefull() const;
	short unsigned index() const;
	area* parent() const;
	void clear();
	bool is(areafn v) const { return flags.is(v); }
	void set(areafn v) { flags.set(v); }
};
extern area* last_area;
extern area* next_area;

extern arean enviroment;

struct settlement {
	arean		type;
	kindomn		kindom;
	unsigned	buildings;
	areafna		flags;
	bool		is(arean v) const { return (buildings & (1 << v)) != 0; }
	bool		is(areafn v) const { return flags.is(v); }
};
extern settlement settlements[32]; // All world settlements
extern settlement* last_settlement;
extern settlement* next_settlement; // If none path is not choose

extern int move_distance;

int get_movement_modifier(arean type);

const char* get_name(arean type, int p1, int p2);

void create_area(arean id, short unsigned parent);
bool is_outdoor(unsigned char v);
bool is_settlement(unsigned char v);
void update_area_items();
