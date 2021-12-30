#include "CSTUKA.h"
CSTUKA::CSTUKA()
{
	SetState(CSTUKA_STATE_WALKING);
	StartSwitch_state();
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
	

	CGameObject::Update(dt);
	CPlayScene* playscene = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene());

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	if(state != STATE_DIE)
	CalcPotentialCollisions(coObjects, coEvents);

	if (!spammed && state == STATE_DIE)
	{
		((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->AddKaboomMng(x, y);
		int chance = rand() % 100;
		srand(time(NULL));
		if (chance >= 70)
			playscene->AddItemsMng(x, y, 0);
		spammed = true;
	}

	if (state != STATE_DIE)
	{
		if (state != CSTUKA_STATE_ATTACK && playscene->IsInside(x - CSTUKA_BBOX_WIDTH, y, x, y + 200, playscene->GetPlayer()->GetPositionX(), playscene->GetPlayer()->GetPositionY()))
		{
			SetState(CSTUKA_STATE_ATTACK);
			vx = (playscene->GetPlayer()->GetPositionX() - x) / abs(playscene->GetPlayer()->GetPositionX() - x) * CSTUKA_WALKING_SPEED;
		}

		if (state == CSTUKA_STATE_WALKING)
		{
			switch_state_time = CSTUKA_WALKING_TIME;
		}
		else {
			switch_state_time = CSTUKA_ATTACK_TIME;
		}


		if ((DWORD)GetTickCount64() - switch_state > switch_state_time + tickcount_diff && switch_state != 0)
		{
			vx = -vx;
			pre_tickcount = 0;
			tickcount_diff = 0;
			switch_state = 0;
			StartSwitch_state();
		}
	}
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

		//if (nx != 0) vx = 0;
		//if (ny != 0) vy = 0;

		//
		// Collision logic with other objects
		//
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];
		}
	}
	CGame* game = CGame::GetInstance();
	if (playscene->IsInside(x - SOPHIA_BIG_BBOX_WIDTH, y - SOPHIA_BIG_BBOX_HEIGHT, x + CSTUKA_BBOX_WIDTH, y + CSTUKA_BBOX_HEIGHT, playscene->GetPlayer()->GetPositionX(), playscene->GetPlayer()->GetPositionY()) && !playscene->GetPlayer()->getUntouchable())
	{
		playscene->GetPlayer()->StartUntouchable();
		game->setheath(game->Getheath() - 100);
	}

	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];

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

void CSTUKA::CalcPotentialCollisions(
	vector<LPGAMEOBJECT>* coObjects,
	vector<LPCOLLISIONEVENT>& coEvents)
{
	CGame* game = CGame::GetInstance();
	CPlayScene* playscene = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene());

	vector <LPCOLLISIONEVENT> collisionEvents;

	for (UINT i = 0; i < coObjects->size(); i++)
	{
		LPCOLLISIONEVENT e = SweptAABBEx(coObjects->at(i));

		if (e->t > 0 && e->t <= 1.0f)
		{
			{
				 if (dynamic_cast<CBrick*>(e->obj) && state != CSTUKA_STATE_ATTACK)
					collisionEvents.push_back(e);
			}
		}
		else
			delete e;
	}
}

void CSTUKA::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case CSTUKA_STATE_WALKING:
		vx = CSTUKA_WALKING_SPEED;
		break;
	case CSTUKA_STATE_ATTACK:
		vy = -CSTUKA_WALKING_SPEED;
		break;
	case STATE_DIE:
			vy = -DIE_PULL;
			break;

	}
}
