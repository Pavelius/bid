#include "draw.h"
#include "draw_atg.h"
#include "answers.h"
#include "bsdata.h"
#include "character.h"
#include "collection.h"
#include "message.h"
#include "pushvalue.h"
#include "rand.h"
#include "scene.h"
#include "stringvar.h"

static character* player_avatars[3] = {players, players + 1, players + 2};

static const char* get_avatar(charactern type) {
	return "caf";
}

static void get_avatar(const void* object, stringbuilder& sb) {
	auto p = (character*)object;
	sb.add(get_avatar(p->type));
}

static int get_hits(const void* object) {
	return 100;
}

static void paint_cicle(int size, int value, int maximum) {
	pushrect push;
	pushfore push_fore;
	caret.x += 2 + size / 2;
	caret.y += 6 + size / 2;
	auto fore_half = push_fore.fore.mix(colors::form, 64);
	for(int i = 0; i < maximum; i++) {
		if(i < value)
			fore = push_fore.fore;
		else
			fore = fore_half;
		circlef(size - 1);
		// circle(size - 1);
		caret.x += size * 2;
	}
}

static void paint_avatars() {
	paint_avatars((void**)player_avatars, lenghtof(players), get_avatar, player, get_hits);
}

static void paint_value(attributen v) {
	char temp[260]; stringbuilder sb(temp);
	sb.add("/cb fill ct\n%1", getname(v));
	paint_button(temp, 0, false, 2);
}

static void paint_value(actionn v) {
	pushrect push;
	height = texth();
	button_check(0);
	paint_hilite();
	set_hilite_state(getinfo(v));
	paint_cicle(6, player->actions[v], 4);
	caret.x += 64;
	text(getname(v));
	push.caret.y += texth() + 1;
}

static void page_characters() {
	paint_avatars();
	pushvalue push(player, (character*)current_avatar);
	for(auto i = Insight; i <= Resolve; i = (attributen)(i + 1)) {
		paint_value(i);
		for(auto v : attributes[i])
			paint_value(v);
	}
	paint_separator();
}

static void page_items() {
	paint_avatars();
}

static void paint_main_menu() {
	paint_bar(getname(PageActions), page_characters);
	paint_bar(getname(PageItems), page_items);
}

static void test_game() {
	auto n = make_roll(4);
	if(n == BadOutcome)
		return;
}

void game_run() {
	answers::resid = "Wasteland";
	atg_menu = paint_main_menu;
	srand(13123);
	test_game();
	add_players();
}

static void player_name(stringbuilder& sb) {
}

BSDATA(stringvari) = {
	{"Player", player_name},
};
BSDATAF(stringvari)