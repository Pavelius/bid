#include "creature.h"
#include "gender.h"
#include "portrait.h"
#include "rand.h"
#include "slice.h"

static portraitn human_male[] = {MaleHuman1, MaleHuman2, MaleHuman3, MaleHuman4, MaleHuman5, MaleHuman6, MaleHuman7, MaleHuman8, MaleHuman9, MaleHuman10, MaleHuman11, MaleHuman12};

slice<portraitn> get_source(classn type, gendern gender) {
	return human_male;
}

portraitn random_portrait(classn type, gendern gender) {
	auto source = get_source(type, gender);
	if(!source)
		return (portraitn)0;
	return source.data[rand() % source.count];
}
