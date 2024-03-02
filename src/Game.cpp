#include "Common.h"
#include "LDtkLoader/Level.hpp"
#include "Map.h"
#include "raylib.h"
#include <cmath>
#include "Game.h"

enum GameState {
	LOAD = 1,
	TITLE,
	SAVE,
	GAME,
};

static int ModeLoad() { return TITLE; }
static int ModeTitle() { return SAVE; }
static int ModeSave() { return GAME; }

static int counter = 0;

static int ModeGame()
{
	Texture2D tex = LoadTexture(GetImage("raylib.png"));
	int offsetX = 0;
	int offsetY = 0;

	InitStageInfo("test.ldtk");
	const auto& level = FabricateLevel("Level");
	RenderTexture2D tilemap = LoadRenderTexture(level->size.x, level->size.y);

	RenderAllTiles(tilemap, "Ground");

	while (!WindowShouldClose())
	{
		counter++;

		offsetX = sinf(counter / 16.0f) * 32;
		offsetY = cosf(counter / 16.0f) * 32;

		BeginDefaultTextureMode();
		{
			ClearBackground(RAYWHITE);
			DrawText("congrats bitch", 0, 0, 20.0f, BLACK);

			DrawTexturePro(tilemap.texture, {0.0f, 0.0f, (float)tilemap.texture.width, (float)-tilemap.texture.height}, {
				0.0f, 0.0f, (float)tilemap.texture.width, (float)tilemap.texture.height
			}, {0.0f, 0.0f}, 0.0f, RAYWHITE);

			DrawTexture(tex, (WINDOW_WIDTH / 2) - (tex.width / 2) + offsetX, (WINDOW_HEIGHT / 2) - (tex.height / 2) + offsetY, RAYWHITE);
		}
		EndTextureDrawing();
	}

	UnloadRenderTexture(tilemap);
	UnloadTexture(tex);

	return 0;
}

static int mode = 0;

void EnterGameLoop()
{
	mode = LOAD;
	while (mode)
	{
		// switch current state
		switch (mode)
		{
			case LOAD:
				mode = ModeLoad();
				break;
			case TITLE:
				mode = ModeTitle();
				break;
			case SAVE:
				mode = ModeSave();
				break;
			case GAME:
				mode = ModeGame();
				break;
		}
	}
}