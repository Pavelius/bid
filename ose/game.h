#pragma once

typedef void(*fnevent)();

enum commandn : unsigned char {
	Cancel, Continue, ChangePlayer, ClearAllList, Confirm,
	PageCharacter, PageItems, PageCombatants,
};
enum actionn : unsigned char {
	NoAction,
	MakeCharge, MakeMeleeAttack, MakeMissileAttack, MakeThrownAttack, MakeRunAway,
	MakeHunting, MakeTreatIllness, MakeTendingWounds, MakeGearRepairing, MakeForaging,
	RestParty, MemorizeSpells, ChangeSpellsByLevel, ChangeSpellsByLevelAllowed,
	MakeCamp,
};
enum reactionn : unsigned char {
	Hostile, Unfriendly, Neutral, Indifferent, Friendly,
};
enum globalvarn : unsigned char {
	Turns, Blessing,
};
enum picturen : unsigned char {
	ImageWasteland, ImageWastelandNight,
};

struct gamei {
	unsigned variables[Blessing+1];
	void add(globalvarn v, int i) { variables[v] += i; }
	unsigned get(globalvarn v) const { return variables[v]; }
};

extern gamei game;
extern reactionn last_reaction;
extern int last_number;

void make_reaction_roll(int bonus);
void make_party_move();
long make_player_move(const char* cancel_text = 0);
void make_player_move(fnevent add_answers);
void pause();
void pause(const char* format);
