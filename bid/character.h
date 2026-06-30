#pragma once

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

struct character {
	charactern	type;
	heiretagen	heiretage;
	backgroundn	background;
	vicen		vice;
};
extern character* player;
extern character players[3];

void add_players();