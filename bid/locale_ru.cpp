#include "bsdata.h"
#include "character.h"
#include "message.h"
#include "stringbuilder.h"
#include "stringlocale.h"

BSENUM(messagen) = {
	"Клинки во тьме",
	"Действия", "Предметы",
	"Каким типом персонажа вы будете?",
	"Откуда вы родом?",
	"Кем вы были в прошлом?",
};
assert_enum(bsenum<messagen>::names, ChooseBackground)
BSENUM(charactern) = {
	"Головорез", "Ищейка", "Умелец", "Проныра", "Артист", "Кукловод", "Мистик"
};
assert_enum(bsenum<charactern>::names, Whisper)
BSENUM(heiretagen) = {
	"Акорос", "Ирувия", "Кинжальные острова", "Сиверос", "Сковлан", "Тихерос"
};
assert_enum(bsenum<heiretagen>::names, Tycheros)
BSENUM(vicen) = {
	"Религия", "Азартные игры", "Роскошь", "Долг", "Удовольствие", "Беспамятство", "Потустороннее",
};
assert_enum(bsenum<vicen>::names, Weird)
BSENUM(backgroundn) = {
	"Ученный", "Рабочий", "Законник", "Торговец", "Военный", "Аристократ", "Шпана",
};
assert_enum(bsenum<backgroundn>::names, Underworld)
BSENUM(actionn) = {
	"Резонанс", "Приказ", "Общение", "Сноровка", "Охота", "Проникновение", "Драка", "Изучение", "Разведка", "Манипуляция", "Ремесло", "Разрушение",
};
assert_enum(bsenum<actionn>::names, Wreck)
