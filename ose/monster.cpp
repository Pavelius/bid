#include "creature.h"

struct monsteri {
	classn		race;
	char		ac, hd[2];
	alignmentn	alignment;
	char		saving_throws[2];
	char		appear[2][2];
	const char*	treasure;
};

static monsteri monster_data[] = {
	{Monster},
	{Human},
	{Human},
	{Human},
	{Human},
	{Human},
	{Halfling},
	{Dwarf},
	{Elf},
	{BearGrizzly, 6, {5}, Neutrality, {2}, {{1}, {1, 4}}, "U"},
	{Boar, 7, {3}, Neutrality, {2}, {{1, 6}, {1, 6}}, ""},
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