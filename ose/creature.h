#pragma once

#include "gender.h"
#include "item.h"
#include "spell.h"

typedef flagable<1, unsigned> featf;

typedef void(*fnevent)();

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
	LastAbility = Lore,
};
enum alignmentn : unsigned char {
	Lawful, Neutrality, Chaotic
};
enum classn : unsigned char {
	Animal,
	Fighter, Cleric, Theif, MagicUser,
	Human, Halfling, Dwarf, Elf,
	BearGrizzly, Boar, CatPanther, CatTiger, DogWild, FrogMutant, FrogPoison, Hawk,
	LizardGecko, LizardTuatara, SnakePitViper, SpiderCrab, SpiderPhase, ToadPoisonous, Unicorn,
	Wolf, WolfDire,
};
enum featn : unsigned char {
	Awareness, Backstab,
	MeleeFight, Charged, Stunned, Flee, Ally, Enemy,
	Local, Slow,
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
	bool is(alignmentn v) const { return npc::is(v); }
	bool is(featn v) const { return feats.is(v); }
	bool is(gendern v) const { return npc::is(v); }
	bool is(itemn v) const { return wearable::is(v); }
	bool is(spelln v) const { return active.is(v); }
	bool iscaster() const;
	bool isenemy(const creature* p) const;
	bool isknown(spelln v) const { return known.is(v); }
	bool isminion() const;
	bool isparty() const { return feats.is(Ally); }
	bool isready() const { return hp > 0; }
	bool isbadlyhurt() const { return hp < mhp / 2; }
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

abilityn get_primary(classn v);
portraitn random_portrait(classn type, gendern gender);
classn get_race(classn type);

const char* what_to_do();

bool is_diseased(const void* object);
bool is_enemy(const void* object);
bool is_wounded(const void* object);

int party_average(abilityn v);
int skill_bonus(actionn v, classn type);

void create_creature(classn type, gendern gender);
void create_monster(classn type);
void create_monsters(classn type);
void for_each_party(fnevent proc);
void join_party();
bool make_attack(creature* attacker, creature* enemy, abilityn ability, item& weapon, int attack_bonus);
void make_prepare_spells(messagen id);
void raise_level(int level, bool reroll_lowest = false);
bool use_skill(actionn id, int bonus, bool run);