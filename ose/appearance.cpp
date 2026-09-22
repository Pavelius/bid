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

#include "adat.h"
#include "creature.h"
#include "gender.h"
#include "rand.h"
#include "slice.h"

portraiti portraits[32 * 2] = {
	{Male, Fighter},
	{Female, Fighter},
	{Male, Fighter},
	{Female, Fighter},
	{Male, Fighter},
	{Female, Fighter},
	{Male, Theif},
	{Female, Theif},
	{Male, Theif},
	{Female, Theif},
	{Male, Theif},
	{Female, Theif},
	{Male, Theif},
	{Female, Theif},
	{Male, Theif},
	{Female, Theif},
	{Male, MagicUser},
	{Female, MagicUser},
	{Male, MagicUser},
	{Female, MagicUser},
	{Male, MagicUser},
	{Female, MagicUser},
	{Male, Cleric},
	{Female, Cleric},
	{Male, Cleric},
	{Female, Cleric},
	{Male, Cleric},
	{Female, Cleric},
	{Male, MagicUser},
	{Female, MagicUser},
	{Male, Cleric},
	{Female, Cleric},
	{Male, Elf},
	{Female, Elf},
	{Male, Elf},
	{Female, Elf},
	{Male, Elf},
	{Female, Elf},
	{Male, Elf},
	{Female, Elf},
	{Male, Elf},
	{Female, Elf},
	{Male, Elf},
	{Female, Elf},
	{Male, Elf},
	{Female, Elf},
	{Male, Elf},
	{Female, Elf},
	{Male, Dwarf},
	{Male, Dwarf},
	{Male, Dwarf},
	{Male, Dwarf},
	{Male, Dwarf},
	{Male, Dwarf},
	{Male, Dwarf},
	{Male, Dwarf},
	{Male, Dwarf},
	{Male, Dwarf},
	{Male, Dwarf},
	{Male, Dwarf},
	{Male, Dwarf},
	{Male, Dwarf},
	{Male, Dwarf},
	{Male, Dwarf},
};

typedef adat<portraitn, 256> portraitc;
typedef adat<unsigned char, 256> namec;

static unsigned char human_names[2] = {0, 50};
static unsigned char elf_names[2] = {50, 40};
static unsigned char dwarf_names[2] = {90, 30};

static unsigned char* get_names(classn type) {
	switch(type) {
	case Elf: return elf_names;
	case Dwarf: return dwarf_names;
	default: return human_names;
	}
}

static void add_elements(portraitc& result, gendern gender, classn type, fncfilter filter) {
	for(auto& e : portraits) {
		if(e.gender != gender)
			continue;
		if(type && e.type != type)
			continue;
		auto i = (portraitn)(&e - portraits);
		if(filter && !filter(i))
			continue;
		result.add(i);
	}
}

static gendern get_name_gender(unsigned char v) {
	switch(v % 2) {
	case 1: return Female;
	default: return Male;
	}
}

static void add_elements(namec& result, gendern gender, classn type, fncfilter filter) {
	auto names = get_names(type);
	auto m = names[0] + names[1];
	for(auto i = names[0]; i < m; i++) {
		if(get_name_gender(i) != gender)
			continue;
		if(filter && !filter(i))
			continue;
		result.add(i);
	}
}

portraitn random_portrait(classn type, gendern gender, fncfilter filter) {
	portraitc source;
	add_elements(source, gender, type, filter);
	if(!source)
		add_elements(source, gender, (classn)0, filter);
	if(!source)
		return (portraitn)0;
	return source.data[rand() % source.count];
}

unsigned char random_name(classn type, gendern gender, fncfilter filter) {
	namec source;
	add_elements(source, gender, type, filter);
	if(!source)
		add_elements(source, gender, (classn)0, filter);
	if(!source)
		return 0;
	return source.data[rand() % source.count];
}