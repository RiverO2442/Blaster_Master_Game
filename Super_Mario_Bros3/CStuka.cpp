#include "CSTUKA.h"
#include "PlayScene.h"
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
	CGameObject::Update(dt, coObjects);
	CPlayScene* playscene = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene());

	//
	// TO-DO: make sure Goomba can interact with the world and to each of them too!
	//

	//if (pre_tickcount + 50 <= (DWORD)GetTickCount64() && switch_state != 0)
	//{
	//	tickcount_diff += (DWORD)GetTickCount64() - pre_tickcount;
	//}

	x += dx;
	y += dy;

	if (state != STATE_DIE)
	{
		if (state != CSTUKA_STATE_ATTACK && playscene->IsInside(x - CSTUKA_BBOX_WIDTH, y, x , y + 200, playscene->GetPlayer()->GetPositionX(), playscene->GetPlayer()->GetPositionY()))
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
	case CSTUKA_STATE_WALKING:
		vx = CSTUKA_WALKING_SPEED;
		break;
	case CSTUKA_STATE_ATTACK:
		vy = CSTUKA_WALKING_SPEED;
		break;
	case STATE_DIE:
			vy = DIE_PULL;
			break;

	}
}
