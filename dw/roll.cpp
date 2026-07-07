#include "roll.h"
#include "rand.h"

rolln roll_effect;
int roll_dices_result, roll_result, roll_bonus;

void make_roll_raw() {
	roll_dices_result = d6() + d6();
	roll_result = roll_dices_result + roll_bonus;
	roll_effect = Fail;
	if(roll_result >= 12)
		roll_effect = CriticalSuccess;
	else if(roll_result >= 10)
		roll_effect = Success;
	else if(roll_result >= 7)
		roll_effect = PartialSuccess;
}