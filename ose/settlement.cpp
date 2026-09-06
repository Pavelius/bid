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
	add_market(RandomFood, xrand(2, 4));
	add_market(RandomMarketGood, xrand(1, 6));
}

static item* choose_item(itema& source, messagen v) {
	an.clear();
	for(auto& e : source)
		an.add((long)&e, getname(v), e.name(), e.price());
	return (item*)(what_to_do(), getname(Cancel));
}

bool buy_market_action(bool run) {
	auto& source = market_items;
	if(!source)
		return false;
	if(run) {
		while(true) {
			normalize(source);
			auto pi = choose_item(source, BuyItemForCost);
			if(!pi)
				break;
			if(!pi->countable())
				player->add(*pi);
			else {
				player->add(item(pi->type, 1));
				pi->consume();
			}
		}
	}
	return true;
}