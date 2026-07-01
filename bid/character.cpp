#include "answers.h"
#include "character.h"
#include "choose.h"
#include "game.h"
#include "message.h"

character*	player;
character	players[3];

static void add_actions(actiona& source, int bonus, int maximum) {
	actiona base = source;
	while(bonus > 0) {
		an.clear();
		for(auto i = Attune; i <= Wreck; i = (actionn)(i + 1)) {
			if(source.actions[i] >= maximum)
				continue;
			an.add(i, getname(i));
		}
		an.sort();
		auto n = (actionn)an.choose(getname(ChooseActionDot));
		source.actions[n]++;
		bonus--;
	}
}

static void add_start_actions(actiona& source, charactern type) {
	switch(type) {
	case Cutter:
		source.actions[Skirmish] += 2;
		source.actions[Command] += 1;
		break;
	default:
		break;
	}
}

static void add_player() {
	player->type = (charactern)choosev(0, Whisper, bsenum<charactern>::names, getname(ChoosePlaybook));
	player->heiretage = (heiretagen)choosev(0, Weird, bsenum<heiretagen>::names, getname(ChooseHeiretage));
	player->background = (backgroundn)choosev(0, Underworld, bsenum<backgroundn>::names, getname(ChooseBackground));
	add_start_actions(*player, player->type);
	add_actions(*player, 4, 2);
}

void add_players() {
	for(int i = 0; i < sizeof(players) / sizeof(players[0]); i++) {
		player = players + i;
		add_player();
	}
}