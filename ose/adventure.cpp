#include "area.h"
#include "creature.h"
#include "game.h"
#include "pushvalue.h"
#include "rand.h"
#include "settlement.h"

const int yards_in_miles = 1000;

static int move_distance, move_distance_original;

static void animal_encounter() {
	pushvalue push_player(player);
	encounter_monsters = random_animal(enviroment);
	create_monsters(encounter_monsters, true);
	player->act(PlayerJumpFromBrush);
	combat_encounter();
}

static int get_movement_modifier(arean type) {
	switch(type) {
	case Mountains: case Jungle: case Swamps: return 50;
	case Sands: case Wastes: case Hills: return 67;
	default: return 100;
	}
}

static void check_movement() {
	auto value = yards_in_miles * (party_average(Movement) * 10 / 5);
	auto modifier = get_movement_modifier(enviroment);
	value = value * modifier / 100;
	move_distance -= value;
}

static bool check_encounter(int chance = 1) {
	auto result = 1 + rand() % 6;
	return result <= chance;
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

static void night_encounter() {
	clear_messages();
	if(check_encounter()) {
		player->act(PlayerHearNoiseOnWatch);
		pause();
		animal_encounter();
	} else
		player->act(CampNightEnd);
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

static bool apply_camp(actionn v, bool run) {
	switch(v) {
	case MemorizeSpells:
		if(!player->getspells(1))
			return false;
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

static void camp_move() {
	static actionn actions[] = {MemorizeSpells};
	clear_messages();
	addhdr(getimagenight(enviroment));
	addn(MakeCampInOpenLand);
	camp_actions();
	while(true) {
		for(auto n : actions) {
			if(apply_camp(n, false))
				addopt(n);
		}
		auto result = choose_player_option(action_names[RestParty]);
		if(!result)
			break;
		else if(result == Continue)
			continue;
		apply_camp((actionn)result, true);
	}
}

static void adventure_move() {
	while(true) {
		pause();
		addhdr(getimage(enviroment), "%Area");
		adds(AdventureNextDay);
		addopt(MakeCamp);
		auto result = (actionn)choose_party_option(0);
		switch(result) {
		case MakeCamp: camp_move(); break;
		default: break;
		}
		for_each_party(consume_food);
		check_movement();
		if(move_distance <= 0)
			break;
		else
			night_encounter();
	}
}

void adventure_move(int miles) {
	move_distance += miles * yards_in_miles;
	adventure_move();
}