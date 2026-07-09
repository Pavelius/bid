#include "bsdata.h"
#include "collection.h"
#include "spell.h"
#include "stringbuilder.h"

extern stringbuilder sb;

static spelln air_spells[] = {
	FeatherFall, Longstrider, WardingWind,
	DustDevil, GustOfWind, WindMessage,
	CallLightning, Fly,
	ConjureMinorAirElemental, WindWall,
	ConjureElemental, ControlWinds,
};
static spelln earth_spells[] = {
	EarthTremor, MagicalStone, MoldEarth,
	Earthbind, MeldIntoStone, StoneTell,
	Stoneskin, StoneShape,
	ConjureMinorEarthElemental, WallOfSand,
	ConjureElemental, Earthquake,
};
static spelln fire_spells[] = {
	EndureHeatCold, FireTrap, ProduceFlame,
	HeatMetal, LightAmplifying, Pyrotechnics,
	FlameWalk, ProtectionFromLightning,
	ConjureMinorFireElemental, WallOfFire,
	ConjureFireElemental, Firestorm,
};
static spelln water_spells[] = {
	LocateWater, ShapeWater, WaterBoiling,
	FogCloud, Mirage, WaterBreathing, SiltBreathing,
	CreateWater, WaterWalk, SiltWalk,
	ConjureMinorWaterElemental, TransmuteDustToWater, TransmuteWaterToDust,
	ConjureWaterElemental, Maelstrom,
};
static spelln nature_spells[] = {
	AnimalFriendship, DetectDanger, FaerieFire, LocatePlantOrAnimal, PredictWeather, SpeakWithAnimals,
	AnimalCommand, Obscure, PassWithoutThrace, ProduceFire, Shillelagh, WarpWood,
	HoldAnimal, ProtectionFromPoison, Snare, SpikeGrowth,
	AnimalSummoning, ControlPlant, ProtectionFromFireAndLightning, SpeakWithPlants,
	CommuneWithNature, Goodberry, WallOfThorns, WeatherSummoning,
};
static spelln cosmic_spells[] = {
	CureLightWound, DetectEvil, DetectMagic, Light, ProtectionFromEvil, PurifyFoodAndDrink, RemoveFear, ResistCold,
	Bless, FindTrapsSpell, HoldPerson, KnownAlignment, ResistFireSpell, Sanctuary, Silence15, SnakeCharm,
	ContinualLight, ContinualDarkness, CureDisease, CauseDisease, GrowthOfAnimal, LocateObject, RemoveCurse, Curse, Striking,
	CureSeriousWounds, CauseSeriousWounds, NeutralizePoison, ProtectionFromEvil10, SpeakWithDead, SpellImmunity, SticksToSnakes,
	Commune, CreateFood, DispelEvil, InsectPlague, Quest, RemoveQuest, RaiseDead, FingerOfDeath,
};
static spelln arcane_spells[] = {
	BurningHands, CharmPerson, DetectMagic, DetectPsionics, FindFamiliar, FloatingDisc, FogCloud, HoldPortal, Light, Darkness, MagicMissile, ProtectionFromEvil, ReadLanguages, ReadMagic, ShieldSpell, Sleep, Ventriloquism,
	AcidSplash, ContinualLight, ContinualDarkness, DetectEvil, DetectInvisible, Invisibility, Knock, Levitate, LocateObject, MirrorImage, PhantasmalForce, RayOfEnfeeblement, RopeTrick, Web, WizardLock,
	Clairvoyance, DispelMagic, FireBall, Fly, Haste, HoldPerson, Infravision, Invisibility10, LightningBolt, ProtectionFromEvil10, ProtectionFromNormalMissiles, VampiricTouch, WaterBreathing, SiltBreathing,
	CharmMonster, Confusion, DimensionDoor, FireTrap, HallucinatoryTerrain, Massmorph, PlantGrowth, PlantDefilement, PolymorphOthers, PolymorphSelf, RemoveCurse, Curse, WallOfFire, WallOfIce, WizardEye,
	AnimateDead, Cloudkill, ConjureElemental, ContactHigherPlane, Feeblemind, HoldMonster, MagicJar, PassWall, Telekinesis, Teleport, TransmuteRockToMud, TransmuteMudToRock, WallOfStone,
	AntiMagicShell, ControlWeather, DeathSpell, Disintegrate, Geas, RemoveGeas, InvisibleStalker, LowerWater, LowerSilt, MoveEarth, PartWater, PartSilt, ProjectedImage, Reincarnation, StoneToFlesh, FleshToStone,
};

