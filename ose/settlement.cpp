#include "answers.h"
#include "area.h"
#include "creature.h"
#include "itema.h"
#include "game.h"
#include "slice.h"
#include "rand.h"
#include "settlement.h"
#include "stringbuilder.h"

kindomi kindoms[FrozenNorth + 1];

sitei sites[128];
sitei* last_site;

settlement settlements[32];
settlement* last_settlement;
settlement* next_settlement; // If none path is not choose

stagei stages[4096];

static itema market_items;

static actionn market_actions[] = {BuyTradeGoods, SellTradeGoods};
static actionn inn_actions[] = {RentRoomOnNight};
static actionn tavern_actions[] = {GatherInformation};

static arean small_settlements[] = {SmallTown, Village, Village, Hamlet};
static arean normal_landscape[] = {Plains, Plains, Hills, Forest};
static arean wet_landscapre[] = {Wastes, Sands, Jungle};

static directionn random_direction[] = {North, South, West, East};
static directionn opposite_direction[West+1] = {Center, South, West, North, East};
static directionn kindom_sides[FrozenNorth + 1][5] = {
	{Center, North, East, South, West},
	{Center, South, North, West, East},
	{Center, North, South, West, East},
	{Center, East, West, South, North},
	{Center, West, East, North, South},
	{Center, East, West, North, South},
	{Center, West, East, South, North},
	{Center, South, North, East, West},
};

int settlement::index() const {
	return this - settlements;
}

void sitei::clear() {
	memset((void*)this, 0, sizeof(*this));
	settlement_id = 0xFF;
}

void stagei::clear() {
	memset((void*)this, 0, sizeof(*this));
	site_id = 0xFF;
}

const char* sitei::namefull() const {
	return name();
}

static bool is_exist_name(unsigned char v) {
	for(auto& e : settlements) {
		if(e && e.name_id == v)
			return true;
	}
	return false;
}

static arean random_settlement_landscape(kindomn type) {
	return maprnd(normal_landscape);
}

static unsigned char get_random_name(kindomn kindom, arean type) {
	adat<unsigned char, (FrozenNorth + 1) * 8> source;
	unsigned char i1 = kindom * 8 + ((type == Village || type == Hamlet) ? 4 : 0);
	for(auto i = i1; i < i1 + 4; i++) {
		if(is_exist_name(i))
			continue;
		source.add(i);
	}
	if(!source) {
		unsigned char i1 = kindom * 8;
		for(auto i = i1; i < i1 + 8; i++) {
			if(is_exist_name(i))
				continue;
			source.add(i);
		}
	}
	if(!source) {
		unsigned char i1 = 0;
		for(auto i = i1; i < i1 + 5 * 8; i++) {
			if(is_exist_name(i))
				continue;
			source.add(i);
		}
	}
	if(!source)
		return 0;
	return source.data[rand() % source.count];
}

static void create_settlement(int& index, kindomn kindom, arean type, int number) {
	if(index >= settlement_maximum)
		return;
	auto n = get_random_name(kindom, type);
	auto p = settlements + (index++);
	p->kindom = kindom;
	p->type = type;
	p->landscape = random_settlement_landscape(kindom);
	p->name_id = n;
	if(number < sizeof(kindom_sides[0]) / sizeof(kindom_sides[0][0]))
		p->side = kindom_sides[kindom][number];
	else
		p->side = maprnd(random_direction);
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
		if(chance(60))
			p->set(Tavern);
		if(chance(60))
			p->set(Temple);
		break;
	default:
		if(chance(50))
			p->set(Inn);
		if(chance(30))
			p->set(Tavern);
		break;
	}
}

static void add_small_villages(int& index, kindomn type, int min, int max) {
	if(max <= 0)
		return;
	auto count = xrand(min, max);
	for(auto i = 0; i < count; i++)
		create_settlement(index, type, maprnd(small_settlements), i + 1);
}

static void clear_world() {
	memset(kindoms, 0, sizeof(kindoms));
	memset(settlements, 0, sizeof(settlements));
	for(auto& e : sites)
		e.clear();
	for(auto& e : stages)
		e.clear();
}

