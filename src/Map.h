/*******************************************************************************************
*    Map.h - functions related to the currently rendered map, like tile info,
* 			 entity collision and other stuff
*******************************************************************************************/

#pragma once

#include "LDtkLoader/DataTypes.hpp"
#include "LDtkLoader/Entity.hpp"
#include "LDtkLoader/Tile.hpp"
#include "raylib.h"
#include <string>
#include <vector>

// Wrapper around some stuff because i am LAZY
struct TileInfo 
{
	const ldtk::Tile* underTile; // woooooahoooaaa oooaaa ooooohhhhhhh ooooooh oh story of undertale
	Rectangle hitbox;
};

struct IntGridInfo 
{
	const ldtk::IntGridValue* info;
	Rectangle hitbox;
};

struct MapAttrInfo
{
	TileInfo tile = {nullptr, {}};
	IntGridInfo intGrid = {nullptr, {}};

	const ldtk::Entity* ent = nullptr; // entity info
	Rectangle* hitbox; // final hitbox pointer blah blah
	Rectangle ent_hitbox;
};

// Underlying tilemap for people who want it (you)
extern ldtk::Project warudo; // This is the most dumbest reference I'm doing in this entire codebase.

// Creates the stage info by loading the ldtk world.
void InitStageInfo(std::string proj = "test.ldtk");
// Returns a level from stage info for use.
const ldtk::Level* FabricateLevel(std::string levelName);
// Draw all tiles on a specified render texture. Default layer to draw is `Level`.
void RenderAllTiles(RenderTexture2D renderTex, std::string layer = "Level");
// Get specific entity. Default layer to check for is `Entities`.
const ldtk::Entity* AcquireEntity(std::string entityIdentifier, std::string layer = "Entities");
// Get specific entity on a specific position. Default layer to check for is `Entities`.
const ldtk::Entity* AcquireEntityFromLevel(int x, int y, std::string entityIdentifier, std::string layer = "Entities");
// Get specific tile in world space. Default layer to check for is `Level`.
TileInfo AcquireTileInfo(int x, int y, std::string layer = "Level");
// Get specific tile in world space. Default layer to check for is `Level`.
IntGridInfo AcquireIntGridInfo(int x, int y, std::string layer = "Level");
// Shortcut for various 'get this shit in front of me' functions. Checks layer type
// and uses a specific function on its own.
// Checks for all layers unless specified.
MapAttrInfo AcquireAttribute(int x, int y, std::vector<std::string> layers = {});

// pretty dumb hack
auto operator==(const IntGridInfo& l, const IntGridInfo& r) -> bool;