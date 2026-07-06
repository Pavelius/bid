#pragma once

#include "flagable.h"
#include "slice.h"

enum wearn : unsigned char {
	Hands, Body,
	Backpack, WearLast = Backpack + 15,
};
enum itemn : unsigned char {
	NoItem,
	RaggedBow, FineBow, HuntersBow, Crossbow,
	Arrows, ElvenArrows,
	Club, Staff,
	Knife, ThrowingDagger,
	SwordShort, Mace, Warhammer, Spear, SwordLong, Halberd, Rapier, DuelingRapier,
	FineClothing, LeatherArmour, ChainMail, ScaleMail, PlateMail, Shield,
	AdventuringGear, Bandages, Herbs, HealingPotion, KegOfDwarvenStout,
	BagOfBooks, Antitoxin,
	DungeonRation, PersonalFeats, DwarvenHadrdtack, ElvenBread, HalflingPipeleaf,
	Mandoline, Lute, Pipes,
	HolySymbol, SpellBook,
	Bloodstone, Chrysoprase, Iolite, Jasper, Moonstone, Onyx,
	Map, Note, Journal,
	Alexandrite, Aquamarine, BlackPearl, Topaz,
	Poison,
	SilverCoins, GoldCoins
};
enum itemfn : unsigned char {
	Intimate, Close, Reach, Near, Far,
	Awkward, Clumsy, Messy, Ration, Reloaded, Precise, Slow, Thrown, TwoHanded,
	Armor1, Armor2, Armor4,
	ArmorP1,
	DamageP1, DamageP2, DamageM1, DamageM2,
	Weight1, Weight2, Weight4,
	Pierce1, Pierce2, IgnoreArmor,
	Use1, Use2, Use4, Ammo1, Ammo2, Ammo4,
	// Item upgrades
	Spiked, Sharp, PerfectlyWeighted, SerratedEdges, Glows, HugeWeapon, Versatile, WellCrafted,
};
typedef flagable<2, unsigned> itemf;

struct item {
	itemn		type;
	itemf		flags;
	int			cost;
	constexpr explicit operator bool() const { return type != (itemn)0; }
	itemn		ammunition() const;
	itemn		basic() const;
	const char*	name() const;
	bool		is(itemfn v) const { return flags.is(v); }
	bool		isweapon() const { return is(Intimate) || is(Close) || is(Near) || is(Far) || is(Reach); }
};
struct wearable {
	item wears[WearLast + 1];
	slice<item> backpack() const { return slice<item>(const_cast<item*>(wears) + Backpack, wears + WearLast); }
	item* chooseitem(const char* title, const char* cancel, fnvisible proc, bool keep);
	bool is(itemn type) const;
	bool is(itemfn range) const;
	bool is(fnvisible proc, bool keep) const;
};