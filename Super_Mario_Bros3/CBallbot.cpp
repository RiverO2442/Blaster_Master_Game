#include "CBALLBOT.h"
CBALLBOT::CBALLBOT()
{
	SetState(STATE_IDLE);
}

void CBALLBOT::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + CBALLBOT_BBOX_WIDTH;

	if (state == CBALLBOT_STATE_DIE)
		bottom = y + CBALLBOT_BBOX_HEIGHT_DIE;
	else
		bottom = y + CBALLBOT_BBOX_HEIGHT;
}

void CBALLBOT::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt, coObjects);

	//
	// TO-DO: make sure Goomba can interact with the world and to each of them too!
	// 

	x += dx;
	y += dy;
}

void CBALLBOT::Render()
{
	if (state != STATE_DIE)
	{
		int ani = CBALLBOT_ANI;

		animation_set->at(ani)->Render(x, y);

		//RenderBoundingBox();
	}
}

void CBALLBOT::SetState(int state)
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
