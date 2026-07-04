#include "answers.h"
#include "draw_atg.h"
#include "game.h"
#include "rand.h"

itemfn scene_range;

void game_run() {
	answers::resid = "Wasteland";
	enemy.create(Skeleton);
	srand(13123);
}