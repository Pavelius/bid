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
#include "variant.h"
#include "wise.h"

struct skillusei {
	skilln	skill;
	char	factors;
	skillf	help;
};
static skillusei skill_roll_data[] = {
	{Administrator, 2, {Archivist, Orator}},
	{Apiarist, 3, {Scientist, Insectrist, Loremouse}},
	{Archivist, 2, {Cartographer, Administrator}},
	{Armorer, 2, {Smith, Scientist}},
	{Baker, 2, {Scientist}},
	{Boatcrafter, 2, {Carpenter, Scientist}},
	{Fighter, 0, {Hunter}},
	//	Brewer, Carpenter, Cartographer, Cook, Fighter,
	//	Glazier, Haggler, Harvester, Healer, Hunter,
	//	Insectrist, Instructor, Laborer, Loremouse, Manipulator,
	//	Militarist, Miller, Orator, Pathfinder, Persuader,
	//	Potter, Scientist, Scout, Smith, Stonemason,
	//	Survivalist, WeatherWatcher, Weaver,
};

struct wiseusei {
	wisen	skill;
	skillf	help;
};
static wiseusei wise_use_data[] = {
	{LegendsWise, {Manipulator, Orator, Persuader, Scientist, Insectrist, Loremouse, Hunter, Pathfinder}},
};

static skilln roll_skill;
static bool wise_used;

char roll_base, roll_difficult, roll_dices[16];

static skillusei* find_use(skilln v) {
	for(auto& e : skill_roll_data) {
		if(e.skill == v)
			return &e;
	}
	return 0;
}

static void clear_parcipant() {
	memset(parcipants, 0, sizeof(parcipants));
}

static skilln can_help(character* p, skilln skill) {
	if(p->skills[skill] > 0)
		return skill;
	auto ps = find_use(skill);
	if(!ps)
		return (skilln)0;
	for(auto i = (skilln)1; i <= LastSkill; i = (skilln)(i + 1)) {
		if(!p->skills[i])
			continue;
		if(ps->help.is(i))
			return i;
	}
	return (skilln)0;
}

static void add_help_skill_command() {
	auto skill = (skilln)hparam;
	auto p = (character*)hobject;
	add_parcipant(p);
	roll_base++;
	breakmodal(Continue);
}

static void add_help_skill() {
	for(auto p : party) {
		if(!p)
			continue;
		if(p == player)
			continue;
		if(p->parcipant())
			continue;
		auto help_skill = can_help(p, roll_skill);
		if(!help_skill)
			continue;
		an.addp(add_help_skill_command, help_skill, p, message_names[AskCanHelp], p->name(), skill_names[help_skill]);
	}
}

static void add_help_iam_wise_command() {
	auto skill = (wisen)hparam;
	auto p = (character*)hobject;
	roll_base++;
	wise_used = true;
	breakmodal(Continue);
}

static void add_help_iam_wise() {
	if(wise_used)
		return;
	for(auto p : party) {
		if(!p)
			continue;
		for(auto n = (wisen)0; n <= LastWise; n = (wisen)(n + 1)) {
			if(p->is(n))
				an.addp(add_help_iam_wise_command, n, p, message_names[AskCanHelpWise], p->name(), wise_names[n]);
		}
	}
}

static long choose_before_roll() {
	char temp[260]; stringbuilder sb(temp);
	sb.adds(message_names[MsgMakeRoll], player->name(), skill_names[roll_skill]);
	if(roll_difficult)
		sb.adds(message_names[MsgVsDifficult], roll_difficult);
	sb.add(".");
	sb.adds(message_names[MsgNumberDicesRoll], roll_base);
	add_help_skill();
	add_help_iam_wise();
	return choose_answers(temp, message_names[MakeRoll], 1);
}

static void apply_before_roll() {
	while(true) {
		auto result = choose_before_roll();
		if(!result)
			break; // Start roll
	}
}

void make_roll(skilln skill, int difficult) {
	clear_parcipant();
	wise_used = false;
	roll_skill = skill;
	roll_base = player->get(skill);
	roll_difficult = difficult;
	apply_before_roll();
}



