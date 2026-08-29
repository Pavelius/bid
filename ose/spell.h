#pragma once

#include "flagable.h"
#include "variant.h"

struct collection;

enum domainn : unsigned char {
	NoSpells,
	ArcaneSpells, CosmicSpells, NatureSpells,
};
enum spelln : unsigned char {
	CureLightWounds, CauseLightWounds, DetectEvil, DetectMagic, Light, Darkness,
	ProtectionFromEvil, PurifyFoodAndWater, RemoveFear, CauseFear, ResistCold,
	LastSpell = ResistCold,
};
struct spellable {
	char spells[LastSpell + 1];
	int total(domainn type, int level) const;
	int total(const collection& source) const;
};
typedef flagable<1 + LastSpell / 32, unsigned> spellf;

struct enchanti {
	variant		object;
	spelln		spell;
	unsigned	stop;
	void clear();
};

enchanti* find_enchant(unsigned object, short unsigned spell);

spelln get_reverse(spelln v);

int get_arcane_level(spelln v);
int get_level(spelln v);
int get_level(spelln v, domainn type);

void add_enchant(variant object, unsigned stop, spelln spell);
void add_spells(domainn spells, int level);
void fixlist(const spellable& e);
void remove_enchant(variant object);
void remove_enchant(variant object, spelln spell);
