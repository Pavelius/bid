#include "event.h"

eventi*	last_event;

int eventi::index() const {
	return this - events;
}