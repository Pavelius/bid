#include "bsdata.h"
#include "item.h"
#include "message.h"
#include "stringbuilder.h"
#include "stringlocale.h"

BSENUM(messagen) = {
	"Мир подземелий",
};
assert_enum(bsenum<messagen>::names, AppTitle)

BSENUM(itemn) = {
	"Пусто",
	"Лук", "Отличный лук", "Охотничий лук", "Арбалет",
	"Стрелы", "Эльфийские стрелы",
	"Дубина", "Посох",
	"Нож", "Метательный кинжал",
};
assert_enum(bsenum<itemn>::names, ThrowingDagger)