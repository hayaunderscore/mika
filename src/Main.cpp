/*******************************************************************************************
*
*   raylib [core] example - Basic window
*
*   Welcome to raylib!
*
*   To test examples, just press F6 and execute raylib_compile_execute script
*   Note that compiled executable is placed in the same folder as .c file
*
*   You can find all basic examples on C:\raylib\raylib\examples folder or
*   raylib official webpage: www.raylib.com
*
*   Enjoy using raylib. :)
*
*   Example originally created with raylib 1.0, last time updated with raylib 1.0
*
*   Example licensed under an unmodified zlib/libpng license, which is an OSI-certified,
*   BSD-like license that allows static linking with closed source software
*
*   Copyright (c) 2013-2024 Ramon Santamaria (@raysan5)
*
********************************************************************************************/

#include "Common.h"
#include "raylib.h"
#include "Game.h"

RenderTexture2D defaultRender;

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(void)
{
	// Initialization
	//--------------------------------------------------------------------------------------
	Image icon = LoadImage("resources/img/icon.png");
	// SetWindowState(FLAG_WINDOW_UNDECORATED | FLAG_WINDOW_TRANSPARENT);
	InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Unnamed Platformer [Mika]");
	SetWindowIcon(icon);

	SetTargetFPS(60);               // Set our game to run at 60 frames-per-second

	defaultRender = LoadRenderTexture(WINDOW_WIDTH, WINDOW_HEIGHT);

	InitAudioDevice();
	//--------------------------------------------------------------------------------------

	// Main game loop
	EnterGameLoop();

	// De-Initialization
	//--------------------------------------------------------------------------------------
	UnloadImage(icon);
	CloseAudioDevice();
	UnloadRenderTexture(defaultRender);
	CloseWindow();        // Close window and OpenGL context
	//--------------------------------------------------------------------------------------

	return 0;
}