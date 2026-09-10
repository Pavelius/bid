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
#include "collection.h"
#include "game.h"
#include "gender.h"
#include "message.h"
#include "pushvalue.h"
#include "rand.h"
#include "wise.h"

#define DSARR(N) N, sizeof(N)/sizeof(N[0])

typedef bool(*fntestname)(short unsigned);

struct cityskilli {
	arean area;
	skilln skills[4];
	traitn trait, trait_addition;
};

static cityskilli city_data[] = {
	{Barkstone, {Carpenter, Potter, Glazier}, SteadyPaws},
	{Copperwood, {Smith, Haggler}, Independent},
	{Elmoss, {Carpenter, Harvester}, Alert},
	{Ivydale, {Harvester, Baker}, HardWorker},
	{Lockhaven, {Weaver, Armorer}, Generous, GuardsHonor},
	{PortSumac, {Boatcrafter, WeatherWatcher}, Tough, WeatherSense},
	{Shaleburrow, {Stonemason, Harvester, Miller}, OpenMinded},
	{Sprucetuck, {Scientist, Loremouse}, Inquisitive, Rational},
};

static skilln general_skills[] = {
	Administrator, Glazier, Pathfinder,
	Apiarist, Haggler, Persuader,
	Archivist, Harvester, Potter,
	Armorer, Healer, Scientist,
	Baker, Hunter, Scout,
	Boatcrafter, Insectrist, Smith,
	Brewer, Instructor, Stonemason,
	Carpenter, Laborer, Survivalist,
	Cartographer, Loremouse, Cook, Militarist, WeatherWatcher,
	Manipulator, Miller, Weaver,
	Fighter, Orator
};

static skilln parent_profession_skills[] = {
	Apiarist, Carpenter, Potter,
	Archivist, Cartographer, Smith,
	Armorer, Glazier, Stonemason,
	Baker, Harvester, Weaver,
	Boatcrafter, Insectrist, Brewer, Miller
};

static skilln conversation_skills[] = {
	Manipulator, Orator, Persuader,
};

static skilln senior_artisan_skills[] = {
	Apiarist, Cartographer, Laborer,
	Archivist, Cook, Miller,
	Armorer, Glazier, Potter,
	Baker, Harvester, Smith,
	Brewer, Healer, Stonemason,
	Carpenter, Insectrist, Weaver,
};

static skilln mentor_stressing_skills[] = {
	Fighter, Instructor, Survivalist,
	Healer, Pathfinder, Hunter, Scout,
	WeatherWatcher
};

static traitn general_traits[] = {
	Bigpaw, Bitter, Bodyguard, Bold, Brave,
	Calm, Clever, Compassionate, Cunning, Curious,
	DeepEar, Defender, Determined, Driven, EarlyRiser,
	Extrovert, Fat, Fearful, Fearless, Fiery,
	Generous, Graceful, GuardsHonor, Innocent, Jaded,
	Leader, Longtail, Lost, NaturalBearings, Nimble,
	Nocturnal, Oldfur, QuickWitted, Quiet, Scarred,
	SharpEyed, Sharptooth, Short, Skeptical, Skinny,
	Stoic, Stubborn, Suspicious, Tall, Thoughtful,
	Tough, WeatherSense, Wise, WolfsSnout, Young
};

static traitn tenderpaw_traits[] = {
	Bigpaw, Brave, Calm, Clever, Compassionate,
	Curious, DeepEar, Defender, Determined, EarlyRiser,
	Extrovert, Fearful, Fearless, Fiery, Generous,
	Graceful, Longtail, Lost, NaturalBearings, Nimble,
	QuickWitted, Quiet, Scarred, Sharptooth, Short,
	Skeptical, Skinny, Stubborn, Suspicious, Tall,
	Tough, WolfsSnout
};

