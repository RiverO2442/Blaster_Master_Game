#pragma once
#include "GameObject.h"
#include "algorithm"

class CEvenType1 : public CGameObject
{
	float x, y, stack = 0;
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom) {};
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {};
	void CalcPotentialCollisions(vector<LPGAMEOBJECT>* coObjects, vector<LPCOLLISIONEVENT>& coEvents) {};
	void FilterCollision(vector<LPCOLLISIONEVENT>& coEvents, vector<LPCOLLISIONEVENT>& coEventsResult, float& min_tx, float& min_ty, float& nx, float& ny, float& rdx, float& rdy) {};
	virtual void Render() {};
public:

	CEvenType1(float x, float y, float num = 0)
	{
		this->x = x;
		this->y = y;
		stack = num;
	}
	float getCEventPoisitionX()
	{
		return x;
	}
	float getCEventPoisitionY()
	{
		return y;
	}
	float getCEventStack()
	{
		return stack;
	}
	void setCEventStack(float num)
	{
		stack = num;
	}
};
