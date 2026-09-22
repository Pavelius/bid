#pragma once

enum abilityn : unsigned char;
enum messagen : unsigned char;
enum picturen : unsigned char;

typedef void(*fnevent)();
typedef bool(*fncondition)();

struct eventi {
	picturen	picture;
	messagen	message;
	fncondition	condition;
	abilityn	ability;
	char		bonus;
	fnevent		success, fail;
	int index() const;
};
extern eventi events[];