void generate_world() {
	int index = 0;
	clear_world();
	for(auto i = MiddleKindom; i <= EasternKindom; i = (kindomn)(i + 1))
		create_settlement(index, i, LargeTown, 0);
	create_settlement(index, DwarvenMountains, SmallTown, 0);
	create_settlement(index, ElvishLand, SmallTown, 0);
	create_settlement(index, FrozenNorth, SmallTown, 0);
	for(auto i = MiddleKindom; i <= EasternKindom; i = (kindomn)(i + 1))
		add_small_villages(index, i, 3, 6);
	add_small_villages(index, DwarvenMountains, 1, 3);
	add_small_villages(index, DwarvenMountains, 0, 3);
	add_small_villages(index, FrozenNorth, settlement_maximum - index, settlement_maximum - index);
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

sitei* find_site(settlement* target, arean type) {
	if(!target)
		return 0;
	auto index = target->index();
	for(auto& e : sites) {
		if(e.settlement_id==index && e.type==type)
			return &e;
	}
	return 0;
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
			variables[PartyCoins] += cost;
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
		area = place_to_go;
		addhdr(getimage(area), "%Area");
		add_look();
		for(auto n : getactions(area)) {
			if(apply(n, false))
				addopt(n);
		}
		auto result = choose_player_option(action_names[get_leave_action(area)]);
		if(!result)
			break;
		else if(result == Continue)
			continue;
		apply((actionn)result, true);
	}
}

static bool opposite(directionn d1, directionn d2) {
	return opposite_direction[d1] == d2;
}

static int distance(settlement* p1, settlement* p2) {
	auto days = 2 + (p1->index() + p2->index()) % 3;
	if(opposite(p1->side, p2->side))
		days += 4;
	else if(p1->side != Center && p2->side != Center)
		days += 2;
	return days * 24;
}

static const char* ask_visit_settlement(arean type, unsigned char name) {
	static char temp[128]; stringbuilder sb(temp);
	sb.clear();
	sb.add(area_visit[type]);
	sb.adds(settlement_names[name]);
	return temp;
}

static void kindom_adventure_move() {
	auto kindom = last_settlement->kindom;
	while(doactions()) {
		next_settlement = 0;
		area = last_settlement->landscape;
		addhdr(getimage(area), "%Area");
		add_look();
		for(auto i = 0; i < settlement_maximum; i++) {
			auto p = settlements + i;
			if(p->kindom != kindom)
				continue;
			if(p == last_settlement)
				continue;
			auto miles = distance(last_settlement, p);
			an.add((long)p, message_names[AskRoadAdventure], area_names_to[settlements[i].type], get_range_name(miles), p->name(), direction_names[p->side]);
		}
		auto cancel_text = ask_visit_settlement(last_settlement->type, last_settlement->name_id);
		next_settlement = (settlement*)choose_way_to_go(cancel_text);
		if(!next_settlement)
			break;
		else {
			auto miles = distance(last_settlement, next_settlement);
			clear_messages();
			adventure_move(miles);
			last_settlement = next_settlement;
		}
	}
}

void settlement_move() {
	while(doactions()) {
		last_settlement->set(Known);
		last_settlement->set(Visited);
		area = last_settlement->type;
		addhdr(getimage(area), "%SettlementName");
		sb.clear();
		add_look();
		for(auto i = (arean)1; i <= Palace; i = (arean)(i + 1)) {
			if(last_settlement->is(i))
				addopt(i);
		}
		auto result = choose_party_option(action_names[LeaveSettlement]);
		if(result)
			enter_building((arean)result);
		else
			kindom_adventure_move();
	}
}

static sitei* new_site() {
	for(auto& e : sites) {
		if(!e)
			return &e;
	}
	return sites;
}

void create_site(arean type) {
	if(!last_settlement)
		return;
	last_site = new_site();
	last_site->type = type;
	last_site->settlement_id = last_settlement->index();
}