#include "collection.h"
#include "creature.h"
#include "event.h"
#include "game.h"
#include "pushvalue.h"
#include "stringbuilder.h"

static collection event_deck;

bool eventi::play() const {
	if(!player)
		return false; // No player - event fail
	pushvalue push(last_event, const_cast<eventi*>(this));
	addhdr(picture, "Event");
	sb.clear();
	sb.add(message_names[message]);
	pause();
	if(player->roll(ability, bonus)) {
		if(success)
			success();
		return true;
	} else {
		if(fail)
			fail();
		return false;
	}
}

eventi events[] = {
	{ImageCaveEntrance, PlayerCharged, 0, Strenght, -4},
	{ImageCaveEntrance, PlayerCharged, 0, Dexterity, -2},
	{ImageCaveEntrance, PlayerCharged, 0, Wisdom, 0},
};