#include "adat.h"
#include "answers.h"
#include "area.h"
#include "bsdata.h"
#include "creature.h"
#include "collection.h"
#include "collectiona.h"
#include "dice.h"
#include "draw_atg.h"
#include "game.h"
#include "math.h"
#include "message.h"
#include "pushvalue.h"
#include "rand.h"
#include "stringbuilder.h"

BSDATAC(creature, 256)

creature* player;
creature* opponent;
creature* party[4];

collectiona creatures;

static bool critical_hit, critical_miss;
static int attack_roll;

static char saving_thrown_monsters[][5] = {
	{14, 15, 16, 17, 18},// 0
	{12, 13, 14, 15, 16},// 1
	{12, 13, 14, 15, 16},
	{12, 13, 14, 15, 16},
	{10, 11, 12, 13, 14},
	{10, 11, 12, 13, 14},// 5
	{10, 11, 12, 13, 14},
	{8, 9, 10, 10, 12},
	{8, 9, 10, 10, 12},
	{8, 9, 10, 10, 12},
	{6, 7, 8, 8, 10},// 10
	{6, 7, 8, 8, 10},
	{6, 7, 8, 8, 10},
	{4, 5, 6, 5, 8},
	{4, 5, 6, 5, 8},
	{4, 5, 6, 5, 8},// 15
	{2, 3, 4, 3, 6},
	{2, 3, 4, 3, 6},
	{2, 3, 4, 3, 6},
	{2, 2, 2, 2, 4},
	{2, 2, 2, 2, 4},// 20
	{2, 2, 2, 2, 4},
	{2, 2, 2, 2, 2},// 22
};
static char saving_thrown_fighter[][5] = {
	{12, 13, 14, 15, 16},
	{12, 13, 14, 15, 16}, // 1
	{12, 13, 14, 15, 16},
	{12, 13, 14, 15, 16},
	{10, 11, 12, 13, 14},
	{10, 11, 12, 13, 14}, // 5
	{10, 11, 12, 13, 14},
	{8, 9, 10, 10, 12},
	{8, 9, 10, 10, 12},
	{8, 9, 10, 10, 12},
	{6, 7, 8, 8, 10}, // 10
	{6, 7, 8, 8, 10},
	{6, 7, 8, 8, 10},
	{4, 5, 6, 5, 8},
	{4, 5, 6, 5, 8}, // 14
};
static char saving_thrown_halfling[][5] = {
	{8, 9, 10, 13, 12},
	{8, 9, 10, 13, 12}, // 1
	{8, 9, 10, 13, 12},
	{8, 9, 10, 13, 12},
	{6, 7, 8, 10, 10},
	{6, 7, 8, 10, 10}, // 5
	{6, 7, 8, 10, 10},
	{4, 5, 6, 7, 8},
	{4, 5, 6, 7, 8}, // 8
};
static char saving_thrown_theif[][5] = {
	{13, 14, 13, 16, 15},
	{13, 14, 13, 16, 15}, // 1
	{13, 14, 13, 16, 15},
	{13, 14, 13, 16, 15},
	{13, 14, 13, 16, 15},
	{12, 13, 11, 14, 13}, // 5
	{12, 13, 11, 14, 13},
	{12, 13, 11, 14, 13},
	{12, 13, 11, 14, 13},
	{10, 11, 9, 12, 10},
	{10, 11, 9, 12, 10}, // 10
	{10, 11, 9, 12, 10},
	{10, 11, 9, 12, 10},
	{8, 9, 7, 10, 8},
	{8, 9, 7, 10, 8}, // 14
};
static char attack_bonuses[][15] = {
	{0, 0, 0, 0, 0, 0, 2, 2, 2, 2, 2, 5, 5, 5, 5},
	{0, 0, 0, 0, 0, 2, 2, 2, 2, 5, 5, 5, 5, 7, 7},
	{0, 0, 0, 0, 2, 2, 2, 5, 5, 5, 7, 7, 7, 9, 9},
	{-1, 0, 1, 2, 3, 4, 5, 6, 6, 7, 7, 8, 8, 9, 9},
};
static char ability_bonus[19] = {
	-4, -4, -4, -3, -2, -2, -1, -1, -1, 0,
	0, 0, 0, 1, 1, 1, 2, 2, 3
};
static char reaction_bonus[19] = {
	-3, -3, -3, -2, -1, -1, -1, -1, -1, 0,
	0, 0, 0, 1, 1, 1, 1, 1, 2
};
static char caster_spells[][15][7] = {
	{},
	// 1 - Priest, Druid
	{{0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0}, // 1
	{0, 1, 0, 0, 0, 0, 0},
	{0, 2, 0, 0, 0, 0, 0},
	{0, 2, 1, 0, 0, 0, 0},
	{0, 2, 2, 0, 0, 0, 0}, // 5
	{0, 2, 2, 1, 1, 0, 0},
	{0, 2, 2, 2, 2, 0, 0},
	{0, 3, 3, 2, 2, 1, 0},
	{0, 3, 3, 3, 2, 1, 0},
	{0, 4, 4, 3, 3, 2, 0}, // 10
	{0, 4, 4, 4, 3, 2, 0},
	{0, 5, 5, 4, 4, 3, 0},
	{0, 4, 5, 5, 4, 4, 0},
	{0, 6, 5, 5, 5, 4, 0}}, // 14
	// 2 - Magic-User
	{{0, 0, 0, 0, 0, 0, 0},
	{0, 1, 0, 0, 0, 0, 0}, // 1
	{0, 2, 0, 0, 0, 0, 0},
	{0, 2, 1, 0, 0, 0, 0},
	{0, 2, 2, 0, 0, 0, 0},
	{0, 2, 2, 1, 0, 0, 0}, // 5
	{0, 2, 2, 2, 0, 0, 0},
	{0, 2, 2, 2, 1, 0, 0},
	{0, 3, 3, 2, 2, 0, 0},
	{0, 3, 3, 3, 2, 1, 0},
	{0, 3, 3, 3, 3, 2, 0}, // 10
	{0, 4, 3, 3, 3, 2, 1},
	{0, 4, 4, 3, 3, 3, 2},
	{0, 4, 4, 4, 3, 3, 3},
	{0, 4, 4, 4, 4, 3, 3}}, // 14
};

