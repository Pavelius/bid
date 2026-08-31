#pragma once

struct creature;

enum messagen : unsigned char;

enum damagen : unsigned char {
	Blunt, Slashing, Pierce,
	Fire, Cold, Acid,
	Melee, Range, Thrown, Deadly, Massive, Slowest,
};
enum wearn : unsigned char {
	Backpack, Edible, LastWear = Backpack + 24,
	Body, Head, Neck, Legs, Elbow, Hands, Offhand, LeftFinger, RightFinger, Ammunition,
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
	Apparatus, Amulet, Bag, Book, Boots, Bracers, Brooch, Broom, Candle, Chime,
	Cloack,	CrystalBall, Cube, Decanter, Drums, Dust, Eyes, Feather, Figurine, Carpet,
	Boat, Gauntlets, Gem, Gridle, Helm, Horn, Horseshoes, Incense, Fortress, Flask,
	Jug, Mirror, Net, Oil, Pearl, Pipe, Robe, Rope, Saw, Scarab, Spade,
	Ration, RawMeat, Mushrooms, Herbs, Berry,
	Arrow, Bolt,
	Agate, Malachite, LapisLazuli, Amethyst, Citrine,
	Garnet, Peridot, Aquamarine, Tourmaline, Topaz,
	Opal, Tanzanite, Spinel, Alexandrite, ParaibaTourmaline,
	Sapphire, Emerald, Ruby, Diamond, PinkDiamond,
	RingSignet, SilverBrooch, StrangeIdol,
	CP, SP, EP, GP, PP,
	FirstCountable = Ration, LastNative = Bite2d8, LastItem = PP,
	RandomGem, RandomOrnamentalGem, RandomSemiPreciousGem, RandomPreciousGem, RandomGoodGem, RandomExpensiveGem,
	RandomJewelry,
	RandomMagicItem, RandomMagicItemNoWeapon,
	RandomArmorOrShield, RandomArmor, RandomMisc, RandomRing, RandomRodStaffWand, RandomScroll, RandomSword, RandomWeapon, RandomPotion,
};
enum powern : unsigned char {
	NoPower,
	Magic1, Magic2, Magic3, Cursed, Delusion, Weakness,
	ControlAnimals, ControlHumans, ControlPlants,
	DjinniSummoning, FireResistance, Invisibility,
	Telekinesis, WaterWalking,
	Regeneration, SpellStoring, SpellTurning,
	Cancelation, Commanding, Healing, Power, Snakes, Striking,
	Thunderbolt, Lighting,
	Wishes, WishesII, WishesIII,
};

itemn get_ammo(itemn v);

bool is_melee(itemn v);
bool is_range(itemn v);
bool is_twohanded(itemn v);

struct magici {
	char		chance;
	itemn		type;
	powern		power;
};

struct itemi {
	struct combati {
		char	damage[3];
		char	ac;
	};
	itemn		parent;
	wearn		wear;
	int			cost, weight;
	unsigned	flags; // damagen
	combati		combat; // Only weapon fill this
	magici*		powers;
	bool is(damagen v) const { return (flags & (1<<v))!=0; }
};
extern itemi item_data[LastItem+1];

struct item {
	itemn type;
	union {
		unsigned char count;
		struct {
			unsigned char modification : 4; // 16 separate powers
			unsigned char identified : 1;
			unsigned char lost : 1; // Thrown in combat. Until end of scene item is unavailable.
			unsigned char broken : 2; // 1-2 damaged, 3 is nearly to destroy
		};
	};
	item() : type(), count(0) {}
	item(itemn type) : type(type), count(0) {}
	item(itemn type, unsigned char count) : type(type), count(count) {}
	explicit operator bool() const { return type != (itemn)0; }
	const char* name() const;
	creature* owner() const;
	powern power() const;
	int getcount() const { return countable() ? count : 1; }
	int cost() const { return item_data[type].cost * getcount(); }
	int weight() const { return item_data[type].weight * getcount(); }
	void act(messagen id) const;
	void clear() { type = (itemn)0; count = 0; }
	void consume(messagen crush = (messagen)0, messagen damaged = (messagen)0);
	bool countable() const { return type >= FirstCountable; }
	bool damaged() const { return broken > 0; }
	void drop(groundn ground, short unsigned index);
	bool is(damagen v) const { return item_data[type].is(v); }
	void join(item& it);
	bool native() const { return type <= LastNative; }
	void set(powern v);
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

extern int treasure_coins[PP - CP + 1];

itemn random(itemn v);

bool is_cursed(const void* object);
bool is_damaged(const void* object);
bool is_identified(const void* object);

item some(itemn type, int count = 8);

void add_magic_item(itemn type);
void treasure_generate(const char* type, bool use_lair, bool use_group, bool use_individual);
