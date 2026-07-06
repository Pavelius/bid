#include "answers.h"
#include "bsdata.h"
#include "character.h"
#include "draw_atg.h"
#include "game.h"
#include "math.h"
#include "message.h"
#include "rand.h"
#include "roll.h"
#include "stringvar.h"

itemfn scene_range;

item* wearable::chooseitem(const char* title, const char* cancel, fnvisible proc, bool keep) {
	an.clear();
	for(auto& e : wears) {
		if(!e)
			continue;
		if(proc && proc(&e) != keep)
			continue;
		an.add((long)&e, e.name());
	}
	an.sort();
	return (item*)an.choose(title, cancel);
}

static void stringbuilder_custom(stringbuilder& sb, const char* id) {
	if(stringvar_identifier(sb, id))
		return;
	default_string(sb, id);
}

static void change_weapon() {
	an.clear();
	for(auto& e : slice(player->wears + Hands + 1, player->wears + WearLast + 1)) {
		if(e && e.isweapon())
			an.add((long)&e, e.name());
	}
	if(!an)
		return;
	auto pi = (item*)an.choose();
	if(!pi)
		return;
	iswap(player->wears[Hands], *pi);
	player->act(MsgReadyWeapon);
}

static void prepare_to_worse() {
}

static void deal_damage() {
}

static void enemy_deal_damage() {
}

static void game_master_move() {
}

static bool make_roll(statn stat) {
	auto value = player->get(stat);
	make_roll_raw(value);
	return roll_effect >= PartialSuccess;
}

static bool move_volley(bool run) {
	if(!enemy)
		return false;
	if(!player->is(scene_range))
		return false;
	if(run) {
		make_roll(Dexterity);
		if(roll_effect <= Fail) {
			if(enemy.is(scene_range))
				enemy_deal_damage();
			else
				game_master_move();
		} else if(roll_effect == PartialSuccess) {
			an.clear();
		} else {
			fixmsg(MsgVolleyHit);
			deal_damage();
		}
	}
	return true;
}

void game_run() {
	answers::resid = "Wasteland";
	stringbuilder::custom = stringbuilder_custom;
	enemy.create(Skeleton);
	srand(13123);
}

static void print_hands(stringbuilder& sb) {
	sb.add(player->wears[Hands].name());
}

static void print_player(stringbuilder& sb) {
	sb.add(player->getname());
}

BSDATA(stringvari) = {
	{"Hands", print_hands},
	{"Player", print_player}
};
BSDATAF(stringvari)