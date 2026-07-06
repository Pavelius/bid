#pragma once

enum rolln : unsigned char {
	Fail, PartialSuccess, Success, CriticalSuccess,
};

extern rolln roll_effect;
extern int roll_result, roll_bonus;

void make_roll_raw(int bonus);
