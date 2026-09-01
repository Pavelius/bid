#include "item.h"

typedef const char*(fnautotest)();

const char* test_item();

static int error_count;

static void pass_test(fnautotest proc) {
	auto error_message = proc();
	if(error_message)
		error_count++;
}

bool pass_test() {
	error_count = 0;
	pass_test(test_item);
	return error_count == 0;
}