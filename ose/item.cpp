#include "bsdata.h"
#include "creature.h"
#include "dice.h"
#include "draw_atg.h"
#include "item.h"
#include "math.h"
#include "rand.h"
#include "slice.h"
#include "stringbuilder.h"

const int cp = 1;
const int bp = 5;
const int sp = 10;
const int gp = 100;

const int mp = 16;

BSDATAC(itemground, 4096)

item* last_item;

static itemn random_magic_basic[20] = {
	RandomArmorOrShield, RandomArmorOrShield, RandomMisc,
	RandomPotion, RandomPotion, RandomPotion, RandomPotion, RandomPotion,
	RandomRing, RandomRodStaffWand,
	RandomScroll, RandomScroll, RandomScroll, RandomScroll,
	RandomSword, RandomSword, RandomSword, RandomSword,
	RandomWeapon, RandomWeapon,
};
static itemn random_gems[20] = {
	RandomOrnamentalGem, RandomOrnamentalGem, RandomOrnamentalGem, RandomOrnamentalGem,
	RandomSemiPreciousGem, RandomSemiPreciousGem, RandomSemiPreciousGem, RandomSemiPreciousGem, RandomSemiPreciousGem,
	RandomPreciousGem, RandomPreciousGem, RandomPreciousGem, RandomPreciousGem, RandomPreciousGem, RandomPreciousGem,
	RandomGoodGem, RandomGoodGem, RandomGoodGem, RandomGoodGem,
	RandomExpensiveGem
};
static itemn random_ornamental_gems[] = {Agate, Malachite, LapisLazuli, Amethyst, Citrine};
static itemn random_semi_precious_gems[] = {Garnet, Peridot, Aquamarine, Tourmaline, Topaz};
static itemn random_precious_gems[] = {Opal, Tanzanite, Spinel, Alexandrite};
static itemn random_good_gems[] = {ParaibaTourmaline, Sapphire, Emerald};
static itemn random_expensive_gems[] = {Ruby, Diamond, PinkDiamond};
static itemn random_jewelry[] = {RingSignet, RingSignet, RingSignet, SilverBrooch, SilverBrooch, StrangeIdol};
static itemn random_armor[] = {LeatherArmor, LeatherArmor, ChainArmor, ChainArmor, ChainArmor, ChainArmor, PlateArmor, PlateArmor};

static powern power_armor[mp] = {NoPower, Magic1, Magic2, Magic3, Cursed, Delusion};

itemi item_data[LastItem+1] = {
	{Fist, Hands, 0, 0, FG(Melee), {{1, 2}}},
	{Claws1d4, Hands, 0, 0, FG(Slashing)|FG(Melee), {{1, 4}}},
	{Claws1d4, Hands, 0, 0, FG(Slashing)|FG(Melee), {{1, 6}}},
	{Claws1d4, Hands, 0, 0, FG(Slashing)|FG(Melee), {{1, 8}}},
	{Bite1d6, Head, 0, 0, FG(Pierce)|FG(Melee), {{1, 6}}},
	{Bite1d6, Head, 0, 0, FG(Pierce)|FG(Melee), {{1, 8}}},
	{Bite1d6, Head, 0, 0, FG(Pierce)|FG(Melee), {{1, 12}}},
	{Bite1d6, Head, 0, 0, FG(Pierce)|FG(Melee), {{2, 6}}},
	{Bite1d6, Head, 0, 0, FG(Pierce)|FG(Melee), {{2, 8}}},
	{Dagger, Hands, 0, 0, FG(Pierce)|FG(Slashing)|FG(Melee), {{1, 4}}},
	{HandAxe, Hands, 0, 0, FG(Melee), {{1, 6}}},
	{Javelin, Hands, 0, 0, FG(Pierce)|FG(Thrown), {{1, 4}}},
	{Spear, Hands, 0, 0, FG(Pierce)|FG(Thrown)|FG(Melee), {{1, 6}}},
	{Staff, Hands, 0, 0, FG(Blunt)|FG(Slowest)|FG(Massive)|FG(Melee), {{1, 4}}},
	{BattleAxe, Hands, 7*gp, 0, FG(Slashing)|FG(Slowest)|FG(Massive)|FG(Melee), {{1, 8}}},
	{Mace, Hands, 5*gp, 0, FG(Blunt)|FG(Melee), {{1, 6}}},
	{ShortSword, Hands, 0, 0, FG(Slashing)|FG(Melee), {{1, 6}}},
	{Sword, Hands, 0, 0, FG(Slashing)|FG(Melee), {{1, 6}}},
	{TwohandedSword, Hands, 0, 0, FG(Slashing)|FG(Melee), {{1, 6}}},
	{LongBow, Hands, 0, 0, FG(Slashing)|FG(Range), {{1, 6}}},
	{ShortBow, Hands, 0, 0, FG(Slashing)|FG(Range), {{1, 6}}},
	{Crossbow, Hands, 0, 0, FG(Slashing)|FG(Range), {{1, 6}}},
	{LeatherArmor, Body, 20*gp, 200, 0, {{}, 2}},
	{ChainArmor, Body, 40*gp, 400, 0, {{}, 4}},
	{PlateArmor, Body, 60*gp, 500, 0, {{}, 6}},
	{Shield, Offhand, 10*gp, 0, 0, {{}, 1}},
	{Apparatus, Backpack, 1000*gp, 0, 0, {}},
	{Amulet, Neck, 20*gp, 0, 0, {}},
	{Bag, Backpack, 20*gp, 0, 0, {}},
	{Book, Backpack, 50*gp, 0, 0, {}},
	{Boots, Legs, 3*gp, 0, 0, {}},
	{Bracers, Elbow, 5*gp, 0, 0, {}},
	{Brooch, Head, 2*gp, 0, 0, {}},
	{Broom, Backpack, 2*gp, 0, 0, {}},
	{Candle, Backpack, 2*gp, 0, 0, {}},
	{Chime, Backpack, 2*gp, 0, 0, {}},
};

