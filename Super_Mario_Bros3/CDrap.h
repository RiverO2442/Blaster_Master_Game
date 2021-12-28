#pragma once
#pragma once
#include "GameObject.h"
#include "Game.h"
#include "PlayScene.h"
#include "algorithm"

#define CDRAP_WALKING_SPEED 0.2f;

#define CDRAP_BBOX_WIDTH 17
#define CDRAP_BBOX_HEIGHT 18
#define CDRAP_BBOX_HEIGHT_DIE 9

#define CDRAP_STATE_IDLE 1000
#define CDRAP_STATE_OPEN 1001

#define CDRAP_ANI_IDLE 0
#define CDRAP_ANI_OPEN 1

#define CDRAP_STATE_IDLE 100
#define CDRAP_STATE_DIE 200
#define CDRAP_STATE_WALKING 300
#define CDRAP_ATTACKING_TIME 5000

class CDRAP : public CGameObject
{
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	//void CalcPotentialCollisions(vector<LPGAMEOBJECT>* coObjects, vector<LPCOLLISIONEVENT>& coEvents);
	//void FilterCollision(vector<LPCOLLISIONEVENT>& coEvents, vector<LPCOLLISIONEVENT>& coEventsResult, float& min_tx, float& min_ty, float& nx, float& ny, float& rdx, float& rdy);
	virtual void Render();

public:
	DWORD attacking = 0;
	void StartAttack()
	{
		if (attacking == 0)
		{
			attacking = (DWORD)GetTickCount64();
		}
	}
	CDRAP();
	virtual void SetState(int state);
};