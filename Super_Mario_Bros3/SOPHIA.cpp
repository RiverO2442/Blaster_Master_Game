#include <algorithm>
#include <assert.h>
#include "Utils.h"

#include "SOPHIA.h"
#include "Game.h"

#include "PlayScene.h"
#include "Portal.h"

CSOPHIA::CSOPHIA(float x, float y) : CGameObject()
{
	untouchable = 0;
	SetState(SOPHIA_STATE_IDLE);

	start_x = x;
	start_y = y;
	this->x = x;
	this->y = y;
	
}

void CSOPHIA::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	int id = CGame::GetInstance()->GetCurrentScene()->GetId();
	// Calculate dx, dy 
	CGameObject::Update(dt);

	// Simple fall down
	if(id == 1)
	vy -= SOPHIA_GRAVITY * dt;

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	// turn off collision when die 
	if (state != SOPHIA_STATE_DIE)
		CalcPotentialCollisions(coObjects, coEvents);

	// reset untouchable timer if untouchable time has passed
	if (GetTickCount() - untouchable_start > SOPHIA_UNTOUCHABLE_TIME)
	{
		untouchable_start = 0;
		untouchable = 0;
	}

	if (isAlreadyFired && (DWORD)GetTickCount64() - firing_start > SOPHIA_FIRING_DELAY_TIME)
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

		// how to push back SOPHIA if collides with a moving objects, what if SOPHIA is pushed this way into another object?
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

void CSOPHIA::Render()
{
	//
	//int ani = -1;
	//if (state == SOPHIA_STATE_DIE)
	//	ani = SOPHIA_ANI_DIE;
	//else
	//{
	//	if (vx == 0)
	//	{
	//		if (nx > 0) ani = SOPHIA_ANI_BIG_IDLE_RIGHT;
	//		else ani = SOPHIA_ANI_BIG_IDLE_LEFT;
	//	}
	//	else if (vx > 0)
	//		ani = SOPHIA_ANI_BIG_WALKING_RIGHT;
	//	else ani = SOPHIA_ANI_BIG_WALKING_LEFT;

	//}
	//int alpha = 255;
	//if (untouchable) alpha = 128;

	//animation_set->at(ani)->Render(x, y, alpha);

	////RenderBoundingBox();
}

void CSOPHIA::SetState(int state)
{
	CGameObject::SetState(state);

	switch (state)
	{
	case SOPHIA_STATE_WALKING_DOWN:
		vy = -SOPHIA_WALKING_SPEED;
		break;
	case SOPHIA_STATE_WALKING_UP:
		vy = SOPHIA_WALKING_SPEED;
		break;
	case SOPHIA_STATE_WALKING_RIGHT:
		vx = SOPHIA_WALKING_SPEED;
		nx = 1;
		break;
	case SOPHIA_STATE_WALKING_LEFT:
		vx = -SOPHIA_WALKING_SPEED;
		nx = -1;
		break;
	case SOPHIA_STATE_JUMP:
		// TODO: need to check if SOPHIA is *current* on a platform before allowing to jump again
		vy = SOPHIA_JUMP_SPEED_Y;
		break;
	case SOPHIA_STATE_IDLE:
		vx = 0;
		break;
	case SOPHIA_STATE_DIE:
		vy = -SOPHIA_DIE_DEFLECT_SPEED;
		break;
	}
}

void CSOPHIA::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;

	right = x + SOPHIA_BIG_BBOX_WIDTH;
	bottom = y + SOPHIA_BIG_BBOX_HEIGHT;

	//DebugOut(L"L T R B %f %f %f %f  \n", left, top, right, bottom);
}

/*
	Reset SOPHIA status to the beginning state of a scene
*/
void CSOPHIA::Reset()
{
	SetState(SOPHIA_STATE_IDLE);
}

void CSOPHIA::CalcPotentialCollisions(
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