int get_arcane_level(spelln v) {
	switch(v) {
	case DetectMagic: case Light: case ProtectionFromEvil:
	case BurningHands: case CharmPerson: case DetectPsionics: case FindFamiliar: case FloatingDisc:
	case FogCloud: case HoldPortal: case Darkness: case MagicMissile: case ReadLanguages:
	case ReadMagic: case ShieldSpell: case Sleep: case Ventriloquism:
		return 1;
	case AcidSplash: case ContinualLight: case ContinualDarkness: case DetectEvil: case DetectInvisible: case Invisibility: case Knock:
	case Levitate: case LocateObject: case MirrorImage: case PhantasmalForce: case RayOfEnfeeblement: case RopeTrick: case Web: case WizardLock:
		return 2;
	case Clairvoyance: case DispelMagic: case FireBall: case Fly: case Haste: case HoldPerson: case Infravision:
	case Invisibility10: case LightningBolt: case ProtectionFromEvil10: case ProtectionFromNormalMissiles: case VampiricTouch:
	case WaterBreathing: case SiltBreathing:
		return 3;
	case CharmMonster: case Confusion: case DimensionDoor: case FireTrap: case HallucinatoryTerrain: case Massmorph:
	case PlantGrowth: case PlantDefilement: case PolymorphOthers: case PolymorphSelf: case RemoveCurse: case Curse:
	case WallOfFire: case WallOfIce: case WizardEye:
		return 4;
	case AnimateDead: case Cloudkill: case ConjureElemental: case ContactHigherPlane: case Feeblemind: case HoldMonster:
	case MagicJar: case PassWall: case Telekinesis: case Teleport: case TransmuteRockToMud: case TransmuteMudToRock:
	case WallOfStone:
		return 5;
	case AntiMagicShell: case ControlWeather: case DeathSpell: case Disintegrate: case Geas: case RemoveGeas:
	case InvisibleStalker: case LowerWater: case LowerSilt: case MoveEarth: case PartWater: case PartSilt:
	case ProjectedImage: case Reincarnation: case StoneToFlesh: case FleshToStone:
		return 6;
	default:
		return 0;
	}
}

