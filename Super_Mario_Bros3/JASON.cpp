#include <algorithm>
#include <assert.h>
#include "Utils.h"

#include "JASON.h"
#include "Game.h"

#include "PlayScene.h"
#include "Portal.h"

JASON::JASON(float x, float y) : CGameObject()
{
	untouchable = 0;
	SetState(JASON_STATE_IDLE);

	this->x = x;
	this->y = y;

}

void JASON::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	int id = CGame::GetInstance()->GetCurrentScene()->GetId();
	// Calculate dx, dy 
	CGameObject::Update(dt);

	// Simple fall down
	if (id == 1)
		vy += JASON_GRAVITY * dt;

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	// turn off collision when die 
	if (state != JASON_STATE_DIE)
		CalcPotentialCollisions(coObjects, coEvents);

	// reset untouchable timer if untouchable time has passed
	if (GetTickCount() - untouchable_start > JASON_UNTOUCHABLE_TIME)
	{
		untouchable_start = 0;
		untouchable = 0;
	}

	if (isAlreadyFired && (DWORD)GetTickCount64() - firing_start > JASON_FIRING_DELAY_TIME)
	{
		SetisAlreadyFired(false);
		SetisIsFiring(0);
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

		// how to push back JASON if collides with a moving objects, what if JASON is pushed this way into another object?
		//if (rdx != 0 && rdx!=dx)
		//	x += nx*abs(rdx); 

		// block every object first!
		x += min_tx * dx + nx * 0.4f;
		//y += min_ty * dy + ny * 0.4f;

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

void JASON::Render()
{
	
	int ani = 0;
	switch (state)
	{
	case JASON_STATE_WALKING_DOWN:
		ani = 0;
		pre_ani = ani;
		break;
	case JASON_STATE_WALKING_UP:
		ani = 2;
		pre_ani = ani;
		break;
	case JASON_STATE_WALKING_RIGHT:
		ani = 3;
		pre_ani = ani;
		break;
	case JASON_STATE_WALKING_LEFT:
		ani = 1;
		pre_ani = ani;
		break;
	case JASON_STATE_IDLE:
		ani = pre_ani + 4;
		break;
	}

	int alpha = 255;

	if (untouchable) alpha = 128;

	animation_set->at(ani)->Render(x, y, alpha);

	////RenderBoundingBox();
}

void JASON::SetState(int state)
{
	CGameObject::SetState(state);

	switch (state)
	{
	case JASON_STATE_WALKING_DOWN:
		vy = -JASON_WALKING_SPEED;
		break;
	case JASON_STATE_WALKING_UP:
		vy = JASON_WALKING_SPEED;
		break;
	case JASON_STATE_WALKING_RIGHT:
		vx = JASON_WALKING_SPEED;
		vy = 0;
		nx = 1;
		break;
	case JASON_STATE_WALKING_LEFT:
		vx = -JASON_WALKING_SPEED;
		vy = 0;
		nx = -1;
		break;
	case JASON_STATE_IDLE:
		vx = 0;
		vy = 0;
		break;
	case JASON_STATE_DIE:
		vy = JASON_DIE_DEFLECT_SPEED;
		break;
	}
}

void JASON::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;

	right = x + JASON_BIG_BBOX_WIDTH;
	bottom = y + JASON_BIG_BBOX_HEIGHT;

	//DebugOut(L"L T R B %f %f %f %f  \n", left, top, right, bottom);
}

/*
	Reset JASON status to the beginning state of a scene
*/
void JASON::Reset()
{
	SetState(JASON_STATE_IDLE);
}

void JASON::CalcPotentialCollisions(
	vector<LPGAMEOBJECT>* coObjects,
	vector<LPCOLLISIONEVENT>& coEvents)
{
	vector <LPCOLLISIONEVENT> collisionEvents;

	for (UINT i = 0; i < coObjects->size(); i++)
	{
		LPCOLLISIONEVENT e = SweptAABBEx(coObjects->at(i));

		if (dynamic_cast<CTANKBULLET*>(e->obj) || dynamic_cast<CREDWORM*>(e->obj))
		{
			continue;
		}
		if (dynamic_cast<CEYELET*>(e->obj) && e->obj->GetState() == EYELET_STATE_IDLE)
		{
			continue;
		}
		if (dynamic_cast<CBOOM*>(e->obj))
		{
			continue;
		}
		if (e->t > 0 && e->t <= 1.0f)
			collisionEvents.push_back(e);
		else
			delete e;
	}

	std::sort(collisionEvents.begin(), collisionEvents.end(), CCollisionEvent::compare);

	for (UINT i = 0; i < collisionEvents.size(); i++)
	{
		LPCOLLISIONEVENT e = SweptAABBEx(collisionEvents[i]->obj);
		if (e->t > 0 && e->t <= 1.0f)
		{
			coEvents.push_back(e);
		}
		else
			delete e;
	}
}

