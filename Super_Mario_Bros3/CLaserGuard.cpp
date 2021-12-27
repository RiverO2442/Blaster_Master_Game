#include "Interrupt.h"
CLASERGUARD::CLASERGUARD()
{
	SetState(STATE_IDLE);
}

void CLASERGUARD::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + CLASERGUARD_BBOX_WIDTH;

	if (state == CLASERGUARD_STATE_DIE)
		bottom = y + CLASERGUARD_BBOX_HEIGHT_DIE;
	else
		bottom = y + CLASERGUARD_BBOX_HEIGHT;
}

void CLASERGUARD::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CPlayScene* playscene = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene());
	CGameObject::Update(dt, coObjects);

	//
	// TO-DO: make sure Goomba can interact with the world and to each of them too!
	// 

	x += dx;
	y += dy;

	float px, py;

	((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer2()->GetPosition(px, py);
	if (state != CLASERGUARD_STATE_OPEN)
		if (this->x < px + SOPHIA_BIG_BBOX_WIDTH && this->x + CLASERGUARD_BBOX_WIDTH >= px && this->y < py)
		{
			playscene->AddInterruptBulletMng(this->x, this->y);
		}
}

void CLASERGUARD::Render()
{
	if (state != STATE_DIE)
	{
		int ani = CLASERGUARD_ANI_IDLE;

		animation_set->at(ani)->Render(x, y);

		//RenderBoundingBox();
	}
}

void CLASERGUARD::SetState(int state)
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
