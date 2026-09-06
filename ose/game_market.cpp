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

typedef int(*fnitemget)(const item& v);

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

static int get_sell_price(const item& v) {
	auto n = v.price() / 2;
	return n;
}

static int get_buy_price(const item& v) {
	auto n = v.price() * 2;
	return n;
}

static void addopt(const item& e, messagen v, fnitemget price) {
	char temp[260]; stringbuilder sb(temp);
	sb.add(getname(v), e.name(), price(e));
	if(e.countable() && e.count > 1) {
		sb.adds("[~");
		sb.add(getname(AvailableCount), e.count);
		sb.adds("]");
	}
	an.addv((long)&e, temp, 0);
}

static item* choose_item(itema& source, messagen v, fnitemget price) {
	while(true) {
		for(auto& e : source)
			addopt(e, v, price);
		auto result = (item*)choose_player_option(getname(Cancel));
		if(((long)result) == Continue)
			continue;
		return result;
	}
}

static item* choose_item(wearable& source, messagen v, fnitemget price) {
	while(true) {
		auto pe = source.wears + LastBackpack;
		for(auto p = source.wears + Backpack; p < pe; p++) {
			if(*p)
				addopt(*p, v, price);
		}
		auto result = (item*)choose_player_option(getname(Cancel));
		if(((long)result) == Continue)
			continue;
		return result;
	}
}

static bool buy_action(itema& source, bool run) {
	if(!source)
		return false;
	if(run) {
		while(true) {
			auto pi = choose_item(source, BuyItemForCost, get_buy_price);
			if(!pi)
				break;
			if(!pi->countable())
				player->add(*pi);
			else {
				player->add(item(pi->type, 1));
				pi->consume();
			}
			player->update();
			normalize(source);
		}
	}
	return true;
}

static bool sell_action(bool run) {
	if(!player->isbackpack())
		return false;
	if(run) {
		while(true) {
			auto pi = choose_item(*player, SellItemForCost, get_sell_price);
			if(!pi)
				break;
			auto cost = get_sell_price(*pi);
			game.add(PartyCoins, cost);
			market_items.add(*pi);
			pi->clear();
			player->update();
			normalize(market_items);
		}
	}
	return true;
}

bool buy_market_action(bool run) {
	return buy_action(market_items, run);
}

bool sell_market_action(bool run) {
	return sell_action(run);
}