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

struct character;

enum commandn : unsigned char {
	Cancel, Continue,
	PageCharacter, PageSkills, PageItems, PageCombatants,
	LastCommand = PageCombatants,
};

enum picturen : unsigned char {
	ImageBoating, ImageCity, ImageCityElm, ImageWeapon, ImageTreasureFound,
	ImageHamlet, ImageInvestigateRoom, ImageKingFight, ImageParty, ImageSwearn,
	ImagePathfind, ImageTaint, ImageTreasure, ImageVillage
};

extern character* party[4];

extern const char* command_names[LastCommand + 1];

extern int last_number;

extern char roll_difficult, roll_dices[16];

void apply_result();
void create_game();
void pause();
void pause(const char* format);
