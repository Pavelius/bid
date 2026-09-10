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

enum arean : unsigned char;
enum creaturen : unsigned char;
enum skilln : unsigned char;
enum traitn : unsigned char;
enum wisen : unsigned char;

enum variantn : unsigned char {
	Variant, Area, Creature, Skill, Trait, WiseVariant,
};

union variant {
	short unsigned u;
	struct {
		variantn type;
		unsigned char value;
	};
	constexpr variant() : type(Variant), value(0) {}
	constexpr variant(variantn type, unsigned char value) : type(type), value(value) {}
	constexpr variant(arean v) : type(Area), value(v) {}
	constexpr variant(creaturen v) : type(Creature), value(v) {}
	constexpr variant(skilln v) : type(Skill), value(v) {}
	constexpr variant(traitn v) : type(Trait), value(v) {}
	constexpr variant(wisen v) : type(WiseVariant), value(v) {}
	constexpr variant(short unsigned v) : u(v) {}
	constexpr explicit operator bool() const { return u!=0; }
	constexpr operator long() const { return u; }
	constexpr bool operator==(variant v) const { return u==v.u; }
};