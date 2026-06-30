#pragma once

#include "stringbuilder.h"

enum messagen : unsigned char;

unsigned char choosev(unsigned long t1, unsigned long t2, const char** names, const char* title = 0, const char* cancel = 0);

template<class T> T choose(T t2, messagen title) {
	return (T)choose_enum(0, t2, bsenum<T>::names, getname(title), 0);
}