int get_level(spelln v) {
	switch(v) {
	case FeatherFall: case Longstrider: case WardingWind:
	case EarthTremor: case MagicalStone: case MoldEarth:
	case EndureHeatCold: case FireTrap: case ProduceFlame:
	case LocateWater: case ShapeWater: case WaterBoiling:
	case CureLightWound: case DetectEvil: case DetectMagic: case Light: case ProtectionFromEvil: case PurifyFoodAndDrink: case RemoveFear: case ResistCold:
		return 1;
	case DustDevil: case GustOfWind: case WindMessage:
	case Earthbind: case MeldIntoStone: case StoneTell:
	case HeatMetal: case LightAmplifying: case Pyrotechnics:
	case FogCloud: case Mirage: case WaterBreathing: case SiltBreathing:
	case Bless: case FindTrapsSpell: case HoldPerson: case KnownAlignment: case ResistFireSpell: case Sanctuary: case Silence15: case SnakeCharm:
		return 2;
	case CallLightning: case Fly:
	case Stoneskin: case StoneShape:
	case FlameWalk: case ProtectionFromLightning:
	case CreateWater: case WaterWalk: case SiltWalk:
	case ContinualLight: case ContinualDarkness: case CureDisease: case CauseDisease: case GrowthOfAnimal: case LocateObject: case RemoveCurse: case Curse: case Striking:
		return 3;
	case ConjureMinorAirElemental: case WindWall:
	case WallOfSand:
	case ConjureMinorFireElemental: case WallOfFire:
	case ConjureMinorWaterElemental: case TransmuteDustToWater: case TransmuteWaterToDust:
	case CureSeriousWounds: case CauseSeriousWounds: case NeutralizePoison: case ProtectionFromEvil10: case SpeakWithDead: case SpellImmunity: case SticksToSnakes:
		return 4;
	case ConjureElemental: case ControlWinds:
	case Earthquake:
	case ConjureFireElemental: case Firestorm:
	case ConjureWaterElemental: case Maelstrom:
	case Commune: case CreateFood: case DispelEvil: case InsectPlague: case Quest: case RemoveQuest: case RaiseDead: case FingerOfDeath:
		return 5;
	default:
		return 0;
	}
}

int get_level(spelln v, domainn type) {
	if(type == ArcaneSpells)
		return get_arcane_level(v);
	return get_level(v);
}

static spelln get_reverse(spelln v) {
	switch(v) {
	case CauseLightWound: return CureLightWound;
	case CauseFear: return RemoveFear;
	case ContinualDarkness: return ContinualLight;
	case CauseDisease: return CureDisease;
	case CauseSeriousWounds: return CureSeriousWounds;
	case Darkness: return Light;
	case FingerOfDeath: return RaiseDead;
	case SiltBreathing: return WaterBreathing;
	case SiltWalk: return WaterWalk;
	case TransmuteWaterToDust: return TransmuteDustToWater;
	case DetectPsionics: return DetectMagic;
	case PlantDefilement: return PlantGrowth;
	case Curse: return RemoveCurse;
	case TransmuteMudToRock: return TransmuteRockToMud;
	case RemoveGeas: return Geas;
	case LowerSilt: return LowerSilt;
	case PartSilt: return PartWater;
	case FleshToStone: return StoneToFlesh;
	default: return (spelln)0;
	}
}

static const slice<spelln> get_spells(domainn type) {
	switch(type) {
	case AirSpells: return air_spells;
	case EarthSpells: return earth_spells;
	case FireSpells: return fire_spells;
	case WaterSpells: return water_spells;
	case NatureSpells: return nature_spells;
	case CosmicSpells: return cosmic_spells;
	case ArcaneSpells: return arcane_spells;
	default: return {};
	}
}

int compare_spell(const void* v1, const void* v2) {
	auto p1 = getname(*((spelln*)v1));
	auto p2 = getname(*((spelln*)v2));
	return szcmp(p1, p2);
}

void enchanti::clear() {
	memset((void*)this, 0, sizeof(*this));
}

enchanti* find_enchant(variant object, short unsigned spell) {
	for(auto& e : bsdata<enchanti>()) {
		if(e.object == object && e.spell == spell)
			return &e;
	}
	return 0;
}

void add_enchant(variant object, unsigned stop, spelln spell) {
	auto p = find_enchant(object, spell);
	if(!p) {
		p = bsdata<enchanti>::add();
		p->stop = 0; // Need clear stop, use it below.
	}
	p->object = object;
	p->spell = spell;
	if(stop > p->stop)
		p->stop = stop;
}

void remove_enchant(variant object, short unsigned spell) {
	for(auto& e : bsdata<enchanti>()) {
		if(e.object == object && e.spell == spell)
			e.clear();
	}
}

void remove_enchant(variant object) {
	for(auto& e : bsdata<enchanti>()) {
		if(e.object == object)
			e.clear();
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
		if(level==get_level(v, type))
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