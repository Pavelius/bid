#pragma once

#include "flagable.h"

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
	Hand, Close, Reach, Near, Far,
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
	const char*	name() const;
	bool		is(itemfn v) const { return flags.is(v); }
};