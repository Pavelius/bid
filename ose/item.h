#pragma once

struct creature;

enum messagen : unsigned char;

enum damagen : unsigned char {
	Blunt, Slashing, Pierce,
	Fire, Cold, Acid,
};
enum wearn : unsigned char {
	Backpack, Edible, LastWear = Backpack + 24,
	Body, Hands, Offhand, LeftFinger, RightFinger, Head, Ammunition,
};
enum groundn : unsigned char {
	CharacterOwner, AreaOwner,
};
enum itemn : unsigned char {
	Fist,
	Claws1d4, Claws1d6, Claws1d8,
	Bite1d6, Bite1d8, Bite1d12, Bite2d6, Bite2d8,
	Dagger, HandAxe, Javelin, Spear, Staff, BattleAxe, Mace,
	ShortSword, Sword, TwohandedSword,
	LongBow, ShortBow, Crossbow,
	LeatherArmor, ChainArmor, PlateArmor, Shield,
	Ration, RawMeat, Mushrooms, Herbs, Berry,
	Arrow, Bolt,
	Agate, Malachite, LapisLazuli, Amethyst, Citrine,
	Garnet, Peridot, Aquamarine, Tourmaline, Topaz,
	Opal, Tanzanite, Spinel, Alexandrite, ParaibaTourmaline,
	Sapphire, Emerald, Ruby, Diamond, PinkDiamond,
	CP, SP, EP, GP, PP,
	LastNative = Bite2d8, LastItem = PP,
	RandomMagicItem, RandomWeapon, RandomPotion,
};

int get_cost(itemn v);
int get_weight(itemn v);

itemn get_ammo(itemn v);

bool have(itemn type, damagen v);
bool is_melee(itemn v);
bool is_range(itemn v);
bool is_twohanded(itemn v);

struct item {
	itemn type;
	union {
		unsigned char count;
		struct {
			unsigned char power : 3; // 8 separate powers
			unsigned char identified : 1;
			unsigned char lost : 1; // Thrown in combat. Until end of scene item is unavailable.
			unsigned char broken : 2; // 1-2 damaged, 3 is nearly to destroy
		};
	};
	item() : type(), count(0) {}
	item(itemn type) : type(type), count(0) {}
	item(itemn type, unsigned char count) : type(type), count(count) { }
	explicit operator bool() const { return type > Fist; }
	const char* name() const;
	creature* owner() const;
	int getcount() const { return countable() ? count : 1; }
	int cost() const { return get_cost(type) * getcount(); }
	int weight() const { return get_weight(type) * getcount(); }
	void act(messagen id) const;
	bool broke();
	bool brokened() const { return broken > 0; }
	void clear() { type = (itemn)0; count = 0; }
	bool countable() const { return type >= Ration; }
	bool deadly() const;
	void drop(groundn ground, short unsigned index);
	bool is(damagen v) const { return have(type, v); }
	void join(item& it);
	bool melee() const { return is_melee(type); }
	bool missile() const { return is_range(type) && get_ammo(type); }
	bool throwing() const { return is_range(type) && !get_ammo(type); }
};
extern item* last_item;

struct itemground : item {
	groundn			ground;
	short unsigned	index;
};

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

item some(itemn type, int count = 8);