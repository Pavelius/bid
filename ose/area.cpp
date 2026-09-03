/*
	Copyright 2026 by Pavel Chistyakov

	Licensed under the Apache License, Version 2.0 (the "License");
	you may not use this file except in compliance with the License.
	You may obtain a copy of the License at

	http://www.apache.org/licenses/LICENSE-2.0

	Unless required by applicable law or agreed to in writing, software
	distributed under the License is distributed on an "AS IS" BASIS,
	WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
	See the License for the specific language governing permissions and
	limitations under the License.77
*/

#include "area.h"
#include "bsdata.h"
#include "game.h"
#include "item.h"
#include "slice.h"
#include "stringbuilder.h"
#include "variant.h"

BSDATAC(area, 256)

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

short unsigned area::index() const {
	return this - bsdata<area>::elements;
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

void update_area_items() {
	if(!need_update_items)
		return;
	clear_items();
	add_items(variant(last_area));
	need_update_items = false;
}
