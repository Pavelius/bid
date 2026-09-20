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

#pragma once

enum actionn : unsigned char;
enum picturen : unsigned char;

enum areafn : unsigned char {
	Visited, Known,
	Dusked, Darkened, Fired, Icy,
	Locked, Looted, Opened, Hidden,
	Far, Near,
};
enum arean : unsigned char {
	Plains, Sands, Wastes, Swamps, Hills, Mountains, Jungle, Forest,
	Hamlet, Village, SmallTown, LargeTown,
	Cave, Dungeon, Ruins,
	Market, Garden, Temple, Inn, Tavern, Palace,
	LastArea = Palace,
};

extern const char* area_look[LastArea + 1];
extern const char* area_names[LastArea + 1];
extern const char* area_visit[LastArea + 1];

extern const char* LocationFemaleFirstName[];
extern const char* LocationMaleFirstName[];

extern arean enviroment;

extern int move_distance;

int get_movement_modifier(arean type);

const char* get_name(arean type, int p1, int p2);

bool is_outdoor(unsigned char v);
bool is_settlement(unsigned char v);
void update_area_items();