static int get_caster(classn v) {
	switch(v) {
	case Cleric: case Elf:
		return 1;
	case MagicUser:
		return 2;
	default: return 0;
	}
}

static int get_attacks(classn v) {
	switch(v) {
	case Fighter: case Halfling: case Elf: case Dwarf:
		return 2;
	case Theif: case Cleric:
		return 1;
	default:
		return 0;
	}
}

static int get_hit_die(classn v) {
	switch(v) {
	case Fighter: case Dwarf:
		return 8;
	case Theif: case MagicUser:
		return 4;
	default:
		return 6;
	}
}

bool is_enemy(const void* object) {
	return player->is(Enemy) != ((creature*)object)->is(Enemy);
}

bool is_wounded(const void* object) {
	return ((creature*)object)->hp < ((creature*)object)->mhp;
}

bool is_diseased(const void* object) {
	return ((creature*)object)->disease > 0;
}

static domainn get_spells(classn v) {
	switch(v) {
	case Cleric: return CosmicSpells;
	case MagicUser: return ArcaneSpells;
	default: return (domainn)0;
	}
}

static void add_base(abilityn id, int v) {
	player->basic.abilities[id] += v;
}

static void add_value(abilityn id, int v) {
	player->abilities[id] += v;
}

static void apply_wear(itemn v) {
	switch(v) {
	case LeatherArmor: add_value(AC, 2); break;
	case Shield: add_value(AC, 1); break;
	default: break;
	}
}

abilityn get_primary(classn v) {
	switch(v) {
	case Cleric: return Wisdom;
	case MagicUser: return Intelligence;
	default: return Strenght;
	}
}

static abilityn get_secondary(classn v) {
	return Constitution;
}

void creature::clear() {
	memset((void*)this, 0, sizeof(*this));
}

void creature::act(messagen id) const {
	actv(' ', getname(id), 0);
}

void creature::actv(char separator, const char* format, const char* format_param) const {
	auto push_player = player; player = const_cast<creature*>(this);
	sb.addsep(separator);
	sb.addv(format, format_param);
	player = push_player;
}

