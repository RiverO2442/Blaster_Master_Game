#include "Interrupt.h"
CINTERRUPT::CINTERRUPT()
{
	SetState(STATE_IDLE);
}

void CINTERRUPT::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + CINTERRUPT_BBOX_WIDTH;

	if (state == CINTERRUPT_STATE_DIE)
		bottom = y + CINTERRUPT_BBOX_HEIGHT_DIE;
	else
		bottom = y + CINTERRUPT_BBOX_HEIGHT;
}

void CINTERRUPT::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt, coObjects);

	//
	// TO-DO: make sure Goomba can interact with the world and to each of them too!
	// 

	x += dx;
	y += dy;

	float px, py;

	((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer()->GetPosition(px, py);

	if (this->x < px + TANK_BODY_BIG_BBOX_WIDTH && this->x + CINTERRUPT_BBOX_WIDTH >= px)
		SetState(CINTERRUPT_STATE_OPEN);
	else
		SetState(CINTERRUPT_ANI_IDLE);
}

void CINTERRUPT::Render()
{
	if (state != STATE_DIE)
	{
		int ani = CINTERRUPT_ANI_IDLE;
		switch (state)
		{
			case CINTERRUPT_STATE_OPEN:
				ani = CINTERRUPT_ANI_OPEN;
				break;
		}
		
		animation_set->at(ani)->Render(x, y);

		//RenderBoundingBox();
	}
}

void CINTERRUPT::SetState(int state)
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
