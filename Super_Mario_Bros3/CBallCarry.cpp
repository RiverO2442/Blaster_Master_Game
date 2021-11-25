#include "CBallCarry.h"
CBallCarry::CBallCarry()
{
	SetState(BALLCARRY_STATE_WALKING);
}

void CBallCarry::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + BALLCARRY_BBOX_WIDTH;

	if (state == BALLCARRY_STATE_DIE)
		bottom = y + BALLCARRY_BBOX_HEIGHT_DIE;
	else
		bottom = y + BALLCARRY_BBOX_HEIGHT;
}

void CBallCarry::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt, coObjects);

	//
	// TO-DO: make sure Goomba can interact with the world and to each of them too!
	// 

	x += dx;
	y += dy;
}

void CBallCarry::Render()
{
	int ani = BALLCARRY_ANI;

	animation_set->at(ani)->Render(x, y);

	//RenderBoundingBox();
}

void CBallCarry::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{

	}
}
