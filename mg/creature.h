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

#pragma once

#include "flagable.h"
#include "item.h"

enum arean : unsigned char;

enum creaturen : unsigned char {
	Commoner,
	Tenderpaw, Guardmouse, PatrolGuard, PatrolLeader, GuardCaptain,
	LastCreature = GuardCaptain
};

enum skilln : unsigned char {
	Nature, Will, Health, Resources, Circles,
	Administrator, Apiarist, Archivist, Armorer, Baker, Boatcrafter,
	Brewer, Carpenter, Cartographer, Cook, Fighter,
	Glazier, Haggler, Harvester, Healer, Hunter,
	Insectrist, Instructor, Laborer, Loremouse, Manipulator,
	Militarist, Miller, Orator, Pathfinder, Persuader,
	Potter, Scientist, Scout, Smith, Stonemason,
	Survivalist, WeatherWatcher, Weaver,
	LastSkill = Weaver
};

enum traitn : unsigned char {
	Alert, Bigpaw, Bitter, Bodyguard, Bold,
	Brave, Calm, Clever, Compassionate, Cunning,
	Curious, DeepEar, Defender, Determined, Driven,
	EarlyRiser, Extrovert, Fat, Fearful, Fearless,
	Fiery, Generous, Graceful, GuardsHonor, HardWorker,
	Independent, Innocent, Inquisitive, Jaded, Leader,
	Longtail, Lost, NaturalBearings, Nimble, Nocturnal,
	Oldfur, OpenMinded, QuickWitted, Quiet, Rational,
	Scarred, SharpEyed, Sharptooth, Short, Skeptical,
	Skinny, SteadyPaws, Stoic, Stubborn, Suspicious,
	Tall, Thoughtful, Tough, WeatherSense, Wise,
	WolfsSnout, Young,
	LastTrait = Young
};

enum conditionn : unsigned char {
	HungryAndThirsty, Angry, Tired, Injured, Sick,
};

typedef flagable<1 + LastTrait / 32, unsigned> traitf;
typedef flagable<1 + LastSkill / 32, unsigned> skillf;
typedef flagable<1 + LastSkill / 32, unsigned> skillf;
typedef flagable<1, unsigned char> conditionf;

extern const char* creature_names[LastCreature + 1];
extern const char* skill_names[LastSkill + 1];

struct skillable {
	char skills[LastSkill + 1];
	char fail[LastSkill + 1];
	char success[LastSkill + 1];
	int get(skilln v) const { return skills[v]; }
};

struct creature {
	creaturen type;
	char nature;
	short unsigned birth;
	constexpr explicit operator bool() const { return birth != 0; }
	void setbirth(creaturen type);
};

struct character : creature, skillable, wearable {
	arean home;
	skilln speciality;
	traitf traits, traits_upgraded;
	conditionf conditions;
	void add(skilln v, int i) { skills[v] += i; }
	bool is(skilln v) const { return skills[v] > 0; }
	bool is(traitn v) const { return traits.is(v); }
	bool is(conditionn v) const { return conditions.is(v); }
	void setbirth(creaturen v);
	void update();
};
extern character* player;
extern character character_data[32];
extern creature	creature_data[64];

void create_character();