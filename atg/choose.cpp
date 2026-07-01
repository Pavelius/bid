#include "answers.h"

unsigned char choosev(unsigned long t1, unsigned long t2, const char** names, const char* title = 0, const char* cancel = 0) {
	an.clear();
	for(auto i = t1; i < t2; i++)
		an.add(i, names[i]);
	an.sort();
	return (unsigned char)an.choose(title, cancel);
}