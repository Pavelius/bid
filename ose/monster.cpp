#include "creature.h"

struct monsteri {
	classn		race;
	char		ac, hd[2];
	alignmentn	alignment;
};

static monsteri monster_data[] = {
	{Animal},
	{Human},
	{Human},
	{Human},
	{Human},
	{Human},
	{Halfling},
	{Dwarf},
	{Elf},
	{BearGrizzly, 6, {5}, Neutrality},
	{Boar, 6, {5}, Neutrality},
	{CatPanther},
	{CatTiger},
	{DogWild},
	{FrogMutant},
	{FrogPoison},
	{Hawk},
};
static_assert((sizeof(monster_data) / sizeof(monster_data[0])) == Hawk + 1);

classn get_race(classn type) {
	return monster_data[type].race;
}