static traitn life_on_road_traits[] = {
	Bitter, Bodyguard, Brave, Calm, Clever,
	Compassionate, Cunning, Curious, Defender, Driven,
	EarlyRiser, Fearful, Fearless, Jaded, Leader,
	NaturalBearings, Nocturnal, Oldfur, Quiet, Scarred,
	SharpEyed, Skeptical, Skinny, Stoic, Thoughtful,
	Tough, WeatherSense, Wise
};

static traitn kind_traits[] = {
	Bold, Generous
};

static traitn fearless_traits[] = {
	Fearless, Brave
};

static wisen tenderpaw_wises[] = {
	CodeOfGuardWise, LegendsWise
};

static wisen guard_captain_wises[] = {
	LockhavenWise, MatriachWise
};

static colorn fur_colors[] = {
	Black, White, Gray, Brown
};

static colorn cloack_colors[] = {
	Black, Gray, Brown, Red, Green, Blue, Yellow
};

static int get_life_experience() {
	switch(player->type) {
	case Tenderpaw: case GuardCaptain: return 2;
	case Guardmouse: case PatrolGuard: case PatrolLeader: return 1;
	default: return 0;
	}
}

static int get_mentor_stressing() {
	switch(player->type) {
	case PatrolLeader: return 2;
	default: return 1;
	}
}

static int get_wises() {
	switch(player->type) {
	case Tenderpaw: return 1;
	case Guardmouse: return 1;
	case PatrolGuard: return 2;
	case PatrolLeader: return 3;
	case GuardCaptain: return 4;
	default: return 0;
	}
}

static void add_value(creaturen v) {
	switch(v) {
	case Tenderpaw:
		player->birth = xrand(1135, 1138);
		player->add(Will, 2);
		player->add(Health, 6);
		player->add(Pathfinder, 2);
		player->add(Scout, 2);
		player->add(Laborer, 2);
		break;
	case Guardmouse:
		player->birth = xrand(1127, 1134);
		player->add(Will, 3);
		player->add(Health, 5);
		player->add(Fighter, 3);
		player->add(Haggler, 2);
		player->add(Scout, 2);
		player->add(Pathfinder, 3);
		player->add(Survivalist, 2);
		break;
	case PatrolGuard:
		player->birth = xrand(1102, 1131);
		player->add(Will, 4);
		player->add(Health, 4);
		player->add(Cook, 2);
		player->add(Fighter, 3);
		player->add(Hunter, 3);
		player->add(Scout, 2);
		player->add(Healer, 2);
		player->add(Pathfinder, 2);
		player->add(Survivalist, 2);
		player->add(WeatherWatcher, 2);
		break;
	case PatrolLeader:
		player->birth = xrand(1092, 1131);
		player->add(Will, 5);
		player->add(Health, 4);
		player->add(Fighter, 3);
		player->add(Hunter, 3);
		player->add(Instructor, 2);
		player->add(Loremouse, 2);
		player->add(Persuader, 2);
		player->add(Pathfinder, 3);
		player->add(Scout, 2);
		player->add(Survivalist, 3);
		player->add(WeatherWatcher, 2);
		break;
	case GuardCaptain:
		player->birth = xrand(1092, 1111);
		player->add(Will, 6);
		player->add(Health, 3);
		player->add(Administrator, 3);
		player->add(Fighter, 3);
		player->add(Healer, 2);
		player->add(Hunter, 3);
		player->add(Instructor, 2);
		player->add(Militarist, 3);
		player->add(Orator, 2);
		player->add(Pathfinder, 3);
		player->add(Scout, 3);
		player->add(Survivalist, 3);
		player->add(WeatherWatcher, 3);
		break;
	default:
		player->birth = xrand(1130, 1140);
		break;
	}
}

static void add_value(skilln v, int value = 1) {
	if(value > 0) {
		if(!player->skills[v])
			player->skills[v] = 2;
		else
			player->skills[v] += value;
	} else if(value < 0) {
		value += player->skills[v];
		if(value < 0)
			value = 0;
		player->skills[v] = value;
	}
}

static void add_value(traitn v) {
	player->traits[v]++;
}

static void add_value(wisen v) {
	player->wises.set(v);
}

