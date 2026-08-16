#pragma once

enum itemn : unsigned char {
	NoItem,
	Minerals, Food, Medicals, Techincs, Luxury, Alcohole, Weapons, Drugs, Fuel,
};
struct item {
	itemn type;
	union {
		unsigned char count;
		struct {
			unsigned char damage : 3;
			unsigned char modification : 3;
		};
	};
};