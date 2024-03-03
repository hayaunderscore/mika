#include "MyPlay.h"
#include "Common.h"
#include <cstdlib>
#include <raylib.h>
#include <raymath.h>

MYPLAY gPlayer;

void InitMyPlay()
{
	gPlayer.position = {0.0f}; // init vector
	gPlayer.velocity = {0.0f}; // init velocity

	gPlayer.flags = 0; // no flags
	gPlayer.dir = DIR_RIGHT; // right by default

	gPlayer.rect = {0.0f, 0.0f, 16.0f, 16.0f}; // default collision

	gPlayer.maxMove = 0x4FF;
	gPlayer.maxDash = 0x5FF;
	gPlayer.jump = 0x500;
	gPlayer.dash1 = 0x200 / 6.0f;
	gPlayer.dash2 = 0x200 / 16.0f;

	gPlayer.grounded = true;
}

static inline float GrabDeltaMultiplier()
{
	return GetFrameTime();
}

void UpdateMyPlay()
{
	// store old position for player collision
	gPlayer.old_pos = gPlayer.position;
	float maxMove = gPlayer.maxMove;

	if (gPlayer.grounded)
	{
		if (IsKeyDown(KEY_LEFT))
		{
			gPlayer.dir = DIR_LEFT;
			if (gPlayer.xm > -gPlayer.maxDash)
				gPlayer.xm -= gPlayer.dash1;
		}
		else if (IsKeyDown(KEY_RIGHT))
		{
			gPlayer.dir = DIR_RIGHT;
			if (gPlayer.xm < gPlayer.maxDash)
				gPlayer.xm += gPlayer.dash1;
		}

		if (gPlayer.xm < 0)
		{
			if (gPlayer.xm > -gPlayer.resist)
				gPlayer.xm = 0;
			else
				gPlayer.xm += gPlayer.resist;
		}
		if (gPlayer.xm > 0)
		{
			if (gPlayer.xm < gPlayer.resist)
				gPlayer.xm = 0;
			else
				gPlayer.xm -= gPlayer.resist;
		}
	}
	else
	{
		if (IsKeyDown(KEY_LEFT))
		{
			gPlayer.dir = DIR_LEFT;
			if (gPlayer.xm > -gPlayer.maxDash)
				gPlayer.xm -= gPlayer.dash2;
		}
		else if (IsKeyDown(KEY_RIGHT))
		{
			gPlayer.dir = DIR_RIGHT;
			if (gPlayer.xm < gPlayer.maxDash)
				gPlayer.xm += gPlayer.dash2;
		}
	}
	
	//gPlayer.velocity.x = fminf(abs(gPlayer.velocity.x), maxMove) * (gPlayer.dir == DIR_LEFT ? -1 : 1);

	if (IsKeyDown(KEY_Z) && gPlayer.grounded)
	{
		gPlayer.velocity.y -= 0x600;
	}

	// we are no longer grounded
	if (gPlayer.velocity.y < 0 || gPlayer.velocity.y > 0)
		gPlayer.grounded = false;

	// grav
	if (gPlayer.ym < 0 && IsKeyDown(KEY_Z))
		gPlayer.velocity.y += 0x30;
	else
		gPlayer.velocity.y += 0x60;

	if (gPlayer.xm < -gPlayer.maxMove)
		gPlayer.xm = -gPlayer.maxMove;
	if (gPlayer.ym < -gPlayer.maxMove)
		gPlayer.ym = -gPlayer.maxMove;

	if (gPlayer.xm > gPlayer.maxMove)
		gPlayer.xm = gPlayer.maxMove;
	if (gPlayer.ym > gPlayer.maxMove)
		gPlayer.ym = gPlayer.maxMove;

	if (!(gPlayer.xm <= gPlayer.resist && gPlayer.xm >= -gPlayer.resist))
		gPlayer.position.x += gPlayer.velocity.x / 0x200 * GrabDeltaMultiplier()*60;
	gPlayer.position.y += gPlayer.velocity.y / 0x200 * GrabDeltaMultiplier()*60;
}

// TODO: Make y position not the point where the player touches the ground??????
bool UpdateColBetweenPlayerRect(Rectangle col, bool passable)
{
	Rectangle hitbox = {gPlayer.position.x - gPlayer.rect.width / 2, gPlayer.position.y - gPlayer.rect.height, 
				gPlayer.rect.width, gPlayer.rect.height};

	// no collision.
	if (!CheckCollisionRecs(hitbox, col))
		return false;

	Vector2& newPos = gPlayer.position;
	Vector2& oldPos = gPlayer.old_pos;

	float playerLeft = newPos.x;
	float playerRight = newPos.x + gPlayer.rect.width;
	float playerTop = newPos.y - gPlayer.rect.height;

	float colLeft = col.x;
	float colRight = col.x + col.width;
	float colBottom = col.y + col.height;

	// we landed (top)
	if (oldPos.y <= col.y && newPos.y >= col.y)
	{
		gPlayer.velocity.y = 0;
		newPos.y = col.y;
		// obligatory ground check
		// we want to JUMP bitch
		gPlayer.grounded = true;
	}
	// hit ceiling
	else if (!passable && (oldPos.y - hitbox.height >= colBottom && playerTop <= colBottom))
	{
		gPlayer.velocity.y = 0;
		newPos.y = colBottom + hitbox.height;
	}
	// slapped a wall (left or right)
	else if (!passable && ((playerRight >= colLeft) || (playerLeft <= colRight)))
	{
		gPlayer.velocity.x = 0;
		if (playerRight >= colLeft && (playerLeft < colRight))
			newPos.x = colLeft - gPlayer.rect.width / 2;
		else
			newPos.x = colRight + gPlayer.rect.width / 2;
		// if both of these fail, what the fuck are you supposed to do here??????
	}

	// there was collision!
	return true;
}

void DrawMyPlay()
{
	Rectangle hitbox = {gPlayer.position.x - gPlayer.rect.width / 2, gPlayer.position.y - gPlayer.rect.height, gPlayer.rect.width, gPlayer.rect.height};

	DrawRectangleRec(hitbox, RED);
}

void DestroyMyPlay()
{
	// nothing for now
}
