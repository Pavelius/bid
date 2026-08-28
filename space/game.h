#pragma once

enum locationn : unsigned char {
	Galactic, System,
	Sun, Earth, Mars, Venera, Hupiter,
};
enum itemn : unsigned char {
	Minerals, Food, Medicals, Techincs, Luxury, Alcohole, Weapons, Drugs, Fuel,
	ShardCannon, IndustrialLaser, RocketSystem,
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
struct item {
	itemn type;
	union {
		unsigned char count;
		struct {
			unsigned char damage : 3; // 0 - no damage, 7 - unfunctional.
			unsigned char identified : 1; // Show all statistic
			unsigned char modification : 4; // Modification variations
		};
	};
	constexpr item() : type((itemn)0), count(0) {}
	constexpr item(itemn type) : type(type), count((type < ShardCannon) ? 1 : 0) {}
	constexpr item(itemn type, int count) : type(type), count(count) {}
	constexpr explicit operator bool() const { return type || count; }
	constexpr bool countable() const { return type < ShardCannon; }
	void clear() { type = (itemn)0; count = 0; }
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
struct cargoi {
	short unsigned cargo[Fuel + 1];
	void		add(item& v);
};
struct shipi : cargoi {
	locationn	location;
};

bool is_export(itemn resource, govermentn v);
bool is_import(itemn resource, govermentn v);
bool is_illegal(itemn resource, govermentn v);