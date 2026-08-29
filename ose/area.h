#pragma once

#include "flagable.h"

enum areafn : unsigned char {
	Dusked, Darkened, Fired, Icy,
	Locked, Looted, Opened, Hidden,
};
enum arean : unsigned char {
	NoArea,
	Plains, Sands, Wastes, Hills, Mountains, Forest,
	Cave, Dungeon, Ruins,
	Garden, Temple, Inn, Tavern, Palace,
	Hamlet, Village, SmallTown, LargeTown,
};

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
	area* parent() const;
	void clear();
	bool is(areafn v) const { return flags.is(v); }
	bool outdoor() const;
	void set(areafn v) { flags.set(v); }
};
extern area* last_area;
extern area* next_area;

extern int move_distance;

void create_area(arean id);