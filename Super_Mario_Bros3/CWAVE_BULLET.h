#pragma once
#include "GameObject.h"
#define CWAVE_BULLET_SPEED 0.9f
#define CWAVE_BULLET_STATE_DIE_SPEED 0

#define CWAVE_BULLET_BBOX_WIDTH	7
#define CWAVE_BULLET_BBOX_HEIGHT 9

#define	CWAVE_BULLET_STATE_DIE	90
#define CWAVE_BULLET_STATE_FLYING 100

#define CWAVE_BULLET_ANI_FLYING_RIGHT 0
#define CWAVE_BULLET_ANI_FLYING_LEFT 1

#define CWAVE_BULLET_GRAVITY	0.0008f
#define CWAVE_BULLET_RESET_TIME 2000
#define CWAVE_BULLET_CHANGE_SPEED_TIME 20

#define STORING_LOCATION 5000


class CWAVE_BULLET : public CGameObject
{
	DWORD reset_start = 0;
	DWORD change_speed_start = 0;
	void StartReset() {if(reset_start == 0) reset_start = (DWORD)GetTickCount64(); }
	void StartSpeed() { if (change_speed_start == 0) change_speed_start = (DWORD)GetTickCount64(); }
	bool isUsed = false;
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	void CalcPotentialCollisions(vector<LPGAMEOBJECT>* coObjects, vector<LPCOLLISIONEVENT>& coEvents);

public:
	CWAVE_BULLET();
	void SetisUsed(bool value) { isUsed = value; }
	bool GetisUsed() { return isUsed; }
	virtual void SetState(int state);
};

