#include "bsdata.h"
#include "creature.h"
#include "collectiona.h"
#include "dice.h"
#include "draw_atg.h"
#include "item.h"
#include "math.h"
#include "rand.h"
#include "slice.h"
#include "stringbuilder.h"

const int cp = 1;
const int sp = 10;
const int ep = 50;
const int gp = 100;
const int pp = 500;

BSDATAC(itemground, 4096)
collectiona items;

item* last_item;
bool need_update_items;

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
static itemn random_jewelry[] = {SignetRing, SignetRing, SignetRing, SilverBrooch, SilverBrooch, StrangeIdol};
static itemn random_armor[] = {LeatherArmor, LeatherArmor, ChainArmor, ChainArmor, ChainArmor, ChainArmor, PlateArmor, PlateArmor};
static itemn random_swords[] = {ShortSword, ShortSword, Sword, Sword, Sword, TwohandedSword};

itemi item_data[LastItem + 1] = {
	{Fist, Hands, 0, 0, FG(Melee), {{1, 2}}},
	{Claws1d4, Hands, 0, 0, FG(Slashing) | FG(Melee), {{1, 4}}},
	{Claws1d4, Hands, 0, 0, FG(Slashing) | FG(Melee), {{1, 6}}},
	{Claws1d4, Hands, 0, 0, FG(Slashing) | FG(Melee), {{1, 8}}},
	{Bite1d6, Head, 0, 0, FG(Pierce) | FG(Melee), {{1, 6}}},
	{Bite1d6, Head, 0, 0, FG(Pierce) | FG(Melee), {{1, 8}}},
	{Bite1d6, Head, 0, 0, FG(Pierce) | FG(Melee), {{1, 12}}},
	{Bite1d6, Head, 0, 0, FG(Pierce) | FG(Melee), {{2, 6}}},
	{Bite1d6, Head, 0, 0, FG(Pierce) | FG(Melee), {{2, 8}}},
	// Melee weapons
	{Dagger, Hands, 0, 0, FG(Pierce) | FG(Slashing) | FG(Melee), {{1, 4}}},
	{HandAxe, Hands, 0, 0, FG(Melee), {{1, 6}}},
	{Javelin, Hands, 0, 0, FG(Pierce) | FG(Thrown), {{1, 4}}},
	{Spear, Hands, 0, 0, FG(Pierce) | FG(Thrown) | FG(Melee), {{1, 6}}},
	{Staff, Hands, 0, 0, FG(Blunt) | FG(Slowest) | FG(Massive) | FG(Melee), {{1, 4}}},
	{BattleAxe, Hands, 7 * gp, 0, FG(Slashing) | FG(Slowest) | FG(Massive) | FG(Melee), {{1, 8}}},
	{Mace, Hands, 5 * gp, 0, FG(Blunt) | FG(Melee), {{1, 6}}},
	{ShortSword, Hands, 0, 0, FG(Slashing) | FG(Melee), {{1, 6}}},
	{Sword, Hands, 0, 0, FG(Slashing) | FG(Melee), {{1, 6}}},
	{TwohandedSword, Hands, 0, 0, FG(Slashing) | FG(Melee) | FG(Massive), {{1, 6}}},
	// Range weapons
	{LongBow, Hands, 0, 0, FG(Slashing) | FG(Range) | FG(Massive), {{1, 6}, 0, Arrow}},
	{ShortBow, Hands, 0, 0, FG(Slashing) | FG(Range) | FG(Massive), {{1, 6}, 0, Arrow}},
	{Crossbow, Hands, 0, 0, FG(Slashing) | FG(Range) | FG(Massive), {{1, 6}, 0, Bolt}},
	// Armor
	{LeatherArmor, Body, 20 * gp, 200, 0, {{}, 2}},
	{ChainArmor, Body, 40 * gp, 400, 0, {{}, 4}},
	{PlateArmor, Body, 60 * gp, 500, 0, {{}, 6}},
	{Shield, Offhand, 10 * gp, 0, 0, {{}, 1}},
	// Miscelaneous items
	{Apparatus, Backpack, 1000 * gp, 0, 0, {}},
	{Amulet, Neck, 20 * gp, 0, 0, {}},
	{Bag, Backpack, 20 * gp, 0, 0, {}},
	{Book, Backpack, 50 * gp, 0, 0, {}},
	{Boots, Legs, 3 * gp, 0, 0, {}},
	{Bracers, Elbow, 5 * gp, 0, 0, {}},
	{Brooch, Head, 2 * gp, 0, 0, {}},
	{Broom, Backpack, 2 * gp, 0, 0, {}},
	{Candle, Backpack, 2 * gp, 0, 0, {}},
	{Chime, Backpack, 2 * gp, 0, 0, {}},
	{Cloack, Backpack, 0, 0, 0, {}},
	{CrystalBall, Backpack, 0, 0, 0, {}},
	{Cube, Backpack, 0, 0, 0, {}},
	{Decanter, Backpack, 0, 0, 0, {}},
	{Drums, Backpack, 0, 0, 0, {}},
	{Dust, Backpack, 0, 0, 0, {}},
	{Eyes, Backpack, 0, 0, 0, {}},
	{Feather, Backpack, 0, 0, 0, {}},
	{Figurine, Backpack, 0, 0, 0, {}},
	{Carpet, Backpack, 0, 0, 0, {}},
	{Gauntlets, Backpack, 0, 0, 0, {}},
	{Gem, Backpack, 0, 0, 0, {}},
	{Gridle, Backpack, 0, 0, 0, {}},
	{Helm, Head, 0, 0, 0, {}},
	{Horn, Backpack, 0, 0, 0, {}},
	{Horseshoes, Backpack, 0, 0, 0, {}},
	{Incense, Backpack, 0, 0, 0, {}},
	{Fortress, Backpack, 0, 0, 0, {}},
	{Flask, Backpack, 0, 0, 0, {}},
	{Jug, Backpack, 0, 0, 0, {}},
	{Mirror, Backpack, 0, 0, 0, {}},
	{Net, Backpack, 0, 0, 0, {}},
	{Oil, Backpack, 0, 0, 0, {}},
	{Pearl, Backpack, 0, 0, 0, {}},
	{Pipe, Backpack, 0, 0, 0, {}},
	{Robe, Body, 0, 0, 0, {}},
	{Rope, Backpack, 0, 0, 0, {}},
	{Saw, Backpack, 0, 0, 0, {}},
	{Scarab, Backpack, 0, 0, 0, {}},
	{Spade, Backpack, 0, 0, 0, {}},
	// Drinkable
	{BluePotion, Backpack, 0, 0, 0, {}},
	{GreenPotion, Backpack, 0, 0, 0, {}},
	{RedPotion, Backpack, 0, 0, 0, {}},
	{Bottle, Backpack, 0, 0, 0, {}},
	// Ring
	{SilverRing, LeftFinger, 0, 0, 0, {}},
	{GoldRing, LeftFinger, 0, 0, 0, {}},
	{JeweledRing, LeftFinger, 0, 0, 0, {}},
	// Edible (Countable start here)
	{Ration, Edible, 0, 0, 0, {}},
	{RawMeat, Edible, 0, 0, 0, {}},
	{Mushrooms, Edible, 0, 0, 0, {}},
	{Herbs, Edible, 0, 0, 0, {}},
	{Berry, Edible, 0, 0, 0, {}},
	// Ammunitions
	{Arrow, Ammunition, 0, 0, 0, {}},
	{Arrow, Ammunition, 0, 0, 0, {0, 0, 1, 1}},
	{Arrow, Ammunition, 0, 0, 0, {0, 0, 2, 2}},
	{Bolt, Ammunition, 0, 0, 0, {}},
	{Bolt, Ammunition, 0, 0, 0, {0, 0, 1, 1}},
	{Bolt, Ammunition, 0, 0, 0, {0, 0, 2, 2}},
	// Gems and precious
	{Agate, Backpack, 0, 0, 0, {}},
	{Malachite, Backpack, 0, 0, 0, {}},
	{LapisLazuli, Backpack, 0, 0, 0, {}},
	{Amethyst, Backpack, 0, 0, 0, {}},
	{Citrine, Backpack, 0, 0, 0, {}},
	{Garnet, Backpack, 0, 0, 0, {}},
	{Peridot, Backpack, 0, 0, 0, {}},
	{Aquamarine, Backpack, 0, 0, 0, {}},
	{Tourmaline, Backpack, 0, 0, 0, {}},
	{Topaz, Backpack, 0, 0, 0, {}},
	{Opal, Backpack, 0, 0, 0, {}},
	{Tanzanite, Backpack, 0, 0, 0, {}},
	{Spinel, Backpack, 0, 0, 0, {}},
	{Alexandrite, Backpack, 0, 0, 0, {}},
	{ParaibaTourmaline, Backpack, 0, 0, 0, {}},
	{Sapphire, Backpack, 0, 0, 0, {}},
	{Emerald, Backpack, 0, 0, 0, {}},
	{Ruby, Backpack, 0, 0, 0, {}},
	{Diamond, Backpack, 0, 0, 0, {}},
	{PinkDiamond, Backpack, 0, 0, 0, {}},
	{SignetRing, Backpack, 0, 0, 0, {}},
	{SilverBrooch, Backpack, 0, 0, 0, {}},
	{StrangeIdol, Backpack, 0, 0, 0, {}},
	// Coins
	{CP, Backpack, cp, 0, 0, {}},
	{SP, Backpack, sp, 0, 0, {}},
	{EP, Backpack, ep, 0, 0, {}},
	{GP, Backpack, gp, 0, 0, {}},
	{PP, Backpack, pp, 0, 0, {}},
};

