#include "answers.h"
#include "bsdata.h"
#include "character.h"
#include "draw_atg.h"
#include "game.h"
#include "rand.h"
#include "stringvar.h"

itemfn scene_range;

static void stringbuilder_custom(stringbuilder& sb, const char* id) {
	if(stringvar_identifier(sb, id))
		return;
	default_string(sb, id);
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

BSDATA(stringvari) = {
	{"Hands", print_hands}
};
BSDATAF(stringvari)