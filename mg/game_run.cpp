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

#include "answers.h"
#include "creature.h"
#include "draw.h"
#include "draw_atg.h"
#include "game.h"
#include "gender.h"
#include "message.h"
#include "print.h"
#include "pushvalue.h"
#include "rand.h"
#include "stringbuilder.h"
#include "stringset.h"
#include "stringvar.h"

void stringbuilder_custom(stringbuilder& sb, const char* id);
void main_util();
bool pass_test();

extern unsigned char bin_avatars[];
extern unsigned char bin_images[];

static void paint_value(skilln id) {
	char temp[260]; stringbuilder sb(temp);
	sb.add("/x 130 text %2i\n%1", skill_names[id], player->skills[id]);
	paint_button(temp, 0, false);
	if(button_hilited && tips_text[0] == 0) {
		stringbuilder sb(tips_text);
		fistatus<skilln>(id, sb);
	}
}

static void paint_value(traitn id) {
	char temp[260]; stringbuilder sb(temp);
	auto level = player->traits[id];
	if(level > 1)
		sb.add("%1 (%2i)", trait_names[id], level);
	else
		sb.add(trait_names[id]);
	paint_button(temp, 0, false);
	if(button_hilited && tips_text[0] == 0) {
		//stringbuilder sb(tips_text);
		//sb.add("Test");
	}
}

static void paint_value(wisen id) {
	paint_button(wise_names[id], 0, false);
	if(button_hilited && tips_text[0] == 0) {
		//stringbuilder sb(tips_text);
		//sb.add("Test");
	}
}

static int get_avatar(const void* object) {
	return ((character*)object)->index();
}

static int get_hits(const void* object) {
	return 100;
}

static void paint_avatars() {
	auto po = hilite_object;
	paint_avatars((void**)party, sizeof(party) / sizeof(party[0]), get_avatar, player, get_hits);
	if(!po && hilite_object) {
		stringbuilder sb(tips_text);
		auto p = (character*)hilite_object;
		sb.add(p->name());
	}
}

static void page_characters() {
	paint_avatars();
	pushvalue push(player, (character*)current_avatar);
	if(!player)
		return;
	for(auto n = Nature; n <= Circles; n = (skilln)(n + 1)) {
		if(player->skills[n])
			paint_value(n);
	}
	paint_separator();
	for(auto n = (traitn)0; n <= LastTrait; n = (traitn)(n + 1)) {
		if(player->traits[n])
			paint_value(n);
	}
	paint_separator();
	for(auto n = (wisen)0; n <= LastWise; n = (wisen)(n + 1)) {
		if(player->is(n))
			paint_value(n);
	}
}

static void page_skills() {
	paint_avatars();
	pushvalue push(player, (character*)current_avatar);
	if(!player)
		return;
	for(auto n = (skilln)Administrator; n <= LastSkill; n = (skilln)(n + 1)) {
		if(player->skills[n])
			paint_value(n);
	}
}

static void paint_main_menu() {
	paint_bar(message_names[PageCharacter], page_characters);
	paint_bar(message_names[PageSkills], page_skills);
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

static void initialize_resources() {
	metrics::avatars = (sprite*)bin_avatars;
	metrics::images = (sprite*)bin_images;
	stringbuilder::custom = stringbuilder_custom;
	atg_menu = paint_main_menu;
}

void game_run() {
	// srand(rseed());
	srand(112281);
	initialize_resources();
	//if(!pass_test())
	//	return;
#ifdef _DEBUG
	main_util();
#endif
	create_game();
}