static int get_magic(powern v) {
	switch(v) {
	case Magic2:
		return 2;
	case Magic3:
	case Defender: case Holiness: case Corruption:
		return 3;
	case Cursed:
		return -1;
	case Delusion:
		return -2;
	default: return 1;
	}
}

itemn random(itemn v) {
	switch(v) {
	case RandomArmor: return random(maprnd(random_armor));
	case RandomExpensiveGem: return random(maprnd(random_expensive_gems));
	case RandomGem: return random(maprnd(random_gems));
	case RandomGoodGem: return random(maprnd(random_good_gems));
	case RandomJewelry: return random(maprnd(random_jewelry));
	case RandomMagicItem: return random(maprnd(random_magic_basic));
	case RandomOrnamentalGem: return random(maprnd(random_ornamental_gems));
	case RandomPreciousGem: return random(maprnd(random_precious_gems));
	case RandomSemiPreciousGem: return random(maprnd(random_semi_precious_gems));
	case RandomSword: return random(maprnd(random_swords));
	default: return v;
	}
}

bool is_cursed(const void* object) {
	auto p = (item*)object;
	auto n = p->power();
	return (n == Cursed || n == Weakness || n == Delusion);
}

bool is_damaged(const void* object) {
	return ((item*)object)->broken > 0;
}

