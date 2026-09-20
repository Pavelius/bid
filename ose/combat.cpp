#include "answers.h"
#include "creature.h"
#include "collectiona.h"
#include "game.h"
#include "pushvalue.h"
#include "stringbuilder.h"

extern collectiona creatures;
extern stringbuilder sb;

static void generate_loot(classn type, int count) {
	treasure_generate(get_treasure(encounter_monsters), false, true, false);
	for(auto i = 0; i < count; i++)
		treasure_generate(get_treasure(encounter_monsters), false, false, true);
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

bool enemy_present() {
	return find_creature(Enemy, true, true);
}

static bool party_present() {
	return find_creature(Enemy, false, true);
}

static bool apply_combat(actionn v, bool run) {
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
	addn(NothingValuableHere);
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

void combat_encounter() {
	variant result;
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
				result.u = (unsigned short)choose_answers(what_to_do());
			} else
				result.u = (unsigned short)an.random();
			switch(result.type) {
			case Action: apply_combat((actionn)result.value, true); break;
			default: break;
			}
		}
		if(enemy_present())
			pause();
	}
	if(party_present()) {
		loot_enemies();
		combat_experience();
	}
}