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

static actionn enemy_actions[3];
static actionn party_actions[3];

static void determine_disposition() {
	answer_header = "%Animal. %Conflict.";
	// Enemy disposition
	enemy_disposition = enemy_nature;
	enemy_disposition += make_roll_dices(enemy_nature, 0);
	// Player disposition
	party_disposition = imax(player->get(Nature), player->get(Fighter));
	party_disposition += make_roll(Fighter, 0, true);
}

static void choose_captain() {
	for(auto p : party) {
		if(p)
			an.add((long)p, p->name());
	}
	player = (character*)choose_answers(message_names[ChooseCaptain], 0, 0);
}

int make_conflict() {
	choose_captain();
	determine_disposition();
	while(enemy_disposition>0 && party_disposition > 0) {

	}
}