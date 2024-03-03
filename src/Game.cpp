#include "Common.h"
#include "LDtkLoader/Level.hpp"
#include "Map.h"
#include "raylib.h"
#include "fmt/core.h"
#include "Game.h"

#include "entity/MyPlay.h"
#include "helper/pxtoneStream.h"
#include "raymath.h"

enum GameState {
	LOAD = 1,
	TITLE,
	SAVE,
	GAME,
};

static Font font;
static const char* fnny = 
"This is still a work in progess.\n\
\n\
Numerous things are still in development, and\n\
therefore, a lot of stuff might change.\n\
\n\
For now, please enjoy what the game has so far.\n\
I hope you like it :>\n\
\n\
\n\
- haya\
\n\
\n\
Press Z to start\
";

static void DrawCustomFPS()
{
	std::string fpsText = fmt::format("fps: {}", GetFPS());
	DrawTextPro(font, fpsText.c_str(), {WINDOW_WIDTH - (TextLength(fpsText.c_str())/2.0f)*20.0f - 14.0f, WINDOW_HEIGHT - 28.0f}, {}, 0.0f, 20.0f, 0.5f, WHITE);
}

static int ModeLoad() 
{ 
	font = LoadFont(GetFont("default.fnt"));
	float fadeOpacity = 0.0f;
	int counter = 0;

	Texture2D raylib_logo = LoadTexture(GetImage("raylib.png"));

	AudioStream pxtnStream;
	auto serv = LoadPxtoneStream("resources/mus/dataslot.ptcop", pxtnStream);

	PlayAudioStream(pxtnStream);

	while (!WindowShouldClose())
	{
		if (IsKeyPressed(KEY_Z))
			counter++;

		if (counter > 0)
		{
			if (fadeOpacity < 1)
				fadeOpacity += 0.075f;
			if (fadeOpacity > 1) fadeOpacity = 1.0f;
			counter++;
			serv->moo_set_master_volume(1.0f - fadeOpacity);
		}

		if (counter == 60)
			goto goto_shit;

		BeginDefaultTextureMode();
		{
			ClearBackground({0, 0, 22, 255});

			DrawTexture(raylib_logo, WINDOW_WIDTH / 2 - (raylib_logo.width/2), WINDOW_HEIGHT / 2 - (raylib_logo.height/2), {255, 255, 255, 32});

			DrawTextPro(font, fnny, {24.0f, 80.0f}, {}, 0.0f, 20.0f, 0.5f, WHITE);

			DrawRectangle(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, {0, 0, 22, static_cast<unsigned char>(255 * fadeOpacity)});

			DrawCustomFPS();
		}
		EndTextureDrawing();
	}

goto_shit:
	StopAudioStream(pxtnStream);
	UnloadTexture(raylib_logo);

	return GAME; 
}
static int ModeTitle() { return SAVE; }
static int ModeSave() { return GAME; }

static int counter = 0;

