#include "EFFECT.h"
#include <algorithm>
#include "PlayScene.h"
#include "Brick.h"

EFFECT::EFFECT(float time)
{
	duration = time;
}

void EFFECT::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CPlayScene* playscene = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene());
	CGameObject::Update(dt, coObjects);
	if (switch_state != 0)
	{
		if ((DWORD)GetTickCount64() - switch_state >= duration)
		{
			active = false;
			switch_state = 0;
		}
	}
	if(!active)
	{
		x = STORING_LOCATION;
		y = STORING_LOCATION;
	}
	if (active == false)
	{
		if (((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->CheckKaboomMng())
		{
			this->SetPosition(playscene->GetKaboomMng()->getCEventPoisitionX(), playscene->GetKaboomMng()->getCEventPoisitionY());
			playscene->DeleteKaboomMng();
			active = true;
			StartSwitch_state();
		}
	}
}

void EFFECT::Render()
{
	if(active)
	animation_set->at(0)->Render(x, y);

	//RenderBoundingBox();
}
