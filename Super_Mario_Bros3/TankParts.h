#pragma once
#include "GameObject.h"

#define TankParts_STATE_DIE 0
#define TankParts_BBOX_WIDTH 10
#define TankParts_BBOX_HEIGHT 10

#define TankParts_WHEEL_DISTANT_X 9
#define TankParts_WHEEL_DISTANT_Y 0
#define TankParts_TURRET_DISTANT_X 9
#define TankParts_TURRET_DISTANT_Y 10

#define TankParts_LEFT_WHEEL 0
#define TankParts_RIGHT_WHEEL 1
#define TankParts_TURRET 2

#define WHEELING_ANI_RIGHT 0
#define WHEELING_ANI_LEFT 1
#define WHEELING_ANI_IDLE 2

#define TURRET_ANI_IDLE_LEFT 1
#define TURRET_ANI_IDLE_RIGHT 0






class TankParts : public CGameObject
{
	int part = 0;
	int pre_ani = 0;
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	void CalcPotentialCollisions(vector<LPGAMEOBJECT>* coObjects, vector<LPCOLLISIONEVENT>& coEvents);

public:
	float upBoudary;
	TankParts(int part);
	virtual void SetState(int state);
};

