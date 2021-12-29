#include "CTANKWHEELS.h"
#include <algorithm>
#include "PlayScene.h"


TANKWHEEL::TANKWHEEL(int part)
{
	this->part = part;
}

void TANKWHEEL::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
}

void TANKWHEEL::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	
	CGameObject::Update(dt, coObjects);

	CSOPHIA* SOPHIA = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer();

	switch(part)
	{
	case TANKWHEEL_LEFT_WHEEL:
			x = SOPHIA->x + TANKWHEEL_WHEEL_DISTANT_X_1;
			if (SOPHIA->GetisAimingUp())
			{
				if (nx > 0)
				{
					x = x + 4;
				}
			}
			break;
	case TANKWHEEL_RIGHT_WHEEL:
			x = SOPHIA->x + TANKWHEEL_WHEEL_DISTANT_X_2;
			if (SOPHIA->GetisAimingUp())
			{
				if (nx > 0)
				{
					x = x - 2;
				}
			}
			break;
			break;
	}

	y = SOPHIA->y + TANKWHEEL_WHEEL_DISTANT_Y;

	float Py, Px;
	SOPHIA->GetSpeed(Px, Py);

	if (SOPHIA->GetIsJumping() && Py > 0)
	{
		y = y + 6;
	}
	
		x += dx;
		y += dy;

}


void TANKWHEEL::Render()
{
	CSOPHIA* SOPHIA = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer();

	int ani = 0;

	if (SOPHIA->GetState() == SOPHIA_STATE_WALKING_RIGHT)
	{
		switch (part)
		{
		case TANKWHEEL_RIGHT_WHEEL:
			ani = WHEELING_ANI_RIGHT;
			break;
		case TANKWHEEL_LEFT_WHEEL:
			ani = WHEELING_ANI_RIGHT;
			break;
		}
	}

	else if (SOPHIA->GetState() == SOPHIA_STATE_WALKING_LEFT)
	{
		switch (part)
		{
		case TANKWHEEL_RIGHT_WHEEL:
			ani = WHEELING_ANI_LEFT;
			break;
		case TANKWHEEL_LEFT_WHEEL:
			ani = WHEELING_ANI_LEFT;
			break;
		}
	}

	else if(SOPHIA->vx == 0)
	{
		if (part != TANKWHEEL_TURRET)
			ani = WHEELING_ANI_IDLE;
		else
			ani = pre_ani;
	}

	animation_set->at(ani)->Render(x, y);

	//RenderBoundingBox();
}

void TANKWHEEL::SetState(int state)
{
	CGameObject::SetState(state);
	//switch (state)
	//{
	//case TANKWHEEL_STATE_DIE:
	//	vx = FIRE_BALL_STATE_DIE_VX;
	//	vy = FIRE_BALLSTATE_DIE_VY;
	//	break;

	//}

}
