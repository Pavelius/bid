#include "area.h"
#include "item.h"
#include "rand.h"
#include "slice.h"

struct magici {
	char		chance;
	itemn		type;
	powern		power;
};
struct treasurei {
	struct record {
		char	chance;
		dice	range;
		int		multiplier;
	};
	char		letter;
	record		cp, sp, ep, gp, pp, gems, jewelry, magic;
	itemn		additional, additional2;
};

static treasurei treasure_data[] = {
	{'A', {25, {1, 6}, 1000}, {30, {1, 6}, 1000}, {20, {1, 4}, 1000}, {35, {2, 6}, 1000}, {25, {1, 2}, 1000}, {50, {6, 6}}, {50, {6, 6}}, {100, {3}, RandomMagicItem}},
	{'B', {50, {1, 8}, 1000}, {25, {1, 6}, 1000}, {25, {1, 4}, 1000}, {25, {1, 3}, 1000}, {}, {25, {1, 6}}, {25, {1, 6}}, {10, {3}, RandomWeapon}},
	{'C', {20, {1, 12}, 1000}, {30, {1, 4}, 1000}, {10, {1, 4}, 1000}, {}, {}, {25, {1, 4}}, {25, {1, 4}}, {10, {2}, RandomMagicItem}},
	{'D', {10, {1, 12}, 1000}, {15, {1, 4}, 1000}, {}, {60, {1, 6}, 1000}, {}, {30, {1, 8}}, {30, {1, 8}}, {10, {2}, RandomMagicItem}, RandomPotion},
	{'E', {5, {1, 10}, 1000}, {30, {1, 12}, 1000}, {25, {1, 4}, 1000}, {25, {1, 8}, 1000}, {}, {10, {1, 10}}, {10, {1, 10}}, {25, {3}, RandomMagicItem}, RandomScroll},
	{'F', {}, {10, {2, 10}, 1000}, {20, {1, 8}, 1000}, {45, {1, 12}, 1000}, {30, {1, 3}, 1000}, {20, {2, 12}}, {10, {1, 12}}, {30, {3}, RandomMagicItemNoWeapon}, RandomScroll, RandomPotion},
	{'G', {}, {}, {}, {50, {1, 4}, 1000}, {50, {1, 6}, 1000}, {25, {3, 6}}, {25, {1, 10}}, {35, {4}, RandomMagicItem}, RandomScroll},
	{'H', {25, {1, 10}, 1000}, {50, {1, 12}, 1000}, {50, {1, 4}, 1000}, {50, {1, 8}, 1000}, {25, {1, 8}, 1000}, {50, {1, 100}}, {50, {1, 4}, 10}, {15, {4}, RandomMagicItem}, RandomScroll, RandomPotion},
	{'I', {}, {}, {}, {}, {30, {1, 8}, 1000}, {50, {2, 6}}, {50, {2, 6}}, {15, {1}, RandomMagicItem}},
	{'J', {25, {1, 4}, 1000}, {10, {1, 3}, 1000}, {}, {}, {}, {}, {}, {}},
	{'K', {}, {30, {1, 6}, 1000}, {10, {1, 2}, 1000}, {}, {}, {}, {}, {}},
	{'L', {}, {}, {}, {}, {}, {50, {1, 4}}, {}, {}},
	{'M', {}, {}, {}, {40, {2, 4}, 1000}, {50, {5, 6}, 1000}, {55, {5, 4}}, {45, {2, 6}}, {35, {4}, RandomMagicItem}, RandomScroll},
	{'N', {}, {}, {}, {}, {}, {}, {}, {40, {2, 4}, RandomPotion}},
	{'O', {}, {}, {}, {}, {}, {}, {}, {50, {1, 4}, RandomScroll}},
	{'P', {100, {3, 8}}},
	{'Q', {}, {100, {3, 6}}},
	{'R', {}, {}, {100, {2, 6}}},
	{'S', {}, {}, {}, {100, {2, 4}}},
	{'T', {}, {}, {}, {}, {100, {1, 6}}},
	{'U', {10, {1, 100}}, {10, {1, 100}}, {}, {5, {1, 100}}, {}, {5, {1, 4}}, {5, {1, 4}}, {2, {1}, RandomMagicItem}},
	{'V', {}, {10, {1, 100}}, {5, {1, 100}}, {10, {1, 100}}, {5, {1, 100}}, {10, {1, 4}}, {10, {1, 4}}, {5, {1}, RandomMagicItem}},
};

