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