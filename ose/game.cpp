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
#include "bsdata.h"
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

const int yards_in_miles = 1000;

static variant last_result;
static bool need_break_actions;

unsigned game_var[PartyCoins + 1];

int last_number;

classn encounter_monsters;
reactionn last_reaction;

extern collectiona creatures;

template<> variant::variant(const settlement* p) : variant(Settlement, p - settlements) {}
template<> variant::variant(const creature* p) : variant(CreatureRef, p - bsdata<creature>::elements) {}

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

void make_party_move(const char* cancel_text) {
	last_result.u = (unsigned short)choose_answers(message_names[AskWhatToDo], cancel_text);
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

void make_player_move(const char* cancel_text) {
	last_result.u = (unsigned short)choose_answers(what_to_do(), cancel_text);
}

void make_any_player_move(const char* cancel_text) {
	last_result.u = (unsigned short)choose_player_option(cancel_text);
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

static creature* find_creature(featn v, bool keep, bool alive) {
	for(auto p : creatures.records<creature>()) {
		if(alive && !p->isready())
			continue;
		if(p->is(v) == keep)
			return p;
	}
	return 0;
}

static creature* get_enemy() {
	return find_creature(Enemy, !player->is(Enemy), true);
}

static bool enemy_present() {
	return find_creature(Enemy, true, true);
}

static bool party_present() {
	return find_creature(Enemy, false, true);
}

bool apply_combat(actionn v, bool run) {
	switch(v) {
	case MakeCharge:
		if(player->is(MeleeFight))
			return false;
		if(run) {
			player->act(PlayerCharged);
			player->set(Charged);
			player->set(MeleeFight);
			opponent->set(MeleeFight);
			make_attack(player, opponent, MeleeAttack, player->wears[MeleeWeapon], 2);
		}
		break;
	case MakeMeleeAttack:
		if(!player->is(MeleeFight))
			return false;
		if(run)
			make_attack(player, opponent, MeleeAttack, player->wears[MeleeWeapon], 0);
		break;
	case MakeMissileAttack:
		if(player->is(MeleeFight) || !player->wears[RangeWeapon] || !player->wears[Ammunition])
			return false;
		if(run) {
			make_attack(player, opponent, MissileAttack, player->wears[RangeWeapon], 0);
			player->useammo();
		}
		break;
	case MakeThrownAttack:
		if(player->is(MeleeFight) || !player->wears[MeleeWeapon].is(Thrown) || player->wears[MeleeWeapon].lost)
			return false;
		if(run) {
			make_attack(player, opponent, ThrownAttack, player->wears[MeleeWeapon], 0);
			player->wears[MeleeWeapon].lost = 1;
		}
		break;
	default:
		return false;
	}
	return true;
}

bool apply_camp(actionn v, bool run) {
	switch(v) {
	case MemorizeSpells:
		if(run)
			make_prepare_spells(PlayerMemorizeSpells);
		break;
	case RestParty:
		break;
	case MakeCamp:
		break;
	default:
		return false;
	}
	return true;
}

static void use_skill(actionn id) {
	auto bonus = skill_bonus(id, player->type) - 2;
	use_skill(id, bonus, true);
}

static void camp_actions() {
	pushvalue push(player);
	for(auto p : party) {
		if(!p)
			continue;
		player = p;
		use_skill(MakeGearRepairing);
		use_skill(MakeTendingWounds);
		use_skill(MakeTreatIllness);
		use_skill(MakeHunting);
		use_skill(MakeForaging);
	}
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
	if(consume(Berry))
		return;
	if(consume(Ration))
		return;
	player->act(PlayerSufferStarvation);
	player->starvation += d6();
}

static void camp_move() {
	answer_picture = ImageWastelandNight;
	sb.clear();
	fixmsg(MakeCampInOpenLand);
	camp_actions();
	while(true) {
		addopt(RestParty);
		if(player->getspells(1))
			addopt(MemorizeSpells);
		make_player_move();
		if(!last_result)
			break;
		else if(last_result == Continue)
			continue;
		apply_result();
	}
}

static void check_movement() {
	auto value = yards_in_miles * (party_average(Movement) * 10 / 5);
	auto modifier = get_movement_modifier(enviroment);
	value = value * modifier / 100;
	move_distance -= value;
}

static void take_items_options() {
	update_area_items();
	sb.add("Среди сундуков были горы разнообразных монет. В общей сложности вы насчитали %TreasureCoins монет. В куче монет вы заметили %Items.");
	an.add(0, "Забрать все");
}

static void generate_loot(classn type, int count) {
	treasure_generate(get_treasure(encounter_monsters), false, true, false);
	for(auto i = 0; i < count; i++)
		treasure_generate(get_treasure(encounter_monsters), false, false, true);
}

static int creature_count(bool is_dead, bool is_party) {
	auto result = 0;
	for(auto p : creatures.records<creature>()) {
		if(p->isparty() != is_party)
			continue;
		if(p->isdead() != is_dead)
			continue;
		result++;
	}
	return result;
}

static void loot_enemies() {
	if(!encounter_monsters)
		return;
	generate_loot(encounter_monsters, creature_count(true, false));
	pause(message_names[SearchBodies]);
	fixmsg(NothingValuableHere);
	pause();
}

static void combat_experience() {
	auto player_count = creature_count(false, true);
	if(!player_count)
		return;
	auto total = 0;
	for(auto p : creatures.records<creature>()) {
		if(p->isparty() || p->isdead())
			continue;
		total += p->award();
	}
	auto per_player = total / player_count;
	for(auto p : creatures.records<creature>()) {
		if(!p->isparty() || p->isdead())
			continue;
		p->addexp(per_player);
	}
}

static void combat_encounter() {
	pushvalue push_player(player);
	select_creatures();
	initiative_roll();
	while(enemy_present()) {
		for(auto p : creatures.records<creature>()) {
			if(!p->isready())
				continue;
			player = p;
			opponent = get_enemy();
			if(!opponent)
				continue;
			sb.addsep('\n');
			addopt(MakeCharge);
			addopt(MakeMeleeAttack);
			addopt(MakeThrownAttack);
			addopt(MakeMissileAttack);
			if(player->isparty()) {
				addopt(MakeRunAway);
				make_player_move();
			} else
				last_result.u = (unsigned short)an.random();
			apply_result();
		}
		if(enemy_present())
			pause();
	}
	if(party_present()) {
		loot_enemies();
		combat_experience();
	}
}

static void animal_encounter() {
	pushvalue push_player(player);
	encounter_monsters = random_animal(enviroment);
	create_monsters(encounter_monsters, true);
	player->act(PlayerJumpFromBrush);
	combat_encounter();
}

static bool check_encounter(int chance = 1) {
	auto result = 1 + rand() % 6;
	return result <= chance;
}

static void night_encounter() {
	sb.clear();
	if(check_encounter()) {
		player->act(PlayerHearNoiseOnWatch);
		pause();
		animal_encounter();
	} else
		player->act(CampNightEnd);
}

static void adventure_move() {
	pushvalue push_header(answer_header, "%AreaName");
	while(true) {
		add_header(ImageWasteland, "%AreaName");
		addopt(MakeCamp);
		make_party_move();
		camp_move();
		for_each_party(consume_food);
		check_movement();
		if(move_distance <= 0) {
			break;
		} else {
			night_encounter();
			sb.adds(message_names[AdventureNextDay]);
		}
	}
}

static void adventure_move(int miles) {
	move_distance += miles * yards_in_miles;
	adventure_move();
}

//void area_move() {
//	last_area->set(Known);
//	last_area->set(Visited);
//	while(true) {
//		auto type = last_area->type;
//		answer_picture = areasa[type].picture;
//		answer_header = "%AreaNameFull";
//		sb.clear();
//		sb.addn(area_look[type]);
//		add_area_visit(last_area->index());
//		add_area_actions(type);
//		const char* cancel_text = 0;
//		if(areasa[type].leave)
//			cancel_text = action_names[areasa[type].leave];
//		make_any_player_move(cancel_text);
//		if(!last_result)
//			break;
//		else if(last_result == Continue)
//			continue;
//		apply_result();
//	}
//}

//////////////////////////////////////////////////////
// WORK WITH SCENE

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

void add_header(picturen picture, const char* header) {
	answer_picture = picture;
	answer_header = header;
}

void add_look() {
	sb.clear();
	sb.addn(area_look[enviroment]);
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
	select_creatures();
	// treasure_generate("A", true, false, false);
	// add_magic_item(RandomMagicItem);
	// make_player_move(take_items_options);
	// adventure_move(50);
	last_settlement = find_settlement(MiddleKindom, Village);
	if(!last_settlement)
		last_settlement = find_settlement(MiddleKindom, SmallTown);
	create_market_items();
	settlement_move();
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