bool creature::iscaster() const {
	return get_caster(type) != 0;
}

int creature::getbonus(abilityn v) const {
	auto m = player->abilities[v];
	return maptbl(ability_bonus, m);
}

const char* creature::name() const {
	if(customname)
		return getname(customname);
	return getname(type);
}

int creature::getspells(int level) const {
	char caster = get_caster(type);
	if(!caster)
		return 0;
	char hd = imin(abilities[HD], 14);
	return caster_spells[caster][hd][level];
}

int creature::getskill(abilityn v) const {
	switch(v) {
	case Strenght: case Dexterity: case Constitution:
	case Wisdom: case Charisma: case Intelligence:
		return abilities[v] * 5;
	default:
		return abilities[v];
	}
}

int creature::getspellsprepared(int level) const {
	auto result = prepare.total(get_spells(type), level);
	return result;
}

void creature::damage(int value) {
	last_number = value;
	hp -= value;
	if(hp > 0)
		act(MsgDamage);
	else
		act(MsgDamageAndDead);
}

bool creature::roll(abilityn v, int bonus) const {
	auto chance = getskill(v) + bonus * 5;
	if(chance <= 0)
		return false;
	else if(chance >= 95)
		chance = 95;
	auto result = d100();
	return result < chance;
}

static int get_base_movement(classn type) {
	return 12;
}

static void attack_bonus(int bonus) {
	add_value(MeleeAttack, bonus);
	add_value(ThrownAttack, bonus);
	add_value(MissileAttack, bonus);
}

static void update_abilities() {
	auto bonus = player->getbonus(Strenght);
	add_value(MeleeAttack, bonus);
	add_value(MeleeDamage, bonus);
	add_value(ThrownDamage, bonus);
	bonus = player->getbonus(Dexterity);
	add_value(MissileAttack, bonus);
	add_value(ThrownAttack, bonus);
	add_value(AC, 10 + bonus);
	if(player->is(Charged))
		add_value(AC, -1); // RULE: Charged creatures have little balace and gain penalty to AC.
	attack_bonus(attack_bonuses[get_attacks(player->type)][player->abilities[HD]]);
	bonus = maptbl(reaction_bonus, player->abilities[Dexterity]);
	add_value(Initiative, bonus);
	if(player->is(Flee))
		attack_bonus(-4);
	bonus = player->getbonus(Constitution);
	add_value(Movement, get_base_movement(player->type));
	player->mhp = imax((int)player->abilities[HD], player->abilities[Hits] + player->abilities[HD] * bonus);
}

static void update_wear() {
	for(auto& e : player->wears) {
		if(!e)
			continue;
		if(!player->isusable(e))
			continue;
		apply_wear(e.type);
	}
}

static void update_enchant(variant object) {
	if(!object)
		return;
	for(auto& e : bsdata<enchanti>()) {
		if(e.object == object)
			player->active.set(e.spell);
	}
}

static char* get_saving_throws(classn type, int level) {
	switch(type) {
	case Fighter: return maptbl(saving_thrown_fighter, level);
	case Halfling: return maptbl(saving_thrown_halfling, level);
	case Monster: return maptbl(saving_thrown_monsters, level / 2);
	case Theif: return maptbl(saving_thrown_theif, level);
	default: return maptbl(saving_thrown_monsters, level);
	}
}

static void update_saving_throws() {
	auto pn = get_saving_throws(get_race(player->type), player->abilities[HD]);
	for(auto i = SaveDeath; i <= SaveSpells; i = (abilityn)(i + 1))
		player->abilities[i] += pn[i - SaveDeath];
}

static void copy_abilities(statable& dest, const statable& source) {
	memcpy(dest.abilities, source.abilities, Lore * sizeof(abilityn));
}

static void update_player() {
	copy_abilities(*player, player->basic);
	update_enchant(player);
	// update_enchant(player->getarea());
	update_abilities();
	update_saving_throws();
	update_wear();
}

void creature::update() {
	pushvalue push(player, this);
	update_player();
}

static int compare_char(const void* v1, const void* v2) {
	return *((char*)v2) - *((char*)v1);
}

