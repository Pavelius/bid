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
#include "message.h"
#include "rand.h"

character* player;
character character_data[32];

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

void character::update() {
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
	auto v = (creaturen)choose_value(Tenderpaw, PatrolLeader, 0, creature_names, message_names[ChooseGuardRang], 0, false);
	player->setbirth(v);
}

void create_character() {
	player = add_character();
	choose_rang();
}