#pragma once

enum attributen : unsigned char {
	Insight, Prowess, Resolve,
};
enum actionn : unsigned char {
	Attune, Command, Consort, Finesse, Hunt, Prowl, Skirmish, Study, Survey, Sway, Tinker, Wreck,
};
enum charactern : unsigned char {
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

struct actiona {
	char		actions[Wreck + 1];
};
struct character : actiona {
	charactern	type;
	heiretagen	heiretage;
	backgroundn	background;
	vicen		vice;
	int			stress;
	int			get(actionn v) const { return actions[v]; }
	int			get(attributen i) const;
};
extern character* player;
extern character players[3];
extern actionn attributes[3][4];

void add_players();