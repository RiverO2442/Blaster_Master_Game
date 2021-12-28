#include "CLASER_BULLET.h"
#include <algorithm>
#include "PlayScene.h"
#include "Brick.h"

CLASER_BULLET::CLASER_BULLET()
{
	SetState(CLASER_BULLET_STATE_IDLE);
	nx = 0;
}

void CLASER_BULLET::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + CLASER_BULLET_BBOX_WIDTH;

	if (state == CLASER_BULLET_STATE_DIE)
		y = y + CLASER_BULLET_BBOX_HEIGHT;
	else bottom = y + CLASER_BULLET_BBOX_HEIGHT;
}

void CLASER_BULLET::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CPlayScene* playscene = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene());
	CGameObject::Update(dt, coObjects);

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	// turn off collision when die 
	if (state != CLASER_BULLET_STATE_DIE)
		CalcPotentialCollisions(coObjects, coEvents);
	else
	{
		isUsed = false;
		x = STORING_LOCATION;
		y = STORING_LOCATION;
		SetState(CLASER_BULLET_STATE_DIE);
	}
	if (isUsed == false)
	{
		if (((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->CheckInterruptBulletMng())
		{
			this->SetPosition(playscene->GetInterruptBulletMng()->getCEventPoisitionX(), playscene->GetInterruptBulletMng()->getCEventPoisitionY());
			playscene->DeleteInterruptBulletMng();
			isUsed = true;
			SetState(CLASER_BULLET_STATE_IDLE);
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
			if (dynamic_cast<CBrick*>(e->obj)) 
			{
				SetState(CLASER_BULLET_STATE_DIE);
			}
		}
		// clean up collision events
		for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
	}
}

void CLASER_BULLET::CalcPotentialCollisions(
	vector<LPGAMEOBJECT>* coObjects,
	vector<LPCOLLISIONEVENT>& coEvents)
{
	for (UINT i = 0; i < coObjects->size(); i++)
	{
		LPCOLLISIONEVENT e = SweptAABBEx(coObjects->at(i));
		if (!dynamic_cast<CBrick*>(e->obj))
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

void CLASER_BULLET::Render()
{
	int ani = 0;

	switch (state)
	{
	case CLASER_BULLET_STATE_IDLE:
		ani = CLASER_BULLET_ANI_IDLE;
		break;
	case CLASER_BULLET_STATE_DIE:
		return;
	}

	animation_set->at(ani)->Render(x, y);

	//RenderBoundingBox();
}

void CLASER_BULLET::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case CLASER_BULLET_STATE_IDLE:
		vy = -CLASER_BULLET_SPEED;
		break;

	}

}
