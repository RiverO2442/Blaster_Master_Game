#include "CBallBot.h"
CBallBot::CBallBot()
{
	SetState(BALLBOT_STATE_WALKING);
}

void CBallBot::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + BALLBOT_BBOX_WIDTH;

	if (state == BALLBOT_STATE_DIE)
		bottom = y + BALLBOT_BBOX_HEIGHT_DIE;
	else
		bottom = y + BALLBOT_BBOX_HEIGHT;
}

void CBallBot::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt, coObjects);

	//
	// TO-DO: make sure Goomba can interact with the world and to each of them too!
	// 

	x += dx;
	y += dy;
}

void CBallBot::Render()
{
	int ani = BALLBOT_ANI;

	animation_set->at(ani)->Render(x, y);

	//RenderBoundingBox();
}

void CBallBot::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{

	}
}