static magici magic_armor[] = {
	{28, RandomArmor, Magic1},
	{42, RandomArmor, Magic2},
	{48, RandomArmor, Magic3},
	{51, RandomArmor, Cursed},
	{54, RandomArmor, Delusion},
	{56, RandomArmor, Cursed},
	{100},
};
static magici magic_shield[] = {
	{16},
	{25, Shield, Magic1},
	{27, Shield, Magic2},
	{28, Shield, Magic3},
	{33},
	{36, Shield, Magic1},
	{41, Shield, Magic2},
	{42, Shield, Magic3},
	{45},
	{46, Shield, Magic1},
	{47, Shield, Magic2},
	{48, Shield, Magic3},
	{53},
	{54, Shield, Magic1},
	{56},
	{62, Shield, Delusion},
	{65, Shield, Weakness},
	{85, Shield, Magic1},
	{95, Shield, Magic2},
	{100, Shield, Magic3},
};
static magici magic_misc1[] = {
	{3, Amulet, ControlLycanthropes},
	{6, Amulet, ControlCharmed},
	{8, Apparatus, Magic1},
	{12, Bag, Devouring},
	{17, Bag, Holding},
	{18, Bag, Transformation},
	{19, Book, Corruption},
	{20, Book, Wizardy},
	{21, Book, Holiness},
	{22, Boots, Dancing},
	{27, Boots, Flying},
	{31, Boots, Speed},
	{35, Boots, Leaping},
	{43, Bracers, Magic1},
	{48, Bracers, Magic2},
	{50, Bracers, Magic3},
	{52, Bracers, Cursed},
	{59, Brooch, ControlSpells},
	{64, Broom, Flying},
	{70, Candle, Invocating},
	{72, Chime, Opening},
	{73, Chime, Ravening},
	{85, Cloack, Magic1},
	{91, Cloack, Magic2},
	{94, Cloack, Magic3},
	{95, Cloack, Flying},
	{97, Cloack, Poisoning},
	{100, Cloack, Transformation},
};
static magici magic_misc2[] = {
	{5, CrystalBall, Magic1},
	{7, CrystalBall, Magic2},
	{8, CrystalBall, Magic3},
	{9, CrystalBall, Delusion},
	{11, Cube, Striking},
	{13, Cube, Frozing},
	{16, Decanter, Magic1},
	{20, Deck, Transformation},
	{24, Cloack, Displacing},
	{26, Drums, Scarying},
	{27, Drums, Thundering},
	{33, Dust, Appearance},
	{39, Dust, Invisibility},
	{40, Dust, Choking},
	{41, Bottle, Summoning},
	{43, Apparatus, SummonAir},
	{45, Apparatus, SummonFire},
	{47, Apparatus, SummonEarth},
	{49, Apparatus, SummonWater},
	{59, Cloack, Invisibility},
	{60, Eyes, Charming},
	{62, Eyes, Microscoping},
	{63, Eyes, Petrification},
	{65, Eyes, Zooming},
	{80, Feather, Zooming},
	{95, Figurine, FormTiger},
	{98, Figurine, FormBoat},
	{100, Gauntlets, Mighty},
};
static magici magic_potions[] = {
	{3, Potion, Clairaudience},
	{4, Potion, Clairvoyance},
	{10, Potion, ControlAnimals},
	{13, Potion, ControlDragons},
	{16, Potion, ControlGiants},
	{19, Potion, ControlHumans},
	{22, Potion, ControlPlants},
	{25, Potion, ControlUndead},
	{32, Potion, Delusion},
	{35, Potion, Diminution},
	{39, Potion, ESP},
	{43, Potion, Flaming},
	{47, Potion, Flying},
	{51, Potion, GaseousForm},
	{55, Potion, Mighty},
	{59, Potion, Growth},
	{63, Potion, Healing},
	{68, Potion, Heroism},
	{72, Potion, Invisibility},
	{76, Potion, Invulnerability},
	{80, Potion, Levitation},
	{84, Potion, Longevity},
	{86, Potion, Poisoning},
	{89, Potion, Transformation},
	{97, Potion, Speed},
	{100, Potion, TreasureDetection},
};
static magici magic_rings[] = {
	{5, Ring, ControlAnimals},
	{10, Ring, ControlHumans},
	{16, Ring, ControlPlants},
	{26, Ring, Delusion},
	{29, Ring, Summoning},
	{39, Ring, Flaming},
	{50, Ring, Invisibility},
	{55, Ring, Magic1},
	{70, Ring, Magic2},
	{72, Ring, Regeneration},
	{74, Ring, SpellStoring},
	{80, Ring, ControlSpells},
	{82, Ring, Telekinesis},
	{88, Ring, WaterWalking},
	{94, Ring, Weakness},
	{98, Ring, Wishes},
	{100, Ring, XRays}
};
static magici magic_swords[] = {
	{3, ShortSword, Quickness},
	{9, RandomSword, Cursed},
	{12, RandomSword, Delusion},
	{28, RandomSword, Magic1},
	{31, RandomSword, ControlLycanthropes},
	{34, RandomSword, ControlSpells},
	{37, RandomSword, ControlDragons},
	{40, RandomSword, ControlCharmed},
	{46, RandomSword, ControlGoblinoid},
	{52, RandomSword, ControlUndead},
	{55, RandomSword, ControlDragons},
	{56, RandomSword, Draining},
	{59, RandomSword, Flaming},
	{61, RandomSword, Frozing},
	{64, RandomSword, ControlGiants},
	{69, RandomSword, Lighting},
	{71, RandomSword, Locating},
	{72, RandomSword, Luck},
	{73, RandomSword, Sharpness},
	{78, RandomSword, Sun},
	{79, RandomSword, Wishes},
	{80, RandomSword, Wounding},
	{85, RandomSword, Magic2},
	{87, RandomSword, Charming},
	{88, RandomSword, Dancing},
	{89, RandomSword, Devouring},
	{94, RandomSword, Venger},
	{95, RandomSword, Vorpal},
	{98, RandomSword, Magic3},
	{99, RandomSword, Defender},
	{100, RandomSword, Holiness},
};
static magici magic_weapons[] = {
	{12, ArrowM1},
	{15, ArrowM2},
	{19, HandAxe, Magic1},
	{21, HandAxe, Magic2},
	{24, LongBow, Magic1},
	{25, Crossbow, Distance},
	{26, Crossbow, Speed},
	{27, Crossbow, Accuracy},
	{31, BoltM1},
	{36, BoltM2},
	{39, Dagger, Magic1},
	{40, Dagger, Transformation},
	{41, Dagger, Flying},
	{42, Dagger, Poisoning},
	{45, Dagger, ControlGoblinoid},
	{46, Dagger, Mighty},
	{50, Javelin, Lighting},
	{55, Javelin, Locating},
	{58, Mace, Magic1},
	{59, Mace, ControlUndead},
	{62, Mace, Magic2},
	{63, Mace, Magic3},
	{69, Sling, Magic1},
	{71, RandomWeapon, Cursed},
	{75, Spear, Magic1},
	{77, Spear, Magic2},
	{78, Spear, Magic3},
	{80, Staff, Magic1},
	{82, RandomWeapon, Delusion},
	{87, Trident, ControlFish},
	{89, Trident, Scarying},
	{93, Trident, Locating},
	{96, WarHammer, Magic1},
	{98, WarHammer, Magic2},
	{99, WarHammer, DwarvenThrower},
	{100, WarHammer, Thundering},
};
static magici magic_rod_staff_wands[] = {
	{5, Rod, ControlSpells},
	{11, Rod, Dispelling},
	{12, Rod, Charming},
	{14, Rod, Mighty},
	{15, Rod, Parrying},
	{16, Rod, Ressurection},
	{17, Rod, Striking},
	{18, Staff, Commanding},
	{20, Staff, Dispelling},
	{26, Staff, Healing},
	{27, Staff, Power},
	{30, Staff, Snakes},
	{33, Staff, Striking},
	{36, Staff, SwarmingInsects},
	{38, Staff, Healing},
	{40, Staff, Withering},
	{41, Staff, Wizardy},
	{44, Staff, Woodlands},
	{47, Wand, Frozing},
	{51, Wand, EnemyDetection},
	{54, Wand, Scarying},
	{57, Wand, Flaming},
	{61, Wand, Illusion},
	{64, Wand, Lighting},
	{69, Wand, MagicDetection},
	{74, Wand, MagicMissiles},
	{79, Wand, TreasureDetection},
	{84, Wand, Dispelling},
	{87, Wand, Paralysation},
	{90, Wand, Transformation},
	{94, Wand, Radiance},
	{97, Wand, SecretDoorDetection},
	{98, Wand, Summoning},
	{100, Wand, TrapDetection},
};

