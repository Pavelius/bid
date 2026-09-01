#include "itema.h"

// itema items;

void itema::add(item& v) {
	if(v.countable()) {
		for(auto& e : *this) {
			e.join(v);
			if(!v)
				break;
		}
	}
	if(!v)
		return;
	last_item = adat::add();
	*last_item = v;
	v.clear();
}