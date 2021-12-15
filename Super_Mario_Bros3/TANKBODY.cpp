#include "TANKBODY.h"
#include <algorithm>
#include "PlayScene.h"
#include "DF.h"


TANKBODY::TANKBODY()
{
}

void TANKBODY::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
}

void TANKBODY::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{

	CGameObject::Update(dt, coObjects);

	CSOPHIA* SOPHIA = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer();

	x = SOPHIA->x + BODY_DISTANT_X;

	y = SOPHIA->y + BODY_DISTANT_Y;

	x += dx;
	y += dy;

}


void TANKBODY::Render()
{
	CSOPHIA* SOPHIA = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer();

	int ani = 0;

	ani = TANKBODY_ANI_IDLE;

	animation_set->at(ani)->Render(x, y);

	//RenderBoundingBox();
}

void TANKBODY::SetState(int state)
{
	CGameObject::SetState(state);
	//switch (state)
	//{
	//case TANKBODY_STATE_DIE:
	//	vx = FIRE_BALL_STATE_DIE_VX;
	//	vy = FIRE_BALLSTATE_DIE_VY;
	//	break;

	//}

}
