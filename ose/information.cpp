#include "area.h"
#include "bsdata.h"
#include "creature.h"
#include "dice.h"
#include "draw_atg.h"
#include "game.h"
#include "math.h"
#include "stringbuilder.h"
#include "stringvar.h"
#include "variant_no_use.h"

static void addv(stringbuilder& sb, const dice& v) {
	sb.add("%1id%2i", v.c, v.d);
	if(v.b)
		sb.add("%+1i", v.b);
}

static void player_name(stringbuilder& sb) {
	sb.add(player->name());
}

static void item_name(stringbuilder& sb) {
	sb.add(last_item->name());
}

static void player_class(stringbuilder& sb) {
	sb.add(getname(player->type));
}

static void player_weapon(stringbuilder& sb) {
	sb.add(player->wears[Hands].name());
}

static void area_first(stringbuilder& sb) {
}

static void area_second(stringbuilder& sb) {
}

static void area_name(stringbuilder& sb) {
	sb.add(last_area->name());
}

static void area_type(stringbuilder& sb) {
	sb.add(getname(last_area->type));
}

static void print_last_number(stringbuilder& sb) {
	sb.add("%1i", last_number);
}

bool apply_gender(const char* identifier, stringbuilder& sb, gendern gender) {
	struct gender_change_string {
		const char*	female;
		const char*	male;
		const char*	multiply;
	};
	static gender_change_string player_gender[] = {
		{"госпожа", "господин", "господа"},
		{"женщина", "мужчина", "господа"},
		{"стерва", "ублюдок", "ублюдки"},
		{"миледи", "милорд", "милорды"},
		{"такая", "такой", "такие"},
		{"леди", "лорд", "лорды"},
		{"ась", "ся", "ись"},
		{"нее", "него", "них"},
		{"она", "он", "они"},
		{"шла", "шел", "шли"},
		{"ая", "ый", "ые"},
		{"ее", "его", "их"},
		{"ей", "ему", "им"},
		{"ла", "", "ли"},
		{"а", "", "и"},
	};
	for(auto& e : player_gender) {
		if(!equal(e.female, identifier))
			continue;
		if(gender == NoGender)
			sb.add(e.multiply);
		else if(gender == Female)
			sb.add(e.female);
		else
			sb.add(e.male);
		return true;
	}
	return false;
}
void stringbuilder_custom(stringbuilder& sb, const char* id) {
	if(stringvar_identifier(sb, id))
		return;
	if(player) {
		if(apply_gender(id, sb, player->gender))
			return;
	}
	default_string(sb, id);
}

BSDATA(stringvari) = {
	{"AreaName", area_name},
	{"AreaType", area_type},
	{"AreaFirst", area_first},
	{"AreaSecond", area_second},
	{"Class", player_class},
	{"Item", item_name},
	{"Number", print_last_number},
	{"Player", player_name},
	{"Weapon", player_weapon},
};
BSDATAF(stringvari)