#include "CBOOM.h"
#include "DF.h"



CBOOM::CBOOM()
{
	SetState(CBOOM_STATE_IDLE);
	SetPosition(STORING_LOCATION_X, STORING_LOCATION_Y);
}




void CBOOM::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + CREDWORM_BBOX_WIDTH;

	if (state == CREDWORM_STATE_DIE)
		y = y + CREDWORM_BBOX_HEIGHT;
	else bottom = y + CREDWORM_BBOX_HEIGHT;
}

void CBOOM::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt);
	CPlayScene* playscene = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene());

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;
	if (isUsed)
		if ((DWORD)GetTickCount64() - timing_start >= EXPLOSING_TIMING && timing_start != 0)
		{
			playscene->AddKaboomMng(x, y - CREDWORM_BBOX_HEIGHT/2);
			isUsed = false;
			SetPosition(STORING_LOCATION_X, STORING_LOCATION_X);
			timing_start = 0;
		}

	vy += CBOOM_GRAVITY * dt;



	if (!isUsed)
	if (((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->CheckBoomCarryMng())
	{
		this->SetPosition(playscene->GetBoomCarryMng()->getCEventPoisitionX(), playscene->GetBoomCarryMng()->getCEventPoisitionY());
		SetState(rand() % 4);
		playscene->CheckStackBoomCarryMng();
		isUsed = true;
		StartTiming();
	}

	if(isUsed)
	CalcPotentialCollisions(coObjects, coEvents);

	// No collision occured, proceed normally
	if (isUsed)
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
	if (isUsed)for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];


}

void CBOOM::Render()
{
	if (isUsed)
	{
		animation_set->at(0)->Render(x, y);
	}
	else return;


	//RenderBoundingBox();
}

void CBOOM::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case  CBOOM_STATE_IDLE:
		vx = 0;
		vy = 0;
		break;
	case CBOOM_STATE_TOP_RIGHT:
		vx = CBOOM_VX;
		vy = -CBOOM_VY;
		break;
	case CBOOM_STATE_TOP_LEFT:
		vx = -CBOOM_VX;
		vy = -CBOOM_VY;
		break;
	case CBOOM_STATE_BOTTOM_RIGHT:
		vx = -2 * CBOOM_VX;
		vy = -CBOOM_VY;
		break;
	case CBOOM_STATE_BOTTOM_LEFT:
		vx = 2*CBOOM_VX;
		vy = -CBOOM_VY;
		break;
	}
}

