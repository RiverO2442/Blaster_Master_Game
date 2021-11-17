#include <algorithm>
#include <assert.h>
#include "Utils.h"

#include "TANK_BODY.h"
#include "Game.h"

#include "Eye.h"
#include "Portal.h"

CTANK_BODY::CTANK_BODY(float x, float y) : CGameObject()
{
	level = TANK_BODY_LEVEL_BIG;
	untouchable = 0;
	SetState(TANK_BODY_STATE_IDLE);

	start_x = x;
	start_y = y;
	this->x = x;
	this->y = y;
}

void CTANK_BODY::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	// Calculate dx, dy 
	CGameObject::Update(dt);

	// Simple fall down
	/*vy += TANK_BODY_GRAVITY * dt;*/

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	// turn off collision when die 
	if (state != TANK_BODY_STATE_DIE)
		CalcPotentialCollisions(coObjects, coEvents);

	// reset untouchable timer if untouchable time has passed
	if (GetTickCount() - untouchable_start > TANK_BODY_UNTOUCHABLE_TIME)
	{
		untouchable_start = 0;
		untouchable = 0;
	}

	// No collision occured, proceed normally
	if (coEvents.size() == 0)
	{
		x += dx;
		y += dy;
	}
	else
	{
		float min_tx, min_ty, nx = 0, ny;
		float rdx = 0;
		float rdy = 0;

		// TODO: This is a very ugly designed function!!!!
		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);

		// how to push back TANK_BODY if collides with a moving objects, what if TANK_BODY is pushed this way into another object?
		//if (rdx != 0 && rdx!=dx)
		//	x += nx*abs(rdx); 

		// block every object first!
		x += min_tx * dx + nx * 0.4f;
		y += min_ty * dy + ny * 0.4f;

		if (nx != 0) vx = 0;
		if (ny != 0) vy = 0;

		//
		// Collision logic with other objects
		//
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];
		}
	}

	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void CTANK_BODY::Render()
{
	int ani = -1;
	if (state == TANK_BODY_STATE_DIE)
		ani = TANK_BODY_ANI_DIE;
	else
		if (level == TANK_BODY_LEVEL_BIG)
		{
			if (vx == 0)
			{
				if (nx > 0) ani = TANK_BODY_ANI_BIG_IDLE_RIGHT;
				else ani = TANK_BODY_ANI_BIG_IDLE_LEFT;
			}
			else if (vx > 0)
				ani = TANK_BODY_ANI_BIG_WALKING_RIGHT;
			else ani = TANK_BODY_ANI_BIG_WALKING_LEFT;
		}
		else if (level == TANK_BODY_LEVEL_SMALL)
		{
			if (vx == 0)
			{
				if (nx > 0) ani = TANK_BODY_ANI_SMALL_IDLE_RIGHT;
				else ani = TANK_BODY_ANI_SMALL_IDLE_LEFT;
			}
			else if (vx > 0)
				ani = TANK_BODY_ANI_SMALL_WALKING_RIGHT;
			else ani = TANK_BODY_ANI_SMALL_WALKING_LEFT;
		}

	int alpha = 255;
	if (untouchable) alpha = 128;

	animation_set->at(ani)->Render(x, y, alpha);

	//RenderBoundingBox();
}

void CTANK_BODY::SetState(int state)
{
	CGameObject::SetState(state);

	switch (state)
	{
	case TANK_BODY_STATE_WALKING_DOWN:
		vy = TANK_BODY_WALKING_SPEED;
		break;
	case TANK_BODY_STATE_WALKING_UP:
		vy = -TANK_BODY_WALKING_SPEED;
		break;
	case TANK_BODY_STATE_WALKING_RIGHT:
		vx = TANK_BODY_WALKING_SPEED;
		nx = 1;
		break;
	case TANK_BODY_STATE_WALKING_LEFT:
		vx = -TANK_BODY_WALKING_SPEED;
		nx = -1;
		break;
	case TANK_BODY_STATE_JUMP:
		// TODO: need to check if TANK_BODY is *current* on a platform before allowing to jump again
		vy = -TANK_BODY_JUMP_SPEED_Y;
		break;
	case TANK_BODY_STATE_IDLE:
		vx = 0;
		vy = 0;
		break;
	case TANK_BODY_STATE_DIE:
		vy = -TANK_BODY_DIE_DEFLECT_SPEED;
		break;
	}
}

void CTANK_BODY::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x - 10;
	top = y - 9;

	right = x + TANK_BODY_BIG_BBOX_WIDTH + 9;
	bottom = y + TANK_BODY_BIG_BBOX_HEIGHT;
}

/*
	Reset TANK_BODY status to the beginning state of a scene
*/
void CTANK_BODY::Reset()
{
	SetState(TANK_BODY_STATE_IDLE);
	SetLevel(TANK_BODY_LEVEL_BIG);
	SetPosition(start_x, start_y);
	SetSpeed(0, 0);
}

