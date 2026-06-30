#include "character.h"
#include "game.h"
#include "message.h"

character* player;
character players[3];

static void add_player() {
	player->type = choose(Whisper, ChoosePlaybook);
	player->heiretage = choose(Tycheros, ChooseHeiretage);
}

void add_players() {
	for(int i = 0; i < sizeof(players) / sizeof(players[0]); i++) {
		player = players + i;
		add_player();
	}
}