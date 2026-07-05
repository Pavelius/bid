#include "roll.h"
#include "rand.h"

rolln roll_effect;
char roll_result;

void make_roll_raw(int bonus) {
	bonus += d6() + d6();
	roll_result = Fail;
	if(bonus >= 12)
		roll_effect = CriticalSuccess;
	else if(bonus >= 10)
		roll_effect = Success;
	else if(bonus >= 7)
		roll_effect = PartialSuccess;
}