#pragma once

typedef bool(*fnuctest)(int v);

unsigned char choosev(unsigned long t1, unsigned long t2, const char** names, const char* title = 0, const char* cancel = 0);
unsigned char choosev(unsigned long t1, unsigned long t2, fnuctest condition, const char** names, const char* title = 0, const char* cancel = 0);
