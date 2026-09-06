#include "itema.h"

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

void itema::add(const item& v) {
	item v1 = v;
	add(v1);
}

bool itema::present(wearn v) const {
	for(auto& e : *this) {
		if(e && e.is(v))
			return true;
	}
	return false;
}

bool itema::present(itemn v) const {
	for(auto& e : *this) {
		if(e && e.is(v))
			return true;
	}
	return false;
}