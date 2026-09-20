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
#include "game.h"
#include "item.h"
#include "stringbuilder.h"
#include "math.h"

arean area;

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

const char* get_range_name(int miles) {
	auto n = miles / 24;
	return maptbl(DayDistance, n);
}

bool is_outdoor(unsigned char v) {
	switch(v) {
	case Sands: case Wastes: case Hills: case Mountains: case Forest: case Jungle: return true;
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