static int ability_index(abilityn* source, abilityn v) {
	for(auto i = 0; i < 6; i++) {
		if(source[i] == v)
			return i;
	}
	return 0;
}

static void roll_abilities(char* abilities, abilityn p1, abilityn p2) {
	char rolled[6] = {};
	abilityn indecies[6] = {Strenght, Dexterity, Constitution, Intelligence, Wisdom, Charisma};
	for(auto& v : rolled)
		v = 6 + d6() + d6();
	zshuffle(indecies, sizeof(indecies) / sizeof(indecies[0]));
	if(p1)
		iswap(indecies[0], indecies[ability_index(indecies, p1)]);
	if(p2)
		iswap(indecies[1], indecies[ability_index(indecies, p2)]);
	qsort(rolled, sizeof(rolled), sizeof(rolled[0]), compare_char);
	for(auto i = 0; i < 6; i++)
		abilities[indecies[i]] = rolled[i];
}

static void create_ability(classn type) {
	auto p1 = get_primary(type);
	auto p2 = get_secondary(type);
	roll_abilities(player->basic.abilities, p1, p2);
}

static void create_average_ability() {
	for(auto i = Strenght; i <= Charisma; i = (abilityn)(i + 1))
		player->basic.abilities[i] = 10;
}

static void start_equip(classn type) {
	switch(type) {
	case Fighter:
		player->equip(LeatherArmor);
		player->equip(ShortSword);
		break;
	case Cleric:
		player->equip(LeatherArmor);
		player->equip(Mace);
		break;
	case Theif:
		player->equip(LeatherArmor);
		player->equip(Dagger);
		break;
	default:
		break;
	}
}

static void add_hit_points(bool reroll_low) {
	auto hd = get_hit_die(player->type);
	if(!player->mhp) {
		auto rolled = 0;
		if(reroll_low && hd >= 4) {
			while(rolled < 3)
				rolled = xrand(1, hd);
		} else
			rolled = xrand(1, hd);
		player->basic.abilities[Hits] = rolled;
	} else
		player->basic.abilities[Hits] += xrand(1, hd);
}

void raise_level(int level, bool reroll_lowest) {
	auto hit_percent = 100;
	if(player->mhp)
		hit_percent = player->hp * 100 / player->mhp;
	while(player->basic.abilities[HD] < level) {
		add_hit_points(reroll_lowest);
		player->basic.abilities[HD]++;
	}
	update_player();
	player->hp = player->mhp * hit_percent / 100;
}

static void create_finish() {
	player->hp = player->mhp;
}

void create_creature(classn type, gendern gender) {
	player = bsdata<creature>::addz();
	player->clear();
	player->type = type;
	player->gender = gender;
	player->portrait = random_portrait(type, gender);
	//	player->setname();
	create_ability(type);
	start_equip(type);
	raise_level(1, true);
	create_finish();
}

static int get_level(classn v) {
	return 1;
}

static int get_dungeon_count(classn type) {
	switch(type) {
	case Elf: return xrand(2, 12);
	default: return 1;
	}
}

static int get_wilderness_count(classn type) {
	return 1;
}

void create_monster(classn type) {
	player = bsdata<creature>::addz();
	player->clear();
	player->type = type;
	// player->gender = Male;
	create_average_ability();
	start_equip(type);
	raise_level(get_level(type));
	player->set(Local);
	create_finish();
}

void create_monsters(classn type) {
	auto count = get_wilderness_count(type);
	for(auto n = 0; n < count; n++)
		create_monster(type);
}

static void update_party() {
	auto ps = party;
	for(auto p : party) {
		if(p)
			*ps++ = p;
	}
	while(ps < party + sizeof(party) / sizeof(party[0]))
		*ps++ = 0;
}

void join_party() {
	for(auto& e : party) {
		if(!e) {
			e = player;
			return;
		}
	}
}

static int compare_initiative(const void* v1, const void* v2) {
	auto p1 = *((creature**)v1);
	auto p2 = *((creature**)v2);
	auto s1 = p1->is(Slow) ? 1 : 0;
	auto s2 = p2->is(Slow) ? 1 : 0;
	if(s1 != s2)
		return s1 - s2;
	return p2->initiative - p1->initiative;
}