static magici* table_magic_misc[] = {magic_misc1, magic_misc2};

int treasure_coins[PP - CP + 1];

static const magici* find_magic(const magici* p, int index) {
	while(p->chance < index)
		p++;
	return p;
}

static void add_item(item& it) {
	if(!it)
		return;
	it.drop(AreaIndex, last_area->index());
}

static void add_magic_item(const magici* p, int result) {
	p = find_magic(p, result);
	if(!p || !p->type)
		return;
	item it(random(p->type));
	if(it.countable())
		it.count = xrand(2, 12);
	else
		it.set(p->power);
	add_item(it);
}

void add_magic_item(itemn type) {
	auto result = 1 + rand() % 100;
	switch(type) {
	case RandomArmorOrShield:
		add_magic_item(magic_armor, result);
		add_magic_item(magic_shield, result);
		break;
	case RandomMisc:
		add_magic_item(maprnd(table_magic_misc), result);
		break;
	case RandomPotion:
		add_magic_item(magic_potions, result);
		break;
	case RandomRing:
		add_magic_item(magic_rings, result);
		break;
	case RandomRodStaffWand:
		add_magic_item(magic_rod_staff_wands, result);
		break;
	case RandomSword:
		add_magic_item(magic_swords, result);
		break;
	case RandomWeapon:
		add_magic_item(magic_weapons, result);
		break;
	default:
		break;
	}
}

