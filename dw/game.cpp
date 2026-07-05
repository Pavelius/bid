#include "answers.h"
#include "bsdata.h"
#include "character.h"
#include "draw_atg.h"
#include "game.h"
#include "math.h"
#include "message.h"
#include "rand.h"
#include "stringvar.h"

itemfn scene_range;

static void stringbuilder_custom(stringbuilder& sb, const char* id) {
	if(stringvar_identifier(sb, id))
		return;
	default_string(sb, id);
}

static bool have_weapon(itemfn range) {
	for(auto& e : player->equipments()) {
		if(e.range(range))
			return true;
	}
	return false;
}

static void choose_weapon() {
	an.clear();
	for(auto& e : player->wears) {
		if(e || e.isweapon())
			an.add((long) & e, e.name());
	}
	if(!an)
		return;
	auto pi = (item*)an.choose();
	if(pi) {
		iswap(player->wears[Hands], *pi);
		player->act(MsgReadyWeapon);
	}
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