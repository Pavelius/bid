#pragma once

#include "item.h"

enum locationn : unsigned char {
	Galactic, System,
	Sun, Earth, Mars, Venera, Hupiter,
};
enum govermentn : unsigned char {
	Anarchy, Monarchy, Republic, Democraty,
};
enum enviromentn : unsigned char {
	Space,
	Gas, Water, Lava, Snow, Sand, Rocky, Jungle
};
enum contractn : unsigned char {
	DeliverCargo, SellTradeGoods, KillRefuges,
};
enum picturen : unsigned char {
	ImageWasteland, ImageWastelandNight,
};
struct locationi {
	locationn	parent;
	enviromentn	type;
};
struct contracti {
	unsigned	start;
	locationn	employer;
	contractn	type;
	locationn	location;
	itemn		item;
};
struct shipi : cargoi {
	locationn	location;
};

bool is_export(itemn resource, govermentn v);
bool is_import(itemn resource, govermentn v);
bool is_illegal(itemn resource, govermentn v);