static void add_item(itemn v) {
	item it(v);
	if(it.countable())
		it.count = 1;
	add_item(it);
}

static void add_coins(itemn coin, const treasurei::record& e) {
	if(!e.chance)
		return;
	if(e.chance < 100) {
		if(d100() >= e.chance)
			return;
	}
	auto count = e.range.roll();
	if(e.multiplier)
		count = count * e.multiplier;
	treasure_coins[coin - CP] += count;
}

static void add_items(itemn type, const treasurei::record& e) {
	if(!type)
		return;
	if(!e.chance)
		return;
	if(e.chance < 100) {
		if(d100() >= e.chance)
			return;
	}
	auto count = e.range.roll();
	for(auto i = 0; i < count; i++)
		add_item(random(type));
}

static void add_magic_items(itemn type, const treasurei::record& e) {
	if(!type)
		return;
	if(!e.chance)
		return;
	if(e.chance < 100) {
		if(d100() >= e.chance)
			return;
	}
	auto count = e.range.roll();
	for(auto i = 0; i < count; i++)
		add_magic_item(random_basic(type));
}

static void treasure_clear() {
	memset(treasure_coins, 0, sizeof(treasure_coins));
}

static void treasure_generate(const treasurei& e) {
	add_item(Opal);
	add_item(Opal);
	add_coins(CP, e.cp);
	add_coins(SP, e.sp);
	add_coins(EP, e.ep);
	add_coins(GP, e.gp);
	add_coins(PP, e.pp);
	add_items(RandomGem, e.gems);
	add_items(RandomJewelry, e.jewelry);
	add_magic_items((itemn)e.magic.multiplier, e.magic);
	if(e.additional)
		add_magic_item(random_basic(e.additional));
	if(e.additional2)
		add_magic_item(random_basic(e.additional2));
}

void treasure_generate(const char* type, bool use_lair, bool use_group, bool use_individual) {
	treasure_clear();
	for(auto p = type; *p; p++) {
		auto s = *p;
		if(s >= 'A' && s <= 'O') {
			if(use_lair)
				treasure_generate(treasure_data[s - 'A']);
		} else if(s >= 'P' && s <= 'T') {
			if(use_individual)
				treasure_generate(treasure_data[s - 'A']);
		} else if(s >= 'U' && s <= 'V') {
			if(use_group)
				treasure_generate(treasure_data[s - 'A']);
		}
	}
}