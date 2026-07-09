#pragma once

enum actionn : unsigned char {
	NoAction,
	MakeCharge, MakeMeleeAttack, MakeMissileAttack, MakeThrownAttack, MakeRunAway,
	RestParty, MemorizeSpells, ChangeSpellsByLevel, ChangeSpellsByLevelAllowed,
	MakeCamp,
};
