#pragma once

enum gendern : unsigned char;
enum messagen : unsigned char;
enum namen : unsigned char;

enum attributen : unsigned char {
	Insight, Prowess, Resolve,
};
enum actionn : unsigned char {
	Attune, Command, Consort, Finesse, Hunt, Prowl, Skirmish, Study, Survey, Sway, Tinker, Wreck,
	LastAction = Wreck
};
enum charactern : unsigned char {
	None,
	Cutter, Hound, Leech, Lurk, Slide, Spider, Whisper,
};
enum heiretagen : unsigned char {
	Koros, DaggerIsles, Iruvia, Severos, Skovlan, Tycheros,
};
enum backgroundn : unsigned char {
	Academic, Labor, Law, Trade, Military, Noble, Underworld,
};
enum vicen : unsigned char {
	Faith, Gambling, Luxury, Obligation, Pleasure, Stupor, Weird,
};

extern const char* action_names[LastAction + 1];
extern const char* attribute_names[Resolve + 1];
extern const char* background_names[Underworld + 1];
extern const char* character_names[Whisper + 1];
extern const char* heiretage_names[Tycheros + 1];
extern const char* npc_names[];
extern const char* vice_names[Weird + 1];

struct npci {
	unsigned char name_id;
	gendern		gender;
	heiretagen	heiretage;
	backgroundn	background;
	const char* name() const { return npc_names[name_id]; }
	void		setname();
};
struct actiona {
	char		actions[Wreck + 1];
	void		apply(charactern type);
};
struct character : npci, actiona {
	vicen		vice;
	charactern	type;
	char		stress;
	constexpr explicit operator bool() const { return type != None; }
	character*	ally(int number) const;
	void		apply(charactern type) { this->type = type; actiona::apply(type); }
	bool		apply(actionn action, messagen command, bool run);
	bool		canstress(int v) const { return 6 - stress; }
	void		clear();
	int			get(actionn v) const { return actions[v]; }
	int			get(attributen i) const;
	int			getindex() const;
};

extern const char* action_info[LastAction + 1];

inline const char* getname(actionn v) { return action_names[v]; }

extern character* roll_help;
extern character* player;
extern character players[3];
extern actionn attributes[3][4];

void add_players();