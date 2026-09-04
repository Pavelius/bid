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

enum areafn : unsigned char {
	Dusked, Darkened, Fired, Icy,
	Locked, Looted, Opened, Hidden,
};
enum arean : unsigned char {
	NoArea,
	Plains, Sands, Wastes, Swamps, Hills, Mountains, Jungle, Forest,
	Hamlet, Village, SmallTown, LargeTown,
	Cave, Dungeon, Ruins,
	Market, Garden, Temple, Inn, Tavern, Palace,
	LastArea = Palace,
};
extern const char* area_look[Palace+1];
extern const char* area_visit[Palace+1];

struct arealink {
	short unsigned	from, to;
};

struct area {
	arean			type; // Area main type
	unsigned char	param; // Area subtype
	unsigned short	parent_id; // Parent area id
	unsigned		timestamp; // Creation time in turns (10 minutes each) from zero year.
	flag32			flags;
	constexpr explicit operator bool() const { return type!=NoArea; }
	const char* name() const;
	short unsigned index() const;
	area* parent() const;
	void clear();
	bool is(areafn v) const { return flags.is(v); }
	bool outdoor() const;
	void set(areafn v) { flags.set(v); }
};
extern area* last_area;
extern area* next_area;

extern int move_distance;

int get_movement_modifier(arean type);

void create_area(arean id, short unsigned parent);
void update_area_items();
