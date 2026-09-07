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

enum creaturen : unsigned char {
	Commoner, Tenderpaws,
}

enum skilln : unsigned char {
	Nature, Will, Health, Resources, Circles,
	Apiarist, Archivist, Armorer, Baker, Boatcrafter,
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

typedef flagable<1+LastTrait/32> traitf;
typedef flagable<1+LastSkill/32> skillf;

extern const char* skill_names[LastSkill+1];

struct skillable {
	char skills[LastSkill+1];
	char fail[LastSkill+1];
	char success[LastSkill+1];
};

struct traitable {
	traitf traits, traits_upgraded;
};

struct creature {
	creaturen type;
	char nature;
};

struct character : creature, skillable, traitable {
	void update();
}
