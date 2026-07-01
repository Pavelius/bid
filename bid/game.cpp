#include "draw.h"
#include "draw_atg.h"
#include "answers.h"
#include "bsdata.h"
#include "character.h"
#include "collection.h"
#include "message.h"
#include "pushvalue.h"
#include "stringvar.h"

static character* player_avatars[3] = {players, players + 1, players + 2};
static actionn attributes[3][4] = {
	{Hunt, Study, Survey, Tinker},
	{Finesse, Prowl, Skirmish, Wreck},
	{Attune, Command, Consort, Sway},
};

static const char* get_avatar(charactern type) {
	return "ca";
}

static void get_avatar(const void* object, stringbuilder& sb) {
	auto p = (character*)object;
	sb.add(get_avatar(p->type));
}

static int get_hits(const void* object) {
	return 100;
}

static void paint_avatars() {
	paint_avatars((void**)player_avatars, lenghtof(players), get_avatar, player, get_hits);
}

static void paint_value(actionn v) {
	char temp[260]; stringbuilder sb(temp);
	sb.add("/x 120 text %2i\n%1", getname(v), player->actions[v]);
	auto object = &player->actions[v];
	paint_button(temp, object, false);
	if(button_hilited && tips_text[0] == 0) {
		stringbuilder sb(tips_text);
		// sb.add(bsenum<actionn>::info[v]);
	}
}

static void page_characters() {
	paint_avatars();
	pushvalue push(player, (character*)current_avatar);
	for(auto i = Insight; i <= Resolve; i = (attributen)(i + 1)) {
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

void game_run() {
	answers::resid = "Wasteland";
	atg_menu = paint_main_menu;
	add_players();
}

static void player_name(stringbuilder& sb) {
}

BSDATA(stringvari) = {
	{"Player", player_name},
};
BSDATAF(stringvari)