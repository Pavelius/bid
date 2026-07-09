#pragma once

#include "flagable.h"
#include "variant.h"

struct collection;

enum domainn : unsigned char {
	NoSpells,
	ArcaneSpells, CosmicSpells, NatureSpells,
	AirSpells, EarthSpells, FireSpells, WaterSpells,
};
enum spelln : unsigned char {
	CauseLightWound, CureLightWound, DetectEvil, DetectMagic, Light, Darkness,
	ProtectionFromEvil, PurifyFoodAndDrink, RemoveFear, CauseFear, ResistCold,
	Bless, FindTrapsSpell, HoldPerson, KnownAlignment, ResistFireSpell,
	Sanctuary, Silence15, SnakeCharm,
	ContinualLight, ContinualDarkness, CureDisease, CauseDisease, GrowthOfAnimal,
	LocateObject, RemoveCurse, Curse, Striking,
	CureSeriousWounds, CauseSeriousWounds, NeutralizePoison, ProtectionFromEvil10, SpeakWithDead,
	SpellImmunity, SticksToSnakes,
	Commune, CreateFood, DispelEvil, InsectPlague, Quest,
	RemoveQuest, RaiseDead, FingerOfDeath,
	FeatherFall, Longstrider, WardingWind,
	DustDevil, GustOfWind, WindMessage,
	CallLightning, Fly,
	ConjureMinorAirElemental, WindWall,
	ConjureAirElemental, ControlWinds,
	EarthTremor, MagicalStone, MoldEarth,
	Earthbind, MeldIntoStone, StoneTell,
	Stoneskin, StoneShape,
	ConjureMinorEarthElemental, WallOfSand,
	ConjureElemental, Earthquake,
	EndureHeatCold, FireTrap,
	ProduceFlame,
	HeatMetal, LightAmplifying, Pyrotechnics,
	FlameWalk, ProtectionFromLightning,
	ConjureMinorFireElemental, WallOfFire,
	ConjureFireElemental, Firestorm,
	LocateWater, ShapeWater, WaterBoiling,
	FogCloud, Mirage, WaterBreathing, SiltBreathing,
	CreateWater, WaterWalk, SiltWalk,
	ConjureMinorWaterElemental, TransmuteDustToWater, TransmuteWaterToDust,
	ConjureWaterElemental, Maelstrom,
	AnimalFriendship, DetectDanger, FaerieFire, LocatePlantOrAnimal, PredictWeather, SpeakWithAnimals,
	AnimalCommand, Obscure, PassWithoutThrace, ProduceFire, Shillelagh, WarpWood,
	HoldAnimal, ProtectionFromPoison, Snare, SpikeGrowth,
	AnimalSummoning, ControlPlant, ProtectionFromFireAndLightning, SpeakWithPlants,
	CommuneWithNature, Goodberry, WallOfThorns, WeatherSummoning,
	BurningHands, CharmPerson, DetectPsionics, FindFamiliar,
	FloatingDisc, HoldPortal, MagicMissile, ReadLanguages,
	ReadMagic, ShieldSpell, Sleep, Ventriloquism,
	AcidSplash,
	DetectInvisible, Invisibility, Knock, Levitate,
	MirrorImage, PhantasmalForce, RayOfEnfeeblement, RopeTrick,
	Web, WizardLock,
	Clairvoyance, DispelMagic, FireBall, Haste,
	Infravision, Invisibility10, LightningBolt,
	ProtectionFromNormalMissiles, VampiricTouch,
	CharmMonster, Confusion, DimensionDoor, HallucinatoryTerrain,
	Massmorph, PlantGrowth, PlantDefilement, PolymorphOthers, PolymorphSelf,
	WallOfIce, WizardEye,
	AnimateDead, Cloudkill, ContactHigherPlane,
	Feeblemind, HoldMonster, MagicJar, PassWall, Telekinesis,
	Teleport, TransmuteRockToMud, TransmuteMudToRock, WallOfStone,
	AntiMagicShell, ControlWeather, DeathSpell, Disintegrate, Geas,
	RemoveGeas, InvisibleStalker, LowerWater, LowerSilt, MoveEarth,
	PartWater, PartSilt, ProjectedImage, Reincarnation, StoneToFlesh,
	FleshToStone,
	LastSpell = FleshToStone
};
struct spellable {
	char spells[FleshToStone + 1];
	int total(domainn type, int level) const;
	int total(const collection& source) const;
};
typedef flagable<1 + FleshToStone / 32, unsigned> spellf;

struct enchanti {
	variant		object;
	spelln		spell;
	unsigned	stop;
	void clear();
};
enchanti* find_enchant(unsigned object, short unsigned spell);

int compare_spell(const void* v1, const void* v2);

int get_arcane_level(spelln v);
int get_level(spelln v);
int get_level(spelln v, domainn type);

void add_spells(domainn spells, int level);
void add_enchant(variant object, unsigned stop, spelln spell);
void fixlist(const spellable& e);
void remove_enchant(variant object);
void remove_enchant(variant object, spelln spell);