#include "action.h"
#include "answers.h"
#include "area.h"
#include "bsdata.h"
#include "class.h"
#include "creature.h"
#include "collection.h"
#include "collectiona.h"
#include "draw.h"
#include "draw_atg.h"
#include "game.h"
#include "message.h"
#include "pushvalue.h"
#include "rand.h"
#include "reaction.h"
#include "stringbuilder.h"
#include "variant.h"

gamei game;
int last_number;

reactionn last_reaction;

static variant last_result;
extern collectiona creatures;

variant::variant(creature* v) : type(Creature), value(v - bsdata<creature>::elements) {}

void pause() {
	if(!sb)
		return;
	an.clear();
	an.add(1, getname(Continue));
	an.choose(0, 0);
	sb.clear();
	an.clear();
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

void make_party_move() {
	last_result.u = (unsigned short)an.choose(0, 0);
	an.clear();
}

long make_player_move(const char* cancel_text) {
	auto result = an.choose(what_to_do(), cancel_text);
	an.clear();
	return result;
}

void make_player_move(fnevent options_proc) {
	pushvalue push(current_avatar_post, (long)ChangePlayer);
	current_avatar = (void*)player;
	while(true) {
		an.clear();
		options_proc();
		last_result.u = (short unsigned)make_player_move();
		if(last_result.u==ChangePlayer) {
			player = (creature*)current_avatar;
			continue;
		} else
			break;
	}
}

static void select_creatures() {
	creatures.clear();
	for(auto& e : bsdata<creature>()) {
		if(!e)
			continue;
		creatures.add(&e);
	}
}

static creature* find_creature(fnvisible proc, bool keep) {
	for(auto p : creatures.records<creature>()) {
		if(proc(p) == keep)
			return p;
	}
	return 0;
}

static void addan(actionn v) {
	an.add(variant(v), getname(v));
}

static void apply_effect(actionn v) {
	switch(v) {
	case MakeCharge:
		player->act(PlayerCharged);
		player->set(Charged);
		player->set(MeleeFight);
		opponent->set(MeleeFight);
		make_attack(player, opponent, MeleeAttack, player->wears[Hands], 2);
		break;
	case MakeMeleeAttack:
		make_attack(player, opponent, MeleeAttack, player->wears[Hands], 0);
		break;
	case MakeMissileAttack:
		make_attack(player, opponent, MissileAttack, player->wears[Hands], 0);
		player->useammo();
		break;
	case MakeThrownAttack:
		make_attack(player, opponent, ThrownAttack, player->wears[Hands], 0);
		player->useammo();
		break;
	case MemorizeSpells:
		make_prepare_spells(PlayerMemorizeSpells);
		break;
	default:
		break;
	}
}

static void apply_result() {
	switch(last_result.type) {
	case Action: apply_effect((actionn)last_result.value); break;
	default: break;
	}
}

static void camp_actions() {
	pushvalue push(player);
	for(auto p : party) {
		if(!p)
			continue;
		player = p;
		use_skill(GearRepairing, true);
		use_skill(TendingWounds, true);
		use_skill(TreatIllness, true);
		use_skill(Hunting, true);
		use_skill(Foraging, true);
	}
}

static void camp_options() {
	addan(RestParty);
	if(player->getspells(1))
		addan(MemorizeSpells);
}

static bool consume(itemn v) {
	for(auto p : party) {
		if(p && p->consume(v))
			return true;
	}
	return false;
}

static void consume_food() {
	if(consume(RawMeat))
		return;
	if(consume(Mushrooms))
		return;
	player->act(PlayerSufferStarvation);
	player->starvation += d6();
}

static void camp_move() {
	sb.clear();
	fixmsg(MakeCampInOpenLand);
	camp_actions();
	while(true) {
		make_player_move(camp_options);
		if(!last_result.u || last_result==variant(RestParty))
			return;
		apply_result();
	}
}

static void check_movement() {
	auto value = party_average(Movement) * 10 / 5;
	move_distance -= value;
}

static void night_encounter() {
	pushvalue push_header(answers::resid, "WastelandNight");
	sb.clear();
	sb.add(getname(PlayerHearNoiseOnWatch));
	pause();
}

static void adventure_move() {
	answers::header = "%AreaName";
	answers::resid = "Wasteland";
	while(true) {
		sb.adds(getinfo(last_area->type));
		addan(MakeCamp);
		make_party_move();
		camp_move();
		for_each_party(consume_food);
		move_distance -= 30;
		if(move_distance <= 0) {
			break;
		} else {
			night_encounter();
			pause();
			sb.addn(getname(AdventureNextDay));
		}
	}
}

static void combat_options() {
	opponent = find_creature(is_enemy, true);
	if(opponent) {
		if(player->is(MeleeFight))
			addan(MakeMeleeAttack);
		else {
			addan(MakeCharge);
			if(player->wears[Hands].throwing() && !player->wears[Hands].lost)
				addan(MakeThrownAttack);
			addan(MakeMissileAttack);
		}
		addan(MakeRunAway);
		make_player_move();
		apply_result();
	} else
		pause();
}

static void paint_value(abilityn id) {
	char temp[260]; stringbuilder sb(temp);
	sb.add("/x 120 text %2i\n%1", getname(id), player->abilities[id]);
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

static const char* get_avatar(classn type) {
	switch(type) {
	case ClericAir: return "ca";
	case Ranger: return "rn";
	default: return "hg";
	}
}

static void get_avatar(const void* object, stringbuilder& sb) {
	auto p = (creature*)object;
	sb.add(get_avatar(p->type));
	sb.add(p->isfemale() ? "f" : "m");
}

static int get_hits(const void* object) {
	auto p = (creature*)object;
	if(p->hp == p->mhp || !p->mhp)
		return 100;
	return p->hp * 100 / p->mhp;
}

static void paint_avatars() {
	paint_avatars((void**)party, lenghtof(party), get_avatar, player, get_hits);
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

static void paint_main_menu() {
	paint_bar(getname(PageCharacter), page_characters);
	paint_bar(getname(PageItems), page_items);
}

static void test_game() {
	create_area(Waste);
	create_creature(Fighter, Male);
	join_party();
	player->set(Enemy);
	opponent = player;
	create_creature(Ranger, Female);
	join_party();
	create_creature(Gladiator, Male);
	join_party();
	create_creature(ClericAir, Female);
	raise_level(3);
	join_party();
	select_creatures();
	move_distance = 100;
	adventure_move();
}

void stringbuilder_custom(stringbuilder& sb, const char* id);

void game_run() {
	srand(11299);
	stringbuilder::custom = stringbuilder_custom;
	atg_menu = paint_main_menu;
	test_game();
}