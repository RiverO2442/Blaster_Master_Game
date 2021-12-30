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
	CGameObject::Update(dt);
	CPlayScene* playscene = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene());

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	if (!spammed && state == STATE_DIE)
	{
		((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->AddKaboomMng(x, y);
		int chance = rand() % 100;
		srand(time(NULL));
		if (chance >= 70)
			playscene->AddItemsMng(x, y, 0);
		spammed = true;
	}

	float px, py;

	if (state != STATE_DIE)
	{
		((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer()->GetPosition(px, py);
		if (state != CINTERRUPT_STATE_OPEN)
			if (this->x < px + SOPHIA_BIG_BBOX_WIDTH && this->x + CINTERRUPT_BBOX_WIDTH >= px && this->y < py)
			{
				SetState(CINTERRUPT_STATE_OPEN);
				playscene->AddInterruptBulletMng(this->x, this->y);
			}
	}

	if (state != STATE_DIE)
		CalcPotentialCollisions(coObjects, coEvents);

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

		// block every object first!
		//x += min_tx * dx + nx * 0.4f;
		//y += min_ty * dy + ny * 0.4f;

		if (nx != 0) vx = 0;
		if (ny != 0) vy = 0;

		//
		// Collision logic with other objects
		//
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];
			CGame* game = CGame::GetInstance();
			if (dynamic_cast<CSOPHIA*>(e->obj) && !playscene->GetPlayer()->getUntouchable())
			{
				playscene->GetPlayer()->StartUntouchable();
				game->setheath(game->Getheath() - 100);
			}
		}
	}

	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
	
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