bool is_identified(const void* object) {
	return ((item*)object)->identified != 0;
}

item some(itemn type, int count) {
	item v(type);
	if(v.countable()) {
		if(count > 1)
			v.count = xrand(imax(1, count / 2), count);
		else
			v.count = 1;
	}
	return v;
}

void clear_items() {
	items.clear();
}

void add_items(groundn ground, short unsigned index) {
	for(auto& e : bsdata<itemground>()) {
		if(!e || e.ground!=ground || e.index!=index)
			continue;
		items.add(&e);
	}
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
	const unsigned max_count = 0xFFFF;
	if(!operator bool()) {
		*this = v;
		v.clear();
	} else {
		if(type != v.type || !countable())
			return;
		if(count >= max_count)
			return;
		int new_count = count + v.count;
		if(new_count > max_count) {
			v.count = new_count - max_count;
			count = max_count;
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

const char* item::namefull() const {
	static char temp[256]; stringbuilder sb(temp); sb.clear();
	sb.add(getname(type));
	if(countable() && count > 1)
		sb.adds("x%2i", name(), count);
	return temp;
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

bool wearable::isusable(const item& it) const {
	auto slot = getwear(&it);
	switch(slot) {
	case Offhand: return !it.is(Massive);
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
	auto slot = it.wear();
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
	auto pi = weapon.ammo();
	if(!pi)
		return;
	if(wears[Ammunition].parent() != pi)
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