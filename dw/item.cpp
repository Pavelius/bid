#include "bsdata.h"
#include "item.h"
#include "stringbuilder.h"
#include "stringlocale.h"

BSDATA(item) = {
	{},
	{RaggedBow, {Near, Weight2}, 15},
	{FineBow, {Near, Far, Weight2}, 60},
	{HuntersBow, {Near, Far, Weight1}, 100},
	{Crossbow, {Near, DamageP1, Reloaded, Weight1, Weight2}, 35},
	{Arrows, {Ammo1, Ammo2, Weight1}, 1},
	{ElvenArrows, {Ammo4, Weight1}, 20},
	{Club, {Close, Weight2}, 1},
};
assert_enum(bsdata<item>::elements, Club)

const char* item::name() const {
	return getname(type);
}

itemn item::ammunition() const {
	switch(type) {
	case RaggedBow: case FineBow: case HuntersBow: return Arrows;
	default: return NoItem;
	}
}

itemn item::basic() const {
	switch(type) {
	case ElvenArrows: return Arrows;
	default: return type;
	}
}

bool wearable::is(itemfn range) const {
	for(auto& e : wears) {
		if(e && e.is(range))
			return true;
	}
	return false;
}

bool wearable::is(itemn type) const {
	for(auto& e : wears) {
		if(e && e.basic() == type)
			return true;
	}
	return false;
}

bool wearable::is(fnvisible proc, bool keep) const {
	for(auto& e : wears) {
		if(proc(&e) == keep)
			return true;
	}
	return false;
}