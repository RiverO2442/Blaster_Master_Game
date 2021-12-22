#include "CREDWORM.h"
#include <algorithm>
#include "PlayScene.h"
#include "Brick.h"

CREDWORM::CREDWORM()
{
	SetState(CREDWORM_STATE_DIE);
	nx = 0;
}

void CREDWORM::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + CREDWORM_BBOX_WIDTH;

	if (state == CREDWORM_STATE_DIE)
		y = y + CREDWORM_BBOX_HEIGHT;
	else bottom = y + CREDWORM_BBOX_HEIGHT;
}

void CREDWORM::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CPlayScene* playscene = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene());
	CGameObject::Update(dt, coObjects);

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	// Simple fall down
	if (state != CREDWORM_STATE_DIE)
		vy += SOPHIA_GRAVITY * dt;

	coEvents.clear();

	// turn off collision when die 
	if (state != CREDWORM_STATE_DIE)
		CalcPotentialCollisions(coObjects, coEvents);
	else
	{
		isUsed = false;
		x = STORING_LOCATION;
		y = STORING_LOCATION;
		SetState(CREDWORM_STATE_DIE);
	}
	if (isUsed == false)
	{
		if (((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->CheckWormSpamMng())
		{
			SetState(CREDWORM_STATE_WALKING);
			this->SetPosition(playscene->GetWormSpamMng()->getCEventPoisitionX(), playscene->GetWormSpamMng()->getCEventPoisitionY());
			playscene->DeleteWormSpamMng();
			isUsed = true;
			
		}
	}

	if (playscene->GetPlayer()->GetPositionX() > this->x)
	{
		vx = CREDWORM_SPEED;
		nx = 1;
	}
	else 
	{
		vx = -CREDWORM_SPEED;
		nx = -1;
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
		// clean up collision events
		for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
		if (x <= 0)
			if (vx < 0)
				vx = -vx;
	}
}

void CREDWORM::CalcPotentialCollisions(
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

void CREDWORM::Render()
{
	int ani = 0;

	switch (nx)
	{
	case 1:
		ani = CREDWORM_ANI_WALKING_RIGHT;
		break;
	case -1:
		ani = CREDWORM_ANI_WALKING_LEFT;
		break;
	}

	animation_set->at(ani)->Render(x, y);

	//RenderBoundingBox();
}

void CREDWORM::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{

	}

}
