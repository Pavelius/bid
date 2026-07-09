#include "npc.h"
#include "rand.h"
#include "stringbuilder.h"

void npci::setname() {
	auto i = xrand(1, 99) * 2;
	if(isfemale())
		i++;
	customname = (namen)i;
}