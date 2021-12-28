#include "CLaserGuard.h"
CLASERGUARD::CLASERGUARD()
{
	SetState(STATE_IDLE);
}

void CLASERGUARD::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	if (state != CLASERGUARD_STATE_DIE) {
		left = x;
		top = y;
		right = x + CLASERGUARD_BBOX_WIDTH;
		bottom = y + CLASERGUARD_BBOX_HEIGHT;
	}
}

void CLASERGUARD::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt);
	CPlayScene* playscene = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene());

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;



	if (attacking != 0 && (DWORD)GetTickCount64() - attacking >= CLASERGUARD_ATTACKING_TIME)
	{
		attacking = 0;
	}

	float px, py;

	((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer2()->GetPosition(px, py);

	if (playscene->IsInside(x, y, x + CLASERGUARD_BBOX_WIDTH, y + 500, px + JASON_BIG_BBOX_WIDTH/2, py) && attacking == 0)
	{
		playscene->AddInterruptBulletMng(this->x, this->y);
		StartAttack();
	}

		CalcPotentialCollisions(coObjects, coEvents);

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

		// block every object first!
		x += min_tx * dx + nx * 0.4f;
		y += min_ty * dy + ny * 0.4f;

		//if (nx != 0) vx = 0;
		//if (ny != 0) vy = 0;

		//
		// Collision logic with other objects
		//
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];
			if (dynamic_cast<CBrick*>(e->obj))
			{
				if(ny == 0 && nx != 0)
				vx = -vx;
			}
		}
	}

	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
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
		vx = CLASERGUARD_WALKING_SPEED;
		vy = 0;
		break;
	case STATE_DIE:
		vy = DIE_PULL;
		break;
	}
}
