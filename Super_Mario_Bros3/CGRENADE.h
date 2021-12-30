#pragma once
#include "GameObject.h"
#define CGRENADE_SPEED 0.2f
#define CGRENADE_STATE_DIE_SPEED 0

#define CGRENADE_BBOX_WIDTH	7
#define CGRENADE_BBOX_HEIGHT 9

#define	CGRENADE_STATE_DIE	90
#define CGRENADE_STATE_FLYING 100

#define CGRENADE_ANI_FLYING_RIGHT 0
#define CGRENADE_ANI_FLYING_LEFT 1

#define CGRENADE_GRAVITY	0.0008f
#define CGRENADE_RESET_TIME 300
#define CGRENADE_CHANGE_SPEED_TIME 20

#define STORING_LOCATION 5000


class CGRENADE : public CGameObject
{
	int dir = 0;
	DWORD reset_start = 0;
	DWORD change_speed_start = 0;
	void StartReset() { if (reset_start == 0) reset_start = (DWORD)GetTickCount64(); }
	void StartSpeed() { if (change_speed_start == 0) change_speed_start = (DWORD)GetTickCount64(); }
	bool isUsed = false;
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	void CalcPotentialCollisions(vector<LPGAMEOBJECT>* coObjects, vector<LPCOLLISIONEVENT>& coEvents);

public:
	CGRENADE();
	void SetisUsed(bool value) { isUsed = value; }
	bool GetisUsed() { return isUsed; }
	virtual void SetState(int state);
};

