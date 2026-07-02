#pragma once

#include "dice.h"
#include "item.h"

enum classn : unsigned char {
	Cleric, Fighter, Mage, Theif,
};
enum racen : unsigned char {
	Human, Elf, Dwarf, Halfling,
};
enum alignmentn : unsigned char {
	Good, Lawful, Neutral, Chaotic, Evil,
};
enum gendern : unsigned char {
	NoGender, Male, Female,
};
enum statn : unsigned char {
	Strenght, Dexterity, Constitution, Intellegence, Wisdow, Charisma,
};
enum moven : unsigned char {
	ArcaneArt, BardicLore, CharmingAndOpen, PortInTheStorm,
	Deity, DivineGuidance, TurnUndead, Commune, CastASpell,
	BornOfTheSoil, ByNatureSustained, SpiritTongue, Shapeshifter, StudiedEssence,
	BendBarsLiftGates, Armored, SignatureWeapon,
	LayOnHands, IAmTheLaw, Quest,
	HuntAndTrack, CalledShot, AnimalCompanion, Command,
	TrapExpert, TricksOfTheTrade, Backstab, FlexibleMorals, Poisoner,
	Spellbook, PrepareSpells, SpellDefense, Ritual,
	// Fighter
	Merciless, Heirloom, ImprovedWeapon,
	SeeingRed, Interrogator, ScentOfBlood, IronHide, Blacksmith, SuperiorWarrior,
	// Wizard
	Prodigy, EmpoweredMagic, FountOfKnowledge, KnownItAll, ExpandedSpellbook,
	Enchanter, Logical, ArcaneWard, Counterspell, QuickStudy,
	// Common Moves
	HackAndSlash, Volley,
	DefyDangerStreght, DefyDangerDexterity, DefyDangerConstitution, DefyDangerIntellegence, DefyDangerWisdow, DefyDangerCharisma,
	Parley, SpoutLore, DiscernRealities, Supply,
};
enum monstern : unsigned char {
	Bandit, Goblin, Kobold,
	Skeleton, Zombie,
	LastMonster = Zombie,
};
enum monsterfn : unsigned char {
	Amorphous, Cautions, Construct, Devious, Hoarder,
	Intellegent, Magical, Organized, Planar, Stealthy, Undead,
	Terrifing,
};
enum sizen : unsigned char {
	Tiny, Small, Medium, Large, Huge
};
enum organizationn : unsigned char {
	Horde, Group, Solitary
};
enum rolln : unsigned char {
	D3, D4, D6, D8, D10, D12, D20,
};

struct monsteri {
	monstern		type;
	organizationn	organization;
	gendern			gender;
	char			hits_maximum;
	rolln			damage_dice;
	itemf			flags;
	racen			race;
	char			count_maximum;
	char			hits, count;
	explicit operator bool() const { return isalive(); }
	dice			damage() const;
	const char*		getname() const { return "Name"; }
	bool			isalive() const { return hits > 0 && count > 0; }
	bool			iswounded() const { return hits < hits_maximum; }
	void			heal(int value, int* result_value = 0);
	void			sufferharm(int value, int pierce = 0, int* result_value = 0, int* killed = 0);
};
extern monsteri enemy;

struct character : monsteri, wearable {
	classn			type;
};