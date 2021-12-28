#include "CGRENADE.h"
#include <algorithm>
#include "PlayScene.h"
#include "JASON.h"
#include "Brick.h"

CGRENADE::CGRENADE()
{
	SetState(CGRENADE_STATE_FLYING);
	nx = 0;
}

void CGRENADE::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + CGRENADE_BBOX_WIDTH;

	if (state == CGRENADE_STATE_DIE)
		y = y + CGRENADE_BBOX_HEIGHT;
	else bottom = y + CGRENADE_BBOX_HEIGHT;
}

void CGRENADE::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{

	CGameObject::Update(dt, coObjects);
	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	// turn off collision when die 
	if (state != CGRENADE_STATE_DIE)
		CalcPotentialCollisions(coObjects, coEvents);
	else
	{
		isUsed = false;
		x = STORING_LOCATION;
		y = STORING_LOCATION;
		SetState(CGRENADE_STATE_FLYING);
	}
	if (isUsed == false)
	{
		JASON* JASON = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer2();
		if (JASON->GetisFiring() == true && JASON->getWeapon() == -1)
		{
			if (JASON->GetisAlreadyFired() == false)
			{
				isUsed = true;
				x = JASON->x;
				y = JASON->y;
				/*SetSpeed(JASON->nx * CGRENADE_SPEED, CGRENADE_SPEED);*/
				switch (JASON->GetPre_ani())
				{
				case JASON_ANI_WALK_DOWN:
					vy = -CGRENADE_SPEED;
					vx = 0;
					dir = 0;
					break;
				case JASON_ANI_WALK_LEFT:
					vx = -CGRENADE_SPEED;
					vy = 0;
					dir = 1;
					break;
				case JASON_ANI_WALK_UP:
					vy = CGRENADE_SPEED;
					vx = 0;
					dir = 2;
					break;
				case JASON_ANI_WALK_RIGHT:
					vx = CGRENADE_SPEED;
					vy = 0;
					dir = 3;
					break;
				}
				JASON->SetisAlreadyFired(true);
				JASON->StartFiring();
				StartReset();
			}
		}
	}
	// No collision occured, proceed normally
	if (coEvents.size() == 0)
	{
		x += dx;
		y += dy;
		if ((DWORD)GetTickCount64() - reset_start > CGRENADE_RESET_TIME && reset_start != 0)
		{
			((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->AddKaboomMng(x, y);
			state = CGRENADE_STATE_DIE;
			reset_start = 0;
			dir = 0;
		}
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
			if (!dynamic_cast<CBrick*>(e->obj)) 
			{
				(e->obj)->SetState(STATE_DIE);
				(e->obj)->SetisAlive(false);
				SetState(CGRENADE_STATE_DIE);
				((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->AddKaboomMng((e->obj)->GetPositionX(), (e->obj)->GetPositionY());
			}
			else {
				if (nx != 0)
				{
					((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->AddKaboomMng(x, y);
					SetState(CGRENADE_STATE_DIE);
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

void CGRENADE::CalcPotentialCollisions(
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

void CGRENADE::Render()
{
	if (state == CGRENADE_STATE_DIE)
		return;
	int ani = 0;
	animation_set->at(ani)->Render(x, y);

	//RenderBoundingBox();
}

void CGRENADE::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case CGRENADE_STATE_DIE:
		vx = CGRENADE_STATE_DIE_SPEED;
		vy = CGRENADE_STATE_DIE_SPEED;
		break;

	}

}
