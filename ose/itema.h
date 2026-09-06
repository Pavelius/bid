#pragma once

#include "adat.h"
#include "item.h"

struct itema : adat<item> {
	void add(item& v);
	void add(const item& v);
	bool present(wearn v) const;
	bool present(itemn v) const;
};

extern itema items;