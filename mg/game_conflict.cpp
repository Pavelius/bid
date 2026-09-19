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
#include "draw_atg.h"
#include "game.h"
#include "gender.h"
#include "math.h"
#include "message.h"
#include "pushvalue.h"
#include "rand.h"
#include "slice.h"
#include "stringbuilder.h"
#include "variant.h"

enum reactionn : unsigned char {
	NotRoll, Versus, Independed,
};

static reactionn reaction_type[Maneuver + 1][Maneuver + 1] = {
	{Independed, Versus, Independed, Versus},
	{Versus, Independed, NotRoll, Versus},
	{NotRoll, Independed, Versus, Independed},
	{Versus, Versus, Independed, Independed},
};

conflictn conflict;

int enemy_disposition, party_disposition;
int enemy_nature;

static int party_bonus, enemy_bonus;
static wearable enemy_weapons;

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

static void print_disposition() {
	sb.addn(message_names[MsgDisposition]);
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
		sb.clear();
		print_disposition();
		party_actor[i] = (character*)choose_answers(message_names[ChooseCaptain], 0, 0);
		party_actions[i] = (actionn)choose_value(Attack, Maneuver, 0, action_names, message_names[ChooseAction]);
	}
}

static skilln get_party_skill(actionn action) {
	return Fighter;
}

static void apply_result(actionn action, int value, bool enemy) {
	switch(action) {
	case Feint: case Attack:
		if(enemy)
			party_disposition -= value;
		else
			enemy_disposition -= value;
		break;
	case Defend:
		if(enemy)
			enemy_disposition += value;
		else
			party_disposition += value;
		break;
	case Maneuver:
		break;
	}
}

static void apply_action_result(int index) {
	auto action = party_actions[index];
	auto enemy_action = enemy_actions[index];
	auto skill = get_party_skill(action);
	auto enemy_roll = make_roll_dices(enemy_nature, 0);
	sb.clear();
	print_disposition();
	switch(reaction_type[action][enemy_action]) {
	case Versus:
		make_roll(skill, enemy_roll, false);
		if(roll_result > 0)
			apply_result(action, roll_result, false);
		else
			apply_result(enemy_action, -roll_result, true);
		break;
	case Independed:
		make_roll(skill, 0, false);
		if(roll_result > 0)
			apply_result(action, roll_result, false);
		if(enemy_roll > 0)
			apply_result(action, enemy_roll, true);
		break;
	case NotRoll:
		break;
	default:
		break;
	}
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