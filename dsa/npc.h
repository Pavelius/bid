#pragma once

#include "gender.h"

enum namen : unsigned char;
enum classn : unsigned char;

struct npci {
	namen	customname;
	classn	type;
	gendern	gender;
	void clearname() { customname = (namen)0; }
	bool isfemale() const { return gender == Female; }
	void setname();
};