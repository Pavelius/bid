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

static void stringbuilder_custom(stringbuilder& sb, const char* id) {
	if(stringvar_identifier(sb, id))
		return;
	default_string(sb, id);
}

static bool choose_weapon() {
	an.clear();
	for(auto& e : player->wears) {
		if(e && e.isweapon() && e.range(scene_range))
			an.add((long)&e, e.name());
	}
	if(!an)
		return false;
	auto pi = (item*)an.choose();
	if(!pi)
		return false;
	iswap(player->wears[Hands], *pi);
	player->act(MsgReadyWeapon);
	return true;
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
	if(!player->wearitem(scene_range))
		return false;
	if(run) {
		if(!choose_weapon())
			return false;
		make_roll(Dexterity);
		if(roll_effect<=Fail) {
			if(enemy.is(scene_range))
				enemy_deal_damage();
			else
				game_master_move();
		} else if(roll_effect == PartialSuccess) {

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