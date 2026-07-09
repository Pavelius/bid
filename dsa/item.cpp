#include "creature.h"
#include "draw_atg.h"
#include "item.h"
#include "math.h"
#include "rand.h"
#include "slice.h"
#include "stringbuilder.h"

#pragma section("items$m", read)

_declspec(allocate("items$m")) const char* str_s1 = "Test1";
_declspec(allocate("items$m")) const char* str_s2 = "Test2";

const int cp = 1;
const int bp = 5;
const int sp = 10;
const int gp = 100;

item* last_item;

static int get_count(int value) {
	switch(value) {
	case 0: return 0;
	case 3: return xrand(1, 3);
	case 6: return xrand(1, 6);
	case 18: return xrand(3, 18);
	default: return 1;
	}
}

int get_break_chance(materialn v) {
	switch(v) {
	case Stone: return 70;
	case Bone: return 50;
	case Obsidian: return 30;
	case Steel: return 5;
	default: return 100;
	}
}

itemn get_ammo(itemn v) {
	switch(v) {
	case ShortBow: case LongBow:
		return Arrow;
	case Crossbow:
		return Bolt;
	default:
		return (itemn)0;
	}
}

static materialn get_material(itemn v) {
	switch(v) {
	case Agate: case  LapisLazuli: case Amethyst: case Citrine:
	case Garnet: case Peridot: case Aquamarine: case Tourmaline: case Topaz:
	case Opal: case Tanzanite: case Spinel: case Alexandrite: case ParaibaTourmaline:
	case Sapphire: case Emerald: case Ruby: case Diamond: case PinkDiamond:
		return Stone;
	case LeatherArmor:
	case Shield:
		return Leather;
	case RawMeat:
	case Mushrooms:
		return Organic;
	default:
		return Wood;
	}
}

int get_base_cost(itemn v) {
	switch(v) {
	case LeatherArmor: return 25;
	case Shield: return 20;
	default: return 0;
	}
}

static wearn get_wear(itemn v) {
	if(v <= PinkDiamond)
		return Backpack;
	else if(v <= Bolt)
		return Ammunition;
	else if(v <= Crossbow)
		return Hands;
	else if(v <= LeatherArmor)
		return Body;
	else if(v <= Shield)
		return Offhand;
	return Backpack;
}

bool have(itemn v, damagen i) {
	switch(i) {
	case Blunt:
		switch(v) {
		case Staff:
		case Crusher:
			return true;
		default:
			return false;
		}
	case Pierce:
		switch(v) {
		case Dagger:
		case LongBow:
		case ShortBow:
		case Spear:
		case Javelin:
			return true;
		default:
			return false;
		}
	case Slashing:
		switch(v) {
		case Dagger:
		case ShortSword:
		case Sword:
		case TwohandedSword:
			return true;
		default:
			return false;
		}
	default:
		return false;
	}
}

bool is_melee(itemn v) {
	return v >= Dagger && v <= TwohandedSword;
}

bool is_range(itemn v) {
	switch(v) {
	case ShortBow: case LongBow: case Javelin:
		return true;
	default:
		return false;
	}
}

bool is_twohanded(itemn v) {
	switch(v) {
	case TwohandedSword:
	case ShortBow:
	case LongBow:
	case Staff:
		return true;
	default:
		return false;
	}
}

bool is_cursed(const void* object) {
	return ((item*)object)->cursed != 0;
}

bool is_damaged(const void* object) {
	return ((item*)object)->broken > 0;
}

bool is_identified(const void* object) {
	return ((item*)object)->identified != 0;
}

item some(itemn type, int count) {
	item v(type, xrand(imax(1, count / 2), count));
	return v;
}

bool item::deadly() const {
	switch(type) {
	case HandAxe:
		return true;
	default:
		return false;
	}
}

bool item::damage() {
	if(broken >= 3) {
		// TODO: break message
		clear();
		auto p = owner();
		if(p)
			p->update();
		return true;
	} else
		broken++;
	return false;
}

void item::join(item& v) {
	if(!count) {
		*this = v;
		v.clear();
	} else {
		if(type != v.type || properties != v.properties)
			return;
		if(count >= 255)
			return;
		int new_count = count + v.count;
		if(new_count > 255) {
			v.count = new_count - 255;
			count = 255;
		} else {
			count = (unsigned char)new_count;
			v.count = 0;
		}
	}
	last_item = this;
}

creature* item::owner() const {
	return 0;
}

const char* item::name() const {
	return getname(type);
}

void item::act(messagen id) const {
	auto push = last_item; last_item = const_cast<item*>(this);
	sb.addsep(' ');
	sb.addv(getname(id), 0);
	last_item = push;
}

void item::setmaterial() {
	material = get_material(type);
}

bool wearable::isusable(const item& it) const {
	auto slot = getwear(&it);
	switch(slot) {
	case Offhand: return !is_twohanded(wears[Hands].type);
	default: return true;
	}
}

void wearable::add(item& it) {
	for(auto& v : slice<item>(wears + Backpack, wears + LastWear + 1)) {
		if(!it)
			break;
		v.join(it);
	}
}

bool wearable::equip(const item& it) {
	auto slot = get_wear(it.type);
	if(slot > Ammunition)
		return false;
	if(wears[slot])
		return false;
	wears[slot] = it;
	last_item = wears + slot;
	return true;
}

void wearable::useammo() {
	auto& weapon = wears[Hands];
	auto pi = get_ammo(weapon.type);
	if(!pi)
		return;
	if(wears[Ammunition].type != pi)
		return;
	if(wears[Ammunition].count > 0)
		wears[Ammunition].count--;
}

bool wearable::consume(itemn v) {
	for(auto& e : wears) {
		if(e && e.type == v) {
			e.count--;
			return true;
		}
	}
	return false;
}