#pragma once

#include "flagable.h"
#include "variant.h"

enum adjectiven : unsigned char {
	OfHigh, OfLow, OfSnakes, OfIron, OfDead,
};
enum areafn : unsigned char {
	Dusked, Darkened, Fired, Icy,
	Locked, Looted, Opened, Hidden,
};
enum arean : unsigned char {
	NoArea,
	Sands, Dunes, Waste, Hills, Mountains,
	Ruins,
	MerchantResidences, NobleQuarters, TemplarQuarters, Freemen, Slums, SlavePits,
	Arena, PublicSquar, Inns, BardAlleys,
	Emporium, Warehouses, CraftmenArtisans, AcademicUniversity,
	MilitaryQuarters, Gardens, Temple, Palace,
	StoneFountain,
	NomandSettlement, BuildingSettlement, SupplyOutpostForCityState,
	FarmerCommunity, TradeOutpostForMerchantHouse, MineOutpost, WoodstockOutpost, LeatherOutpost,
	Hamlet, Village, SmallTown, LargeTown,
};

struct arealink {
	short unsigned	from, to;
};
struct area {
	unsigned short	parent; // Parent area id
	arean			type; // Area main type
	unsigned char	param;
	unsigned		timestamp; // Creation time in turns (10 minutes each) from zero year.
	flag32			flags;
	constexpr explicit operator bool() const { return type!=NoArea; }
	const char* name() const;
	area* getparent() const;
	void clear();
	bool is(areafn v) const { return flags.is(v); }
	bool outdoor() const;
	void set(areafn v) { flags.set(v); }
};
extern area* last_area;
extern area* next_area;
extern int move_distance;

void create_area(arean id);