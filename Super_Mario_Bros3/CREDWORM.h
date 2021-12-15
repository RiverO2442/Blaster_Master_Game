#pragma once
#include "GameObject.h"
#define CREDWORM_SPEED 0.075f;

#define CREDWORM_BBOX_WIDTH	7
#define CREDWORM_BBOX_HEIGHT 9

#define	CREDWORM_STATE_DIE	200
#define CREDWORM_STATE_WALKING 100

#define CREDWORM_ANI_WALKING_RIGHT 1
#define CREDWORM_ANI_WALKING_LEFT 0

#define CREDWORM_GRAVITY	0.0008f
#define CREDWORM_RESET_TIME 5000

#define STORING_LOCATION 5000


class CREDWORM : public CGameObject
{
	DWORD reset_start;
	bool isUsed = false;
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	void CalcPotentialCollisions(vector<LPGAMEOBJECT>* coObjects, vector<LPCOLLISIONEVENT>& coEvents);
	void StartReset() { reset_start = (DWORD)GetTickCount64(); }

public:
	CREDWORM();
	void SetisUsed(bool value) { isUsed = value; }
	bool GetisUsed() { return isUsed; }
	virtual void SetState(int state);
};

