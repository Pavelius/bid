#include "math.h"
#include "game.h"

void cargoi::add(item& v) {
	if(v.type >= sizeof(cargo) / sizeof(cargo[0]))
		return;
	cargo[v.type] += v.count;
	v.count = 0;
}