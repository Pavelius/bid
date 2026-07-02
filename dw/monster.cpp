#include "bsdata.h"
#include "character.h"

BSDATA(monsteri) = {
	{Bandit},
	{Goblin},
	{Kobold},
	{Skeleton, Horde, Male, 7, D6, {Armor1, Close}},
};

monsteri enemy;