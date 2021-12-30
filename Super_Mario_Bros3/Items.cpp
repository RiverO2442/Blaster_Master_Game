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
	CGame* game = CGame::GetInstance();
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
	if (state == STATE_DIE)
	{
		active = false;
	}

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
			state = STATE_IDLE;
		}
	}
	else {
		if (playscene->GetPlayer() && playscene->IsInside(x - SOPHIA_BIG_BBOX_WIDTH, y - SOPHIA_BIG_BBOX_HEIGHT, x + ITEMS_BBOX_WIDTH, y + ITEMS_BBOX_HEIGHT, playscene->GetPlayer()->GetPositionX(), playscene->GetPlayer()->GetPositionY()))
		{
			if (getType() == 0)
			{
				game->setheath(game->Getheath() + 100);
			}
			else
			{
				game->setattack(game->Getattack() + 100);
			}
			SetState(STATE_DIE);
		}
		if (playscene->GetPlayer2() && playscene->IsInside(x - JASON_BIG_BBOX_WIDTH, y - JASON_BIG_BBOX_HEIGHT, x + ITEMS_BBOX_WIDTH, y + ITEMS_BBOX_HEIGHT, playscene->GetPlayer2()->GetPositionX(), playscene->GetPlayer2()->GetPositionY()))
		{
			if (getType() == 0)
			{
				game->setheath(game->Getheath() + 100);
			}
			else
			{
				game->setattack(game->Getattack() + 100);
			}
			SetState(STATE_DIE);
		}
		
	}
}

void Items::Render()
{
	if(active)
	animation_set->at(type)->Render(x, y);

	//RenderBoundingBox();
}
