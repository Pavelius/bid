#pragma once

enum itemn : unsigned char {
	Minerals, Food, Medicals, Techincs, Luxury, Alcohole, Weapons, Drugs, Fuel,
	ShardCannon, IndustrialLaser, RocketSystem,
	LastCountable = Fuel, LastCargo = Fuel, LastItem = RocketSystem,
};

struct item {
	itemn type;
	union {
		unsigned char count;
		struct {
			unsigned char modification : 4; // Modification variations
			unsigned char uses : 3; // 0 - no damage, 7 - unfunctional.
		};
	};
	constexpr item() : type((itemn)0), count(0) {}
	constexpr item(itemn type) : type(type), count((type <= LastCountable) ? 1 : 0) {}
	constexpr item(itemn type, int count) : type(type), count(count) {}
	constexpr explicit operator bool() const { return !type && !count; }
	constexpr bool countable() const { return type <= LastCountable; }
	void clear() { type = (itemn)0; count = 0; }
};

struct cargoi {
	short unsigned cargo[LastCargo + 1];
	void add(item& v);
};
