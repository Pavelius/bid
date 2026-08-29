#include "refid.h"
#include "area.h"
#include "bsdata.h"
#include "creature.h"

template<> refid::refid(const area* p) : refid(Areas, p - bsdata<area>::elements) {}
template<> refid::refid(const creature* p) : refid(Creatures, p - bsdata<creature>::elements) {}
