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
#include "collection.h"
#include "collectiona.h"
#include "creature.h"
#include "draw.h"
#include "draw_atg.h"
#include "game.h"
#include "print.h"
#include "pushvalue.h"
#include "rand.h"
#include "stringbuilder.h"

void stringbuilder_custom(stringbuilder& sb, const char* id);
void main_util();
bool pass_test();

void pause(const char* format) {
	choose_answers(0, format);
}

static void test_game() {
	create_character();
}

extern unsigned char bin_avatars[];
extern unsigned char bin_images[];

static void paint_value(skilln id) {
	char temp[260]; stringbuilder sb(temp);
	sb.add("/x 120 text %2i\n%1", skill_names[id], player->skills[id]);
	paint_button(temp, 0, false);
	if(button_hilited && tips_text[0] == 0) {
		stringbuilder sb(tips_text);
		sb.add("Test");
	}
}

static int get_avatar(const void* object) {
	return 0;
}

static int get_hits(const void* object) {
	return 100;
}

static void paint_avatars() {
	paint_avatars((void**)party, sizeof(party)/sizeof(party[0]), get_avatar, player, get_hits);
}

static void page_characters() {
	paint_avatars();
	pushvalue push(player, (character*)current_avatar);
	if(!player)
		return;
	for(auto n = Nature; n<=Circles; n = (skilln)(n+1)) {
		if(player->skills[n])
			paint_value(n);
	}
	paint_separator();
}

static void page_skills() {
	paint_avatars();
	pushvalue push(player, (character*)current_avatar);
	if(!player)
		return;
	for(auto n = (skilln)Administrator; n<=LastSkill; n = (skilln)(n+1)) {
		if(player->skills[n])
			paint_value(n);
	}
}

static void paint_main_menu() {
	paint_bar(command_names[PageCharacter], page_characters);
	paint_bar(command_names[PageSkills], page_skills);
}

static void initialize_resources() {
	// metrics::avatars = (sprite*)bin_avatars;
	// metrics::images = (sprite*)bin_images;
	// stringbuilder::custom = stringbuilder_custom;
	atg_menu = paint_main_menu;
}

void game_run() {
	srand(rseed());
	// srand(1281);
	initialize_resources();
	//if(!pass_test())
	//	return;
#ifdef _DEBUG
	// main_util();
#endif
	test_game();
}