static skilln choose(messagen id, slice<skilln> source) {
	for(auto v : source) {
		if(!v)
			continue;
		an.add(v, skill_names[v]);
	}
	an.sort();
	return (skilln)choose_answers(message_names[id], 0, -1);
}

static traitn choose(messagen id, slice<traitn> source) {
	for(auto n : source)
		an.add(n, trait_names[n]);
	an.sort();
	return (traitn)choose_answers(message_names[id], 0, -1);
}

static traitn choose(messagen id, traitn t1, traitn t2) {
	adat<traitn, 4> source;
	source.add(t1);
	if(t2)
		source.add(t2);
	return choose(id, source);
}

static void choose_birth_place() {
	for(auto& e : city_data)
		an.add(e.area, area_names[e.area]);
	player->home = (arean)choose_answers(message_names[ChooseBirthPlace]);
	add_value(choose(ChooseBirthPlaceSkill, city_data[player->home].skills));
	add_value(choose(ChooseBirthPlaceTrait, city_data[player->home].trait, city_data[player->home].trait_addition));
}

static void choose(messagen id, slice<skilln> source, int count) {
	flagable<1 + LastSkill / 32, unsigned> marked_skills;
	marked_skills.clear();
	for(auto i = 0; i < count; i++) {
		for(auto v : source) {
			if(marked_skills.is(v))
				continue;
			an.add(v, skill_names[v]);
		}
		an.sort();
		auto v = (skilln)choose_answers(message_names[id], 0, -1);
		add_value(v);
		marked_skills.set(v);
	}
}

static colorn choose(messagen id, slice<colorn> source) {
	for(auto v : source)
		an.add(v, color_names[v]);
	an.sort();
	return (colorn)choose_answers(message_names[id], 0, -1);
}

static character* new_character() {
	for(auto& e : character_data) {
		if(!e)
			return &e;
	}
	return character_data;
}

static creature* new_creature() {
	for(auto& e : creature_data) {
		if(!e)
			return &e;
	}
	return creature_data;
}

static void add_skill(skilln v) {
	if(player->skills[v])
		player->skills[v]++;
	else
		player->skills[v] = 2;
}

static void choose_rang() {
	player->type = (creaturen)choose_value(Tenderpaw, PatrolLeader, 0, creature_names, message_names[ChooseGuardRang], 0, false);
	add_value(player->type);
}

static void add_character(character** source, int count, character* player) {
	if(!player)
		return;
	for(auto i = 0; i < count; i++) {
		if(source[i])
			continue;
		source[i] = player;
		break;
	}
}

void add_party() {
	add_character(party, sizeof(party) / sizeof(party[0]), player);
}

static int choose_question(messagen id, messagen v1, messagen v2) {
	an.add(1, message_names[v1]);
	an.add(2, message_names[v2]);
	return choose_answers(message_names[id]);
}

static void choose_resources() {
	switch(player->type) {
	case Tenderpaw: player->add(Resources, 1); break;
	case Guardmouse: player->add(Resources, 2); break;
	case PatrolGuard: player->add(Resources, 3); break;
	case PatrolLeader: player->add(Resources, 4); break;
	case GuardCaptain: player->add(Resources, 5); break;
	default: break;
	}
}

static void choose_circles() {
	switch(player->type) {
	case Tenderpaw: player->add(Circles, 1); break;
	case Guardmouse: player->add(Circles, 2); break;
	case PatrolGuard: case PatrolLeader: player->add(Circles, 3); break;
	case GuardCaptain: player->add(Circles, 4); break;
	default: break;
	}
}

static void choose_nature() {
	player->add(Nature, 3);
	// Do you save for winter?
	switch(choose_question(DoYouSaveForWinter, Yes, No)) {
	case 1: add_value(Nature, 1); break;
	case 2: add_value(choose(ChooseTrait, kind_traits)); break;
	default: break;
	}
	// Do you stand ground and fight?
	switch(choose_question(DoYouStandGroundAndFight, Yes, No)) {
	case 1: break;
	case 2: add_value(Nature, 1); add_value(Fighter, -1); break;
	default: break;
	}
	// Do you fear predators?
	switch(choose_question(DoYouFearPredators, Yes, No)) {
	case 1: add_value(Nature, 1); break;
	case 2: add_value(choose(ChooseTrait, fearless_traits)); break;
	default: break;
	}
}

