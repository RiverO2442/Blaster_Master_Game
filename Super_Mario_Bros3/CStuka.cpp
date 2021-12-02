#include "CSTUKA.h"
CSTUKA::CSTUKA()
{
	SetState(STATE_IDLE);
}

void CSTUKA::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + CSTUKA_BBOX_WIDTH;

	if (state == CSTUKA_STATE_DIE)
		bottom = y + CSTUKA_BBOX_HEIGHT_DIE;
	else
		bottom = y + CSTUKA_BBOX_HEIGHT;
}

void CSTUKA::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt, coObjects);

	//
	// TO-DO: make sure Goomba can interact with the world and to each of them too!
	// 

	x += dx;
	y += dy;
}

void CSTUKA::Render()
{
	if (state != STATE_DIE)
	{
		int ani = CSTUKA_ANI;

		animation_set->at(ani)->Render(x, y);

		//RenderBoundingBox();
	}
}

void CSTUKA::SetState(int state)
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
