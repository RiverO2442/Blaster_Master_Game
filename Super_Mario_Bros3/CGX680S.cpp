#include "CGX680S.h"
CGX680S::CGX680S()
{
	SetState(STATE_IDLE);
}

void CGX680S::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + CGX680S_BBOX_WIDTH;

	if (state == CGX680S_STATE_DIE)
		bottom = y + CGX680S_BBOX_HEIGHT_DIE;
	else
		bottom = y + CGX680S_BBOX_HEIGHT;
}

void CGX680S::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{

	CGameObject::Update(dt);
	CPlayScene* playscene = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene());

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	if (playscene->IsInside(x - 100, y - 100, x + 100, y + 100, playscene->GetPlayer2()->GetPositionX(), playscene->GetPlayer2()->GetPositionY()))
	{
		StartSwitch_state();
	}

	if ((DWORD)GetTickCount64() - switch_state && switch_state >= CGX680S_WALKING_TIME)
	{
		switch_state = (DWORD)GetTickCount64();
		StartSwitch_state();
		vx = (playscene->GetPlayer2()->GetPositionX() - x) / abs(playscene->GetPlayer2()->GetPositionX() - x) * CGX680S_WALKING_SPEED;
		vy = -(playscene->GetPlayer2()->GetPositionY() - y) / abs(playscene->GetPlayer2()->GetPositionY() - y) * CGX680S_WALKING_SPEED;
	}

	if (state != STATE_IDLE)
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
		}
	}

	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];

}

void CGX680S::Render()
{
	if (state != STATE_DIE)
	{
		int ani = CGX680S_ANI;

		animation_set->at(ani)->Render(x, y);

		//RenderBoundingBox();
	}
}

void CGX680S::SetState(int state)
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
