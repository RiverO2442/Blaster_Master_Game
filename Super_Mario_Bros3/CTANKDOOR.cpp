#include "CTANKDOOR.h"
#include <algorithm>
#include "PlayScene.h"
#include "DF.h"


CTANKDOOR::CTANKDOOR()
{
}

void CTANKDOOR::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
}

void CTANKDOOR::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{

	CGameObject::Update(dt, coObjects);

	CSOPHIA* SOPHIA = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer();

	if (pre_ani == 0)
	{
		x = SOPHIA->x + 10;
		y = SOPHIA->y - 8;
	}
	else {
		x = SOPHIA->x;
		y = SOPHIA->y - 8;
	}

	x += dx;
	y += dy;

}


void CTANKDOOR::Render()
{
	CPlayScene* playscene = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene());
	CSOPHIA* SOPHIA = playscene->GetPlayer();
	


		int ani = 1;
		float vx, vy;
		SOPHIA->GetSpeed(vx, vy);
		if (vx > 0)
		{
			ani = 1;
			pre_ani = ani;
		}
		else if (vx < 0)
		{
			ani = 0;
			pre_ani = ani;
		}
		else {
			ani = pre_ani;
		}
	if (!playscene->getpiloting())
	{
		//ani = CTANKDOOR_ANI_IDLE;

		animation_set->at(ani)->Render(x, y);

		//RenderBoundingBox();
	}
	
}

void CTANKDOOR::SetState(int state)
{
	CGameObject::SetState(state);
	//switch (state)
	//{
	//case CTANKDOOR_STATE_DIE:
	//	vx = FIRE_BALL_STATE_DIE_VX;
	//	vy = FIRE_BALLSTATE_DIE_VY;
	//	break;

	//}

}
