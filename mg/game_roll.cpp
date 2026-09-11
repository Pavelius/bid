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

struct rolluse {
	variant	type;
	character* player;
	int param;
	constexpr explicit operator bool() const { return player != 0; }
	void clear() { memset((void*)this, 0, sizeof(*this)); }
};

struct skilluse {
	skilln id;
	skilln basic;
	skillf help;
	traitf traits, penalty;
	wisef wises;
};

struct traituse {
	traitn id;
	skillf benefit, penalty;
};

struct wiseuse {
	wisen id;
	skillf help;
};

static skilluse skill_use[LastSkill + 1] = {
	{Nature},
	{Will},
	{Health},
	{Resources},
	{Circles},
	{}, {}, {},
	{Administrator, Will, {Archivist, Orator}},
	{Apiarist, Will, {Scientist, Insectrist, Loremouse}},
	{Archivist, Will, {Cartographer, Administrator}},
	{Armorer, Health, {Smith, Scientist}},
	{Baker, Health, {Scientist}},
	{Boatcrafter, Health, {Carpenter, Scientist}},
	{Brewer, Will, {}},
	{Carpenter, Health, {}},
	{Cartographer, Will, {}},
	{Cook, Will, {}},
	{Fighter, Health, {Hunter}},
	{Glazier, Health, {}},
	{Haggler, Will, {}},
	{Harvester, Health, {}},
	{Healer, Will, {}},
	{Hunter, Health, {}},
	{Insectrist, Will, {}},
	{Instructor, Will, {}},
	{Laborer, Health, {}},
	{Loremouse, Will, {}},
	{Manipulator, Will, {}},
	{Militarist, Will, {}},
	{Miller, Health, {}},
	{Orator, Will, {}},
	{Pathfinder, Will, {}},
	{Persuader, Will, {}},
	{Potter, Health, {}},
	{Scientist, Will, {}},
	{Scout, Will, {}},
	{Smith, Health, {}},
	{Stonemason, Health, {}},
	{Survivalist, Health, {}},
	{WeatherWatcher, Will, {}},
	{Weaver, Will, {}},
};

static traituse trait_use[LastTrait + 1] = {
	{Alert, {Apiarist, Fighter, Harvester, Healer, Hunter, Insectrist, Laborer, Militarist, Pathfinder, Survivalist}, {Manipulator, Orator, Administrator, Haggler, WeatherWatcher}},
	{Bigpaw, {Armorer, Carpenter, Fighter, Harvester, Hunter, Insectrist, Laborer, Militarist, Smith, Stonemason, Weaver}, {Administrator, Cook, Glazier, Potter, Cartographer}},
	{Bitter, {Haggler, Manipulator}, {Administrator, Orator, Manipulator, Persuader, Instructor}},
	{Bodyguard, {Fighter, Healer}, {Administrator, Instructor}},
	{Bold, {Fighter, Scout, Pathfinder, Haggler, Hunter, Laborer, Militarist}, {Fighter, Scout, Pathfinder, Haggler, Harvester, Hunter, Laborer, Militarist}},
	{Brave, {Apiarist, Fighter, Hunter, Insectrist, Militarist, Orator}, {Manipulator}},
	{Calm, {Administrator, Haggler, Healer, Manipulator, Scientist}, {Orator, Persuader}},
	{Clever, {Administrator, Apiarist, Archivist, Cartographer, Haggler, Healer, Insectrist, Loremouse, Manipulator, Militarist, Orator, Persuader, Scientist, WeatherWatcher}, {Fighter, Hunter, Harvester, Laborer}},
	{Compassionate, {Haggler, Orator, Healer, Persuader}, {Fighter, Insectrist, Manipulator, Loremouse}},
	{Cunning},
	{Curious},
	{DeepEar},
	{Defender},
	{Determined},
	{Driven},
	{EarlyRiser},
	{Extrovert},
	{Fat},
	{Fearful},
	{Fearless, {Fighter, Orator}, {Scout, Hunter, Haggler, Manipulator, Persuader}},
	{Fiery},
	{Generous},
	{Graceful},
	{GuardsHonor},
	{HardWorker},
	{Independent},
	{Innocent},
	{Inquisitive},
	{Jaded},
	{Leader},
	{Longtail},
	{Lost},
	{NaturalBearings},
	{Nimble},
	{Nocturnal},
	{Oldfur},
	{OpenMinded},
	{QuickWitted},
	{Quiet},
	{Rational},
	{Scarred},
	{SharpEyed},
	{Sharptooth},
	{Short},
	{Skeptical},
	{Skinny},
	{SteadyPaws},
	{Stoic},
	{Stubborn},
	{Suspicious},
	{Tall},
	{Thoughtful},
	{Tough},
	{WeatherSense},
	{Wise},
	{WolfsSnout},
	{Young},
};

