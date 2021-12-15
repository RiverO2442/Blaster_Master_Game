#pragma once
#pragma once
#include "GameObject.h"
#include "Game.h"
#include "PlayScene.h"
#include "algorithm"

#define CINTERRUPT_WALKING_SPEED 0.05f;

#define CINTERRUPT_BBOX_WIDTH 16
#define CINTERRUPT_BBOX_HEIGHT 15
#define CINTERRUPT_BBOX_HEIGHT_DIE 9

#define CINTERRUPT_STATE_IDLE 1000
#define CINTERRUPT_STATE_OPEN 1001

#define CINTERRUPT_ANI_IDLE 0
#define CINTERRUPT_ANI_OPEN 1

#define CINTERRUPT_STATE_IDLE 100
#define CINTERRUPT_STATE_DIE 200
#define CINTERRUPT_STATE_WALKING 300


class CINTERRUPT : public CGameObject
{
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void CalcPotentialCollisions(vector<LPGAMEOBJECT>* coObjects, vector<LPCOLLISIONEVENT>& coEvents);
	void FilterCollision(vector<LPCOLLISIONEVENT>& coEvents, vector<LPCOLLISIONEVENT>& coEventsResult, float& min_tx, float& min_ty, float& nx, float& ny, float& rdx, float& rdy);
	virtual void Render();

public:
	CINTERRUPT();
	virtual void SetState(int state);
};