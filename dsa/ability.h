#pragma once

enum abilityn : unsigned char {
	HD, Strenght, Dexterity, Constitution, Intelligence, Wisdom, Charisma,
	MeleeAttack, MissileAttack, ThrownAttack,
	MeleeDamage, MissileDamage, ThrownDamage,
	AC, Attacks, Initiative, Loyalty, Morale, Movement, Hits,
	SaveDeath, SaveWand, SaveParalysis, SaveBreath, SaveSpells,
	Climb, FindTraps, HearNoises, MoveSilently, OpenLocks, Tracking,
	Pathfinding, GearRepairing, TendingWounds, Hunting, Foraging, TreatIllness, GearCrafting, Lore,
	DoubleAttackPerDay,
};

struct statable {
	char abilities[DoubleAttackPerDay + 1];
};