static variant wise_context[LastWise + 1] = {
	Barkstone, Copperwood, Elmoss, Ivydale, Lockhaven, PortSumac, Shaleburrow, Sprucetuck,
	Forest, Lakes, Streams, TallGrass, Swamps, Mud, Thorns, LeafCover, RockyTerrain, Coast, OpenGround,
	Darkheather,
};

static rolluse roll_use[8];

static skilln roll_skill;

char roll_base, roll_result, roll_difficult, roll_dices[16];

static bool can_use_wise(wisen v) {
	auto context = wise_context[v];
	switch(context.type) {
	case Area: return context.value == location;
	default: return false;
	}
}

static bool can_use_trait(traitn v) {
	return false;
}

static bool use(variantn type) {
	for(auto& e : roll_use) {
		if(e && e.type.type == type)
			return &e;
	}
	return 0;
}

static int compare_dice(const void* v1, const void* v2) {
	return *((char*)v2) - *((char*)v1);
}

static void make_roll_dices(int count) {
	for(auto i = 0; i < count; i++)
		roll_dices[i] = 1 + rand() % 6;
	qsort(roll_dices, sizeof(roll_dices), sizeof(roll_dices[0]), compare_dice);
}

static rolluse* find_help(variant type, character* player, int param) {
	for(auto& e : roll_use) {
		if(e.type == type && e.player == player && e.param == param)
			return &e;
	}
	return 0;
}

static void add_help(variant type, character* player, int param) {
	auto p = find_help(type, player, param);
	if(!p) {
		p = roll_use;
		for(auto& e : roll_use) {
			if(!e) {
				p = &e;
				break;
			}
		}
	}
	p->type = type;
	p->player = player;
	p->param = param;
}

static void add_help() {
	add_help((short unsigned)hparam, (character*)hobject, 0);
	breakmodal(Continue);
}

static void add_benefit() {
	add_help((short unsigned)hparam, (character*)hobject, 1);
	breakmodal(Continue);
}

static void add_impende() {
	add_help((short unsigned)hparam, (character*)hobject, -1);
	breakmodal(Continue);
}

static skilln can_help(character* p, skilln skill) {
	if(p->skills[skill] > 0)
		return skill;
	for(auto i = (skilln)1; i <= LastSkill; i = (skilln)(i + 1)) {
		if(!p->skills[i])
			continue;
		if(skill_use[skill].help.is(i))
			return i;
	}
	return (skilln)0;
}

static void add_help_skill() {
	for(auto p : party) {
		if(!p)
			continue;
		if(p == player)
			continue; // Can't help itelf.
		if(p->parcipant())
			continue;
		auto n = can_help(p, roll_skill);
		if(!n)
			continue;
		if(find_help(variant(n), p, 0))
			continue;
		an.addp(add_help, variant(n), p, message_names[AskCanHelp], p->name(), skill_names[n]);
	}
}

