/*******************************************************************************************
* Common.h - inline functions for various stuff, like rendering shortcuts, general
* 			 info about the current game context, and more
*******************************************************************************************/

#pragma once

#include "raylib.h"

constexpr int WINDOW_WIDTH = 768;
constexpr int WINDOW_HEIGHT = 432;

extern RenderTexture2D defaultRender;

// Shorthand for `BeginTextureMode(defaultRender)` because I am lazy
inline void BeginDefaultTextureMode()
{
	BeginTextureMode(defaultRender);
}

// Rectangle used for most render textures. height is flipped
constexpr Rectangle defaultRRect = {0.0f, 0.0f, (float)WINDOW_WIDTH, (float)-WINDOW_HEIGHT};

// ends drawing to the render texture, renders said render texture
inline void EndTextureDrawing()
{
	EndTextureMode();

	BeginDrawing();
	{
		DrawTexturePro(defaultRender.texture, defaultRRect, {0.0f, 0.0f, (float)WINDOW_WIDTH, (float)WINDOW_HEIGHT}, {}, 0.0f, RAYWHITE);
	}
	EndDrawing();
}

// wrapper around getting assets
constexpr const char* imageFolder = "img/";
constexpr const char* soundFolder = "snd/";
constexpr const char* fontFolder = "font/";
constexpr const char* mapFolder = "world/";

constexpr const char* resourceFolder = "resources/";

// Grabs an image
const char* GetImage(const char* path);
// Grabs a sound
const char* GetSound(const char* path);
// Grabs a font
const char* GetFont(const char* path);
// Grabs an ldtk map
const char* GetMap(const char* path);

// ENUMS //

// Direction. Taken from CSE2.
enum Direction 
{
	DIR_LEFT,
	DIR_UP,
	DIR_RIGHT,
	DIR_DOWN,
	DIR_AUTO,
	DIR_OTHER
};
