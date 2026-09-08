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
#include "message.h"
#include "rand.h"
#include "wise.h"

#define DSARR(N) N, sizeof(N)/sizeof(N[0])

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
	Archivist,  Cartographer, Smith,
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

static traitn kind_traits[] = {
	Bold, Generous
};

static traitn fearless_traits[] = {
	Fearless, Brave
};

void character::setbirth(creaturen v) {
	switch(type) {
	case Tenderpaw:
		xrand(1135, 1138);
		add(Will, 2);
		add(Health, 6);
		add(Pathfinder, 2);
		add(Scout, 2);
		add(Laborer, 2);
		break;
	case Guardmouse:
		xrand(1127, 1134);
		add(Will, 3);
		add(Health, 5);
		add(Fighter, 3);
		add(Haggler, 2);
		add(Scout, 2);
		add(Pathfinder, 3);
		add(Survivalist, 2);
		break;
	case PatrolGuard:
		xrand(1102, 1131);
		add(Will, 4);
		add(Health, 4);
		add(Cook, 2);
		add(Fighter, 3);
		add(Hunter, 3);
		add(Scout, 2);
		add(Healer, 2);
		add(Pathfinder, 2);
		add(Survivalist, 2);
		add(WeatherWatcher, 2);
		break;
	case PatrolLeader:
		xrand(1092, 1131);
		add(Will, 5);
		add(Health, 4);
		add(Fighter, 3);
		add(Hunter, 3);
		add(Instructor, 2);
		add(Loremouse, 2);
		add(Persuader, 2);
		add(Pathfinder, 3);
		add(Scout, 2);
		add(Survivalist, 3);
		add(WeatherWatcher, 2);
		break;
	case GuardCaptain:
		xrand(1092, 1111);
		add(Will, 6);
		add(Health, 3);
		add(Administrator, 3);
		add(Fighter, 3);
		add(Healer, 2);
		add(Hunter, 3);
		add(Instructor, 2);
		add(Militarist, 3);
		add(Orator, 2);
		add(Pathfinder, 3);
		add(Scout, 3);
		add(Survivalist, 3);
		add(WeatherWatcher, 3);
		break;
	default:
		xrand(1130, 1140);
		break;
	}
}

static void add_value(skilln v) {
	if(!player->skills[v])
		player->skills[v] = 2;
	else
		player->skills[v]++;
}

static void add_value(traitn v) {
	player->traits[v]++;
}

static void add_value(wisen v) {
	player->wises.set(v);
}

static skilln choose_skills(messagen id, slice<skilln> source) {
	for(auto v : source) {
		if(!v)
			continue;
		an.add(v, skill_names[v]);
	}
	an.sort();
	return (skilln)choose_answers(message_names[id], 0, -1);
}

static traitn choose_traits(messagen id, slice<traitn> source) {
	for(auto n : source)
		an.add(n, trait_names[n]);
	an.sort();
	return (traitn)choose_answers(message_names[id]);
}

static void add_traits(messagen id, slice<traitn> source) {
	add_value(choose_traits(id, source));
}

static void add_traits(messagen id, traitn t1, traitn t2) {
	adat<traitn, 4> source;
	source.add(t1);
	if(t2)
		source.add(t2);
	auto v = choose_traits(id, source);
	add_value(v);
}

static void choose_birth_place() {
	for(auto& e : city_data)
		an.add(e.area, area_names[e.area]);
	player->home = (arean)choose_answers(message_names[ChooseBirthPlace]);
	auto city_skill = choose_skills(ChooseBirthPlaceSkill, city_data[player->home].skills); add_value(city_skill);
	add_traits(ChooseBirthPlaceTrait, city_data[player->home].trait, city_data[player->home].trait_addition);
}

static void choose_skills(messagen id, slice<skilln> source, int count) {
	flagable<1 + LastSkill/32, unsigned> marked_skills;
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

character* add_character() {
	for(auto& e : character_data) {
		if(!e)
			return &e;
	}
	return character_data;
}

static void add_skill(skilln v) {
	if(player->skills[v])
		player->skills[v]++;
	else
		player->skills[v] = 2;
}

static void choose_rang() {
	player->type = (creaturen)choose_value(Tenderpaw, PatrolLeader, 0, creature_names, message_names[ChooseGuardRang], 0, false);
	player->setbirth(player->type);
}

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
	add_character(party, sizeof(party)/sizeof(party[0]), player);
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
	case Tenderpaw: player->add(Resources, 1); break;
	case Guardmouse: player->add(Resources, 2); break;
	case PatrolGuard: case PatrolLeader: player->add(Resources, 3); break;
	case GuardCaptain: player->add(Resources, 4); break;
	default: break;
	}
}

static void choose_nature() {
	player->add(Nature, 3);
	// Do you save for winter?
	switch(choose_question(DoYouSaveForWinter, Yes, No)) {
	case 1: player->add(Nature, 1); break;
	case 2: add_traits(ChooseTrait, kind_traits); break;
	default: break;
	}
	// Do you stand ground and fight?
	switch(choose_question(DoYouStandGroundAndFight, Yes, No)) {
	case 1: break;
	case 2: player->add(Nature, 1); player->add(Fighter, -1); break;
	default: break;
	}
	// Do you fear predators?
	switch(choose_question(DoYouFearPredators, Yes, No)) {
	case 1: player->add(Nature, 1); break;
	case 2: add_traits(ChooseTrait, fearless_traits); break;
	default: break;
	}
}

static void choose_wises() {
	add_value((wisen)choose_value(0, LastWise, 0, wise_names, message_names[ChooseWises], 0, true, -1));
}

void create_character() {
	player = add_character();
	choose_rang();
	add_party();
	choose_birth_place();
	choose_skills(ChooseLifeExperience, general_skills, get_life_experience());
	auto parent_skills = choose_skills(ChooseParentProffession, parent_profession_skills); add_skill(parent_skills);
	auto converse_skill = choose_skills(ChooseConversationSkills, conversation_skills); add_skill(converse_skill);
	auto senior_artisan = choose_skills(ChooseSeniorArtisanTeaching, senior_artisan_skills); add_skill(senior_artisan);
	choose_skills(ChooseMentorTeaching, mentor_stressing_skills, get_mentor_stressing());
	auto you_speciality = choose_skills(ChooseYouSpeciality, mentor_stressing_skills); add_skill(you_speciality);
	choose_nature();
	choose_wises();
	choose_resources();
	choose_circles();
}