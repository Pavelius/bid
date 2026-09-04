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

typedef void(*fnevent)();

enum classn : unsigned char;

enum commandn : unsigned char {
	Cancel, Continue, ChangePlayer, ClearAllList, Confirm,
	PageCharacter, PageItems, PageCombatants,
};
enum actionn : unsigned char {
	NoAction,
	MakeCharge, MakeMeleeAttack, MakeMissileAttack, MakeThrownAttack, MakeRunAway,
	MakeHunting, MakeTreatIllness, MakeTendingWounds, MakeGearRepairing, MakeForaging,
	RestParty, MemorizeSpells, ChangeSpellsByLevel, ChangeSpellsByLevelAllowed,
	LeaveSettlement, MakeCamp,
};
enum reactionn : unsigned char {
	Hostile, Unfriendly, Neutral, Indifferent, Friendly,
};
enum globalvarn : unsigned char {
	Turns, Blessing,
};
enum picturen : unsigned char {
	ImageWasteland, ImageWastelandNight,
	ImagePlainVillage,
};

struct gamei {
	unsigned variables[Blessing+1];
	void add(globalvarn v, int i) { variables[v] += i; }
	unsigned get(globalvarn v) const { return variables[v]; }
};

extern gamei game;
extern reactionn last_reaction;
extern classn encounter_monsters;
extern int last_number;

void make_reaction_roll(int bonus);
void make_party_move();
long make_player_move(const char* cancel_text = 0);
void make_player_move(fnevent add_answers);
void pass_turn();
void pause();
void pause(const char* format);
