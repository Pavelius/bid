#pragma once

enum globalvarn : unsigned char {
	Turns, Blessing,
};
enum commandn : unsigned char {
	Cancel, Continue, ChangePlayer, ClearAllList, Confirm,
	PageCharacter, PageItems, PageCombatants,
};
struct gamei {
	unsigned variables[Blessing+1];
	void add(globalvarn v, int i) { variables[v] += i; }
	unsigned get(globalvarn v) const { return variables[v]; }
};

extern gamei game;
extern int last_number;

void pause();
void make_reaction_roll(int bonus);
void make_party_move();
long make_player_move(const char* cancel_text = 0);
void make_player_move(fnevent add_answers);
