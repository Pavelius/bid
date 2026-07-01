#include "draw_atg.h"
#include "bsdata.h"
#include "character.h"
#include "message.h"
#include "rand.h"
#include "scene.h"

static int compare_char(const void* v1, const void* v2) {
	return *((char*)v2) - *((char*)v1);
}

rolln roll_result;
positionn roll_position;
int roll_number;

rolln make_roll(int dices) {
	char result[10] = {};
	if(dices > lenghtof(result))
		dices = lenghtof(result);
	else if(dices < 0)
		dices = 0;
	for(int i = 0; i < dices; i++)
		result[i] = 1 + rand() % 6;
	qsort(result, dices, sizeof(result[0]), compare_char);
	roll_number = result[0];
	roll_result = BadOutcome;
	if(result[0] == 6 && result[1] == 6)
		roll_result = CriticalSuccess;
	else if(result[0] == 6)
		roll_result = FullSuccess;
	else if(result[0] >= 4)
		roll_result = PartialSuccess;
	return roll_result;
}

void action_roll(actionn type) {
	auto number = player->get(type);
	make_roll(number);
}

void fortune_roll() {
	make_roll(1);
}

void resistance_roll(attributen type) {
	make_roll(player->get(type));
	roll_number = 6 - roll_number;
	if(roll_number > 0) {
		fixmsg(GainStress);
		player->stress += roll_number;
	}
}