#include "answers.h"
#include "area.h"
#include "adat.h"
#include "bsdata.h"
#include "creature.h"
#include "collectiona.h"
#include "draw_atg.h"
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
	sb.add(message_names[v], e.name(), price(e));
	if(e.countable() && e.count > 1) {
		sb.adds("[~");
		sb.add(message_names[AvailableCount], e.count);
		sb.adds("]");
	}
	an.addv((long)&e, temp, 0);
}

static item* choose_item(itema& source, messagen v, fnitemget price) {
	while(true) {
		for(auto& e : source)
			addopt(e, v, price);
		auto result = (item*)choose_player_option(message_names[Cancel]);
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
		auto result = (item*)choose_player_option(message_names[Cancel]);
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

static bool gather_information(bool run) {
	// 1 - We have some interesting location - lair of some monsters maybe.
	// 2 - We have some group of creatures occupied it. This is a `classn` creatures. Also can a adjective like `vile`, `evil`, `merciless`, `strange` e.t.c
	// 3 - Also you can have a broad group of threat like `undead`, `bandits`, `greenskins`, `dwarves` e.t.c.
	if(run) {
		pass_turn();
		sb.addn("%TavernRumorSpeech");
		sb.addn("\"");
		if(d100() < 60)
			sb.add("%TavernRumorGeneral");
		else {
			sb.add("%TavernAlreadyKnown");
			sb.adds("%TavernNothingToSay");
		}
		sb.add("\"");
		pause();
	}
	return true;
}

static bool leave_settlement(bool run) {
	if(last_area->parent_id != 0)
		return false;
	if(run) {
		last_area = 0;
		pass_turn();
	}
	return true;
}

static bool leave_outside(bool run) {
	if(last_area->parent_id == 0)
		return false;
	if(run) {
		last_area = bsdata<area>::elements + last_area->parent_id;
		pass_turn();
	}
	return true;
}

bool apply_settlement(actionn v, bool run) {
	switch(v) {
	case BuyTradeGoods: return buy_action(market_items, run);
	case SellTradeGoods: return sell_action(run);
	case GatherInformation: return gather_information(run);
	case LeaveSettlement: return leave_settlement(run);
	case LeaveOutside: return leave_outside(run);
	default: return false;
	}
	return true;
}