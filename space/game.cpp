#include "answers.h"
#include "bsdata.h"
#include "draw_atg.h"
#include "math.h"
#include "rand.h"
#include "stringvar.h"

static void stringbuilder_custom(stringbuilder& sb, const char* id) {
	//if(stringvar_identifier(sb, id))
	//	return;
	default_string(sb, id);
}

void game_run() {
	answers::resid = "Wasteland";
	stringbuilder::custom = stringbuilder_custom;
	srand(2311);
}