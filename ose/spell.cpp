#include "bsdata.h"
#include "collection.h"
#include "slice.h"
#include "spell.h"
#include "stringbuilder.h"

BSDATAC(enchanti, 256)

extern stringbuilder sb;

static spelln cosmic_spells[] = {
	CureLightWounds, DetectEvil, DetectMagic, Light, ProtectionFromEvil, PurifyFoodAndWater, RemoveFear, ResistCold,
};

int get_level(spelln v) {
	switch(v) {
	case CureLightWounds: case 	CauseLightWounds: case DetectEvil: case DetectMagic: case Light: case Darkness:
	case ProtectionFromEvil: case PurifyFoodAndWater: case RemoveFear: case CauseFear: case ResistCold:
		return 1;
	default:
		return 0;
	}
}

int get_level(spelln v, domainn type) {
	return get_level(v);
}

spelln get_reverse(spelln v) {
	switch(v) {
	case CureLightWounds: return CauseLightWounds;
	case RemoveFear: return CauseFear;
	default: return (spelln)0;
	}
}

static const slice<spelln> get_spells(domainn type) {
	switch(type) {
	case CosmicSpells: return cosmic_spells;
	// case ArcaneSpells: return arcane_spells;
	default: return {};
	}
}

void add_spells(domainn spells, int level) {
	for(auto v : get_spells(spells)) {
		if(get_level(v) != level)
			continue;
		if(get_reverse(v))
			continue;
		records.add(v);
	}
}

int spellable::total(domainn type, int level) const {
	if(!type)
		return 0;
	auto result = 0;
	for(auto v : get_spells(type)) {
		if(level == get_level(v, type))
			result += spells[v];
	}
	return result;
}

int spellable::total(const collection& source) const {
	auto result = 0;
	for(auto v : source)
		result += spells[v];
	return result;
}

void fixlist(const spellable& e) {
	for(auto i = (spelln)0; i < LastSpell; i = (spelln)(i + 1)) {
		auto n = e.spells[i];
		if(!n)
			continue;
		if(n > 1)
			sb.addn("[+%1 x%2i]", getname(i), n);
		else
			sb.addn("[+%1]", getname(i));
	}
}