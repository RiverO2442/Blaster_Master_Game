#include "TANKTURRET.h"
#include <algorithm>
#include "PlayScene.h"
#include "DF.h"


TANKTURRET::TANKTURRET()
{
}

void TANKTURRET::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
}

void TANKTURRET::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{

	CGameObject::Update(dt, coObjects);

	CSOPHIA* SOPHIA = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer();

	x = SOPHIA->x + TURRET_DISTANT_X;

	y = SOPHIA->y + TURRET_DISTANT_Y;

	if (SOPHIA->GetisAimingUp())
		y = y - 16;

	if (SOPHIA->GetNx() < 0 && SOPHIA->GetisAimingUp())
	{
		x = x + 4;
	}

	x += dx;
	y += dy;

}


void TANKTURRET::Render()
{
	CSOPHIA* SOPHIA = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer();

	int ani = pre_ani;

	if (SOPHIA->vx > 0)
	{
		ani = TURRET_ANI_IDLE_RIGHT;
		pre_ani = ani;
	}
	else if (SOPHIA->vx < 0) {
		ani = TURRET_ANI_IDLE_LEFT;
		pre_ani = ani;
	}
	else ani = pre_ani;
	
	if (SOPHIA->GetisAimingUp())
		ani = ani + 2;

	animation_set->at(ani)->Render(x, y);

	//RenderBoundingBox();
}

void TANKTURRET::SetState(int state)
{
	CGameObject::SetState(state);
	//switch (state)
	//{
	//case TANKTURRET_STATE_DIE:
	//	vx = FIRE_BALL_STATE_DIE_VX;
	//	vy = FIRE_BALLSTATE_DIE_VY;
	//	break;

	//}

}
