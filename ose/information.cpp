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

#include "area.h"
#include "bsdata.h"
#include "collectiona.h"
#include "creature.h"
#include "dice.h"
#include "draw_atg.h"
#include "game.h"
#include "gender.h"
#include "math.h"
#include "message.h"
#include "rand.h"
#include "stringbuilder.h"
#include "stringset.h"
#include "stringvar.h"

extern collectiona items;

static const char* random(const char** names) {
	auto p = stringset_find(names);
	if(p)
		return p->names[rand() % p->count];
	return "";
}

static int random_index(const char** names) {
	auto p = stringset_find(names);
	if(p)
		return rand() % p->count;
	return 0;
}

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

static void item_collection(stringbuilder& sb) {
	auto ps = sb.get();
	auto index = 0;
	auto last_index = items.getcount() - 1;
	for(auto p : items.records<item>()) {
		if(ps[0]) {
			if(index==last_index)
				sb.add(" %1 ", message_names[MsgAnd]);
			else
				sb.add(", ");
		}
		sb.add("%-1", p->namefull());
		index++;
	}
}

static void treasure_coins_name(stringbuilder& sb) {
	auto coins_count = 0;
	for(auto v : treasure_coins) {
		if(v)
			coins_count++;
	}
	auto ps = sb.get();
	auto index = 0;
	for(auto v : treasure_coins) {
		if(!v)
			continue;
		if(ps[0]) {
			if(index==coins_count-1)
				sb.add(" %1 ", message_names[MsgAnd]);
			else
				sb.add(", ");
		}
		sb.add("%1i %-2", v, message_names[CoinsCP+index]);
		index++;
	}
}

static void player_class(stringbuilder& sb) {
	sb.add(class_names[player->type]);
}

static void player_weapon(stringbuilder& sb) {
	sb.add(player->wears[MeleeWeapon].name());
}

static void location_name(stringbuilder& sb) {
	sb.addv(get_name(Forest, random_index(LocationMaleFirstName), rand()), 0);
}

static void area_name(stringbuilder& sb) {
	sb.add(last_area->name());
}

static void area_namefull(stringbuilder& sb) {
	sb.add(last_area->namefull());
}

static void area_type(stringbuilder& sb) {
	sb.add(last_area->name());
}

static void print_last_number(stringbuilder& sb) {
	sb.add("%1i", last_number);
}

void stringbuilder_custom(stringbuilder& sb, const char* id) {
	if(stringvar_identifier(sb, id))
		return;
	if(stringset_identifier(id, sb))
		return;
	if(apply_gender(id, sb, str_gender))
		return;
	default_string(sb, id);
}

BSDATA(stringvari) = {
	{"AreaName", area_name},
	{"AreaNameFull", area_namefull},
	{"AreaType", area_type},
	{"Class", player_class},
	{"Item", item_name},
	{"Items", item_collection},
	{"LocationName", location_name},
	{"Name", print_name},
	{"Number", print_last_number},
	{"Player", player_name},
	{"TreasureCoins", treasure_coins_name},
	{"Weapon", player_weapon},
};
BSDATAF(stringvari)