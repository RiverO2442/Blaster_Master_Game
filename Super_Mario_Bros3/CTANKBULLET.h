#pragma once
#include "GameObject.h"
#define CTANKBULLET_SPEED 0.15f
#define CTANKBULLET_STATE_DIE_SPEED 0

#define CTANKBULLET_BBOX_WIDTH	7
#define CTANKBULLET_BBOX_HEIGHT 9

#define	CTANKBULLET_STATE_DIE	90
#define CTANKBULLET_STATE_FLYING 100

#define CTANKBULLET_ANI_FLYING_RIGHT 0
#define CTANKBULLET_ANI_FLYING_LEFT 1

#define CTANKBULLET_GRAVITY	0.0008f
#define CTANKBULLET_RESET_TIME 5000

#define STORING_LOCATION 5000


class CTANKBULLET : public CGameObject
{
	DWORD reset_start;
	void StartReset() { reset_start = (DWORD)GetTickCount64(); }
	bool isUsed = false;
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	void CalcPotentialCollisions(vector<LPGAMEOBJECT>* coObjects, vector<LPCOLLISIONEVENT>& coEvents);

public:
	CTANKBULLET();
	void SetisUsed(bool value) { isUsed = value; }
	bool GetisUsed() { return isUsed; }
	virtual void SetState(int state);
};

