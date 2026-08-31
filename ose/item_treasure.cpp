#include "itema.h"
#include "dice.h"
#include "rand.h"
#include "slice.h"

struct itemmagic {
	char	chance;
	itemn	type;
	powern	power;
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

static itemmagic magic_armor[] = {
	{28, RandomArmor, Magic1},
	{42, RandomArmor, Magic2},
	{48, RandomArmor, Magic3},
	{51, RandomArmor, Cursed},
	{54, RandomArmor, Delusion},
	{56, RandomArmor, Cursed},
	{100},
};
static itemmagic magic_shield[] = {
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

int treasure_coins[PP - CP + 1];

static const itemmagic* find_magic(const itemmagic* p, int index) {
	while(p->chance < index)
		p++;
	return p;
}

static int rd100() {
	return 1 + rand() % 100;
}

static void add_magic_item(const itemmagic* p, int result) {
	p = find_magic(p, result);
	if(!p || !p->type)
		return;
	item it(random(p->type));
	it.set(p->power);
	if(it)
		items.add(it);
}

static void add_magic_item(const itemmagic* p) {
	add_magic_item(p, rd100());
}

void add_magic_item(itemn type) {
	int result;
	switch(type) {
	case RandomArmorOrShield:
		result = rd100();
		add_magic_item(magic_armor, result);
		add_magic_item(magic_shield, result);
		break;
	default:
		break;
	}
}

static void add_item(itemn v) {
	item it(v);
	if(it.countable())
		it.count = 1;
	items.add(it);
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

static void treasure_clear() {
	items.clear();
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
	add_items((itemn)e.magic.multiplier, e.magic);
	if(e.additional)
		add_item(random(e.additional));
	if(e.additional2)
		add_item(random(e.additional2));
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