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
#include "settlement.h"
#include "stringbuilder.h"
#include "variant.h"

static bool need_break_actions;

unsigned game_var[PartyCoins + 1];

int last_number;

classn encounter_monsters;
reactionn last_reaction;

extern collectiona creatures;

template<> variant::variant(const settlement* p) : variant(Settlement, p - settlements) {}
template<> variant::variant(const creature* p) : variant(Creature, p->index()) {}

void pass_turn() {
	game_var[Turns]++;
}

bool chance(int v) {
	return (rand() % 100) < v;
}

void make_reaction_roll(int bonus) {
	auto result = d6() + d6() + bonus;
	if(result <= 2)
		last_reaction = Hostile;
	else if(result <= 5)
		last_reaction = Unfriendly;
	else if(result <= 8)
		last_reaction = Neutral;
	else if(result <= 11)
		last_reaction = Indifferent;
	else
		last_reaction = Friendly;
}

picturen getimage(arean v) {
	switch(v) {
	case Village: case Hamlet: case SmallTown: return ImageVillage;
	case LargeTown: return ImageLargeCity;
	case Market: return ImageVillageMarket;
	case Inn: return ImageHotel;
	case Tavern: return ImageTavern;
	case Temple: return ImageTemple;
	default: return ImagePlains;
	}
}

picturen getimagenight(arean v) {
	switch(v) {
	case Plains: return ImagePlainsNight;
	case Forest: return ImageForestNight;
	case Wastes: return ImageWastelandNight;
	default: return ImagePlainsNight;
	}
}

bool doactions() {
	if(need_break_actions) {
		need_break_actions = false;
		return false;
	}
	return true;
}

void breakactions() {
	need_break_actions = true;
}

void clear_messages() {
	sb.clear();
}

static void change_player() {
	player = (creature*)current_avatar;
	breakmodal(1);
}

long choose_party_option(const char* cancel_text) {
	return choose_answers(message_names[AskWhatToDo], cancel_text);
}

long choose_player_option(const char* cancel_text) {
	pushvalue push(atg_change_avatar, change_player);
	current_avatar = (void*)player;
	return choose_answers(what_to_do(), cancel_text);
}

void addhdr(picturen picture, const char* header) {
	answer_picture = picture;
	answer_header = header;
}

void addhdr(picturen picture) {
	answer_picture = picture;
}

void add_look() {
	sb.clear();
	sb.addn(area_look[enviroment]);
}

void adds(messagen n) {
	sb.addsep(' ');
	sb.addv(message_names[n], 0);
}

void addn(messagen id) {
	sb.addsep('\n');
	sb.addv(message_names[id], 0);
}

void addopt(const item& e, messagen v, fnitemget price) {
	char temp[260]; stringbuilder sb(temp);
	sb.add(message_names[v], e.name(), price(e));
	if(e.countable() && e.count > 1) {
		sb.adds("[~");
		sb.add(message_names[AvailableCount], e.count);
		sb.adds("]");
	}
	an.addv(buttonparam, (long)&e, 0, temp, 0);
}

void addopt(actionn n) {
	an.add(variant(n), action_names[n]);
}

void addopt(arean v) {
	an.add(v, area_visit[v]);
}

//////////////////////////////////////////////////////
// INTERFACE WINDOW

static void paint_value(abilityn id) {
	char temp[260]; stringbuilder sb(temp);
	sb.add("/x 120 text %2i\n%1", ability_names[id], player->abilities[id]);
	paint_button(temp, 0, false);
	if(button_hilited && tips_text[0] == 0) {
		stringbuilder sb(tips_text);
		sb.add("Test");
	}
}

static void paint_value(const item& e) {
	if(!e)
		return;
	char temp[260]; stringbuilder sb(temp);
	sb.add(e.name());
	if(e.count > 1)
		sb.adds("x%1i", e.count);
	paint_button(temp, 0, false);
	if(button_hilited && tips_text[0] == 0) {
	}
}

static void paint_value(const creature* p) {
	char temp[260]; stringbuilder sb(temp);
	if(p->isdead())
		sb.add("[~%1]", p->name());
	else {
		sb.add(p->name());
		if(p->isbadlyhurt())
			sb.adds("[~%1]", message_names[StateWounded]);
	}
	paint_button(temp, 0, false);
	if(button_hilited && tips_text[0] == 0) {
	}
}

static int get_avatar(const void* object) {
	auto p = (creature*)object;
	return p->portrait;
}

static int get_hits(const void* object) {
	auto p = (creature*)object;
	if(p->hp == p->mhp || !p->mhp)
		return 100;
	return p->hp * 100 / p->mhp;
}

static void paint_avatars() {
	paint_avatars((void**)party, sizeof(party)/sizeof(party[0]), get_avatar, player, get_hits);
}

static void page_characters() {
	paint_avatars();
	pushvalue push(player, (creature*)current_avatar);
	paint_value(Strenght);
	paint_value(Dexterity);
	paint_value(Constitution);
	paint_value(Intelligence);
	paint_value(Wisdom);
	paint_value(Charisma);
	paint_separator();
	paint_value(MeleeAttack);
	paint_value(AC);
	paint_separator();
}

static void page_items() {
	paint_avatars();
	pushvalue push(player, (creature*)current_avatar);
	for(auto& e : player->wears)
		paint_value(e);
}

static void page_combatants() {
	paint_avatars();
	for(auto p : creatures.records<creature>())
		paint_value(p);
}

static void paint_main_menu() {
	paint_bar(message_names[PageCharacter], page_characters);
	paint_bar(message_names[PageItems], page_items);
	if(creatures && enemy_present())
		paint_bar(message_names[PageCombatants], page_combatants);
}

//////////////////////////////////////////////////////
// START GAME

static void test_game() {
	game_var[Turns] = 1000;
	generate_world();
	create_creature(Fighter, Male);
	join_party();
	create_creature(Elf, Female);
	join_party();
	create_creature(Theif, Male);
	join_party();
	create_creature(Cleric, Female);
	raise_level(3);
	join_party();
	// treasure_generate("A", true, false, false);
	// add_magic_item(RandomMagicItem);
	// make_player_move(take_items_options);
	// adventure_move(50);
	last_settlement = find_settlement(MiddleKindom, LargeTown);
	if(!last_settlement)
		last_settlement = find_settlement(MiddleKindom, SmallTown);
	create_market_items();
	kindom_adventure_move();
}

void stringbuilder_custom(stringbuilder& sb, const char* id);

void main_util();
bool pass_test();

extern unsigned char bin_avatars[];
extern unsigned char bin_images[];

static void initialize_resources() {
	metrics::avatars = (sprite*)bin_avatars;
	metrics::images = (sprite*)bin_images;
	stringbuilder::custom = stringbuilder_custom;
	atg_menu = paint_main_menu;
}

void game_run() {
	srand(rseed());
	// srand(1281);
	initialize_resources();
	if(!pass_test())
		return;
#ifdef _DEBUG
	main_util();
#endif
	test_game();
};