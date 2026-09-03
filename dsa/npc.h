#pragma once

#include "gender.h"

enum namen : unsigned char;

enum classn : unsigned char {
	Aaracokra, Anakore, Gith,
	Fighter, Gladiator, Ranger,
	Bard, Theif,
	Templar, ClericAir, ClericFire, Druid,
	Preserver, Defiler, Psionicist,
};

enum portraitn : unsigned char;

struct npci {
	namen customname;
	classn type;
	gendern	gender;
	portraitn portrait;
	void clearname() { customname = (namen)0; }
	bool isfemale() const { return gender == Female; }
	void setname();
	void setportrait();
};