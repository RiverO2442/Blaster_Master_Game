#pragma once
#pragma once
#include "GameObject.h"
#include "PlayScene.h"
#include "algorithm"

#define CGX680S_WALKING_SPEED 0.02f;

#define CGX680S_BBOX_WIDTH 18
#define CGX680S_BBOX_HEIGHT 17
#define CGX680S_BBOX_HEIGHT_DIE 9

#define CGX680S_STATE_WALKING_RIGHT 1000
#define CGX680S_STATE_WALKING_LEFT 1001
#define CGX680S_STATE_WALKING_UP 1002
#define CGX680S_STATE_WALKING_DOWN 1003

#define CGX680S_ANI 0

#define CGX680S_STATE_IDLE 100
#define CGX680S_STATE_DIE 200
#define CGX680S_STATE_WALKING 300
#define CGX680S_WALKING_TIME 100
#define CGX680S_ATTACKING_TIME 3000


class CGX680S : public CGameObject
{
	DWORD switch_state = 0;
	DWORD attacking = 0;
	void StartSwitch_state()
	{
		if (switch_state == 0)
		{
			switch_state = (DWORD)GetTickCount64();
		}
	}
	void StartAttack()
	{
		if (attacking == 0)
		{
			attacking = (DWORD)GetTickCount64();
		}
	}
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();

public:
	CGX680S();
	virtual void SetState(int state);
};