#include "CBALLCARRY.h"

CBALLCARRY::CBALLCARRY()
{
	SetState(STATE_IDLE);
}

void CBALLCARRY::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + CBALLCARRY_BBOX_WIDTH;

	if (state == CBALLCARRY_STATE_DIE)
		bottom = y + CBALLCARRY_BBOX_HEIGHT_DIE;
	else
		bottom = y + CBALLCARRY_BBOX_HEIGHT;
}

void CBALLCARRY::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CPlayScene* playscene = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene());
	CGameObject::Update(dt, coObjects);

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	// Simple fall down
	if (state != CBALLCARRY_STATE_DIE)
		vy += CBALLCARRY_GRAVITY * dt;

	coEvents.clear();

	// turn off collision when die 
	if (state != CBALLCARRY_STATE_DIE)
		CalcPotentialCollisions(coObjects, coEvents);

	if (switch_state != 0 && isUsed)
	{
		if ((DWORD)GetTickCount64() - switch_state >= CBALLCARRY_STTACK_DURATION)
		{
			isActive = false;
			switch_state = 0;
		}
	}

	if (state == STATE_IDLE && playscene->IsInside(x - 400, y - CBALLCARRY_BBOX_HEIGHT, x + 100, y + CBALLCARRY_BBOX_HEIGHT, playscene->GetPlayer()->GetPositionX(), playscene->GetPlayer()->GetPositionY()) && !isUsed)
	{
		SetState(CBALLCARRY_STATE_WALKING);
		vx = -(playscene->GetPlayer()->GetPositionX() - x) / abs(playscene->GetPlayer()->GetPositionX() - x) * CBALLCARRY_WALKING_SPEED;
		playscene->AddBoomCarryMng(x + CBALLCARRY_BBOX_WIDTH / 2, y);
		isUsed = true;
		StartSwitch_state();
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
		//x += min_tx * dx + nx * 0.4f;
		//y += min_ty * dy + ny * 0.4f;

		//if (nx != 0) vx = 0;
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
	}
}

void CBALLCARRY::CalcPotentialCollisions(
	vector<LPGAMEOBJECT>* coObjects,
	vector<LPCOLLISIONEVENT>& coEvents)
{
	vector <LPCOLLISIONEVENT> collisionEvents;

	for (UINT i = 0; i < coObjects->size(); i++)
	{
		LPCOLLISIONEVENT e = SweptAABBEx(coObjects->at(i));

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

void CBALLCARRY::Render()
{
	int ani = 0;
	if (state != STATE_DIE)
	{
		if(vx == 0)
			ani = CBALLCARRY_ANI_IDLE;
		else {
			if (vx > 0)
				ani = CBALLCARRY_ANI_WALKING_RIGHT;
			else ani = CBALLCARRY_ANI_WALKING_LEFT;
		}
		if (isActive)
			ani = CBALLCARRY_ANI_ATTACK;

		animation_set->at(ani)->Render(x, y);

		//RenderBoundingBox();
	}
}

void CBALLCARRY::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case STATE_DIE:
			vy = DIE_PULL;
			break;
	}
}
