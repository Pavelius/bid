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
#include "bsdata.h"
#include "collection.h"
#include "collectiona.h"
#include "draw.h"
#include "draw_atg.h"
#include "game.h"
#include "print.h"
#include "pushvalue.h"
#include "rand.h"
#include "stringbuilder.h"

const int yards_in_miles = 1000;

gamei game;

static void paint_main_menu() {
}

//////////////////////////////////////////////////////
// START GAME

static void test_game() {
	game.add(Turns, 1000);
}

void stringbuilder_custom(stringbuilder& sb, const char* id);

void main_util();
bool pass_test();

extern unsigned char bin_avatars[];
extern unsigned char bin_images[];

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