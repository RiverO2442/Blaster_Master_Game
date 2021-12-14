#pragma once
#include "GameObject.h"
#define CINTERRUPT_BULLET_SPEED 0.075f;
#define CINTERRUPT_BULLET_STATE_DIE_SPEED 0;

#define CINTERRUPT_BULLET_BBOX_WIDTH	7
#define CINTERRUPT_BULLET_BBOX_HEIGHT 9

#define	CINTERRUPT_BULLET_STATE_DIE	90
#define CINTERRUPT_BULLET_STATE_IDLE 100

#define CINTERRUPT_BULLET_ANI_IDLE 0

#define CINTERRUPT_BULLET_GRAVITY	0.0008f
#define CINTERRUPT_BULLET_RESET_TIME 5000

#define STORING_LOCATION 5000


class CINTERRUPT_BULLET : public CGameObject
{
	DWORD reset_start;
	bool isUsed = false;
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	void CalcPotentialCollisions(vector<LPGAMEOBJECT>* coObjects, vector<LPCOLLISIONEVENT>& coEvents);
	void StartReset() { reset_start = (DWORD)GetTickCount64(); }

public:
	CINTERRUPT_BULLET();
	void SetisUsed(bool value) { isUsed = value; }
	bool GetisUsed() { return isUsed; }
	virtual void SetState(int state);
};

