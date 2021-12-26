#include "CWAVE_BULLET.h"
#include <algorithm>
#include "PlayScene.h"
#include "JASON.h"
#include "Brick.h"

CWAVE_BULLET::CWAVE_BULLET()
{
	SetState(CWAVE_BULLET_STATE_FLYING);
	nx = 0;
}

void CWAVE_BULLET::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + CWAVE_BULLET_BBOX_WIDTH;

	if (state == CWAVE_BULLET_STATE_DIE)
		y = y + CWAVE_BULLET_BBOX_HEIGHT;
	else bottom = y + CWAVE_BULLET_BBOX_HEIGHT;
}

void CWAVE_BULLET::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if ((DWORD)GetTickCount64() - reset_start > CWAVE_BULLET_RESET_TIME)
	{
		state = CWAVE_BULLET_STATE_DIE;
		reset_start = 0;
		change_speed_start = 0;
	}
	if ((DWORD)GetTickCount64() - change_speed_start > CWAVE_BULLET_CHANGE_SPEED_TIME)
	{
		vy = -vy;
		change_speed_start = (DWORD)GetTickCount64();
	}
	CGameObject::Update(dt, coObjects);
	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	// turn off collision when die 
	if (state != CWAVE_BULLET_STATE_DIE)
		CalcPotentialCollisions(coObjects, coEvents);
	else
	{
		isUsed = false;
		x = STORING_LOCATION;
		y = STORING_LOCATION;
		SetState(CWAVE_BULLET_STATE_FLYING);
	}
	if (isUsed == false)
	{
		JASON* JASON = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer2();
		if (JASON->GetisFiring() == true)
		{
			if (JASON->GetisAlreadyFired() == false)
			{
				isUsed = true;
				x = JASON->x;
				y = JASON->y;
				SetSpeed(JASON->nx * CWAVE_BULLET_SPEED, CWAVE_BULLET_SPEED);
				JASON->SetisAlreadyFired(true);
				JASON->StartFiring();
				StartReset();
				StartSpeed();
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
				(e->obj)->SetisAlive(false);
				SetState(CWAVE_BULLET_STATE_DIE);
				((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->AddKaboomMng((e->obj)->GetPositionX(), (e->obj)->GetPositionY());
			}
			else {
				if (nx != 0)
				{
					((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->AddKaboomMng(x, y);
					SetState(CWAVE_BULLET_STATE_DIE);
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

void CWAVE_BULLET::CalcPotentialCollisions(
	vector<LPGAMEOBJECT>* coObjects,
	vector<LPCOLLISIONEVENT>& coEvents)
{
	for (UINT i = 0; i < coObjects->size(); i++)
	{
		LPCOLLISIONEVENT e = SweptAABBEx(coObjects->at(i));
		if (dynamic_cast<JASON*>(e->obj))
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

void CWAVE_BULLET::Render()
{
	if (state == CWAVE_BULLET_STATE_DIE)
		return;
	int ani = 0;
	animation_set->at(ani)->Render(x, y);

	//RenderBoundingBox();
}

void CWAVE_BULLET::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case CWAVE_BULLET_STATE_DIE:
		vx = CWAVE_BULLET_STATE_DIE_SPEED;
		vy = CWAVE_BULLET_STATE_DIE_SPEED;
		break;

	}

}
