#include "answers.h"
#include "area.h"
#include "adat.h"
#include "creature.h"
#include "collectiona.h"
#include "game.h"
#include "itema.h"
#include "message.h"
#include "rand.h"
#include "pushvalue.h"

static itema market_items;

static void add_market(itemn v) {
	item it(random(v));
	if(it.countable())
		it.count = xrand(3, 18);
	market_items.add(it);
}

static void add_market(itemn v, int count) {
	for(auto i = 0; i < count; i++)
		add_market(v);
}

static void normalize(itema& source) {
	auto pb = source.data;
	auto ps = source.data;
	auto pe = ps + source.count;
	while(pb < pe) {
		if(*pb)
			*ps++ = *pb;
		pb++;
	}
	source.count = ps - source.data;
}

void create_market_items() {
	market_items.clear();
	add_market(RandomFood);
	add_market(RandomWeapon, xrand(1, 6));
	add_market(RandomArmor, xrand(1, 6));
}

static item* choose_item(collectiona& source, messagen v) {
	an.clear();
	for(auto p : source.records<item>())
		an.add((long)p, getname(v), p->name(), p->price());
	return (item*)an.choose(what_to_do(), getname(Cancel));
}

bool buy_market_action(fnvfilter proc, bool run) {
	auto& items = market_items;
	collectiona source;
	source.select(items.data, items.count, sizeof(items.data[0]), proc);
	if(!source)
		return false;
	if(run) {
		while(true) {
			source.select(items.data, items.count, sizeof(items.data[0]), proc);
			if(!source)
				break;
			auto pi = choose_item(source, BuyItemForCost);
			if(!pi)
				break;
			if(!pi->countable())
				player->add(*pi);
			else {
				player->add(item(pi->type, 1));
				pi->consume();
			}
			normalize(items);
		}
	}
	return true;
}