static wisen choose_wises(messagen id, slice<wisen> source) {
	for(auto n : source)
		an.add(n, wise_names[n]);
	an.sort();
	return (wisen)choose_answers(message_names[id], 0, -1);
}

static void choose_wises(messagen id, int count) {
	flagable<1 + LastSkill / 32, unsigned> marked;
	for(auto i = 0; i < count; i++) {
		for(auto n = (wisen)0; n <= LastWise; n = (wisen)(n + 1)) {
			if(marked.is(n))
				continue;
			an.add(n, wise_names[n]);
		}
		auto v = (wisen)choose_answers(message_names[id], 0, -1);
		marked.set(v);
		add_value(v);
	}
}

static void choose_wises() {
	switch(player->type) {
	case Tenderpaw:
		add_value(choose_wises(ChooseWises, tenderpaw_wises));
		break;
	case GuardCaptain:
		add_value(choose_wises(ChooseWises, guard_captain_wises));
		choose_wises(ChooseWises, get_wises() - 1);
		break;
	default:
		choose_wises(ChooseWises, get_wises());
		break;
	}
}

static void choose_traits() {
	add_value(choose(ChooseTrait, general_traits));
	switch(player->type) {
	case Tenderpaw:
		add_value(choose(ChooseTrait, tenderpaw_traits));
		break;
	case GuardCaptain: case PatrolLeader:
		add_value(choose(ChooseTrait, life_on_road_traits));
		break;
	default:
		break;
	}
}

static bool is_party_name(unsigned char v) {
	for(auto p : party) {
		if(p && p->customname == v)
			return true;
	}
	return false;
}

static void choose_name() {
	static unsigned char start_by_gender[Female + 1] = {0, 0, name_count_per_gender};
	collection source;
	auto gender = player->gender;
	source.select(start_by_gender[gender], start_by_gender[gender] + name_count_per_gender - 1, is_party_name, false);
	source.shuffle();
	source.top(10);
	for(auto v : source)
		an.add(v, name_names[v]);
	an.sort();
	player->customname = (unsigned char)choose_answers(message_names[ChooseName]);
}

static unsigned char random_name() {
	collection source;
	source.select(0, name_count_per_gender - 1, 0, false);
	return source.random();
}

static creature* create_creature(gendern gender, creaturen type, skilln skill) {
	auto p = new_creature();
	p->clear();
	p->gender = gender;
	p->type = type;
	p->customname = random_name();
	return p;
}

void create_character() {
	player = new_character();
	player->clear();
	choose_name();
	choose_rang();
	add_party();
	choose_birth_place();
	choose(ChooseLifeExperience, general_skills, get_life_experience());
	auto parent_skills = choose(ChooseParentProffession, parent_profession_skills); add_skill(parent_skills);
	player->conversation = choose(ChooseConversationSkills, conversation_skills); add_skill(player->conversation);
	auto senior_artisan = choose(ChooseSeniorArtisanTeaching, senior_artisan_skills); add_skill(senior_artisan);
	choose(ChooseMentorTeaching, mentor_stressing_skills, get_mentor_stressing());
	player->speciality = choose(ChooseYouSpeciality, mentor_stressing_skills); add_skill(player->speciality);
	player->parent = create_creature(Male, Commoner, parent_skills)->index();
	choose_nature();
	choose_wises();
	choose_resources();
	choose_circles();
	choose_traits();
	player->skin = choose(ChooseFurColor, fur_colors);
	player->ornament = choose(ChooseCloackColor, cloack_colors);
}

void create_character_silent() {
	pushvalue push(answers::interactive, false);
	create_character();
}