static void add_help_iam_wise() {
	if(use(WiseVariant))
		return;
	for(auto p : party) {
		if(!p)
			continue;
		for(auto n = (wisen)0; n <= LastWise; n = (wisen)(n + 1)) {
			if(!can_use_wise(n))
				continue;
			if(p->is(n))
				an.addp(add_help, variant(n), p, message_names[AskCanHelpWise], p->name(), wise_names[n]);
		}
	}
}

static void add_help_trait() {
	if(use(Trait))
		return;
	for(auto n = (traitn)0; n <= LastTrait; n = (traitn)(n + 1)) {
		auto level = player->traits[n];
		if(level >= 3 || level == 0)
			continue;
		if(player->traits_use[n] >= level)
			continue;
		if(trait_use[n].benefit.is(roll_skill))
			an.addp(add_benefit, variant(n), player, message_names[AskTraitBenefit], player->name(), trait_names[n]);
		if(trait_use[n].penalty.is(roll_skill))
			an.addp(add_impende, variant(n), player, message_names[AskTraitImpende], player->name(), trait_names[n]);
	}
}

static traitn get_active_trait() {
	for(auto n = (traitn)0; n <= LastTrait; n = (traitn)(n + 1)) {
		auto level = player->traits[n];
		if(level == 0)
			continue;
		if(trait_use[n].penalty.is(roll_skill))
			return n;
	}
	return NoTrait;
}

static void update_roll_result() {
	roll_result = roll_base;
	auto ps = parcipants;
	for(auto& e : roll_use) {
		switch(e.type.type) {
		case WiseVariant: roll_result += 1; break;
		case Skill: roll_result += 1; *ps++ = e.player; break;
		case Trait: roll_result += e.param; break;
		default: break;
		}
	}
}

static void mark_trait_use() {
	for(auto& e : roll_use) {
		if(e.type.type == Trait)
			e.player->traits_use[e.param]++;
	}
}

static void mark_iam_wise_success() {
	for(auto& e : roll_use) {
		if(e.type.type == Trait)
			e.player->wises_success.set(e.param);
	}
}

static void mark_iam_wise_fail() {
	for(auto& e : roll_use) {
		if(e.type.type == Trait)
			e.player->wises_success.set(e.param);
	}
}

static void clear_roll_use() {
	memset(parcipants, 0, sizeof(parcipants));
	memset(roll_use, 0, sizeof(roll_use));
	breakmodal();
}

static gendern get_parcipant_gender() {
	if(parcipants[1] || !parcipants[0])
		return NoGender;
	else if(parcipants[0]->gender == Female)
		return Female;
	return Male;
}

static void fixgroup(stringbuilder& sb, messagen id) {
	pushvalue push(str_gender, get_parcipant_gender());
	sb.adds(message_names[id]);
}

static int get_dices_result(int margin) {
	auto r = 0;
	for(auto n : roll_dices) {
		if(n && n >= margin)
			r++;
	}
	return r;
}

static void apply_before_roll() {
	char temp[260]; stringbuilder sb(temp);
	while(true) {
		update_roll_result();
		sb.clear();
		sb.adds(message_names[MsgMakeRoll], player->name(), skill_names[roll_skill]);
		if(roll_difficult)
			sb.adds(message_names[MsgVsDifficult], roll_difficult);
		sb.add(".");
		if(parcipants[0])
			fixgroup(sb, MsgParcipants);
		sb.adds(message_names[MsgNumberDicesRoll], roll_result);
		add_help_trait();
		add_help_skill();
		add_help_iam_wise();
		if(roll_use[0])
			an.addp(clear_roll_use, 0, 0, message_names[AskClearAllAndStartAgain]);
		if(!choose_answers(temp, message_names[MakeRoll], 1))
			break; // Start roll
	}
}

static int failed_dices() {
	auto r = 0;
	for(auto n : roll_dices) {
		if(n && n < 4)
			r++;
	}
	return r;
}

