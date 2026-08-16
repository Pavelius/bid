#include "message.h"
#include "stringbuilder.h"
#include "stringlocale.h"

BSENUM(messagen) = {
	"Земля 2141",
};
assert_enum(bsenum<messagen>::names, AppTitle)