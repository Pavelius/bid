#include "creature.h"
#include "event.h"
#include "pushvalue.h"

eventi* last_event;

int eventi::index() const {
	return this - events;
}