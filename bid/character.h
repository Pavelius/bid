#pragma once

enum messagen : unsigned char;

enum attributen : unsigned char {
	Insight, Prowess, Resolve,
};
enum actionn : unsigned char {
	Attune, Command, Consort, Finesse, Hunt, Prowl, Skirmish, Study, Survey, Sway, Tinker, Wreck,
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

struct npci {
	heiretagen	heiretage;
	backgroundn	background;
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
	void		apply(charactern type) { actiona::apply(type); }
	bool		apply(actionn action, messagen command, bool run);
	bool		canstress(int v) const { return 6 - stress; }
	void		clear();
	int			get(actionn v) const { return actions[v]; }
	int			get(attributen i) const;
	int			getindex() const;
	const char*	getname() const { return "Ralf"; }
};
extern character* roll_help;
extern character* player;
extern character players[3];
extern actionn attributes[3][4];

void add_players();