void initiative_roll() {
	for(auto p : creatures.records<creature>())
		p->initiative = d6() + p->get(Initiative);
	qsort(creatures.data, creatures.count, sizeof(creatures.data[0]), compare_initiative);
}

static int critical_damage(const item& weapon, const dice& damage) {
	auto hits = damage.roll(); // Apply damage
	if(weapon.is(Pierce))
		hits += damage.roll(); // Apply another damage (can be third)
	if(weapon.deadly())
		hits += damage.maximum(); // Deadly apply maximum damage
	else
		hits += damage.roll(); // Apply damage
	return hits;
}

dice get_attack(creature* attacker, abilityn id, const item& weapon, int bonus) {
	dice result = {1, 2};
	return result;
}

static bool check_attack(creature* attacker, creature* enemy, int attack_bonus, int attack_sharpness) {
	auto rolled = d20();
	auto critical_range = 20 - attack_sharpness;
	attack_roll = rolled + attack_bonus;
	critical_miss = (rolled == 1);
	critical_hit = (rolled >= critical_range);
	if(critical_miss) {
		attacker->act(PlayerCriticalMiss);
		return false;
	} else if(critical_hit) {
		attacker->act(PlayerCriticalHit);
		return true;
	} else if(attack_roll >= enemy->get(AC)) {
		attacker->act(PlayerHit);
		return true;
	}
	attacker->act(PlayerMiss);
	return false;
}

static bool weapon_damage(item& weapon) {
	auto chance_break = 20;
	if(d100() < chance_break) {
		if(weapon.damage())
			weapon.act(WeaponBroken);
		else
			weapon.act(WeaponDamage);
		return true;
	}
	return false;
}

bool make_attack(creature* attacker, creature* enemy, abilityn ability, item& weapon, int attack_bonus) {
	if(check_attack(attacker, enemy, attack_bonus + attacker->get(ability), weapon.is(Slashing))) {
		auto damage = get_attack(attacker, ability, weapon, attack_bonus);
		if(critical_hit) {
			enemy->damage(critical_damage(weapon, damage));
			if(weapon.is(Blunt) && enemy->isready()) {
				if(!enemy->roll(SaveParalysis)) {
					enemy->set(Stunned);
					enemy->act(PlayerStunned);
				}
			}
		} else
			enemy->damage(damage.roll());
		return true;
	} else if(critical_miss)
		weapon_damage(weapon);
	return false;
}

static void select_items(creature* player) {
	if(!player)
		return;
	for(auto& e : player->wears) {
		if(e)
			targets.add(&e);
	}
}

static void select_party_items() {
	targets.clear();
	for(auto p : party)
		select_items(p);
}

void for_each_party(fnevent proc) {
	pushvalue push(player);
	for(auto p : party) {
		player = p;
		proc();
	}
}

static itemn forage_result[] = {Mushrooms, Berry, Herbs};

static bool have_hunting_weapons() {
	return ((player->is(ShortBow) || player->is(LongBow)) && player->is(Arrow))
		|| player->is(Javelin);
}

int skill_bonus(actionn v, classn type) {
	auto r = 0;
	switch(v) {
	case MakeForaging:
		switch(type) {
		case Elf: r += 3; break;
		case Cleric: r += 2; break;
		case MagicUser: r += 1; break;
		default: break;
		}
		break;
	case MakeGearRepairing:
		switch(type) {
		case Theif: case Fighter: r += 1; break;
		case Dwarf: r += 3; break;
		default: break;
		}
		break;
	case MakeTreatIllness:
		r -= 3;
		switch(type) {
		case Cleric: r += 3; break;
		case Elf: r += 2; break;
		default: break;
		}
		break;
	case MakeTendingWounds:
		switch(type) {
		case Cleric: r += 3; break;
		case Fighter: r += 1; break;
		default: break;
		}
		break;
	case MakeHunting:
		switch(type) {
		case Elf: r += 3; break;
		case Fighter: r += 1; break;
		default: break;
		}
		break;
	default: break;
	}
	return r;
}

