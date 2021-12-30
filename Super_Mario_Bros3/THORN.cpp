#include "THORN.h"
#include "PlayScene.h"

void THORN::Render()
{
	if (state != STATE_DIE)
	{
		animation_set->at(1)->Render(x, y);
		//RenderBoundingBox();
	}
	else {
		animation_set->at(0)->Render(x, y);
	}
}

void THORN::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt);
	CPlayScene* playscene = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene());
	CGame* game = CGame::GetInstance();

	if (reset_start != 0 && (DWORD)GetTickCount64() - reset_start >= 2000)
	{
		reset_start = 0;
	}

	if(reset_start == 0)
	if (playscene->IsInside(x - JASON_BIG_BBOX_WIDTH, y - JASON_BIG_BBOX_HEIGHT, x + THORN_BRICK_BBOX_WIDTH, y + THORN_BRICK_BBOX_HEIGHT, playscene->GetPlayer2()->GetPositionX(), playscene->GetPlayer2()->GetPositionY()) && !playscene->GetPlayer2()->getUntouchable())
	{
		playscene->GetPlayer2()->StartUntouchable();
		game->setheath(game->Getheath() - 100);
	}
}

void THORN::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	/*if (state != STATE_DIE)
	{
		l = x;
		t = y;
		r = x + THORN_BRICK_BBOX_WIDTH;
		b = y + THORN_BRICK_BBOX_HEIGHT;
	}*/
}