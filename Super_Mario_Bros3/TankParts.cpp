#include "TankParts.h"
#include "TankParts.h"
#include <algorithm>
#include "PlayScene.h"


TankParts::TankParts(int part)
{
	this->part = part;
}

void TankParts::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
}

void TankParts::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	
	CGameObject::Update(dt, coObjects);

	CTANK_BODY* TANK_BODY = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer();

	switch(part)
	{
	case TankParts_LEFT_WHEEL:
			x = TANK_BODY->x - TankParts_WHEEL_DISTANT_X;
			break;
	case TankParts_RIGHT_WHEEL:
			x = TANK_BODY->x + TankParts_WHEEL_DISTANT_X;
			break;
	case TankParts_TURRET:
			x = TANK_BODY->x - TankParts_TURRET_DISTANT_X;
			break;
	}
	if(part == TankParts_TURRET)
		y = TANK_BODY->y - TankParts_TURRET_DISTANT_Y;
	else
		y = TANK_BODY->y + TankParts_WHEEL_DISTANT_Y;
	
		x += dx;
		y += dy;

}


void TankParts::Render()
{
	CTANK_BODY* TANK_BODY = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer();

	int ani = 0;

	if (TANK_BODY->vx > 0)
	{
		switch (part)
		{
		case TankParts_TURRET:
			ani = TURRET_ANI_IDLE_RIGHT;
			pre_ani = ani;
			break;
		case TankParts_RIGHT_WHEEL:
			ani = WHEELING_ANI_RIGHT;
			break;
		case TankParts_LEFT_WHEEL:
			ani = WHEELING_ANI_RIGHT;
			break;
		}
	}

	else if (TANK_BODY->vx < 0)
	{
		switch (part)
		{
		case TankParts_TURRET:
			ani = TURRET_ANI_IDLE_LEFT;
			pre_ani = ani;
			break;
		case TankParts_RIGHT_WHEEL:
			ani = WHEELING_ANI_LEFT;
			break;
		case TankParts_LEFT_WHEEL:
			ani = WHEELING_ANI_LEFT;
			break;
		}
	}

	else if(TANK_BODY->vx == 0)
	{
		if (part != TankParts_TURRET)
			ani = WHEELING_ANI_IDLE;
		else
			ani = pre_ani;
	}

	animation_set->at(ani)->Render(x, y);

	//RenderBoundingBox();
}

void TankParts::SetState(int state)
{
	CGameObject::SetState(state);
	//switch (state)
	//{
	//case TankParts_STATE_DIE:
	//	vx = FIRE_BALL_STATE_DIE_VX;
	//	vy = FIRE_BALLSTATE_DIE_VY;
	//	break;

	//}

}
