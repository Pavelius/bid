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

enum itemn : unsigned char {
	NoItem,
	Axe, Bow, Halberd, HookAndLine, Knife, Shield, Sling, Spear, Staff, Sword,
	LightArmor, HeavyArmor,
	LastItem = HeavyArmor
};

enum wearn : unsigned char {
	Backpack, LastBackpach = Backpack + 3,
	Weapon, Armor,
	LastWear = Armor
};

extern const char* item_names[LastItem + 1];

struct item {
	itemn type;
	union {
		unsigned char count;
		struct {
			unsigned char lost : 1;
		};
	};
	const char* name() const { return item_names[type]; }
};

struct wearable {
	item wears[LastWear + 1];
};