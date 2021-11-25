#include "Eyelet.h"
CEYELET::CEYELET()
{
	SetState(EYELET_STATE_IDLE);
}

void CEYELET::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + EYELET_BBOX_WIDTH;

	if (state == EYELET_STATE_DIE)
		bottom = y + EYELET_BBOX_HEIGHT_DIE;
	else
		bottom = y + EYELET_BBOX_HEIGHT;
}

void CEYELET::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt, coObjects);

	//
	// TO-DO: make sure Goomba can interact with the world and to each of them too!
	// 

	x += dx;
	y += dy;
}

void CEYELET::Render()
{
	int ani = EYELET_ANI;

	animation_set->at(ani)->Render(x, y);

	//RenderBoundingBox();
}

void CEYELET::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case EYELET_STATE_WALKING:
		vx = EYELET_WALKING_SPEED;
		break;

	}
}
