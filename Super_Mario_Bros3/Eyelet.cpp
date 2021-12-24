#include "Eyelet.h"
#include "PlayScene.h"
CEYELET::CEYELET(float kill_point)
{
	this->kill_point = kill_point;
	SetState(STATE_IDLE);
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
	CPlayScene* playscene = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene());
	//
	// TO-DO: make sure Goomba can interact with the world and to each of them too!
	// 

	if (state != EYELET_STATE_ATTACK && playscene->IsInside(x, y, x + kill_point, y + 50, playscene->GetPlayer()->GetPositionX(), playscene->GetPlayer()->GetPositionY()) && kill_point >= 0)
	{
		moving_limit_bottom = this->y + 10;
		moving_limit_top = this->y - MOVING_LIMIT_RANGE;
		SetState(EYELET_STATE_ATTACK);
	}
	else
	if (state != EYELET_STATE_ATTACK && playscene->IsInside(x + kill_point, y, x, y + 50, playscene->GetPlayer()->GetPositionX(), playscene->GetPlayer()->GetPositionY()) && kill_point < 0)
	{
		moving_limit_bottom = this->y + 10;
		moving_limit_top = this->y - MOVING_LIMIT_RANGE;
		SetState(EYELET_STATE_ATTACK);
	}
	if (state == EYELET_STATE_ATTACK)
	{
		if (this->y <= moving_limit_top)
		{
			this->vy = -EYELET_WALKING_SPEED;
		}
		if (this->y >= moving_limit_bottom)
		{
			this->vy = EYELET_WALKING_SPEED;
		}
	}

	x += dx;
	y += dy;
}

void CEYELET::Render()
{
	if (state == EYELET_STATE_ATTACK)
	{
		int ani = 0;
		if (vx < 0)
			ani = EYELET_ANI_LEFT;
		else
			ani = EYELET_ANI_RIGHT;

		animation_set->at(ani)->Render(x, y);

		//RenderBoundingBox();
	}
}

void CEYELET::CalcPotentialCollisions(
	vector<LPGAMEOBJECT>* coObjects,
	vector<LPCOLLISIONEVENT>& coEvents)
{
	for (UINT i = 0; i < coObjects->size(); i++)
	{
		LPCOLLISIONEVENT e = SweptAABBEx(coObjects->at(i));
		continue;
		if (e->t > 0 && e->t <= 1.0f)
			coEvents.push_back(e);
		else
			delete e;
	}
	std::sort(coEvents.begin(), coEvents.end(), CCollisionEvent::compare);
}

void CEYELET::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case EYELET_STATE_ATTACK:
		vx = kill_point / abs(kill_point) * EYELET_WALKING_SPEED;
		vy = -EYELET_WALKING_SPEED;
		break;
	case STATE_IDLE:
		vx = 0;
		vy = 0;
		break;
	case STATE_DIE:
		vy = -DIE_PULL;
		break;

	}
}
