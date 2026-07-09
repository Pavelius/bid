#pragma once

#include "ability.h"
#include "feat.h"
#include "item.h"
#include "npc.h"
#include "spell.h"

enum messagen : unsigned char;

typedef void(*fnevent)();

abilityn get_primary(classn v);

struct area;

struct creature : npci, statable, spellable, wearable {
	statable	basic;
	featf		feats;
	char		initiative, disease, starvation, paralize;
	short		hp, mhp;
	spellf		known, active;
	spellable	prepare;
	constexpr explicit operator bool() const { return hp > 0; }
	const char* name() const;
	int get(abilityn v) const { return abilities[v]; }
	int getbonus(abilityn v) const;
	int getskill(abilityn v) const;
	int getspells(int level) const;
	int getspellsprepared(int level) const;
	void act(messagen id) const;
	void actv(char separator, const char* format, const char* format_param) const;
	void clear();
	void damage(int value);
	bool is(itemn v) const { return wearable::is(v); }
	bool is(featn v) const { return feats.is(v); }
	bool is(spelln v) const { return active.is(v); }
	bool iscaster() const;
	bool isenemy(const creature* p) const;
	bool isknown(spelln v) const { return known.is(v); }
	bool isminion() const;
	bool isparty() const { return feats.is(Ally); }
	bool isready() const { return hp > 0; }
	bool isbadlyhurt() const { return hp < mhp/2; }
	void kill();
	void leave();
	bool roll(abilityn v, int bonus = 0) const;
	void set(featn v) { feats.set(v); }
	void update();
	bool wounded() const { return hp < mhp; }
};
extern creature* player;
extern creature* opponent;
extern creature* party[4];

const char* what_to_do();

bool is_diseased(const void* object);
bool is_enemy(const void* object);
bool is_wounded(const void* object);

int party_average(abilityn v);

void create_creature(classn type, gendern gender);
void for_each_party(fnevent proc);
void join_party();
bool make_attack(creature* attacker, creature* enemy, abilityn ability, item& weapon, int attack_bonus);
void make_prepare_spells(messagen id);
void raise_level(int level);
bool use_skill(abilityn id, bool run);