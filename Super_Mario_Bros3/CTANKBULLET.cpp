#include "CTANKBULLET.h"
#include <algorithm>
#include "PlayScene.h"
#include "SOPHIA.h"
#include "Brick.h"

CTANKBULLET::CTANKBULLET()
{
	SetState(CTANKBULLET_STATE_FLYING);
	nx = 0;
}

void CTANKBULLET::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + CTANKBULLET_BBOX_WIDTH;

	if (state == CTANKBULLET_STATE_DIE)
		y = y + CTANKBULLET_BBOX_HEIGHT;
	else bottom = y + CTANKBULLET_BBOX_HEIGHT;
}

void CTANKBULLET::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if ((DWORD)GetTickCount64() - reset_start > CTANKBULLET_RESET_TIME)
	{
		state = CTANKBULLET_STATE_DIE;
		reset_start = 0;
	}
	CGameObject::Update(dt, coObjects);
	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();
	
	// turn off collision when die 
	if (state != CTANKBULLET_STATE_DIE)
		CalcPotentialCollisions(coObjects, coEvents);
	else
	{
		isUsed = false;
		x = STORING_LOCATION;
		y = STORING_LOCATION;
		SetState(CTANKBULLET_STATE_FLYING);
	}
	if (isUsed == false)
	{
		CSOPHIA* SOPHIA = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
		if (SOPHIA->GetisFiring() == true)
		{
			if (SOPHIA->GetisAlreadyFired() == false)
			{
				isUsed = true;
				x = SOPHIA->x;
				y = SOPHIA->y;
				SetSpeed(SOPHIA->nx * 0.15);
				SOPHIA->SetisAlreadyFired(true);
				SOPHIA->StartFiring();
				StartReset();
				DebugOut(L"FIRED \n");
			}
		}
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

		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];
			if (!dynamic_cast<CBrick*>(e->obj)) // if e->obj is Goomba
			{
				(e->obj)->SetState(STATE_DIE);
				((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->AddKaboomMng(e->obj->x, e->obj->y);
				SetState(CTANKBULLET_STATE_DIE);
			}
			else {
				if (nx != 0)
				{
					((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->AddKaboomMng(x, y);
					SetState(CTANKBULLET_STATE_DIE);
				}
			}
		}
		// clean up collision events
		for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
		if (x <= 0)
			if (vx < 0)
				vx = -vx;
	}
}

void CTANKBULLET::CalcPotentialCollisions(
	vector<LPGAMEOBJECT>* coObjects,
	vector<LPCOLLISIONEVENT>& coEvents)
{
	for (UINT i = 0; i < coObjects->size(); i++)
	{
		LPCOLLISIONEVENT e = SweptAABBEx(coObjects->at(i));
		if (dynamic_cast<CSOPHIA*>(e->obj))
		{
				continue;
		}
		if (e->t > 0 && e->t <= 1.0f)
			coEvents.push_back(e);
		else
			delete e;
	}
	std::sort(coEvents.begin(), coEvents.end(), CCollisionEvent::compare);
}

void CTANKBULLET::Render()
{
	if (state == CTANKBULLET_STATE_DIE)
		return;
	int ani;
	switch (state)
	{
	case CTANKBULLET_STATE_FLYING:
		if(vx > 0)
				ani = CTANKBULLET_ANI_FLYING_RIGHT;
		else
		ani = CTANKBULLET_ANI_FLYING_LEFT;
		break;
	}

	animation_set->at(ani)->Render(x, y);

	//RenderBoundingBox();
}

void CTANKBULLET::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case CTANKBULLET_STATE_DIE:
		vx = CTANKBULLET_STATE_DIE_SPEED;
		vy = CTANKBULLET_STATE_DIE_SPEED;
		break;

	}

}
