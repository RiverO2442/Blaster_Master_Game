#pragma once
#include "GameObject.h"
#include "DF.h"

class TANKWHEEL : public CGameObject
{
	int part = 0;
	int pre_ani = 0;
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	void CalcPotentialCollisions(vector<LPGAMEOBJECT>* coObjects, vector<LPCOLLISIONEVENT>& coEvents);

public:
	float upBoudary;
	TANKWHEEL(int part);
	virtual void SetState(int state);
};

