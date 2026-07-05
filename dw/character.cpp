#include "bsdata.h"
#include "character.h"
#include "math.h"

BSDATAC(character, 8)

character* player;
character* party[4];

static int ability_bonus[] = {
	-3, -3, -3, -3, -2, -2, -1, -1, -1, 0,
	0, 0, 0, 1, 1, 1, 2, 2, 3, 3, 3
};

bool allow_class(int v) {
	for(auto p : party) {
		if(!p)
			continue;
		if(p->is((classn)v))
			return false;
	}
	return true;
}

int	character::getbonus(statn v) const {
	return maptbl(ability_bonus, v);
}