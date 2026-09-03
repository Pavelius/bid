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

#include "gender.h"
#include "item.h"
#include "spell.h"

typedef flagable<1, unsigned> featf;

typedef void(*fnevent)();
typedef bool (*fncfilter)(unsigned char v);

enum arean : unsigned char;
enum portraitn : unsigned char;
enum messagen : unsigned char;
enum namen : unsigned char;

enum abilityn : unsigned char {
	HD, Strenght, Dexterity, Constitution, Intelligence, Wisdom, Charisma,
	MeleeAttack, MissileAttack, ThrownAttack,
	MeleeDamage, MissileDamage, ThrownDamage,
	AC, Attacks, Initiative, Loyalty, Morale, Movement, Hits,
	SaveDeath, SaveWand, SaveParalysis, SaveBreath, SaveSpells,
	Climb, FindTraps, HearNoises, MoveSilently, OpenLocks, Tracking, Lore,
	HorribleWail,
	LastAbility = HorribleWail,
};
enum alignmentn : unsigned char {
	Lawful, Neutrality, Chaotic
};
enum classn : unsigned char {
	Monster, Fighter, Cleric, Theif, MagicUser, Halfling, Dwarf, Elf, Human,
	BearGrizzly, Boar, CatPanther, CatTiger, DogWild, FrogMutant, FrogPoison, Hawk,
	LizardGecko, LizardTuatara, SnakePitViper, SpiderCrab, SpiderPhase, ToadPoisonous, Unicorn,
	Wolf, WolfDire,
};
enum featn : unsigned char {
	Awareness, Backstab,
	MeleeFight, Charged, Stunned, Flee, Ally, Enemy,
	Local, Slow, Undead, StealthySurprise, SwallowWhole,
	BlinkAttack, HugAttack, PetrifyingTouch, PetrifyingGaze,
	ImmuneBlind, ImmuneCold, ImmuneDamage, ImmuneFire, ImmuneLighting, ImmuneSurprise,
};

struct area;

struct npc {
	classn		type;
	gendern		gender;
	alignmentn	alignment;
	namen		customname;
	portraitn	portrait;
	bool is(alignmentn v) const { return alignment == v; }
	bool is(gendern v) const { return gender == v; }
};
struct statable {
	char		abilities[LastAbility + 1];
};
struct creature : npc, statable, wearable, spellable {
	statable	basic;
	featf		feats;
	char		initiative, disease, starvation, paralize;
	short		hp, mhp;
	spellf		known, active;
	spellable	prepare;
	int			experience;
	constexpr explicit operator bool() const { return hp > 0; }
	const char* name() const;
	int award() const;
	int get(abilityn v) const { return abilities[v]; }
	int getbonus(abilityn v) const;
	int getskill(abilityn v) const;
	int getspells(int level) const;
	int getspellsprepared(int level) const;
	void act(messagen id) const;
	void actv(char separator, const char* format, const char* format_param) const;
	void addexp(int value) { experience += value; }
	void clear();
	void damage(int value);
	bool is(alignmentn v) const { return npc::is(v); }
	bool is(featn v) const { return feats.is(v); }
	bool is(gendern v) const { return npc::is(v); }
	bool is(itemn v) const { return wearable::is(v); }
	bool is(spelln v) const { return active.is(v); }
	bool isbadlyhurt() const { return hp < mhp / 2; }
	bool iscaster() const;
	bool isdead() const { return hp <= 0; }
	bool isenemy(const creature* p) const;
	bool isknown(spelln v) const { return known.is(v); }
	bool isminion() const;
	bool isparty() const;
	bool isready() const { return hp > 0; }
	void kill();
	void leave();
	bool roll(abilityn v, int bonus = 0) const;
	void set(featn v) { feats.set(v); }
	void update();
};
extern creature* player;
extern creature* opponent;
extern creature* party[4];

abilityn get_primary(classn v);
portraitn random_portrait(classn type, gendern gender, fncfilter filter = 0);
classn get_race(classn type);

classn random_animal(arean area);

const char* get_treasure(classn type);
const char* what_to_do();

bool is_diseased(const void* object);
bool is_enemy(const void* object);
bool is_wounded(const void* object);

int appear_count(classn type, int index = 0);
int party_average(abilityn v);
int skill_bonus(actionn v, classn type);

void add_monster(classn type);
void create_creature(classn type, gendern gender);
void create_monster(classn type);
void create_monsters(classn type, bool hostile);
void for_each_party(fnevent proc);
void join_party();
void initiative_roll();
void make_attack(creature* attacker, creature* enemy, abilityn ability, item& weapon, int attack_bonus);
void make_prepare_spells(messagen id);
void raise_level(int level, bool reroll_lowest = false);
bool use_skill(actionn id, int bonus, bool run);