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
#include "creature.h"
#include "game.h"
#include "message.h"
#include "slice.h"
#include "stringbuilder.h"

static skilln roll_skill;

char roll_base, roll_difficult, roll_dices[16];

static void clear_parcipant() {
	memset(parcipants, 0, sizeof(parcipants));
}

static long choose_before_roll() {
	char temp[260]; stringbuilder sb(temp);
	sb.adds(message_names[AskMakeRoll], player->name(), skill_names[roll_skill]);
	if(roll_difficult)
		sb.adds(message_names[AskVsDifficult], roll_difficult);
	sb.add(".");
	return choose_answers(temp, message_names[MakeRoll], 1);
}

void make_roll(skilln skill, int difficult) {
	clear_parcipant();
	roll_skill = skill;
	roll_base = player->get(skill);
	roll_difficult = difficult;
	choose_before_roll();
}



