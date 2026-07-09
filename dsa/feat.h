#pragma once

#include "flagable.h"

enum featn : unsigned char {
	SlaveryBackground, WeaponMastery, PoisonMaster,
	Awareness, Backstab,
	MeleeFight, Charged, Stunned, Flee, Ally, Enemy,
	Local, Slow,
};
typedef flagable<1 + Enemy / 32, unsigned> featf;