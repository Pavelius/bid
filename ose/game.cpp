#include "area.h"
#include "answers.h"
#include "bsdata.h"
#include "collection.h"
#include "collectiona.h"
#include "creature.h"
#include "draw.h"
#include "draw_atg.h"
#include "game.h"
#include "message.h"
#include "print.h"
#include "pushvalue.h"
#include "rand.h"
#include "stringbuilder.h"
#include "variant.h"

const int yards_in_miles = 1000;

gamei game;
int last_number;

classn encounter_monsters;
reactionn last_reaction;
static variant last_result;

extern collectiona creatures;
extern collectiona items;

template<> variant::variant(const area* p) : variant(AreaRef, p - bsdata<area>::elements) {}
template<> variant::variant(const creature* p) : variant(CreatureRef, p - bsdata<creature>::elements) {}

void pause(const char* format) {
	if(!sb)
		return;
	an.clear();
	an.add(1, format);
	an.choose(0, 0);
	sb.clear();
	an.clear();
}

void pause() {
	pause(getname(Continue));
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
	auto p_console = sb.get();
	pushvalue push(current_avatar_post, (long)ChangePlayer);
	current_avatar = (void*)player;
	while(true) {
		sb.set(p_console);
		an.clear();
		options_proc();
		last_result.u = (short unsigned)make_player_move();
		if(last_result.u == ChangePlayer) {
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

static bool apply_effect(actionn v, bool run) {
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
			player->useammo();
		}
		break;
	case MemorizeSpells:
		if(run)
			make_prepare_spells(PlayerMemorizeSpells);
		break;
	case RestParty:
		break;
	default:
		return false;
	}
	return true;
}

static void addan(actionn n) {
	if(apply_effect(n, false))
		an.add(variant(n), getname(n));
}

static void apply_result() {
	switch(last_result.type) {
	case Action: apply_effect((actionn)last_result.value, true); break;
	default: break;
	}
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
	if(consume(Berry))
		return;
	if(consume(Ration))
		return;
	player->act(PlayerSufferStarvation);
	player->starvation += d6();
}

static void camp_move() {
	pushvalue push_header(answers::picture, ImageWastelandNight);
	sb.clear();
	fixmsg(MakeCampInOpenLand);
	camp_actions();
	while(true) {
		make_player_move(camp_options);
		if(!last_result || last_result == variant(RestParty))
			return;
		apply_result();
	}
}

static void check_movement() {
	auto value = yards_in_miles * (party_average(Movement) * 10 / 5);
	auto modifier = get_movement_modifier(last_area->type);
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

static int body_count(bool is_party) {
	auto result = 0;
	for(auto p : creatures.records<creature>()) {
		if(p->isparty() != is_party)
			continue;
		if(!p->isdead())
			continue;
		result++;
	}
	return result;
}

static void loot_enemies() {
	if(!encounter_monsters)
		return;
	generate_loot(encounter_monsters, /*чbody_count(false)*/7);
	pause(getname(SearchBodies));
	fixmsg(NothingValuableHere);
	pause();
}

static void combat_experience() {
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
			an.clear();
			addan(MakeCharge);
			addan(MakeMeleeAttack);
			addan(MakeThrownAttack);
			addan(MakeMissileAttack);
			if(player->isparty()) {
				addan(MakeRunAway);
				last_result.u = (unsigned short)make_player_move();
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
	encounter_monsters = random_animal(last_area->type);
	create_monsters(encounter_monsters, true);
	player->act(PlayerJumpFromBrush);
	combat_encounter();
}

static void night_encounter() {
	pushvalue push_header(answers::picture, ImageWastelandNight);
	sb.clear();
	player->act(PlayerHearNoiseOnWatch);
	pause();
	animal_encounter();
}

static void adventure_move() {
	pushvalue push_header(answers::header, "%AreaName");
	answers::picture = ImageWasteland;
	while(true) {
		sb.adds(getinfo(last_area->type));
		addan(MakeCamp);
		make_party_move();
		camp_move();
		for_each_party(consume_food);
		check_movement();
		if(move_distance <= 0) {
			break;
		} else {
			night_encounter();
			pause();
			sb.addn(getname(AdventureNextDay));
		}
	}
}

static void adventure_move(int miles) {
	move_distance += miles * yards_in_miles;
	adventure_move();
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

static void paint_value(const creature* p) {
	char temp[260]; stringbuilder sb(temp);
	if(p->isdead())
		sb.add("[~%1]", p->name());
	else {
		sb.add(p->name());
		if(p->isbadlyhurt())
			sb.adds("[~%1]", getname(StateWounded));
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
	paint_bar(getname(PageCharacter), page_characters);
	paint_bar(getname(PageItems), page_items);
	if(creatures && enemy_present())
		paint_bar(getname(PageCombatants), page_combatants);
}

static void test_game() {
	create_area(Forest);
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
	adventure_move(50);
}

void stringbuilder_custom(stringbuilder& sb, const char* id);

void main_util();
bool pass_test();

void game_run() {
	// srand(rseed());
	srand(1281);
	stringbuilder::custom = stringbuilder_custom;
	atg_menu = paint_main_menu;
	if(!pass_test())
		return;
#ifdef _DEBUG
	main_util();
#endif
	test_game();
}