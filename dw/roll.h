#pragma once

enum rolln : unsigned char {
	Fail, PartialSuccess, Success, CriticalSuccess,
};

extern rolln roll_effect;
extern char roll_result;

void make_roll_raw(int bonus);
