#include "ability.h"
#include "area.h"
#include "bsdata.h"
#include "class.h"
#include "creature.h"
#include "draw_atg.h"
#include "game.h"
#include "gender.h"
#include "math.h"
#include "stringbuilder.h"
#include "stringvar.h"
#include "variant.h"

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

bool apply_gender(const char* identifier, stringbuilder& sb, gendern gender);

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