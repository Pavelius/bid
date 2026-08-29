#include "area.h"
#include "bsdata.h"
#include "game.h"
#include "slice.h"
#include "stringbuilder.h"

BSDATAC(area, 1024)

area* last_area;
area* next_area;

int move_distance;

void area::clear() {
	memset((void*)this, 0, sizeof(*this));
	parent_id = 0xFFFF;
}

bool area::outdoor() const {
	switch(type) {
	case Sands:
	case Wastes:
	case Hills:
	case Mountains:
	case Forest:
		return true;
	default:
		return false;
	}
}

const char* area::name() const {
	return getname(type);
}

area* area::parent() const {
	if(parent_id == 0xFFFF)
		return 0;
	return bsdata<area>::elements + parent_id;
}

void create_area(arean id) {
	last_area = bsdata<area>::addz();
	last_area->clear();
	last_area->type = id;
	last_area->timestamp = game.get(Turns);
}

int get_movement_modifier(arean type) {
	switch(type) {
	case Mountains: case Jungle: case Swamps: return 50;
	case Sands: case Wastes: case Hills: return 67;
	default: return 100;
	}
}