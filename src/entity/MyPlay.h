#pragma once

#include <raylib.h>
#include <raymath.h>

#include "Common.h"
#include "LDtkLoader/Entity.hpp"

typedef struct MYPLAY {
	Vector2 position; // current position
	float& x = position.x; // reference to `position.x`
	float& y = position.y; // reference to `position.y`
	
	Vector2 velocity; // current velocity
	float& xm = velocity.x; // reference to `velocity.x`
	float& ym = velocity.y; // reference to `velocity.y`

	float maxMove; // max
	float maxDash; // max
	float resist = 0x200 / 10.0f;
	float jump;
	float dash1;
	float dash2;

	int flags; // TODO: Add enums for this
	Direction dir; // current direction

	float coyote_time = 0;
	float jump_buffer = 0;

	bool grounded;

	Rectangle rect; // hitbox

	// collsion
	Vector2 old_pos;
} MYPLAY;

extern MYPLAY gPlayer;

void InitMyPlay(void);
void UpdateMyPlay(void);
bool UpdateColBetweenPlayerRect(Rectangle col, bool passable = false);
bool CollideEntity(const ldtk::Entity& ent);
void DrawMyPlay(void);
void DestroyMyPlay(void);
