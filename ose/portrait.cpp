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

struct portraiti {
	const char*	id;
	gendern		gender;
	classn		type;
};

static portraiti portrait_data[] = {
	{"fe1", Female, Elf},
	{"fe2", Female, Elf},
	{"fe3", Female, Elf},
	{"fh1", Female, Human},
	{"fh2", Female, Human},
	{"fh3", Female, Human},
	{"fh4", Female, Human},
	{"fh5", Female, Human},
	{"fh6", Female, Human},
	{"fh7", Female, Human},
	{"fh8", Female, Human},
	{"fh9", Female, Human},
	{"fo1", Female, Human},
	{"ma1", Male, Human},
	{"md1", Male, Dwarf},
	{"md2", Male, Dwarf},
	{"me1", Male, Elf},
	{"mh1", Male, Human},
	{"mh2", Male, Human},
	{"mh3", Male, Human},
	{"mh4", Male, Human},
	{"mh5", Male, Human},
	{"mh6", Male, Human},
	{"mh7", Male, Human},
	{"mh8", Male, Human},
	{"mh9", Male, Human},
	{"mh10", Male, Human},
	{"mh11", Male, Human},
	{"mh12", Male, Human},
	{"mo1", Male, Human},
	{"mo2", Male, Human},
	{"mo3", Male, Human}
};

int portrait_count = sizeof(portrait_data) / sizeof(portrait_data[0]);

typedef adat<portraitn> portraita;

static void add_elements(portraita& result, gendern gender, classn type, fncfilter filter) {
	for(auto& e : portrait_data) {
		if(e.gender != gender)
			continue;
		if(type && e.type != type)
			continue;
		auto i = (portraitn)(&e - portrait_data);
		if(filter && !filter(i))
			continue;
		result.add(i);
	}
}

portraitn random_portrait(classn type, gendern gender, fncfilter filter) {
	portraita source;
	add_elements(source, gender, type, filter);
	if(!source)
		add_elements(source, gender, (classn)0, filter);
	if(!source)
		return (portraitn)0;
	return source.data[rand() % source.count];
}

const char* portrait_name(int i) {
	return portrait_data[i].id;
}