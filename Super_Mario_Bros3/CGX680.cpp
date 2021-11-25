#include "CGX680.h"
CGX680::CGX680()
{
	SetState(CGX680_STATE_IDLE);
}

void CGX680::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + CGX680_BBOX_WIDTH;

	if (state == CGX680_STATE_DIE)
		bottom = y + CGX680_BBOX_HEIGHT_DIE;
	else
		bottom = y + CGX680_BBOX_HEIGHT;
}

void CGX680::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt, coObjects);

	//
	// TO-DO: make sure Goomba can interact with the world and to each of them too!
	// 

	x += dx;
	y += dy;
}

void CGX680::Render()
{
	int ani = CGX680_ANI;

	animation_set->at(ani)->Render(x, y);

	//RenderBoundingBox();
}

void CGX680::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case CGX680_STATE_WALKING:
		vx = CGX680_WALKING_SPEED;
		break;

	}
}
