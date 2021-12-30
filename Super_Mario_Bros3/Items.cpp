#include "Items.h"
#include <algorithm>
#include "PlayScene.h"
#include "Brick.h"

Items::Items(float num)
{
	//type = num;
}

void Items::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CPlayScene* playscene = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene());
	CGameObject::Update(dt, coObjects);
	//if (switch_state != 0)
	//{
	//	if ((DWORD)GetTickCount64() - switch_state >= duration)
	//	{
	//		active = false;
	//		switch_state = 0;
	//	}
	//}
	//if (state == STATE_DIE)
	//{
	//	active = false;
	//}

	if(!active)
	{
		x = STORING_LOCATION;
		y = STORING_LOCATION;
	}
	if (active == false)
	{
		if (((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->CheckItemsMng())
		{
			this->SetPosition(playscene->GetItemsMng()->getCEventPoisitionX(), playscene->GetItemsMng()->getCEventPoisitionY());
			type = playscene->GetItemsMng()->getCEventStack();
			playscene->DeleteItemsMng();
			active = true;
		}
	}
}

void Items::Render()
{
	if(active)
	animation_set->at(type)->Render(x, y);

	//RenderBoundingBox();
}
