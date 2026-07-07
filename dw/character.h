#pragma once

#include "dice.h"
#include "item.h"

enum messagen : unsigned char;

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
	EnemyCatchMelee,
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
enum dicen : unsigned char {
	D3, D4, D6, D8, D10, D12, D20,
};

typedef flagable<(EnemyCatchMelee + 31) / 32, unsigned> movef;

struct monsteri {
	monstern		type;
	organizationn	organization;
	gendern			gender;
	char			hits_maximum;
	dicen			damage_dice;
	itemf			flags;
	flag32			flags_monsters;
	racen			race;
	char			count_maximum;
	char			hits, count;
	explicit operator bool() const { return isalive(); }
	void			create(monstern type);
	dice			damage() const;
	const char*		getname() const { return "Name"; }
	bool			is(itemfn v) const { return flags.is(v); }
	bool			is(monsterfn v) const { return flags_monsters.is(v); }
	bool			is(organizationn v) const { return organization == v; }
	bool			is(racen v) const { return race == v; }
	bool			isalive() const { return hits > 0 && count > 0; }
	bool			iswounded() const { return hits < hits_maximum; }
	void			heal(int value, int* result_value = 0);
	void			sufferharm(int value, int pierce = 0, int* result_value = 0, int* killed = 0);
};
extern monsteri enemy;

struct character : monsteri, wearable {
	classn			type;
	movef			moves;
	char			abilities[Charisma + 1];
	void			act(messagen m) const;
	int				get(statn v) const { return abilities[v]; }
	int				getbonus(statn v) const;
	bool			haveitem(itemfn v) const { return wearable::is(v); }
	bool			is(classn v) const { return type == v; }
	bool			is(itemn v) const { return wearable::is(v); }
	bool			is(itemfn v) const { return wears[Hands].is(v); }
	bool			is(moven v) const { return moves.is(v); }
	bool			is(racen v) const { return race == v; }
	void			markexperience() {}
	void			remove(moven v) { moves.remove(v); }
	void			set(moven v) { moves.set(v); }
	bool			use(itemfn v);
};
extern character* player;
extern character* party[4];

bool allow_class(int v);
void generate_character();