#include "area.h"
#include "creature.h"
#include "itema.h"
#include "game.h"
#include "slice.h"
#include "rand.h"
#include "settlement.h"
#include "stringbuilder.h"

extern stringbuilder sb;

settlement settlements[32];
settlement* last_settlement;
settlement* next_settlement; // If none path is not choose

static itema market_items;

static actionn market_actions[] = {BuyTradeGoods, SellTradeGoods};
static actionn inn_actions[] = {RentRoomOnNight};
static actionn tavern_actions[] = {GatherInformation};

static void create_settlement(int& index, kindomn kindom, arean type) {
	if(index >= settlement_maximum)
		return;
	auto p = settlements + (index++);
	p->kindom = kindom;
	p->type = type;
	p->set(Market);
	switch(p->type) {
	case LargeTown:
		p->set(Inn);
		p->set(Tavern);
		p->set(Temple);
		p->set(Palace);
		break;
	case SmallTown:
		p->set(Inn);
		p->set(Tavern);
		if(chance(60))
			p->set(Temple);
		break;
	default:
		if(chance(60))
			p->set(Inn);
		if(chance(40))
			p->set(Tavern);
		break;
	}
}

static arean get_settlement_type(int index) {
	static arean second_types[] = {SmallTown, SmallTown, Village};
	static arean third_types[] = {Village, Hamlet, Hamlet};
	switch(index) {
	case 0: return LargeTown; // Only one capital city
	case 1: return SmallTown;
	case 2: case 3: return maprnd(second_types);
	default: return maprnd(third_types);
	}
}

void generate_world() {
	int index = 0;
	memset(settlements, 0, sizeof(settlements));
	// 1 - Generate human kindoms
	for(auto i = MiddleKindom; i <= EasternKindom; i = (kindomn)(i + 1)) {
		auto settlement_count = xrand(3, 6);
		for(auto n = 0; n < settlement_count; n++)
			create_settlement(index, i, get_settlement_type(n));
	}
	// 2 - Mistic lands
	create_settlement(index, DwarvenMountains, SmallTown);
	create_settlement(index, ElvishLand, SmallTown); // Elves have no large settlements
	for(auto i = 1 + rand() % 3; i > 0; i--)
		create_settlement(index, DwarvenMountains, get_settlement_type(4));
	for(auto i = rand() % 4; i > 0; i--)
		create_settlement(index, ElvishLand, get_settlement_type(4));
	// 3 - Frozen north
	while(index < settlement_maximum)
		create_settlement(index, FrozenNorth, get_settlement_type(4));
}

settlement* find_settlement(kindomn kindom, arean type) {
	for(auto& e : settlements) {
		if(e.kindom == kindom && e.type == type)
			return &e;
	}
	return 0;
}

settlement* find_settlement(kindomn kindom, arean type, settlement* start) {
	if(!start)
		return 0;
	auto pe = settlements + settlement_maximum;
	for(auto p = start; p < pe; p++) {
		if(p->kindom == kindom && p->type == type)
			return p;
	}
	return 0;
}

static picturen getimage(arean v) {
	switch(v) {
	case Village: case Hamlet: case SmallTown: case LargeTown:
		return ImagePlainVillage;
	case Market: return ImageVillageMarket;
	case Inn: return ImageHotel;
	case Tavern: return ImageTavern;
	default:
		return ImagePlains;
	}
}

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

static int get_sell_price(const item& v) {
	auto n = v.price() / 2;
	return n;
}

static int get_buy_price(const item& v) {
	auto n = v.price() * 2;
	return n;
}

void create_market_items() {
	market_items.clear();
	add_market(RandomFood, xrand(2, 4));
	add_market(RandomMarketGood, xrand(1, 6));
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
			game_var[PartyCoins] += cost;
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

static bool apply(actionn v, bool run) {
	switch(v) {
	case BuyTradeGoods: return buy_action(market_items, run);
	case SellTradeGoods: return sell_action(run);
	case GatherInformation: return gather_information(run);
	default: return true;
	}
}

static slice<actionn> getactions(arean v) {
	switch(v) {
	case Market: return market_actions;
	case Inn: return inn_actions;
	case Tavern: return tavern_actions;
	default: return slice<actionn>();
	}
}

static actionn get_leave_action(arean v) {
	switch(v) {
	case Market: return LeaveBack;
	default: return LeaveOutside;
	}
}

static void enter_building(arean place_to_go) {
	while(doactions()) {
		enviroment = place_to_go;
		add_header(getimage(enviroment), "%AreaNameFull");
		add_look();
		for(auto n : getactions(enviroment)) {
			if(apply(n, false))
				addopt(n);
		}
		auto result = choose_player_option(action_names[get_leave_action(enviroment)]);
		if(!result)
			break;
		else if(result == Continue)
			continue;
		apply((actionn)result, true);
	}
}

void settlement_move() {
	last_settlement->set(Known);
	last_settlement->set(Visited);
	while(doactions()) {
		enviroment = last_settlement->type;
		add_header(getimage(enviroment), "%AreaNameFull");
		add_look();
		for(auto i = (arean)1; i <= Palace; i = (arean)(i + 1)) {
			if(last_settlement->is(i))
				addopt(i);
		}
		auto result = choose_party_option(action_names[LeaveSettlement]);
		if(!result)
			break;
		enter_building((arean)result);
	}
}