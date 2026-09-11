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
#include "variant.h"

class stringbuilder;

enum arean : unsigned char;
enum gendern : unsigned char;
enum wisen : unsigned char;

enum creaturen : unsigned char {
	Mouse,
	Tenderpaw, Guardmouse, PatrolGuard, PatrolLeader, GuardCaptain,
	Snake, Weasel,
	LastCreature = Weasel
};
enum skilln : unsigned char {
	Nature, Will, Health, Resources, Circles,
	FreeChecks, PersonaPoints, FatePoints,
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
	LastTrait = Young,
	NoTrait = 0xFF
};
enum wisen : unsigned char {
	BarkstoneWise, CopperwoodWise, ElmossWise, IvydaleWise, LockhavenWise, PortSumacWise, ShaleburrowWise, SprucetuckWise,
	ForestWise, LakesWise, StreamsWise, TallGrassWise, SwampsWise, MudWise, ThornsWise, LeafCoverWise, RockyTerrainWise, CoastWise, OpenGroundWise,
	DarkheatherWise, CodeOfGuardWise, MatriachWise, LegendsWise,
	LastWise = LegendsWise,
	NoWise = 0xFF,
};
enum conditionn : unsigned char {
	HungryAndThirsty, Angry, Tired, Injured, Sick,
};
enum colorn : unsigned char {
	NoColor, White, Black, Gray,
	Red, Green, Blue, Yellow, Brown,
	LastColor = Brown,
};

typedef flagable<1 + LastTrait / 32, unsigned> traitf;
typedef flagable<1 + LastSkill / 32, unsigned> skillf;
typedef flagable<1 + LastSkill / 32, unsigned> skillf;
typedef flagable<1, unsigned char> conditionf;
typedef flagable<1, unsigned> wisef;

const int name_count_per_gender = 33;

extern const char* color_names[LastColor + 1];
extern const char* creature_names[LastCreature + 1];
extern const char* name_names[name_count_per_gender * 2];
extern const char* skill_names[LastSkill + 1];
extern const char* trait_names[LastTrait + 1];
extern const char* wise_names[LastWise + 1];

struct skillable {
	char skills[LastSkill + 1];
	char fail[LastSkill + 1];
	char success[LastSkill + 1];
	int get(skilln v) const { return skills[v]; }
};

struct creature {
	creaturen		type;
	char			nature;
	gendern			gender;
	unsigned char	customname; // 0xFF if no custom name.
	short unsigned	birth;
	skilln			speciality;
	colorn			skin, ornament;
	unsigned char	parent; // Your father
	constexpr explicit operator bool() const { return birth != 0; }
	const char* name() const { return (customname == 0xFF) ? creature_names[type] : name_names[customname]; }
	unsigned char index() const;
	void clear();
};

struct character : creature, skillable, wearable {
	arean		home;
	skilln		conversation;
	char		traits[LastTrait + 1], traits_use[LastTrait + 1];
	wisef		wises, wises_success, wises_fail, wises_deeper, wises_ofcourse;
	conditionf	conditions;
	unsigned char index() const;
	int get(skilln v) const { return skills[v]; };
	void add(skilln v, int i) { skills[v] += i; }
	void add(wisen v) { wises.set(v); }
	bool allow(traitn v) const;
	void clear();
	void info(stringbuilder& sb) const;
	bool is(skilln v) const { return skills[v] > 0; }
	bool is(conditionn v) const { return conditions.is(v); }
	bool is(traitn v) const { return traits[v] > 0; }
	bool is(wisen v) const { return wises.is(v); }
	bool parcipant() const;
	void setname();
};

extern creaturen animal;
extern character* player;
extern character* party[4];
extern character* parcipants[4];
extern character character_data[32];
extern creature	creature_data[128];

void add_party();
void create_character();
void create_character_silent();

int make_conflict();
int make_roll_dices(int number, int difficult);
int make_roll(skilln skill, int difficult, bool mark_progress = true);
int make_roll_silent(skilln skill, int difficult);

