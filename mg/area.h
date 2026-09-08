/*
	Copyright 2026 by Pavel Chistyakov

	Licensed under the Apache License, Version 2.0 (the "License");
	you may not use this file except in compliance with the License.
	You may obtain a copy of the License at

	http://www.apache.org/licenses/LICENSE-2.0

	Unless required by applicable law or agreed to in writing, software
	distributed under the License is distributed on an "AS IS" BASIS,
	WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
	See the License for the specific language governing permissions and
	limitations under the License.77
*/

#pragma once

enum arean : unsigned char {
	Barkstone, Copperwood, Elmoss, Ivydale, Lockhaven, PortSumac, Shaleburrow, Sprucetuck,
	Appleloft, Blackrock, Calogero, Darkwater, Dorigift,
	Flintrust, Frostic, Gilpledge, Ironwood, Mapleharbor,
	Pebblebrook, Rootwallow, Rustleaf, Sandmason, Shorestone,
	Windselm, Wolfepointe,
	Forest, Lakes, Streams, TallGrass, Swamps, Mud, Thorns, LeafCover, RockyTerrain, Coast, OpenGround,
	Darkheather,
	LastArea = Darkheather
};

extern const char* area_names[LastArea + 1];

extern arean location;