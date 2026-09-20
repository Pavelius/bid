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

struct item;

typedef bool(*fnaction)(bool run);
typedef void(*fnevent)();
typedef int(*fnitemget)(const item& v);

enum arean : unsigned char;
enum classn : unsigned char;

enum actionn : unsigned char {
	MakeCharge, MakeMeleeAttack, MakeMissileAttack, MakeThrownAttack, MakeRunAway,
	MakeHunting, MakeTreatIllness, MakeTendingWounds, MakeGearRepairing, MakeForaging,
	RestParty, MemorizeSpells, ChangeSpellsByLevel, ChangeSpellsByLevelAllowed,
	LeaveSettlement, LeaveOutside, LeaveBack,
	BuyTradeGoods, SellTradeGoods, GatherInformation, RentRoomOnNight, MakeCamp,
	LastAction = MakeCamp
};
enum messagen : unsigned char {
	Cancel, Continue, GameTitle, ClearAllList, Confirm,
	PageCharacter, PageItems, PageCombatants,
	MsgAnd,
	CoinsPl, CoinsCP, CoinsSP, CoinsEP, CoinsGP, CoinsPP,
	PlayerCharged,
	PlayerCriticalMiss, PlayerMiss, PlayerHit, PlayerCriticalHit,
	MsgDamage, MsgDamageAndDead,
	PlayerStunned, PlayerSufferStarvation,
	PlayerTreatedIllness, PlayerTreatedWounds, PlayerForageItem, PlayerHuntingGame, PlayerRepairGear,
	WeaponBroken, WeaponDamage,
	PlayerMemorizeSpells,
	PlayerJumpFromBrush, PlayerJumpFromTree,
	PartyMakeCamp,
	MakeCampInSafeCave, MakeCampInOpenLand,
	CampNightEnd, AdventureNextDay, PlayerHearNoiseOnWatch,
	SearchBodies, NothingValuableHere,
	StateWounded,
	PlayerThink, PlayerSay, PlayerCry,
	SayWhoIsHere,
	BuyItemForCost, SellItemForCost, AvailableCount,
	AskLeft, AskRoadAdventure, AskEnterSettlement, AskMemorizeSpells, AskWhatToDoPlayer, AskWhatToDo,
	LastMessage = AskWhatToDo,
};
enum reactionn : unsigned char {
	Hostile, Unfriendly, Neutral, Indifferent, Friendly,
};
enum variablen : unsigned char {
	Turns, Reputation, Blessing, PartyCoins,
};
enum picturen : unsigned char {
	ImageWasteland, ImageWastelandNight, ImagePlains, ImagePlainsNight, ImageForest, ImageForestNight,
	ImageVillage, ImageSmallCity, ImageLargeCity,
	ImageVillageMarket, ImageTavern, ImageHotel, ImageTemple,
};

extern const char* action_names[LastAction + 1];
extern const char* message_names[LastMessage + 1];

extern unsigned game_var[PartyCoins + 1];

extern reactionn last_reaction;
extern classn encounter_monsters;
extern int last_number;

picturen getimage(arean v);
picturen getimagenight(arean v);

void add_look();
void addhdr(picturen picture);
void addhdr(picturen picture, const char* header);
void adds(messagen id);
void addn(messagen id);
void addopt(actionn n);
void addopt(arean v);
void addopt(const item& e, messagen v, fnitemget price);
void breakactions(int level = 1);
void clear_messages();
bool chance(int v);
long choose_answers_random();
long choose_party_option(const char* cancel_text);
long choose_player_option(const char* cancel_text);
void combat_encounter();
void create_market_items();
bool doactions();
void make_any_player_move(const char* cancel_text = 0);
void make_reaction_roll(int bonus);
void pass_turn();
void pause();
void pause(const char* format);
void generate_world();
