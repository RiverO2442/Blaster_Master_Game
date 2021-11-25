#include "CDrap.h"
CDrap::CDrap()
{
	SetState(DRAP_STATE_WALKING);
}

void CDrap::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + DRAP_BBOX_WIDTH;

	if (state == DRAP_STATE_DIE)
		bottom = y + DRAP_BBOX_HEIGHT_DIE;
	else
		bottom = y + DRAP_BBOX_HEIGHT;
}

void CDrap::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt, coObjects);

	//
	// TO-DO: make sure Goomba can interact with the world and to each of them too!
	// 

	x += dx;
	y += dy;
}

void CDrap::Render()
{
	int ani = DRAP_ANI;

	animation_set->at(ani)->Render(x, y);

	//RenderBoundingBox();
}

void CDrap::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{

	}
}
