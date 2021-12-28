#pragma once
#pragma once
#include "GameObject.h"
#include "algorithm"
#include "PlayScene.h"

#define CBALLCARRY_WALKING_SPEED 0.1f;
#define CBALLCARRY_GRAVITY		0.002f

#define CBALLCARRY_BBOX_WIDTH 18
#define CBALLCARRY_BBOX_HEIGHT 17
#define CBALLCARRY_BBOX_HEIGHT_DIE 9

#define CBALLCARRY_STATE_WALKING_RIGHT 1000
#define CBALLCARRY_STATE_WALKING_LEFT 1001
#define CBALLCARRY_STATE_WALKING_UP 1002
#define CBALLCARRY_STATE_WALKING_DOWN 1003

#define CBALLCARRY_ANI_IDLE 0
#define CBALLCARRY_ANI_WALKING_LEFT 1
#define CBALLCARRY_ANI_WALKING_RIGHT 2
#define CBALLCARRY_ANI_ATTACK 3

#define CBALLCARRY_STATE_DIE 200
#define CBALLCARRY_STATE_WALKING 300

#define CBALLCARRY_STTACK_DURATION 200


class CBALLCARRY : public CGameObject
{
	bool isActive = false;
	bool isUsed = false;
	DWORD switch_state = 0;
	void StartSwitch_state()
	{
		if (switch_state == 0)
		{
			switch_state = (DWORD)GetTickCount64();
			isActive = true;
		}
	}
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void CalcPotentialCollisions(vector<LPGAMEOBJECT>* coObjects, vector<LPCOLLISIONEVENT>& coEvents);
	//void FilterCollision(vector<LPCOLLISIONEVENT>& coEvents, vector<LPCOLLISIONEVENT>& coEventsResult, float& min_tx, float& min_ty, float& nx, float& ny, float& rdx, float& rdy);
	virtual void Render();

public:
	CBALLCARRY();
	virtual void SetState(int state);
};