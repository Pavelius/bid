#include "answers.h"
#include "character.h"
#include "choose.h"
#include "game.h"
#include "message.h"
#include "scene.h"

character*	player;
character*	roll_help;
character	players[3];

actionn attributes[3][4] = {
	{Hunt, Study, Survey, Tinker},
	{Finesse, Prowl, Skirmish, Wreck},
	{Attune, Command, Consort, Sway},
};

bool allow_character(int v) {
	for(auto& e : players) {
		if(e.type == v)
			return false;
	}
	return true;
}

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

void actiona::apply(charactern type) {
	switch(type) {
	case Cutter:
		actions[Skirmish] += 2;
		actions[Command] += 1;
		break;
	case Hound:
		actions[Hunt] += 2;
		actions[Survey] += 1;
		break;
	case Leech:
		actions[Tinker] += 2;
		actions[Wreck] += 1;
		break;
	case Lurk:
		actions[Prowl] += 2;
		actions[Finesse] += 1;
		break;
	case Slide:
		actions[Sway] += 2;
		actions[Consort] += 1;
		break;
	case Spider:
		actions[Consort] += 2;
		actions[Study] += 1;
		break;
	case Whisper:
		actions[Attune] += 2;
		actions[Study] += 1;
		break;
	default:
		break;
	}
}

static void add_player() {
	player->clear();
	player->type = (charactern)choosev(0, Whisper, allow_character, bsenum<charactern>::names, getname(ChoosePlaybook));
	player->apply(player->type);
	player->heiretage = (heiretagen)choosev(0, Weird, bsenum<heiretagen>::names, getname(ChooseHeiretage));
	player->background = (backgroundn)choosev(0, Underworld, bsenum<backgroundn>::names, getname(ChooseBackground));
	add_actions(*player, 4, 2);
}

void add_players() {
	for(int i = 0; i < sizeof(players) / sizeof(players[0]); i++) {
		player = players + i;
		add_player();
	}
}

void character::clear() {
	memset(this, 0, sizeof(*this));
}

int character::get(attributen i) const {
	auto result = 0;
	for(auto v : attributes[i]) {
		if(actions[v])
			result++;
	}
	return result;
}

int	character::getindex() const {
	return this - players;
}

character* character::ally(int number) const {
	return 0;
}

bool character::apply(actionn action, messagen command, bool run) {
	switch(command) {
	case ActionRollHelp:
		if(roll_help)
			return false;
		if(!canstress(1))
			return false;
		if(run) {
			roll_help = this;
			stress += 1;
			roll_dices++;
		}
		break;
	case MakeRoll:
		return true;
	default:
		return false;
	}
	return true;
}