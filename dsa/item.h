#pragma once

#include "damage.h"
#include "dice.h"

struct creature;

enum messagen : unsigned char;

enum materialn : unsigned char {
	Organic,
	Wood, Stone, Bone, Obsidian, Steel,
	Leather, Hide, Chitin,
	Glass, Ceramic, Paper,
};
enum wearn : unsigned char {
	Backpack, LastWear = Backpack + 16,
	Body, Hands, Offhand, Ammunition,
};
enum itemn : unsigned char {
	Fist, Claws,
	Agate, Malachite, LapisLazuli, Amethyst, Citrine,
	Garnet, Peridot, Aquamarine, Tourmaline, Topaz,
	Opal, Tanzanite, Spinel, Alexandrite, ParaibaTourmaline,
	Sapphire, Emerald, Ruby, Diamond, PinkDiamond,
	Arrow, Bolt,
	Dagger, HandAxe, Javelin, Spear, Staff, Crusher,
	ShortSword, Sword, TwohandedSword,
	LongBow, ShortBow, Crossbow,
	LeatherArmor, Shield,
	RawMeat, Mushrooms, Herbs, Berry,
};

int get_base_cost(itemn v);
int get_break_chance(materialn v);
int get_weight(itemn v);

itemn get_ammo(itemn v);

bool have(itemn v, damagen i);

bool is_melee(itemn v);
bool is_range(itemn v);
bool is_twohanded(itemn v);

struct item {
	itemn			type;
	unsigned char	count;
	union {
		struct {
			unsigned char power : 5; // 32 separate powers
			unsigned char cursed : 1;
			unsigned char identified : 1;
			unsigned char masterwork : 1;
			//
			unsigned char lost : 1;
			unsigned char broken : 2; // 3 is nearly to destroy
			materialn material : 4; // Item material
		};
		unsigned short properties; // Properties in one word
	};
	item() : type(), count(0), properties(0) {}
	item(itemn type, unsigned char count = 1) : type(type), count(count), properties(0) { setmaterial(); }
	explicit operator bool() const { return count > 0; }
	const char* name() const;
	creature* owner() const;
	int cost() const { return get_base_cost(type) * count; }
	int slashing() const { return have(type, Slashing) ? 1 : 0; }
	int weight() const { return get_weight(type) * count; }
	void act(messagen id) const;
	bool blunt() const { return have(type, Blunt); }
	void clear() { type = (itemn)0; count = 0; properties = 0; }
	bool damage();
	bool damage(damagen type) { return damage(); }
	bool damaged() const { return broken > 0; }
	bool deadly() const;
	void join(item& it);
	bool melee() const { return is_melee(type); }
	bool missile() const { return is_range(type) && get_ammo(type); }
	bool pierce() const { return have(type, Pierce); }
	void setmaterial();
	bool throwing() const { return is_range(type) && !get_ammo(type); }
	bool weapon() const;
};
extern item* last_item;
struct wearable {
	item wears[Ammunition + 1];
	wearn getwear(const void* object) const { return iswear(object) ? wearn((item*)object - wears) : Backpack; }
	void add(item& value);
	bool add(const item& value) { item v = value; add(v); return v.operator bool(); }
	bool consume(itemn v);
	bool equip(const item& value);
	bool is(itemn type) const { for(auto& e : wears) if(e && e.type == type) return true; return false; }
	bool isusable(const item& it) const;
	bool iswear(const void* object) const { return object >= wears && object < wears + sizeof(wears) / sizeof(wears[0]); }
	void useammo();
};

bool is_cursed(const void* object);
bool is_damaged(const void* object);
bool is_identified(const void* object);

item some(itemn type, int count = 3);