#include "CGX_BULLET.h"
#include <algorithm>
#include "PlayScene.h"
#include "Brick.h"

CGX_BULLET::CGX_BULLET()
{
	SetState(CGX_BULLET_STATE_IDLE);
	nx = 0;
}

void CGX_BULLET::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	if (state != CGX_BULLET_STATE_DIE)
	{
		left = x;
		top = y;
		right = x + CGX_BULLET_BBOX_WIDTH;
		bottom = y + CGX_BULLET_BBOX_HEIGHT;
	}
}

void CGX_BULLET::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CPlayScene* playscene = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene());
	CGameObject::Update(dt, coObjects);

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	if ((DWORD)GetTickCount64() - reset_start > CGX_BULLET_RESET_TIME && reset_start != 0)
	{
		state = CGX_BULLET_STATE_DIE;
		reset_start = 0;
	}

	// turn off collision when die 
	if (state != CGX_BULLET_STATE_DIE)
		CalcPotentialCollisions(coObjects, coEvents);
	else
	{
		isUsed = false;
		x = STORING_LOCATION;
		y = STORING_LOCATION;
		SetState(CGX_BULLET_STATE_DIE);
	}
	if (isUsed == false)
	{
		if (((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->CheckCGXMng())
		{
			this->SetPosition(playscene->GetCGXMng()->getCEventPoisitionX(), playscene->GetCGXMng()->getCEventPoisitionY());
			isUsed = true;
			SetState(CGX_BULLET_STATE_IDLE);
			SetSpeed(playscene->GetCGXMng()->getCEventSpeedX(), playscene->GetCGXMng()->getCEventSpeedY());
			playscene->DeleteCGXMng();
			StartReset();
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
				SetState(CGX_BULLET_STATE_DIE);
			}
		}
		// clean up collision events
		for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
	}
}

void CGX_BULLET::CalcPotentialCollisions(
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

void CGX_BULLET::Render()
{
	int ani = 0;

	switch (state)
	{
	case CGX_BULLET_STATE_IDLE:
		ani = CGX_BULLET_ANI_IDLE;
		break;
	case CGX_BULLET_STATE_DIE:
		return;
	}

	animation_set->at(ani)->Render(x, y);

	//RenderBoundingBox();
}

void CGX_BULLET::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	}

}
