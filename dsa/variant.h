#pragma once

enum abilityn : unsigned char;
enum actionn : unsigned char;
enum alignmentn : unsigned char;
enum arean : unsigned char;
enum classn : unsigned char;
enum itemn : unsigned char;
enum materialn : unsigned char;
enum gendern : unsigned char;
enum racen : unsigned char;
enum spelln : unsigned char;

struct creature;

enum variantn : unsigned char {
	NoVariant,
	Ability, Action, Alignment, Area, Class, Creature, Gender, Item, Material, Race, Spell,
};
union variant {
	struct {
		unsigned char value;
		variantn type;
	};
	short unsigned u;
	constexpr variant() : u(0) {}
	constexpr variant(abilityn v) : value(v), type(Ability) {}
	constexpr variant(actionn v) : value(v), type(Action) {}
	constexpr variant(alignmentn v) : value(v), type(Alignment) {}
	constexpr variant(arean v) : value(v), type(Area) {}
	constexpr variant(classn v) : value(v), type(Class) {}
	constexpr variant(itemn v) : value(v), type(Item) {}
	constexpr variant(materialn v) : value(v), type(Material) {}
	constexpr variant(gendern v) : value(v), type(Gender) {}
	constexpr variant(racen v) : value(v), type(Race) {}
	constexpr variant(spelln v) : value(v), type(Spell) {}
	variant(creature* v);
	//constexpr bool operator==(short unsigned v) { return v == u; }
	constexpr bool operator==(const variant& v) { return v.u == u; }
	constexpr bool operator!=(const variant& v) { return v.u != u; }
	constexpr void operator++() { value++; }
	constexpr explicit operator bool() { return u!=0; }
	constexpr operator int() { return u; }
	void clear() { u = 0; }
};