#include "itema.h"
#include "dice.h"
#include "rand.h"
#include "slice.h"
#include "treasure.h"

struct treasurei {
	struct record {
		char	chance;
		dice	range;
		int		multiplier;
	};
	char		letter;
	record		cp, sp, ep, gp, pp, gems, jewelry, magic;
	itemn		additional;
};

static treasurei data[] = {
	{'A', {25, {1, 6}, 1000}, {30, {1, 6}, 1000}, {20, {1, 4}, 1000}, {35, {2, 6}, 1000}, {25, {1, 2}, 1000}, {50, {6, 6}}, {50, {6, 6}}, {0, {3}, RandomMagicItem}},
	{'B', {50, {1, 8}, 1000}, {25, {1, 6}, 1000}, {25, {1, 4}, 1000}, {25, {1, 3}, 1000}, {}, {25, {1, 6}}, {25, {1, 6}}, {10, {3}, RandomWeapon}},
	{'C', {20, {1, 12}, 1000}, {30, {1, 4}, 1000}, {10, {1, 4}, 1000}, {}, {}, {25, {1, 4}}, {25, {1, 4}}, {10, {2}, RandomMagicItem}},
	{'D', {10, {1, 12}, 1000}, {15, {1, 4}, 1000}, {}, {60, {1, 6}, 1000}, {}, {30, {1, 8}}, {30, {1, 8}}, {10, {2}, RandomMagicItem}, RandomPotion},
};

int treasure_coins[PP - CP + 1];

static void add_item(itemn v) {
	item it(v);
	if(it.countable())
		it.count = 1;
	items.add(it);
}

static void add_coins(itemn coin, const treasurei::record& e) {
	if(e.chance) {
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
	if(e.chance) {
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
}

void treasure_generate(const char* type) {
	treasure_clear();
	for(auto p = type; *p; p++)
		treasure_generate(data[*p - 'A']);
}