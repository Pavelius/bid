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
static itemn random_magic_item_no_weapon[20] = {
	RandomMisc,
	RandomPotion, RandomPotion, RandomPotion, RandomPotion, RandomPotion,
	RandomRing, RandomRodStaffWand,
	RandomScroll, RandomScroll, RandomScroll, RandomScroll
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
static itemn random_good_gems[] = {ParaibaTourmaline, PinkDiamond};
static itemn random_expensive_gems[] = {Sapphire, Emerald, Ruby, Diamond};
static itemn random_jewelry[] = {SignetRing, SignetRing, SignetRing, SilverBrooch, SilverBrooch, StrangeIdol};
static itemn random_misc[] = {Amulet, Bag, Book, Rope, Robe, Carpet, Mirror, Net, Horseshoes};
static itemn random_armor[] = {LeatherArmor, LeatherArmor, ChainArmor, ChainArmor, ChainArmor, ChainArmor, PlateArmor, PlateArmor};
static itemn random_armor_or_shield[] = {RandomArmor, RandomArmor, RandomArmor, Shield};
static itemn random_swords[] = {ShortSword, ShortSword, Sword, Sword, Sword, TwohandedSword};
static itemn random_potions[] = {Potion};
static itemn random_rings[] = {Ring};
static itemn random_scrolls[] = {ArcaneScroll, ArcaneScroll, ArcaneScroll, DivineScroll};
static itemn random_range_weapons[] = {ShortBow, ShortBow, ShortBow, LongBow, LongBow, Crossbow, Sling};
static itemn random_weapons[] = {Dagger, Dagger, HandAxe, HandAxe, Javelin, Spear, Spear, Spear, Trident, Staff, BattleAxe, WarHammer, Mace, RandomRangeWeapon, RandomRangeWeapon};
static itemn random_rod_staff_wand[] = {Rod, Staff, Staff, Staff, Wand, Wand};
static itemn random_food[] = {Bread, Bread, Corn, RawMeat, RawMeat};

itemi item_data[LastItem + 1] = {
	{MeleeWeapon, Fist, 0, 0, 0, {{1, 2}}},
	{MeleeWeapon, Claws1d4, 0, 0, 0, {{1, 4}, 0, 2}},
	{MeleeWeapon, Claws1d4, 0, 0, 0, {{1, 6}, 0, 2}},
	{MeleeWeapon, Claws1d4, 0, 0, 0, {{1, 8}, 0, 2}},
	{Head, Bite1d6, 0, 0, Pierce, {{1, 6}}},
	{Head, Bite1d6, 0, 0, Pierce, {{1, 8}}},
	{Head, Bite1d6, 0, 0, Pierce, {{1, 12}}},
	{Head, Bite1d6, 0, 0, Pierce, {{2, 6}}},
	{Head, Bite1d6, 0, 0, Pierce, {{2, 8}}},
	// Melee weapons
	{MeleeWeapon, Dagger, 0, 0, {Pierce, Slashing}, {{1, 4}}},
	{MeleeWeapon, HandAxe, 0, 0, {}, {{1, 6}}},
	{MeleeWeapon, Javelin, 0, 0, {Pierce, Thrown}, {{1, 4}}},
	{MeleeWeapon, Spear, 0, 0, {Pierce, Thrown}, {{1, 6}}},
	{MeleeWeapon, Trident, 0, 0, {Pierce, Slashing}, {{1, 6}}},
	{MeleeWeapon, Staff, 0, 0, {Blunt, Slowest, Large}, {{1, 4}}},
	{MeleeWeapon, BattleAxe, 7 * gp, 0, {Slashing, Slowest, Large}, {{1, 8}}},
	{MeleeWeapon, WarHammer, 5 * gp, 0, {Blunt, Large}, {{1, 6, 1}}},
	{MeleeWeapon, Mace, 5 * gp, 0, {Blunt}, {{1, 6}}},
	{MeleeWeapon, ShortSword, 0, 0, 0, {{1, 6}}},
	{MeleeWeapon, Sword, 0, 0, 0, {{1, 8}}},
	{MeleeWeapon, TwohandedSword, 0, 0, {Large}, {{1, 10}}},
	// Range weapons
	{RangeWeapon, LongBow, 0, 0, {Pierce, Large}, {{1, 6}, 0, 0, Arrow}},
	{RangeWeapon, ShortBow, 0, 0, {Pierce, Large}, {{1, 6}, 0, 0, Arrow}},
	{RangeWeapon, Sling, 0, 0, {Large}, {{1, 3}, 0, 0, Bolt}},
	{RangeWeapon, Crossbow, 0, 0, {Pierce, Large}, {{1, 6}, 0, 0, Bolt}},
	// Armor
	{Body, LeatherArmor, 20 * gp, 200, 0, {{}, 2}},
	{Body, ChainArmor, 40 * gp, 400, 0, {{}, 4}},
	{Body, PlateArmor, 60 * gp, 500, 0, {{}, 6}},
	{MeleeOffhand, Shield, 10 * gp, 0, 0, {{}, 1}},
	// Miscelaneous items
	{Backpack, Apparatus, 1000 * gp, 0, 0, {}},
	{Neck, Amulet, 20 * gp, 0, 0, {}},
	{Backpack, Bag, 20 * gp, 0, 0, {}},
	{Backpack, Book, 50 * gp, 0, 0, {}},
	{Legs, Boots, 3 * gp, 0, 0, {}},
	{Elbow, Bracers, 5 * gp, 0, 0, {}},
	{Head, Brooch, 2 * gp, 0, 0, {}},
	{Backpack, Broom, 2 * gp, 0, 0, {}},
	{Backpack, Candle, 2 * gp, 0, 0, {}},
	{Backpack, Chime, 2 * gp, 0, 0, {}},
	{Backpack, Cloack, 0, 0, 0, {}},
	{Backpack, CrystalBall, 0, 0, 0, {}},
	{Backpack, Cube, 0, 0, 0, {}},
	{Backpack, Decanter, 0, 0, 0, {}},
	{Backpack, Deck, 0, 0, 0, {}},
	{Backpack, Drums, 0, 0, 0, {}},
	{Backpack, Dust, 0, 0, 0, {}},
	{Backpack, Eyes, 0, 0, 0, {}},
	{Backpack, Feather, 0, 0, 0, {}},
	{Backpack, Figurine, 0, 0, 0, {}},
	{Backpack, Carpet, 0, 0, 0, {}},
	{Backpack, Gauntlets, 0, 0, 0, {}},
	{Backpack, Gridle, 0, 0, 0, {}},
	{Head, Helm, 0, 0, 0, {}},
	{Backpack, Horn, 0, 0, 0, {}},
	{Backpack, Horseshoes, 0, 0, 0, {}},
	{Backpack, Incense, 0, 0, 0, {}},
	{Backpack, Fortress, 0, 0, 0, {}},
	{Backpack, Flask, 0, 0, 0, {}},
	{Backpack, Jug, 0, 0, 0, {}},
	{Backpack, Mirror, 0, 0, 0, {}},
	{Backpack, Net, 0, 0, 0, {}},
	{Backpack, Oil, 0, 0, 0, {}},
	{Backpack, Pearl, 0, 0, 0, {}},
	{Backpack, Pipe, 0, 0, 0, {}},
	{Body, Robe, 0, 0, 0, {}},
	{Backpack, Rope, 0, 0, 0, {}},
	{Backpack, Saw, 0, 0, 0, {}},
	{Backpack, Scarab, 0, 0, 0, {}},
	{Backpack, Spade, 0, 0, 0, {}},
	{Backpack, Stone, 0, 0, 0, {}},
	// Gemstones
	{Backpack, Sapphire, 0, 0, 0, {}},
	{Backpack, Emerald, 0, 0, 0, {}},
	{Backpack, Ruby, 0, 0, 0, {}},
	{Backpack, Diamond, 0, 0, 0, {}},
	// Other items
	{Backpack, Potion, 0, 0, 0, {}},
	{Backpack, Bottle, 0, 0, 0, {}},
	{LeftFinger, Ring, 0, 0, 0, {}},
	{MeleeWeapon, Rod, 0, 0, 0, {1, 4}},
	{MeleeOffhand, Wand, 0, 0, 0, {}},
	{Backpack, ArcaneScroll},
	{Backpack, DivineScroll},
	// Edible (Countable start here)
	{Edible, Ration, 5 * sp, 0, 0, {}},
	{Edible, Bread, 3 * sp, 0, 0, {}},
	{Edible, Corn, 2 * sp, 0, 0, {}},
	{Edible, RawMeat, 2 * sp, 0, 0, {}},
	{Edible, Mushrooms, 4 * sp, 0, 0, {}},
	{Edible, Herbs, 1 * gp, 0, 0, {}},
	{Edible, Berry, 2 * sp, 0, 0, {}},
	// Ammunitions
	{Ammunition, Arrow, 1 * sp, 2, 0, {}},
	{Ammunition, Arrow, 1 * gp, 2, 0, {0, 0, 1, 1}},
	{Ammunition, Arrow, 1 * gp, 2, 0, {0, 0, 2, 2}},
	{Ammunition, Bolt, 1 * sp, 3, 0, {}},
	{Ammunition, Bolt, 1 * gp, 3, 0, {0, 0, 1, 1}},
	{Ammunition, Bolt, 1 * gp, 3, 0, {0, 0, 2, 2}},
	// Gems and precious
	{Backpack, Agate, 0, 0, 0, {}},
	{Backpack, Malachite, 0, 0, 0, {}},
	{Backpack, LapisLazuli, 0, 0, 0, {}},
	{Backpack, Amethyst, 0, 0, 0, {}},
	{Backpack, Citrine, 0, 0, 0, {}},
	{Backpack, Garnet, 0, 0, 0, {}},
	{Backpack, Peridot, 0, 0, 0, {}},
	{Backpack, Aquamarine, 0, 0, 0, {}},
	{Backpack, Tourmaline, 0, 0, 0, {}},
	{Backpack, Topaz, 0, 0, 0, {}},
	{Backpack, Opal, 0, 0, 0, {}},
	{Backpack, Tanzanite, 0, 0, 0, {}},
	{Backpack, Spinel, 0, 0, 0, {}},
	{Backpack, Alexandrite, 0, 0, 0, {}},
	{Backpack, ParaibaTourmaline, 0, 0, 0, {}},
	{Backpack, PinkDiamond, 0, 0, 0, {}},
	{Backpack, SignetRing, 0, 0, 0, {}},
	{Backpack, SilverBrooch, 0, 0, 0, {}},
	{Backpack, StrangeIdol, 0, 0, 0, {}},
	// Coins
	{Backpack, CP, 0, 0, 0, {}},
	{Backpack, SP, sp, 0, 0, {}},
	{Backpack, EP, ep, 0, 0, {}},
	{Backpack, GP, gp, 0, 0, {}},
	{Backpack, PP, pp, 0, 0, {}},
};

itemn random(itemn v) {
	switch(v) {
	case RandomGem: return random(maprnd(random_gems)); // Gems and jewelry
	case RandomOrnamentalGem: return random(maprnd(random_ornamental_gems));
	case RandomSemiPreciousGem: return random(maprnd(random_semi_precious_gems));
	case RandomPreciousGem: return random(maprnd(random_precious_gems));
	case RandomGoodGem: return random(maprnd(random_good_gems));
	case RandomExpensiveGem: return random(maprnd(random_expensive_gems));
	case RandomJewelry: return random(maprnd(random_jewelry));
	case RandomMagicItem: return random(maprnd(random_magic_basic));
	case RandomMagicItemNoWeapon: return random(maprnd(random_magic_item_no_weapon));
	case RandomArmorOrShield: return random(maprnd(random_armor_or_shield));
	case RandomArmor: return random(maprnd(random_armor));
	case RandomMisc: return random(maprnd(random_misc));
	case RandomRing: return random(maprnd(random_rings));
	case RandomRodStaffWand: return random(maprnd(random_rod_staff_wand));
	case RandomScroll: return random(maprnd(random_scrolls));
	case RandomSword: return random(maprnd(random_swords));
	case RandomWeapon: return random(maprnd(random_weapons));
	case RandomPotion: return random(maprnd(random_potions));
	case RandomRangeWeapon: return random(maprnd(random_range_weapons));
	case RandomFood: return random(maprnd(random_food));
	default: return v;
	}
}

itemn random_basic(itemn v) {
	switch(v) {
	case RandomMagicItem: return random_basic(maprnd(random_magic_basic));
	case RandomMagicItemNoWeapon: return random_basic(maprnd(random_magic_item_no_weapon));
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

void add_items(short unsigned index) {
	for(auto& e : bsdata<itemground>()) {
		if(!e || e.index!=index)
			continue;
		items.add(&e);
	}
}

item* find_item(short unsigned index) {
	for(auto& e : bsdata<itemground>()) {
		if(e && e.index == index)
			return &e;
	}
	return 0;
}

void clear_items(short unsigned index) {
	for(auto& e : bsdata<itemground>()) {
		if(e && e.index == index)
			e.clear();
	}
}

void item::consume(messagen msg_broke, messagen msg_damage) {
	if(native())
		return;
	if(countable()) {
		if(count)
			count--;
		else
			clear();
		return;
	}
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

void item::drop(short unsigned index) {
	for(auto& e : bsdata<itemground>()) {
		if(!e) {
			e.type = type;
			e.count = count;
			e.index = index;
			clear();
			last_item = &e;
			return;
		} else if(e.index!=index)
			continue;
		e.join(*this);
		if(!(*this))
			return;
	}
	auto p = bsdata<itemground>::add();
	p->type = type;
	p->count = count;
	p->index = index;
	clear();
	last_item = p;
}

bool wearable::isusable(const item& it) const {
	auto slot = getwear(&it);
	switch(slot) {
	case MeleeOffhand: return !it.is(Large);
	default: return true;
	}
}

void wearable::add(item& it) {
	for(auto& v : slice<item>(wears + Backpack, wears + LastBackpack + 1)) {
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
	auto& weapon = wears[RangeWeapon];
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

bool is_item_weapon(const void* object) {
	auto p = (item*)object;
	return p->is(MeleeWeapon) || p->is(MeleeOffhand) || p->is(RangeWeapon);
}

bool is_item_armor(const void* object) {
	auto p = (item*)object;
	return p->is(Body) || p->is(Head);
}

bool is_item_food(const void* object) {
	auto p = (item*)object;
	return p->is(Edible);
}

const char* test_item() {
	if(item_data[WarHammer].parent!=WarHammer)
		return "Not valid item for warhammer";
	if(item_data[Cloack].parent!=Cloack)
		return "Not valid item for Cloack";
	if(item_data[Gauntlets].parent!=Gauntlets)
		return "Not valid item for Gauntlets";
	if(item_data[RawMeat].parent!=RawMeat)
		return "Not valid item for RawMeat";
	if(item_data[SignetRing].parent!=SignetRing)
		return "Not valid item for SignetRing";
	if(item_data[PP].parent != PP)
		return "Not valid item for PP";
	return 0;
}
