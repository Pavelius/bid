#include "event.h"

int eventi::index() const {
	return this - events;
}