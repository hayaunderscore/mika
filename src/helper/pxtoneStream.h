/*******************************************************************************************
*	pxtone is pretty cool
*	This file is under the public domain.
*******************************************************************************************/

#include "pxtnService.h"
#include "raylib.h"

pxtnService* LoadPxtoneStream(const char* file, AudioStream& stream);
void ExplodePxtoneStream(AudioStream& stream, pxtnService* pxtn);