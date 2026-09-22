#include "creature.h"
#include "game.h"
#include "event.h"

int eventi::index() const {
	return this - events;
}

eventi events[] = {
	{ImageCaveEntrance, PlayerCharged, 0, Strenght, -4},
};