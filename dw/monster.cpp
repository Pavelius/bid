#include "bsdata.h"
#include "character.h"
#include "rand.h"

BSDATA(monsteri) = {
	{Bandit},
	{Goblin},
	{Kobold},
	{Skeleton, Horde, Male, 7, D6, {Armor1, Close}},
};

monsteri enemy;

void monsteri::create(monstern v) {
	memcpy(this, bsdata<monsteri>::elements + v, sizeof(bsdata<monsteri>::elements[0]));
	count_maximum = xrand(2, 12);
	hits = hits_maximum;
	count = count_maximum;
}