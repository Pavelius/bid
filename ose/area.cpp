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
#include "answers.h"
#include "bsdata.h"
#include "game.h"
#include "item.h"
#include "slice.h"
#include "stringbuilder.h"
#include "variant.h"

arean enviroment;

int move_distance;

const char* get_name(arean type, int p1, int p2) {
	static char temp[260]; stringbuilder sb(temp);
	switch(type) {
	case Forest:
		sb.add("%1 %2", LocationMaleFirstName[p1], area_names[type]);
		break;
	default:
		return area_names[type];
	}
	return temp;
}

int get_movement_modifier(arean type) {
	switch(type) {
	case Mountains: case Jungle: case Swamps: return 50;
	case Sands: case Wastes: case Hills: return 67;
	default: return 100;
	}
}

bool is_outdoor(unsigned char v) {
	switch(v) {
	case Sands: case Wastes: case Hills: case Mountains: case Forest: return true;
	default: return false;
	}
}

bool is_settlement(unsigned char v) {
	switch(v) {
	case Hamlet: case Village: case SmallTown: case LargeTown: return true;
	default: return false;
	}
}

void update_area_items() {
	if(!need_update_items)
		return;
	clear_items();
	// add_items(variant(last_area));
	need_update_items = false;
}