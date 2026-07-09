#include "area.h"
#include "bsdata.h"
#include "game.h"
#include "slice.h"
#include "stringbuilder.h"

area* last_area;
area* next_area;
int move_distance;

void area::clear() {
	memset((void*)this, 0, sizeof(*this));
	parent = 0xFFFF;
}

bool area::outdoor() const {
	switch(type) {
	case Sands:
	case Dunes:
	case Waste:
	case Hills:
	case Mountains:
		return true;
	default:
		return false;
	}
}

const char* area::name() const {
	return getname(type);
}

area* area::getparent() const {
	if(parent == 0xFFFF)
		return 0;
	return bsdata<area>::elements + parent;
}

void create_area(arean id) {
	last_area = bsdata<area>::addz();
	last_area->clear();
	last_area->type = id;
	last_area->timestamp = game.get(Turns);
}