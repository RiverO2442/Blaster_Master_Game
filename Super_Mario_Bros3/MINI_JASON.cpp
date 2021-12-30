#include <algorithm>
#include <assert.h>
#include "Utils.h"

#include "MINI_JASON.h"
#include "Game.h"

#include "PlayScene.h"
#include "Portal.h"

MINI_JASON::MINI_JASON(float x, float y) : CGameObject()
{
	untouchable = 0;
	SetState(MINI_JASON_STATE_IDLE);
	start_x = x;
	start_y = y;
	this->x = x;
	this->y = y;

}

void MINI_JASON::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	int id = CGame::GetInstance()->GetCurrentScene()->GetId();
	CGame* game = CGame::GetInstance();
	// Calculate dx, dy 
	CGameObject::Update(dt);
	CPlayScene* playscene = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene());

	// Simple fall down
	if(!playscene->getpiloting())
	vy -= MINI_JASON_GRAVITY * dt;

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	
	if (playscene->getpiloting())
	{
		y = playscene->GetPlayer()->GetPositionY();
		x = playscene->GetPlayer()->GetPositionX() + 5;
	}

	// turn off collision when die 
	if (state != MINI_JASON_STATE_DIE)
		CalcPotentialCollisions(coObjects, coEvents);

	// reset untouchable timer if untouchable time has passed
	if (GetTickCount() - untouchable_start > MINI_JASON_UNTOUCHABLE_TIME)
	{
		untouchable_start = 0;
		untouchable = 0;
	}

	if (isAlreadyFired && (DWORD)GetTickCount64() - firing_start > MINI_JASON_FIRING_DELAY_TIME)
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

		// how to push back MINI_JASON if collides with a moving objects, what if MINI_JASON is pushed this way into another object?
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
			if (ny < 0)
			{
				SetIsJumping(false);
			}
		}
	}

	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void MINI_JASON::Render()
{
	CPlayScene* playscene = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene());
	if (!playscene->getpiloting())
	{
		int ani = -1;
		if (state == MINI_JASON_STATE_DIE)
			return;

		if (state != MINI_JASON_STATE_IDLE)
		{
			if (state == MINI_JASON_STATE_WALKING_RIGHT)
			{
				ani = MINI_JASON_ANI_WALKING_RIGHT;
				pre_ani = ani;
			}
			if (state == MINI_JASON_STATE_WALKING_LEFT) {
				ani = MINI_JASON_ANI_WALKING_LEFT;
				pre_ani = ani;
			}
		}
		else
			ani = pre_ani + 2;
		if (isJumping)
			ani = pre_ani + 4;

		int alpha = 255;

		if (untouchable) alpha = 128;

		animation_set->at(ani)->Render(x, y, alpha);
	}
}

void MINI_JASON::SetState(int state)
{
	CGameObject::SetState(state);

	switch (state)
	{
	case MINI_JASON_STATE_WALKING_RIGHT:
		vx = MINI_JASON_WALKING_SPEED;
		nx = 1;
		dir = 1;
		break;
	case MINI_JASON_STATE_WALKING_LEFT:
		vx = -MINI_JASON_WALKING_SPEED;
		nx = -1;
		dir = -1;
		break;
	case MINI_JASON_STATE_JUMP:
		// TODO: need to check if MINI_JASON is *current* on a platform before allowing to jump again
		vy = MINI_JASON_JUMP_SPEED_Y;
		break;
	case MINI_JASON_STATE_IDLE:
		vx = 0;
		break;
	case MINI_JASON_STATE_DIE:
		vy = -MINI_JASON_DIE_DEFLECT_SPEED;
		break;
	}
}

void MINI_JASON::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	CPlayScene* playscene = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene());
	if (!playscene->getpiloting())
	{
		left = x;
		top = y;
		right = x + MINI_JASON_BBOX_WIDTH;
		bottom = y + MINI_JASON_BBOX_HEIGHT;

		//DebugOut(L"L T R B %f %f %f %f  \n", left, top, right, bottom);
	}
}

/*
	Reset MINI_JASON status to the beginning state of a scene
*/
void MINI_JASON::Reset()
{
	SetState(MINI_JASON_STATE_IDLE);
}

void MINI_JASON::CalcPotentialCollisions(
	vector<LPGAMEOBJECT>* coObjects,
	vector<LPCOLLISIONEVENT>& coEvents)
{
	CGame* game = CGame::GetInstance();
	CPlayScene* playscene = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene());

	vector <LPCOLLISIONEVENT> collisionEvents;

	for (UINT i = 0; i < coObjects->size(); i++)
	{
		LPCOLLISIONEVENT e = SweptAABBEx(coObjects->at(i));

		if (dynamic_cast<CTANKBULLET*>(e->obj) || dynamic_cast<CREDWORM*>(e->obj))
		{
			continue;
		}
		if (dynamic_cast<CSOPHIA*>(e->obj))
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
		{
			if (dynamic_cast<CPortal*>(e->obj))
			{
				CPortal* portal = dynamic_cast<CPortal*>(e->obj);
				playscene->StartFilming();
				game->setFilming(true);
				if(portal->GetCamState() != -1)
					playscene->setCamState(portal->GetCamState());
				if (portal->GetSceneId() != -1)
					game->SwitchScene(portal->GetSceneId());
				continue;
			}
			else
			{
				collisionEvents.push_back(e);
			}
		}
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

