#include "Common.h"
#include "Map.h"
#include "LDtkLoader/Entity.hpp"
#include "LDtkLoader/Level.hpp"
#include "LDtkLoader/Project.hpp"
#include "LDtkLoader/DataTypes.hpp"
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

const ldtk::Entity* AcquireEntity(std::string entityIdentifier, std::string layer)
{
	const auto& _layer = level->getLayer(layer);
	for (const ldtk::Entity& spawn : _layer.getEntitiesByName(entityIdentifier)) 
	{
		// find first instance
		return &spawn;
	}
	return nullptr;
}

const ldtk::Entity* AcquireEntityFromLevel(int x, int y, std::string entityIdentifier, std::string layer)
{
	const auto& _layer = level->getLayer(layer);
	if (x > level->size.x || y > level->size.y) return nullptr;

	// Returns tile ID using world position
	x /= _layer.getCellSize();
	y /= _layer.getCellSize();

	const ldtk::Entity* ent = nullptr;

	for (const ldtk::Entity& spawn : _layer.getEntitiesByName(entityIdentifier)) 
	{
		// found an entity, break out
		if (spawn.getGridPosition().x == x && spawn.getGridPosition().y == y)
		{
			ent = &spawn;
			break;
		}
	}

	return ent;
}

TileInfo AcquireTileInfo(int x, int y, std::string layer)
{
	TileInfo defaultVal = {nullptr, {}};
	const auto& _layer = level->getLayer(layer);

	// out of bounds!
	if (x > level->size.x || y > level->size.y) return defaultVal;

	// Returns tile ID using world position
	x /= _layer.getCellSize();
	y /= _layer.getCellSize();

	const auto& tile = _layer.getTile(x, y);
	if (tile == ldtk::Tile::None) return defaultVal;

	const auto& position = tile.getPosition();
	return {
		&tile,
		{
			float(position.x),
			float(position.y),
			float(_layer.getCellSize()),
			float(_layer.getCellSize())
		}
	};
}

auto operator==(const ldtk::IntGridValue& l, const ldtk::IntGridValue& r) -> bool {
    return (l.value == r.value) && (l.value == r.value);
}

IntGridInfo AcquireIntGridInfo(int x, int y, std::string layer)
{
	IntGridInfo defaultVal = {nullptr, {}};
	const auto& _layer = level->getLayer(layer);

	// out of bounds!
	if (x > level->size.x || y > level->size.y) return defaultVal;

	// Returns tile ID using world position
	x /= _layer.getCellSize();
	y /= _layer.getCellSize();

	const auto& intgrid = _layer.getIntGridVal(x, y);
	if (intgrid == ldtk::IntGridValue::None) return defaultVal;

	return {
		&intgrid,
		{
			float(x * _layer.getCellSize()),
			float(y * _layer.getCellSize()),
			float(_layer.getCellSize()),
			float(_layer.getCellSize())
		}
	};
}

MapAttrInfo AcquireAttribute(int x, int y, std::vector<std::string> layers)
{
	if (layers.empty())
	{
		// literally all this does
		for (const auto& _lay : level->allLayers())
			layers.push_back(_lay.getName());
	}

	MapAttrInfo attr = {}; // init default attributes

	for (const auto& _lay : layers)
	{
		const auto& _layer = level->getLayer(_lay);
		switch (level->getLayer(_lay).getType())
		{
			case ldtk::LayerType::IntGrid:
				attr.intGrid = AcquireIntGridInfo(x, y, _lay);
				attr.hitbox = &attr.intGrid.hitbox;
				break;
			case ldtk::LayerType::Tiles:
				attr.tile = AcquireTileInfo(x, y, _lay);
				attr.hitbox = &attr.tile.hitbox;
				break;
			case ldtk::LayerType::Entities:
				{
					int temp_x = x, temp_y = y;
					if (temp_x > level->size.x || temp_y > level->size.y) break;

					// Returns tile ID using world position
					temp_x /= _layer.getCellSize();
					temp_y /= _layer.getCellSize();

					for (const ldtk::Entity& spawn : _layer.allEntities()) 
					{
						// found an entity, break out
						if (spawn.getGridPosition().x == temp_x && spawn.getGridPosition().y == temp_y)
						{
							attr.ent = &spawn;
							const auto pos = &attr.ent->getPosition(), size = &attr.ent->getSize();
							attr.ent_hitbox = {
								(float)((*pos).x),
								(float)((*pos).y),
								(float)((*size).x),
								(float)((*size).y)
							};
							attr.hitbox = &attr.ent_hitbox;
							break;
						}
					}
					break;
				}
			case ldtk::LayerType::AutoLayer:
				// TODO: Not implemented
				break;
		}
	}

	return attr;
}