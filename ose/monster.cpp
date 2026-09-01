#include "creature.h"
#include "dice.h"
#include "rand.h"

struct monsterkpi : statable, spellable {
	itemn items[4];
	featf feats;
	constexpr monsterkpi() : statable(), items{} {}
	template<typename T, typename... Ts> constexpr monsterkpi(T v, Ts... args) : monsterkpi(args...) { set(v); }
	constexpr void set(featn v) { feats.set(v); }
	constexpr void set(spelln v) { spellable::spells[v]++; }
	constexpr void set(itemn v) {
		for(auto& e : items) {
			if(!e) {
				e = v;
				break;
			}
		}
	}
};
struct monsteri {
	classn		race;
	char		ac, hd[2];
	alignmentn	alignment;
	char		saving_throws[2];
	dice		appear[2];
	const char*	treasure;
	monsterkpi	stats;
};

static monsteri monster_data[] = {
	{Monster},
	{Human},
	{Human},
	{Human},
	{Human},
	{Halfling},
	{Dwarf},
	{Elf},
	{Human},
	{BearGrizzly, 6, {5}, Neutrality, {2}, {{1}, {1, 4}}, "U", {BearHug}},
	{Boar, 7, {3}, Neutrality, {2}, {{1, 6}, {1, 6}}, ""},
	{CatPanther, 4, {4}, Neutrality, {2}, {{1, 2}, {1, 6}}, "U", {Claws1d4, Bite1d8}},
	{CatTiger, 6, {6}, Neutrality, {3}, {{1}, {1, 3}}, "U", {Claws1d6, Bite2d6}},
	{DogWild},
	{FrogMutant},
	{FrogPoison},
	{Hawk},
};
static_assert((sizeof(monster_data) / sizeof(monster_data[0])) == Hawk + 1);

static classn forest_animals[] = {BearGrizzly, Boar, CatPanther, CatTiger}; // , DogWild, FrogMutant, FrogPoison, Hawk};

classn get_race(classn type) {
	return monster_data[type].race;
}

static void add_monster_items(classn type) {
	auto& ei = monster_data[type];
	for(auto v : ei.stats.items) {
		if(v)
			player->equip(v);
	}
}

static void add_monster_stats(classn type) {
	auto& ei = monster_data[type];
	player->abilities[HD] = ei.hd[0];
	player->abilities[Hits] = ei.hd[1];
	player->feats = ei.stats.feats;
}

void add_monster(classn type) {
	add_monster_stats(type);
	add_monster_items(type);
}

int appear_count(classn type, int index) {
	return monster_data[type].appear[index].roll();
}

classn random_animal(arean area) {
	return maprnd(forest_animals);
}