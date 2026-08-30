#pragma once

#include "adat.h"
#include "item.h"

struct itema : adat<item, 256> {
	void add(item& v);
};

extern itema items;