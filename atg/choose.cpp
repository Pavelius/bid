#include "answers.h"
#include "choose.h"

unsigned char choosev(unsigned long t1, unsigned long t2, const char** names, const char* title, const char* cancel) {
	an.clear();
	for(auto i = t1; i < t2; i++)
		an.add(i, names[i]);
	an.sort();
	return (unsigned char)an.choose(title, cancel);
}

unsigned char choosev(unsigned long t1, unsigned long t2, fnuctest condition, const char** names, const char* title, const char* cancel) {
	an.clear();
	for(auto i = t1; i < t2; i++) {
		if(condition(i))
			an.add(i, names[i]);
	}
	an.sort();
	return (unsigned char)an.choose(title, cancel);
}