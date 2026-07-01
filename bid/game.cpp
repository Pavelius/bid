#include "answers.h"
#include "bsdata.h"
#include "character.h"
#include "collection.h"
#include "message.h"
#include "stringvar.h"

void game_run() {
	add_players();
}

static void player_name(stringbuilder& sb) {

}

BSDATA(stringvari) = {
	{"Player", player_name},
};
BSDATAF(stringvari)