#include "collection.h"
#include "creature.h"
#include "event.h"
#include "game.h"
#include "pushvalue.h"
#include "stringbuilder.h"

static collection event_cards;

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

void play_event_card() {
	unsigned int skip_count = 0;
	while(true) {
		auto card = event_cards.pick();
		if(!events[card].allow()) {
			event_cards.add(card);
			if(skip_count++ < event_cards.count)
				continue;
			break;
		}
		events[card].play();
		if(events[card].is(InfiniteEvent))
			event_cards.add(card);
		break;
	}
}

eventi events[] = {
	{ImageCaveEntrance, PlayerCharged, 0, {StartEvent}, Strenght, -4},
	{ImageCaveEntrance, PlayerCharged, 0, {StartEvent}, Dexterity, -2},
	{ImageCaveEntrance, PlayerCharged, 0, {StartEvent}, Wisdom, 0},
};