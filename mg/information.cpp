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

#include "bsdata.h"
#include "creature.h"
#include "game.h"
#include "message.h"
#include "stringbuilder.h"
#include "stringvar.h"

static const char* get(messagen id) {
	return message_names[id];
}

static void add(stringbuilder& sb, skilln id, int value, int value_maximum) {
	sb.adds(skill_names[id], value, value_maximum);
}

static void player_name(stringbuilder& sb) {
	sb.add(player->name());
}

static void player_class(stringbuilder& sb) {
	sb.add(creature_names[player->type]);
}

static void player_weapon(stringbuilder& sb) {
	sb.add(player->wears[Weapon].name());
}

static void parcipants_names(stringbuilder& sb) {
	auto count = 0;
	for(auto p : parcipants) {
		if(p)
			count++;
	}
	auto index = 0;
	for(auto p : parcipants) {
		if(p) {
			if(count > 1 && index == count - 1)
				sb.adds(message_names[And]);
			else if(index)
				sb.adds(", ");
			sb.adds(p->name());
			index++;
		}
	}
}

static void rolled_dices_result(stringbuilder& sb) {
	auto ps = sb.get(); ps[0] = 0;
	for(auto n : roll_dices) {
		if(!n)
			break;
		if(ps[0])
			sb.add(", ");
		if(n >= 4)
			sb.add("[%1i]", n);
		else
			sb.add("%1i", n);
	}
}

static void player_speciality(stringbuilder& sb) {
	if(player->speciality)
		sb.add(skill_names[player->speciality]);
}

template<> void fistatus<skilln>(unsigned char id, stringbuilder& sb) {
	auto v = (skilln)id;
	auto n = player->skills[v];
	sb.adds("%1 %2i/%3i", get(Passed), player->success[v], n);
	if(n > 1)
		sb.adds("%1 %2i/%3i", get(Failed), player->fail[v], n - 1);
}

BSDATA(stringvari) = {
	{"Class", player_class},
	{"Parcipants", parcipants_names},
	{"Player", player_name},
	{"RolledDices", rolled_dices_result},
	{"Speciality", player_speciality},
	{"Weapon", player_weapon},
};
BSDATAF(stringvari)