static void apply_of_cource() {
	auto p = (character*)hobject;
	auto n = (traitn)hparam;
	p->add(PersonaPoints, -1);
	p->wises_ofcourse.set(n);
	// Reroll all failed dices
	for(auto& e : roll_dices) {
		if(e && e < 4)
			e = 1 + rand() % 6;
	}
	breakmodal();
}

static void add_of_course_wises() {
	if(!failed_dices())
		return;
	for(auto p : party) {
		if(!p)
			continue;
		if(!p->get(PersonaPoints))
			continue;
		for(auto n = (wisen)0; n <= LastWise; n = (wisen)(n + 1)) {
			if(!can_use_wise(n))
				continue;
			if(p->is(n))
				an.addp(apply_of_cource, n, p, message_names[AskUseOfCourseWise], p->name(), wise_names[n]);
		}
	}
}

static void apply_deeper_undestand() {
	auto p = (character*)hobject;
	auto n = (traitn)hparam;
	p->add(FatePoints, -1);
	p->wises_deeper.set(n);
	// Reroll all failed dices
	for(auto& e : roll_dices) {
		if(e && e < 4) {
			e = 1 + rand() % 6;
			break; // Just sigle one
		}
	}
	breakmodal();
}

static void add_deeper_undestand_wises() {
	if(!failed_dices())
		return;
	for(auto p : party) {
		if(!p)
			continue;
		if(!p->get(FatePoints))
			continue;
		for(auto n = (wisen)0; n <= LastWise; n = (wisen)(n + 1)) {
			if(!can_use_wise(n))
				continue;
			if(p->is(n))
				an.addp(apply_deeper_undestand, n, p, message_names[AskUseDeeperWise], p->name(), wise_names[n]);
		}
	}
}

static void add_break_tie_trait() {
	if(!roll_result) {
		auto trait = get_active_trait();
		if(trait != NoTrait)
			an.add(Tied, message_names[AskTraitBreakTie], trait_names[trait]);
	}
}

static int apply_after_roll() {
	char temp[260]; stringbuilder sb(temp);
	make_roll_dices(roll_result);
	while(true) {
		qsort(roll_dices, sizeof(roll_dices), sizeof(roll_dices[0]), compare_dice);
		roll_result = get_dices_result(4);
		sb.clear();
		sb.adds(message_names[MsgRollResult], roll_result);
		if(roll_difficult)
			sb.adds(message_names[MsgVsDifficult], roll_difficult);
		sb.add(".");
		if(roll_difficult > 0) {
			if(roll_result > roll_difficult)
				sb.adds("[+%1].", message_names[Passed]);
			else if(roll_result < roll_difficult)
				sb.adds("[-%1].", message_names[Failed]);
			else
				sb.adds("%1.", message_names[Tied]);
		}
		roll_result -= roll_difficult;
		add_break_tie_trait();
		add_of_course_wises();
		add_deeper_undestand_wises();
		auto result = choose_answers(temp, message_names[ApplyRollResult], 1);
		if(!result) {
			return roll_result;
		} else if(result == Tied) {
			player->add(FreeChecks, 2);
			return -1;
		}
	}
}

int make_roll(skilln skill, int difficult, bool mark_progress) {
	clear_roll_use();
	roll_skill = skill;
	roll_base = player->get(skill);
	roll_difficult = difficult;
	apply_before_roll();
	mark_trait_use();
	auto result = apply_after_roll();
	if(mark_progress) {
		if(result > 0)
			player->success[skill]++;
		else
			player->fail[skill]++;
	}
	if(result > 0)
		mark_iam_wise_success();
	else
		mark_iam_wise_fail();
	return result;
}

int make_roll_dices(int number, int difficult) {
	make_roll_dices(number);
	return imax(0, get_dices_result(4) - difficult);
}

int make_roll_silent(skilln skill, int difficult) {
	return make_roll_dices(player->get(skill), difficult);
}