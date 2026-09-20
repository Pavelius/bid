#include "win.h"

void print_debug(const char* format) {
	OutputDebugStringA(format);
}