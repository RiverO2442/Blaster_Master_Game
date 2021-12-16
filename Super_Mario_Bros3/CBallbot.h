#pragma once
#pragma once
#include "GameObject.h"
#include "algorithm"

#define CBALLBOT_FLYING_SPEED 0.05f;

#define CBALLBOT_BBOX_WIDTH 16
#define CBALLBOT_BBOX_HEIGHT 15
#define CBALLBOT_BBOX_HEIGHT_DIE 9

#define CBALLBOT_STATE_IDLE 1000
#define CBALLBOT_STATE_FALLING 1001
#define CBALLBOT_STATE_FLY_UP 1002

#define CBALLBOT_ANI_RIGHT 1
#define CBALLBOT_ANI_LEFT 0

#define CBALLBOT_STATE_DIE 200
#define CBALLBOT_STATE_WALKING 300

#define CBALLBOT_SWITCH_STATE_TIME 1000

#define CBALLBOT_X_RANGE 20

class CBALLBOT : public CGameObject
{
	bool triggered = false;
	int pre_ani;
	DWORD pre_tickcount;
	DWORD tickcount_diff = 0;
	DWORD switch_state = 0;
	void StartSwitch_state() 
	{
		if (switch_state == 0) 
		{
			pre_tickcount = (DWORD)GetTickCount64();
			switch_state = (DWORD)GetTickCount64();
		}
	}
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void CalcPotentialCollisions(vector<LPGAMEOBJECT>* coObjects, vector<LPCOLLISIONEVENT>& coEvents);
	virtual void Render();

public:
	CBALLBOT();
	virtual void SetState(int state);
};