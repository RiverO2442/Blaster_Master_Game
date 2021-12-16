#pragma once
#include "GameObject.h"
#include "algorithm"

class CEvenType1 : public CGameObject
{
	float x, y;
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom) {};
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {};
	void CalcPotentialCollisions(vector<LPGAMEOBJECT>* coObjects, vector<LPCOLLISIONEVENT>& coEvents) {};
	void FilterCollision(vector<LPCOLLISIONEVENT>& coEvents, vector<LPCOLLISIONEVENT>& coEventsResult, float& min_tx, float& min_ty, float& nx, float& ny, float& rdx, float& rdy) {};
	virtual void Render() {};
public:

	CEvenType1(float x, float y)
	{
		this->x = x;
		this->y = y;
	}
	float getInterrupt_FiringPoisitionX()
	{
		return x;
	}
	float getInterrupt_FiringPoisitionY()
	{
		return y;
	}
};
