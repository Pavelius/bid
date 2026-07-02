#pragma once

enum spelln : unsigned char {
	SpellGuidance, SpellLight, SpellPrestidigitation, SpellSanctify, SpellUnseenServant,
	SpellBless, SpellCauseFear, SpellContactSpirits, SpellCureLightWounds, SpellDetectAlignment, SpellDetectMagic, SpellTelepathy, SpellCharmPerson, SpellInvisibility, SpellMagicMissile, SpellMagicWeapon, SpellSanctuary, SpellSpeakWithDead, SpellAlarm,
	SpellDispelMagic, SpellVisionsThroughTime, SpellFireball, SpellMimic, SpellMirrorImage, SpellSleep,
	SpellCage, SpellContactOtherPlane, SpellPolymorph, SpellSummonMonster,
	SpellDominate, SpellTrueSeeing, SpellShadowWalk, SpellContingency, SpellCloudkill,
	SpellAntipathy, SpellAlert, SpellSoulGem, SpellShelter, SpellPerfectSummons,
	FirstSpell = SpellLight, LastSpell = SpellPerfectSummons,
};
enum schooln : unsigned char {
	NoSchool,
	Divination, Enchantment, Evocation, Illusion, Summoning,
};
enum durationn : unsigned char {
	Instantaneous,
	Duration1Minute, Duration10Minute, Duration30Minute,
	Duration1Hour, Duration8Hour,
	Duration1Day,
	Permanent
};
enum targetn : unsigned char {
	Self, Ally, Enemy,
};
enum effectn : unsigned char {
	Choose1, Choose2, Choose3, Choose4,
	Damage, DamageAll, DamageIA, DamageIAAll,
	Heal, HealParty,
	Regroup, Summon, Debility,
	BonusForward,
	LooseItem, LooseMoney,
	UseAmmo, UseCharge,
};
