#include "rand.h"
#include "stringbuilder.h"
#include "stringset.h"

const stringset* stringset_find(const char* id) {
	for(auto p = stringset_data; *p; p++) {
		if(equal(p->id, id))
			return p;
	}
	return 0;
}

const stringset* stringset_find(const char** names) {
	for(auto p = stringset_data; *p; p++) {
		if(p->names == names)
			return p;
	}
	return 0;
}

bool stringset_identifier(const char* identifier, stringbuilder& sb) {
	auto p = stringset_find(identifier);
	if(!p)
		return false;
	sb.addv(p->names[rand() % p->count], 0);
	return true;
}