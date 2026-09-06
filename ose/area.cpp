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

BSDATAC(area, 256)

areai area_data[] = {
	{}, // Plains
	{}, // Sands
	{}, // Wastes
	{}, // Swamps
	{}, // Hills
	{}, // Mountains
	{}, // Jungle
	{}, // Forest
	{}, // Hamlet
	{LeaveSettlement, {}, ImagePlainVillage}, // Village
	{LeaveSettlement}, // SmallTown
	{LeaveSettlement}, // LargeTown
	{LeaveOutside}, // Cave
	{LeaveOutside}, // Dungeon
	{LeaveOutside}, // Ruins
	{LeaveBack, {BuyTradeGoods, SellTradeGoods}, ImageVillageMarket}, // Market
	{LeaveOutside}, // Garden
	{LeaveOutside}, // Temple
	{LeaveOutside}, // Inn
	{LeaveOutside, {GatherInformation}, ImageTavern}, // Tavern
	{}, // Palace
};
static_assert(sizeof(area_data)/ sizeof(area_data[0]) == LastArea + 1);

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

const char* area::namefull() const {
	static char temp[260]; stringbuilder sb(temp);
	switch(type) {
	case Forest:
		sb.add("%LocationMaleFirstName %1", getname(type));
		break;
	default:
		return getname(type);
	}
	return temp;
}

short unsigned area::index() const {
	return this - bsdata<area>::elements;
}

area* area::parent() const {
	if(parent_id == 0xFFFF)
		return 0;
	return bsdata<area>::elements + parent_id;
}

void create_area(arean id, short unsigned parent_id) {
	last_area = bsdata<area>::addz();
	last_area->clear();
	last_area->type = id;
	last_area->timestamp = game.get(Turns);
	last_area->parent_id = parent_id;
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