static void UpdateCameraCenterInsideMap(Camera2D &camera, const ldtk::Level& curLevel, int width, int height)
{
	camera.offset = { width/2.0f, height/2.0f + 16.0f};
	float minX = 1000, minY = 1000, maxX = -1000, maxY = -1000;

	minX = 0;
	minY = 0;
	maxX = curLevel.size.x;
	maxY = curLevel.size.y;

	Vector2 max = GetWorldToScreen2D({ maxX, maxY }, camera);
	Vector2 min = GetWorldToScreen2D({ minX, minY }, camera);

	static float minSpeed = 30;
	static float minEffectLength = 10;
	static float fractionSpeed = 3.5f;

	Vector2 diff = Vector2Subtract(gPlayer.position, camera.target);
	if (gPlayer.dir == DIR_LEFT && !((max.x < width) || (min.x > 0)))
		diff.x -= 32.0f;
	if (gPlayer.dir == DIR_RIGHT && !((max.x < width) || (min.x > 0)))
		diff.x += 32.0f;
	float length = Vector2Length(diff);

	if (length > minEffectLength)
	{
		float speed = fmaxf(fractionSpeed*length, minSpeed);
		Vector2 final = Vector2Add(camera.target, Vector2Scale(diff, speed*GetFrameTime()/length));
		camera.target.x = final.x;
		camera.target.y = final.y;
	}

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

	// int pxtone_size;
	// unsigned char* pxtone = LoadFileData("cubes.ptcop", &pxtone_size);
	// loadPxtoneFile(pxtone, pxtone_size);

	// AudioStream pxtnStream = LoadAudioStream(44100, 16, 2);
	// SetAudioStreamCallback(pxtnStream, pxtoneCallback);

	// PlayAudioStream(pxtnStream);

	AudioStream pxtnStream;
	auto serv = LoadPxtoneStream("resources/mus/cubes.ptcop", pxtnStream);

	PlayAudioStream(pxtnStream);
	float fadeOpacity = 1.0f;

	while (!WindowShouldClose())
	{
		counter++;
		if (fadeOpacity > 0)
			fadeOpacity -= 0.075f;
		if (fadeOpacity < 0) fadeOpacity = 0.0f;

		// offsetX = sinf(counter / 16.0f) * 32;
		// offsetY = cosf(counter / 16.0f) * 32;

		UpdateMyPlay();
		{
			const Vector2 origin = {gPlayer.x, gPlayer.y - gPlayer.rect.height / 2};
			const Vector2 halfsize = {gPlayer.rect.width / 2, gPlayer.rect.height / 2};

			// TODO: MAKE THIS INTO AN ARRAY. THIS IS FUCKING SHIT
			const auto& tile_top = AcquireIntGridInfo(origin.x, origin.y - halfsize.y - 8, "Collisions").hitbox;
			const auto& tile_bottom = AcquireIntGridInfo(origin.x, gPlayer.y + 8, "Collisions").hitbox;
			const auto& tile_left = AcquireIntGridInfo(origin.x - halfsize.x / 2 - 8, origin.y, "Collisions").hitbox;
			const auto& tile_right = AcquireIntGridInfo(origin.x + halfsize.x / 2 + 8, origin.y, "Collisions").hitbox;

			const auto& tile_top_left = AcquireIntGridInfo(origin.x - 16, origin.y - halfsize.y - 8, "Collisions").hitbox;
			const auto& tile_top_right = AcquireIntGridInfo(origin.x + 16, origin.y - halfsize.y - 8, "Collisions").hitbox;
			const auto& tile_bottom_left = AcquireIntGridInfo(origin.x - 16, origin.y + 8, "Collisions").hitbox;
			const auto& tile_bottom_right = AcquireIntGridInfo(origin.x + 16, origin.y + 8, "Collisions").hitbox;

			UpdateColBetweenPlayerRect(tile_top);
			UpdateColBetweenPlayerRect(tile_bottom);
			UpdateColBetweenPlayerRect(tile_left);
			UpdateColBetweenPlayerRect(tile_right);

			UpdateColBetweenPlayerRect(tile_top_left);
			UpdateColBetweenPlayerRect(tile_top_right);
			UpdateColBetweenPlayerRect(tile_bottom_left);
			UpdateColBetweenPlayerRect(tile_bottom_right);
		}

		if (gPlayer.y - gPlayer.rect.height > level->size.y)
		{
			// Set player position
			const auto& spawnP = AcquireEntity("Player_Spawn");
			if (spawnP != nullptr)
			{
				const auto& spawnPos = spawnP->getPosition();

				gPlayer.x = spawnPos.x;
				gPlayer.y = spawnPos.y + gPlayer.rect.height / 2;
			}
		}

		UpdateCameraCenterInsideMap(cam, *level, WINDOW_WIDTH, WINDOW_HEIGHT);
		cam.zoom = 3.0f;

		BeginDefaultTextureMode();
		{
			ClearBackground({0, 0, 22, 255});

			BeginMode2D(cam);
				DrawTexturePro(tilemap.texture, {0.0f, 0.0f, (float)tilemap.texture.width, (float)-tilemap.texture.height}, {
					0.0f, 0.0f, (float)tilemap.texture.width, (float)tilemap.texture.height
				}, {0.0f, 0.0f}, 0.0f, RAYWHITE);

				DrawMyPlay();

				DrawTexture(tex, 4*16 + offsetX, 400 + offsetY, RAYWHITE);
			EndMode2D();

			DrawRectangle(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, {0, 0, 22, static_cast<unsigned char>(255 * fadeOpacity)});
		
			DrawCustomFPS();
		}
		EndTextureDrawing();
	}

	ExplodePxtoneStream(pxtnStream, serv);
	DestroyMyPlay();

	UnloadFont(font);
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