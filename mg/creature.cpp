/*
	Copyright 2026 by Pavel Chistyakov

	Licensed under the Apache License, Version 2.0 (the "License");
	you may not use this file except in compliance with the License.
	You may obtain a copy of the License at

	http://www.apache.org/licenses/LICENSE-2.0

	Unless required by applicable law or agreed to in writing, software
	distributed under the License is distributed on an "AS IS" BASIS,
	WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
	See the License for the specific language governing permissions and
	limitations under the License.77
*/

#include "answers.h"
#include "area.h"
#include "creature.h"
#include "message.h"
#include "rand.h"
#include "wise.h"

character* player;
character* party[4];
character character_data[32];

int character::index() const {
	return this - character_data;
}

bool character::allow(traitn v) const {
	switch(v) {
	case Brave: return !is(Fearful);
	case Fearful: return !is(Brave);
	case Oldfur: return !is(Young);
	case Young: return !is(Oldfur);
	default: return true;
	}
}