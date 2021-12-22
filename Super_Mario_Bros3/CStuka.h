#pragma once
#pragma once
#include "GameObject.h"
#include "algorithm"
#include "PlayScene.h"

#define CSTUKA_WALKING_SPEED 0.07f
#define CSTUKA_WALKING_DOWN_SPEED 0.05f

#define CSTUKA_BBOX_WIDTH 16
#define CSTUKA_BBOX_HEIGHT 15
#define CSTUKA_BBOX_HEIGHT_DIE 9

#define CSTUKA_STATE_WALKING_RIGHT 1000
#define CSTUKA_STATE_WALKING_LEFT 1001
#define CSTUKA_STATE_WALKING_UP 1002
#define CSTUKA_STATE_WALKING_DOWN 1003

#define CSTUKA_ANI 0

#define CSTUKA_STATE_IDLE 100
#define CSTUKA_STATE_DIE 200
#define CSTUKA_STATE_WALKING 300
#define CSTUKA_STATE_ATTACK 400


#define CSTUKA_WALKING_TIME 2000
#define CSTUKA_ATTACK_TIME 800


class CSTUKA : public CGameObject
{
	int pre_ani;
	DWORD pre_tickcount;
	DWORD tickcount_diff = 0;
	DWORD switch_state = 0;
	DWORD switch_state_time = 0;
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
	CSTUKA();
	virtual void SetState(int state);
};