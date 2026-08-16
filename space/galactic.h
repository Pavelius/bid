#pragma once

#include "flagable.h"

enum itemn : unsigned char;

enum govermentn : unsigned char {
	Anarchy, Monarchy, Republic, Democraty,
};
enum landscapen : unsigned char {
	Gas, Water, Lava, Snow, Sand, Rocky, Jungle,
};

bool is_import(itemn resource, govermentn v);
bool is_illegal(itemn resource, govermentn v);