static int get_magic(powern v) {
	switch(v) {
	case Magic1: return 1;
	case Magic2: return 2;
	case Magic3: return 3;
	case Cursed: return -1;
	case Delusion: return -2;
	default: return 0;
	}
}

itemn random(itemn v) {
	switch(v) {
	case RandomGem: return random(maprnd(random_gems));
	case RandomOrnamentalGem: return random(maprnd(random_ornamental_gems));
	case RandomSemiPreciousGem: return random(maprnd(random_semi_precious_gems));
	case RandomPreciousGem: return random(maprnd(random_precious_gems));
	case RandomGoodGem: return random(maprnd(random_good_gems));
	case RandomExpensiveGem: return random(maprnd(random_expensive_gems));
	case RandomJewelry: return random(maprnd(random_jewelry));
	case RandomMagicItem: return random(maprnd(random_magic_basic));
	case RandomArmor: return random(maprnd(random_armor));
	default: return v;
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

static powern* get_power(itemn type) {
	switch(type) {
	case LeatherArmor: case ChainArmor: case PlateArmor: return power_armor;
	case Shield: return power_armor;
	default: return 0;
	}
}

static wearn get_wear(itemn v) {
	if(v >= Agate)
		return Backpack;
	else if(v >= Arrow)
		return Ammunition;
	else if(v >= Ration)
		return Edible;
	else if(v >= Shield)
		return Offhand;
	else if(v >= LeatherArmor)
		return Body;
	return Hands;
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
	return false;
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

void item::consume(messagen msg_broke, messagen msg_damage) {
	if(native())
		return;
	if(broken >= 3) {
		if(msg_broke)
			act(msg_broke);
		clear();
		auto p = owner();
		if(p)
			p->update();
	} else {
		if(msg_damage)
			act(msg_damage);
		broken++;
	}
}

void item::join(item& v) {
	if(!operator bool()) {
		*this = v;
		v.clear();
	} else {
		if(type != v.type || !countable())
			return;
		if(count >= 255)
			return;
		int new_count = count + v.count;
		if(new_count > 255) {
			v.count = new_count - 255;
			count = 255;
		} else {
			count = (unsigned char)new_count;
			v.clear();
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

void item::drop(groundn ground, short unsigned index) {
	for(auto& e : bsdata<itemground>()) {
		if(e.ground != ground || e.index != index)
			continue;
		e.join(*this);
		if(!(*this))
			return;
	}
	auto p = bsdata<itemground>::add();
	p->type = type;
	p->count = count;
	p->ground = ground;
	p->index = index;
	clear();
	last_item = p;
}

powern item::power() const {
	auto pi = get_power(type);
	if(pi)
		return pi[modification];
	return NoPower;
}

void item::set(powern v) {
	auto p = get_power(type);
	if(!p)
		return;
	modification = 0;
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