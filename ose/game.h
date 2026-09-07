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

typedef bool(*fnaction)(bool run);
typedef void(*fnevent)();

enum classn : unsigned char;

enum commandn : unsigned char {
	Cancel, Continue, ClearAllList, Confirm,
	PageCharacter, PageItems, PageCombatants,
};
enum actionn : unsigned char {
	MakeCharge, MakeMeleeAttack, MakeMissileAttack, MakeThrownAttack, MakeRunAway,
	MakeHunting, MakeTreatIllness, MakeTendingWounds, MakeGearRepairing, MakeForaging,
	RestParty, MemorizeSpells, ChangeSpellsByLevel, ChangeSpellsByLevelAllowed,
	LeaveSettlement, LeaveOutside, LeaveBack,
	BuyTradeGoods, SellTradeGoods, GatherInformation, MakeCamp,
	LastAction = MakeCamp
};
enum reactionn : unsigned char {
	Hostile, Unfriendly, Neutral, Indifferent, Friendly,
};
enum globalvarn : unsigned char {
	Turns, Blessing, PartyCoins,
};
enum picturen : unsigned char {
	ImageWasteland, ImageWastelandNight,
	ImagePlainVillage, ImageVillageMarket, ImageTavern
};

extern const char* action_names[LastAction + 1];
extern const char* command_names[PageCombatants + 1];

struct gamei {
	unsigned variables[PartyCoins + 1];
	void add(globalvarn v, int i) { variables[v] += i; }
	unsigned get(globalvarn v) const { return variables[v]; }
};

extern gamei game;
extern reactionn last_reaction;
extern classn encounter_monsters;
extern int last_number;

void addopt(actionn n);
void addoptn(actionn n);
void apply_result();
bool apply_camp(actionn v, bool run);
bool apply_combat(actionn v, bool run);
bool apply_settlement(actionn v, bool run);
void area_move();
long choose_player_option(const char* cancel_text);
void create_market_items();
void make_any_player_move(const char* cancel_text = 0);
void make_reaction_roll(int bonus);
void make_party_move(const char* cancel_text = 0);
void make_player_move(const char* cancel_text = 0);
void pass_turn();
void pause();
void pause(const char* format);
