#pragma once
#pragma once
#include "GameObject.h"
#include "algorithm"

#define CGX680S_WALKING_SPEED 0.05f;

#define CGX680S_BBOX_WIDTH 16
#define CGX680S_BBOX_HEIGHT 15
#define CGX680S_BBOX_HEIGHT_DIE 9

#define CGX680S_STATE_WALKING_RIGHT 1000
#define CGX680S_STATE_WALKING_LEFT 1001
#define CGX680S_STATE_WALKING_UP 1002
#define CGX680S_STATE_WALKING_DOWN 1003

#define CGX680S_ANI 0

#define CGX680S_STATE_IDLE 100
#define CGX680S_STATE_DIE 200
#define CGX680S_STATE_WALKING 300



class CGX680S : public CGameObject
{
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void CalcPotentialCollisions(vector<LPGAMEOBJECT>* coObjects, vector<LPCOLLISIONEVENT>& coEvents);
	void FilterCollision(vector<LPCOLLISIONEVENT>& coEvents, vector<LPCOLLISIONEVENT>& coEventsResult, float& min_tx, float& min_ty, float& nx, float& ny, float& rdx, float& rdy);
	virtual void Render();

public:
	CGX680S();
	virtual void SetState(int state);
};