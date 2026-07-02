#pragma once

enum actionn : unsigned char;

enum rolln : unsigned char {
	BadOutcome, PartialSuccess, FullSuccess, CriticalSuccess,
};
enum positionn : unsigned char {
	ControlledPosition, RiscyPosition, DesperatePosition,
};

extern int roll_number;
extern int roll_dices;
extern rolln roll_result;
extern positionn roll_position;

rolln make_roll(int dices);

void action_roll(actionn type);
void fortune_roll();
void resistance_roll(attributen type);