#include "Common.h"
#include "LDtkLoader/Level.hpp"
#include "Map.h"
#include "raylib.h"
#include "fmt/core.h"
#include "Game.h"

#include "entity/MyPlay.h"

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

static void UpdateCameraCenterInsideMap(Camera2D &camera, const ldtk::Level& curLevel, int width, int height)
{
	camera.target = gPlayer.position;
	camera.offset = { width/2.0f, height/2.0f + 16.0f};
	float minX = 1000, minY = 1000, maxX = -1000, maxY = -1000;

	minX = 0;
	minY = 0;
	maxX = curLevel.size.x;
	maxY = curLevel.size.y;

	Vector2 max = GetWorldToScreen2D({ maxX, maxY }, camera);
	Vector2 min = GetWorldToScreen2D({ minX, minY }, camera);

	if (max.x < width) camera.offset.x = width - (max.x - width/2);
	if (max.y < height) camera.offset.y = height - (max.y - height/2) + 16.0f;
	if (min.x > 0) camera.offset.x = width/2 - min.x;
	if (min.y > 0) camera.offset.y = height/2 + 16.0f - min.y;
}


static int ModeGame()
{
	Texture2D tex = LoadTexture(GetImage("raylib.png"));
	int offsetX = 0;
	int offsetY = 0;

	InitStageInfo("test.ldtk");
	const auto& level = FabricateLevel("Level");
	RenderTexture2D tilemap = LoadRenderTexture(level->size.x, level->size.y);

	RenderAllTiles(tilemap, "Ground");

	// Init player
	InitMyPlay();

	// Set player position
	const auto& spawnP = AcquireEntity("Player_Spawn");
	if (spawnP != nullptr)
	{
		const auto& spawnPos = spawnP->getPosition();

		gPlayer.x = spawnPos.x;
		gPlayer.y = spawnPos.y + gPlayer.rect.height / 2;
	}

	// Init camera
	Camera2D cam = {0};
	cam.target = gPlayer.position;
	cam.offset = { WINDOW_WIDTH/2.0f, WINDOW_HEIGHT/2.0f };
	cam.rotation = 0.0f;
	cam.zoom = 1.0f;

	while (!WindowShouldClose())
	{
		counter++;

		// offsetX = sinf(counter / 16.0f) * 32;
		// offsetY = cosf(counter / 16.0f) * 32;

		UpdateMyPlay();
		{
			const Vector2 origin = {gPlayer.x, gPlayer.y - gPlayer.rect.height / 2};
			const Vector2 halfsize = {gPlayer.rect.width / 2, gPlayer.rect.height / 2};

			// TODO: MAKE THIS INTO AN ARRAY. THIS IS FUCKING SHIT
			const auto& tile_top = AcquireTileInfo(origin.x, origin.y - halfsize.y - 8, "Ground").hitbox;
			const auto& tile_bottom = AcquireTileInfo(origin.x, gPlayer.y + 8, "Ground").hitbox;
			const auto& tile_left = AcquireTileInfo(origin.x - halfsize.x / 2 - 8, origin.y, "Ground").hitbox;
			const auto& tile_right = AcquireTileInfo(origin.x + halfsize.x / 2 + 8, origin.y, "Ground").hitbox;

			const auto& tile_top_left = AcquireTileInfo(origin.x - 16, origin.y - halfsize.y - 8, "Ground").hitbox;
			const auto& tile_top_right = AcquireTileInfo(origin.x + 16, origin.y - halfsize.y - 8, "Ground").hitbox;
			const auto& tile_bottom_left = AcquireTileInfo(origin.x - 16, origin.y + 8, "Ground").hitbox;
			const auto& tile_bottom_right = AcquireTileInfo(origin.x + 16, origin.y + 8, "Ground").hitbox;

			UpdateColBetweenPlayerRect(tile_top);
			UpdateColBetweenPlayerRect(tile_bottom);
			UpdateColBetweenPlayerRect(tile_left);
			UpdateColBetweenPlayerRect(tile_right);

			UpdateColBetweenPlayerRect(tile_top_left);
			UpdateColBetweenPlayerRect(tile_top_right);
			UpdateColBetweenPlayerRect(tile_bottom_left);
			UpdateColBetweenPlayerRect(tile_bottom_right);
		}

		UpdateCameraCenterInsideMap(cam, *level, WINDOW_WIDTH, WINDOW_HEIGHT);
		cam.zoom = 3.0f;

		BeginDefaultTextureMode();
		{
			ClearBackground(RAYWHITE);
			DrawText(fmt::format("{} FPS", GetFPS()).c_str(), 0, 0, 20.0f, BLACK);

			BeginMode2D(cam);
				DrawTexturePro(tilemap.texture, {0.0f, 0.0f, (float)tilemap.texture.width, (float)-tilemap.texture.height}, {
					0.0f, 0.0f, (float)tilemap.texture.width, (float)tilemap.texture.height
				}, {0.0f, 0.0f}, 0.0f, RAYWHITE);

				DrawMyPlay();
			EndMode2D();

			// DrawTexture(tex, (WINDOW_WIDTH / 2) - (tex.width / 2) + offsetX, (WINDOW_HEIGHT / 2) - (tex.height / 2) + offsetY, RAYWHITE);
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