bool use_skill(actionn id, int bonus, bool run) {
	auto target_count = 1;
	switch(id) {
	case MakeTendingWounds:
		targets = creatures;
		targets.match(is_enemy, false);
		targets.match(is_wounded, true);
		if(!targets)
			return false;
		break;
	case MakeGearRepairing:
		select_party_items();
		targets.match(is_damaged, true);
		if(!targets)
			return false;
		break;
	case MakeTreatIllness:
		targets = creatures;
		targets.match(is_enemy, false);
		targets.match(is_diseased, true);
		if(!targets)
			return false;
		break;
	case MakeForaging:
		if(!last_area->outdoor())
			return false;
		break;
	case MakeHunting:
		if(player->isbadlyhurt())
			return false; // RULE: You can't hunting when you are badly hurt
		if(!have_hunting_weapons())
			return false; // RULE: Hunting work only if you use hunting weapons: bows (and arrows) or javelings.
		break;
	default:
		break;
	}
	if(targets.count > (unsigned)target_count) {
		targets.shuffle();
		targets.top(target_count);
	}
	if(run) {
		if(!player->roll(Wisdom, bonus))
			return false;
		switch(id) {
		case MakeTreatIllness:
			for(auto p : targets.records<creature>()) {
				p->disease--;
				p->act(PlayerTreatedIllness);
			}
			break;
		case MakeTendingWounds:
			for(auto p : targets.records<creature>()) {
				p->hp++;
				p->act(PlayerTreatedIllness);
			}
			break;
		case MakeGearRepairing:
			for(auto p : targets.records<item>()) {
				p->broken--;
				p->act(PlayerRepairGear);
			}
			break;
		case MakeForaging:
			player->add(maprnd(forage_result));
			player->act(PlayerForageItem);
			break;
		case MakeHunting:
			player->add(some(RawMeat));
			player->act(PlayerHuntingGame);
			break;
		default:
			break;
		}
	}
	return true;
}

const char* what_to_do() {
	static char temp[260]; stringbuilder sb(temp);
	sb.clear();
	sb.add(getname(AskWhatToDo), player->name());
	return temp;
}

static void addleft(stringbuilder& sb, int current, int maximum) {
	auto left = maximum - current;
	if(left <= 0)
		return;
	sb.adds(getname(AskLeft), left, current, maximum);
}

static void modify_spells(messagen id, spellable& e, int level) {
	char temp[128]; stringbuilder sbn(temp);
	while(true) {
		auto prepare_spells = e.total(records);
		auto maximum_spells = player->getspells(level);
		sb.clear();
		fixmsg(id);
		fixlist(e);
		an.clear();
		if(prepare_spells < maximum_spells) {
			for(auto v : records)
				an.add(v, getname((spelln)v));
		}
		an.sort();
		an.add(-2, getname(ClearAllList));
		an.add(-1, getname(Confirm));
		sbn.clear(); sbn.add(getname(AskMemorizeSpells), level);
		addleft(sbn, prepare_spells, maximum_spells);
		auto result = an.choose(temp);
		if(result == -1)
			break;
		else if(result == -2) {
			for(auto v : records)
				e.spells[v] = 0;
		} else
			e.spells[result]++;
	}
}

void make_prepare_spells(messagen id) {
	while(true) {
		sb.clear();
		fixmsg(id);
		fixlist(player->prepare);
		an.clear();
		for(auto i = 1; i <= 6; i++) {
			auto total = player->getspells(i);
			auto current = player->getspellsprepared(i);
			if(player->getspells(i)) {
				if(current < total)
					an.add(i, getname(ChangeSpellsByLevelAllowed), i, total - current);
				else
					an.add(i, getname(ChangeSpellsByLevel), i);
			}
		}
		auto level = make_player_move(getname(Confirm));
		if(!level) {
			sb.clear();
			break;
		}
		records.clear();
		add_spells(get_spells(player->type), level);
		modify_spells(id, player->prepare, level);
	}
}

int party_average(abilityn v) {
	auto count = 0;
	auto total = 0;
	for(auto p : party) {
		if(p && p->isready()) {
			total += p->get(v);
			count++;
		}
	}
	if(!count)
		return 0;
	return total / count;
}