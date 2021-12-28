#include "CLaserGuard.h"
CDRAP::CDRAP()
{
	SetState(STATE_IDLE);
}

void CDRAP::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	if (state != CDRAP_STATE_DIE) {
		left = x;
		top = y;
		right = x + CDRAP_BBOX_WIDTH;
		bottom = y + CDRAP_BBOX_HEIGHT;
	}
}

void CDRAP::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt);
	CPlayScene* playscene = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene());

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;



	if (attacking != 0 && (DWORD)GetTickCount64() - attacking >= CDRAP_ATTACKING_TIME)
	{
		attacking = 0;
	}

	float px, py;

	((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer2()->GetPosition(px, py);

	if (playscene->IsInside(x - 500, y, x + 500, y + CDRAP_BBOX_HEIGHT, px , py + JASON_BIG_BBOX_HEIGHT/2) && attacking == 0)
	{
		StartAttack();
		if (px > x)
			vx = CDRAP_WALKING_SPEED
		else
		{
			vx = -CDRAP_WALKING_SPEED;
		}
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
				if (ny == 0 && nx != 0)
					vx = 0;
			}
		}
	}

	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void CDRAP::Render()
{
	if (state != STATE_DIE)
	{
		int ani = CDRAP_ANI_IDLE;

		animation_set->at(ani)->Render(x, y);

		//RenderBoundingBox();
	}
}

void CDRAP::SetState(int state)
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
