#include "answers.h"
#include "bsdata.h"
#include "character.h"
#include "draw_atg.h"
#include "game.h"
#include "math.h"
#include "message.h"
#include "rand.h"
#include "roll.h"
#include "stringvar.h"

itemfn scene_range;
int	bonus_damage, bonus_enemy_damage;

static void fixpause() {
	an.choose(0, getname(Next));
}

void fixclear() {
	if(sb)
		fixpause();
	sb.clear();
}

item* wearable::chooseitem(const char* title, const char* cancel, fnvisible proc, bool keep) {
	an.clear();
	for(auto& e : wears) {
		if(!e)
			continue;
		if(proc && proc(&e) != keep)
			continue;
		an.add((long)&e, e.name());
	}
	an.sort();
	return (item*)an.choose(title, cancel);
}

static void stringbuilder_custom(stringbuilder& sb, const char* id) {
	if(stringvar_identifier(sb, id))
		return;
	default_string(sb, id);
}

static void change_weapon() {
	an.clear();
	for(auto& e : slice(player->wears + Hands + 1, player->wears + WearLast + 1)) {
		if(e && e.isweapon())
			an.add((long)&e, e.name());
	}
	if(!an)
		return;
	auto pi = (item*)an.choose();
	if(!pi)
		return;
	iswap(player->wears[Hands], *pi);
	player->act(MsgReadyWeapon);
}

static void prepare_to_worse() {
}

static void deal_damage() {
}

static void enemy_deal_damage() {
}

static void game_master_move() {
}

static void add_option(messagen move) {
	switch(move) {
	case MsgVolleyUseAmmo:
		if(!player->haveitem(Ammo1))
			return;
		break;
	}
	an.add(move, getname(move));
}

static void enemy_move_forward() {
	if(scene_range >= Close)
		scene_range = (itemfn)(scene_range - 1);
}

static void apply_options(messagen move) {
	switch(move) {
	case MsgVolleyUseAmmo:
		player->use(Ammo1);
		break;
	case MsgVolleyWeak:
		bonus_damage -= d6();
		break;
	case MsgApplyAdditionalDamage:
		bonus_damage += d6();
		break;
	case MsgEvadeEnemyAttack:
		bonus_damage += d6();
		break;
	case MsgVolleyEnemyMove:
		if(scene_range >= Near && enemy.is(scene_range))
			enemy_deal_damage();
		else
			enemy_move_forward();
		break;
	}
}

static void choose_options(slice<messagen> source, int count = 1) {
	char temp[128]; stringbuilder sb(temp);
	bonus_damage = 0;
	bonus_enemy_damage = 0;
	while(count > 0) {
		an.clear();
		for(auto v : source)
			add_option(v);
		sb.clear();
		if(count > 1)
			sb.add(getname(ChooseOptionLeftCount), count);
		else
			sb.add(getname(ChooseOption));
		auto m = (messagen)an.choose(temp, 0);
		apply_options(m);
	}
}

static bool make_roll(statn stat) {
	roll_bonus = player->get(stat);
	make_roll_raw();
	switch(roll_effect) {
	case CriticalSuccess:
	case Success:
		sb.addn("[+{%1i%+2i=%3i}]", roll_dices_result, roll_bonus, roll_result);
		break;
	case PartialSuccess:
		sb.addn("{%1i%+2i=%3i}", roll_dices_result, roll_bonus, roll_result);
		break;
	default:
		sb.addn("[-{%1i%+2i=%3i}]", roll_dices_result, roll_bonus, roll_result);
		player->markexperience();
		break;
	}
	return roll_effect >= PartialSuccess;
}

static bool move_hack_and_slash(bool run) {
	static messagen options[] = {MsgEvadeEnemyAttack, MsgApplyAdditionalDamage};
	if(!enemy)
		return false;
	if(scene_range > Reach)
		return false;
//	if(!player->is(scene_range))
//		return false;
	if(run) {
		make_roll(Strenght);
		if(roll_effect <= Fail)
			enemy_deal_damage();
		else if(roll_effect == PartialSuccess) {
			fixmsg(MsgHackAndSlashHit);
			deal_damage();
			enemy_deal_damage();
		} else {
			fixmsg(MsgHackAndSlashHit);
			choose_options(options, 1);
			deal_damage();
			enemy_deal_damage();
		}
	}
	return true;
}

static bool move_volley(bool run) {
	static messagen options[] = {MsgVolleyUseAmmo, MsgVolleyWeak, MsgVolleyEnemyMove};
	if(!enemy)
		return false;
	if(!player->is(scene_range))
		return false;
	if(run) {
		make_roll(Dexterity);
		if(roll_effect <= Fail) {
			if(enemy.is(scene_range))
				enemy_deal_damage();
			else
				game_master_move();
		} else if(roll_effect == PartialSuccess) {
			fixmsg(MsgVolleyHit);
			choose_options(options, 1);
			deal_damage();
		} else {
			fixmsg(MsgVolleyHit);
			deal_damage();
		}
	}
	return true;
}

void game_run() {
	answers::resid = "Wasteland";
	stringbuilder::custom = stringbuilder_custom;
	enemy.create(Skeleton);
	srand(2311);
	player = bsdata<character>::add();
	move_hack_and_slash(true);
	fixclear();
}

static void print_hands(stringbuilder& sb) {
	sb.add(player->wears[Hands].name());
}

static void print_player(stringbuilder& sb) {
	sb.add(player->getname());
}

static void print_enemy(stringbuilder& sb) {
	sb.add(enemy.getname());
}

BSDATA(stringvari) = {
	{"Enemy", print_enemy},
	{"Hands", print_hands},
	{"Player", print_player}
};
BSDATAF(stringvari)