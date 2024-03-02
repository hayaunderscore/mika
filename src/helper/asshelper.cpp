/*******************************************************************************************
* This file exists to not clutter up Main.cpp too much.
*******************************************************************************************/

#include "Common.h"

// why isnt `asprintf()` in the traditional c standard library yet?????
#include "misc/asprintf.h"
#include <cstdio>
#include <cstdlib>

const char* GetImage(const char* path)
{
	char* str = NULL;
	asprintf(&str, "%s%s%s", resourceFolder, imageFolder, path);
	return str;
}

const char* GetSound(const char* path)
{
	char* str = NULL;
	asprintf(&str, "%s%s%s", resourceFolder, soundFolder, path);
	return str;
}

const char* GetFont(const char* path)
{
	char* str = NULL;
	asprintf(&str, "%s%s%s", resourceFolder, fontFolder, path);
	return str;
}

const char* GetMap(const char* path)
{
	char* str = NULL;
	asprintf(&str, "%s%s%s", resourceFolder, mapFolder, path);
	return str;
}