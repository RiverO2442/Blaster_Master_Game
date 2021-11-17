#include "TankParts.h"
#include "TankParts.h"
#include <algorithm>
#include "PlayScene.h"


TankParts::TankParts(int part)
{
	this->part = part;
}

void TankParts::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + TankParts_BBOX_WIDTH;
	if (state == TankParts_STATE_DIE)
		y = y + TankParts_BBOX_HEIGHT;
	else bottom = y + TankParts_BBOX_HEIGHT;
}

void TankParts::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	
	CGameObject::Update(dt, coObjects);
	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	// turn off collision when die 
	if (state != TankParts_STATE_DIE)
		CalcPotentialCollisions(coObjects, coEvents);

	CMario* mario = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer();

	switch(part)
	{
	case TankParts_LEFT_WHEEL:
			x = mario->x - TankParts_WHEEL_DISTANT_X;
			break;
	case TankParts_RIGHT_WHEEL:
			x = mario->x + TankParts_WHEEL_DISTANT_X;
			break;
	case TankParts_TURRET:
		x = mario->x - TankParts_TURRET_DISTANT_X;
		break;
	}
	if(part == TankParts_TURRET)
		y = mario->y - TankParts_TURRET_DISTANT_Y;
	else
		y = mario->y + TankParts_WHEEL_DISTANT_Y;
	
	// No collision occured, proceed normally
	if (coEvents.size() == 0)
	{
		x += dx;
		y += dy;
	}
	else
	{
		float min_tx, min_ty, nx = 0, ny;
		float rdx = 0;
		float rdy = 0;

		// TODO: This is a very ugly designed function!!!!
		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);

		// how to push back Mario if collides with a moving objects, what if Mario is pushed this way into another object?
		//if (rdx != 0 && rdx!=dx)
		//	x += nx*abs(rdx); 

		// block every object first!
		//x += min_tx * dx + nx * 0.4f;

		//if(nx != 0 && ny != 0)
		//y += min_ty * dy + ny * 0.4f;

		//if (ny != 0) vy = 0;

		//
		// Collision logic with other objects
		//

		for (UINT i = 0; i < coEventsResult.size(); i++)
		{

		}
		// clean up collision events
		for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
	}
}

void TankParts::CalcPotentialCollisions(
	vector<LPGAMEOBJECT>* coObjects,
	vector<LPCOLLISIONEVENT>& coEvents)
{
	for (UINT i = 0; i < coObjects->size(); i++)
	{
		LPCOLLISIONEVENT e = SweptAABBEx(coObjects->at(i));
		if (dynamic_cast<CMario*>(e->obj))
		{
			continue;
		}
		if (e->t > 0 && e->t <= 1.0f)
			coEvents.push_back(e);
		else
			delete e;
	}
	std::sort(coEvents.begin(), coEvents.end(), CCollisionEvent::compare);
}

void TankParts::Render()
{
	CMario* mario = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	int ani = 0;
	if (mario->vx > 0)
	{
		switch (part)
		{
		case TankParts_TURRET:
			ani = TURRET_ANI_IDLE_RIGHT;
			pre_ani = ani;
			break;
		case TankParts_RIGHT_WHEEL:
			ani = WHEELING_ANI_RIGHT;
			break;
		case TankParts_LEFT_WHEEL:
			ani = WHEELING_ANI_RIGHT;
			break;
		}
	}
	else if (mario->vx < 0)
	{
		switch (part)
		{
		case TankParts_TURRET:
			ani = TURRET_ANI_IDLE_LEFT;
			pre_ani = ani;
			break;
		case TankParts_RIGHT_WHEEL:
			ani = WHEELING_ANI_LEFT;
			break;
		case TankParts_LEFT_WHEEL:
			ani = WHEELING_ANI_LEFT;
			break;
		}
	}
	else if(mario->vx == 0)
	{
		if (part != TankParts_TURRET)
			ani = WHEELING_ANI_IDLE;
		else
			ani = pre_ani;
	}

	animation_set->at(ani)->Render(x, y);

	//RenderBoundingBox();
}

void TankParts::SetState(int state)
{
	CGameObject::SetState(state);
	//switch (state)
	//{
	//case TankParts_STATE_DIE:
	//	vx = FIRE_BALL_STATE_DIE_VX;
	//	vy = FIRE_BALLSTATE_DIE_VY;
	//	break;

	//}

}
