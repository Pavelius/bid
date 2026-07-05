#include "bsdata.h"
#include "item.h"
#include "stringbuilder.h"
#include "stringlocale.h"

BSDATA(item) = {
	{},
	{RaggedBow, {Near, Weight2}, 15},
	{FineBow, {Near, Far, Weight2}, 60},
	{HuntersBow, {Near, Far, Weight1}, 100},
	{Crossbow, {Near, DamageP1, Reloaded, Weight1, Weight2}, 35},
	{Arrows, {Ammo1, Ammo2, Weight1}, 1},
	{ElvenArrows, {Ammo4, Weight1}, 20},
	{Club, {Close, Weight2}, 1},
};
assert_enum(bsdata<item>::elements, Club)

const char* item::name() const {
	return getname(type);
}

bool item::range(itemfn start) const {
	for(auto v = start; v <= Far; v = (itemfn)(v + 1)) {
		if(is(v))
			return true;
	}
	return false;
}
