/*
	Copyright 2026 by Pavel Chistyakov

	Licensed under the Apache License, Version 2.0 (the "License");
	you may not use this file except in compliance with the License.
	You may obtain a copy of the License at

	http://www.apache.org/licenses/LICENSE-2.0

	Unless required by applicable law or agreed to in writing, software
	distributed under the License is distributed on an "AS IS" BASIS,
	WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
	See the License for the specific language governing permissions and
	limitations under the License.77
*/

#include "answers.h"
#include "area.h"
#include "creature.h"
#include "game.h"
#include "gender.h"
#include "math.h"
#include "message.h"
#include "pushvalue.h"
#include "rand.h"
#include "slice.h"
#include "stringbuilder.h"
#include "variant.h"

conflictn conflict;

int enemy_disposition;
int enemy_nature;
int party_disposition;

static character* captain;
static character* party_actor[3];
static actionn enemy_actions[3];
static actionn party_actions[3];

static int get_nature(creaturen type) {
	switch(type) {
	case Snake: return 6;
	default: return 0;
	}
}

static void determine_disposition() {
	// Enemy disposition
	enemy_disposition = enemy_nature;
	enemy_disposition += make_roll_dices(enemy_nature, 0);
	// Player disposition
	player = captain;
	party_disposition = imax(player->get(Nature), player->get(Fighter));
	party_disposition += make_roll(Fighter, 0, true);
}

static character* choose_party_member(messagen id) {
	for(auto p : party) {
		if(p)
			an.add((long)p, p->name());
	}
	return (character*)choose_answers(message_names[id], 0, 0);
}

static bool is_party_actor(const character* pv) {
	for(auto p : party_actor) {
		if(p == pv)
			return true;
	}
	return false;
}

static void choose_party_actions() {
	memset(party_actions, 0, sizeof(party_actions));
	memset(party_actor, 0, sizeof(party_actor));
	for(auto i = 0; i < 3; i++) {
		for(auto p : party) {
			if(!p || is_party_actor(p))
				continue;
			an.add((long)p, p->name());
		}
		party_actor[i] = (character*)choose_answers(message_names[ChooseCaptain], 0, 0);
	}
}

static void apply_action_result(int index) {
}

int make_conflict() {
	answer_header = "%Animal. %Conflict.";
	enemy_nature = get_nature(animal);
	captain = choose_party_member(ChooseCaptain);
	determine_disposition();
	while(enemy_disposition > 0 && party_disposition > 0) {
		choose_party_actions();
		apply_action_result(1);
		apply_action_result(2);
		apply_action_result(3);
	}
	return 0;
}