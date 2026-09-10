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
#include "draw_atg.h"
#include "game.h"
#include "message.h"

void create_game() {
	answer_picture = ImageSwearn;
	location = Copperwood;
	create_character_silent();
	create_character_silent();
	create_character_silent();
	sb.add("Show characters");
	make_roll(Fighter, 3);
}