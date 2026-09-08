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

#include "area.h"
#include "creature.h"
#include "game.h"
#include "message.h"
#include "stringbuilder.h"
#include "stringset.h"

const char* command_names[LastCommand + 1] = {
	"Отменить", "Продолжить",
	"Персонаж", "Предметы", "Сражение"
};

const char* creature_names[LastCreature + 1] = {
	"Крестьянин",
	"Новичек", "Страж", "Патрульный", "Лидер", "Капитан"
};

const char* message_names[LastMessage + 1] = {
	"Mouse guard 0.1",
	"Какое положение занимает мышь в гвардии?",
	"Откуда вы родом?",
	"Чему вас учили дома?",
};

const char* area_names[LastArea + 1] = {
	"Древоград", "Меднолесье", "Моховяз", "Плющи", "Локхэйвен", "Порт-Сумах", "Сланцевая нора", "Подьельник",
};

const char* skill_names[LastSkill + 1] = {
	"Натура", "Воля", "Здоровье", "Ресурсы", "Связи",
	"Администратор", "Пчеловод", "Архивариус", "Кузнец", "Пекарь", "Лодочник",
};

stringset stringset_data[] = {
	DSTEXT(message_names),
	{}};
