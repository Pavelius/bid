#pragma once

enum skilln;
enum traitn;

enum arean : unsigned char {
	Barkstone, Copperwood, Elmoss, Ivydale, Lockhaven, PortSumac, Shaleburrow, Sprucetuck,
	Appleloft, Blackrock, Calogero, Darkwater, Dorigift,
	Flintrust, Frostic, Gilpledge, Ironwood, Mapleharbor,
	Pebblebrook, Rootwallow, Rustleaf, Sandmason, Shorestone,
	Windselm, Wolfepointe,
	Forest, Lakes, Streams, TallGrass, Swamps, Mud, Thorns, LeafCover, RockyTerrain, Coast, OpenGround,
	LastArea = OpenGround
};

extern const char* area_names[LastArea + 1];