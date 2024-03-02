#include "Common.h"
#include "Map.h"
#include "LDtkLoader/Entity.hpp"
#include "LDtkLoader/Level.hpp"
#include "LDtkLoader/Project.hpp"
#include "raylib.h"
#include <exception>

ldtk::Project warudo; // This is the most dumbest reference I'm doing in this entire codebase.

void InitStageInfo(std::string proj)
{
	try {
		warudo.loadFromFile(GetMap(proj.c_str()));
	} catch (std::exception& what) {
		TraceLog(LOG_INFO, "LDTK: Load failed! %s", what.what());
	}
}

static const ldtk::Level* level;

const ldtk::Level* FabricateLevel(std::string levelName)
{
	const auto& world = warudo.getWorld();
	level = &world.getLevel(levelName);

	return level;
}

void RenderAllTiles(RenderTexture2D renderTex, std::string layer)
{
	const auto& _layer = level->getLayer(layer);
	Texture2D ldtkTiles = LoadTexture(GetMap(_layer.getTileset().path.c_str()));
	BeginTextureMode(renderTex);
		for (const auto& tile : _layer.allTiles())
		{
			const auto& position = tile.getPosition();
			const auto& texture_rect = tile.getTextureRect();
			Vector2 dest = {
				static_cast<float>(position.x),
				static_cast<float>(position.y),
			};
			Rectangle src = {
				static_cast<float>(texture_rect.x),
				static_cast<float>(texture_rect.y),
				static_cast<float>(texture_rect.width) * (tile.flipX ? -1.0f : 1.0f),
				static_cast<float>(texture_rect.height) * (tile.flipY ? -1.0f : 1.0f)
			};
			DrawTextureRec(ldtkTiles, src, dest, WHITE);
		}
	EndTextureMode();
}

ldtk::Entity AcquireEntity(std::string entityIdentifier, std::string layer)
{
}

TileInfo AcquireTileInfo(int x, int y, std::string layer)
{
	TileInfo defaultVal = {ldtk::Tile::None, {}};
	const auto& _layer = level->getLayer(layer);

	// Returns tile ID using world position
	x /= _layer.getCellSize();
	y /= _layer.getCellSize();

	// out of bounds!
	if (x > level->size.x || y > level->size.y) return defaultVal;

	const auto& tile = _layer.getTile(x, y);
	if (tile == ldtk::Tile::None) return defaultVal;

	const auto& position = tile.getPosition();
	return {
		tile,
		{
			float(position.x),
			float(position.y),
			float(_layer.getCellSize()),
			float(_layer.getCellSize())
		}
	};
}