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
	CGameObject::Update(dt, coObjects);

	//
	// TO-DO: make sure Goomba can interact with the world and to each of them too!
	//


	x += dx;
	y += dy;
}

void CBALLCARRY::Render()
{
	if (state != STATE_DIE)
	{
		int ani = CBALLCARRY_ANI;

		animation_set->at(ani)->Render(x, y);

		//RenderBoundingBox();
	}
}

void CBALLCARRY::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case STATE_IDLE:
		vx = 0;
		vy = 0;
		break;
	case STATE_DIE:
			vy = DIE_PULL;
			break;
	}
}
