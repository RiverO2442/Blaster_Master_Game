#pragma once
#pragma once
#include "GameObject.h"
#include "algorithm"

#define CDRAP_WALKING_SPEED 0.05f;

#define CDRAP_BBOX_WIDTH 16
#define CDRAP_BBOX_HEIGHT 15
#define CDRAP_BBOX_HEIGHT_DIE 9

#define CDRAP_STATE_WALKING_RIGHT 1000
#define CDRAP_STATE_WALKING_LEFT 1001
#define CDRAP_STATE_WALKING_UP 1002
#define CDRAP_STATE_WALKING_DOWN 1003

#define CDRAP_ANI 0

#define CDRAP_STATE_IDLE 100
#define CDRAP_STATE_DIE 200
#define CDRAP_STATE_WALKING 300



class CDRAP : public CGameObject
{
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void CalcPotentialCollisions(vector<LPGAMEOBJECT>* coObjects, vector<LPCOLLISIONEVENT>& coEvents);
	void FilterCollision(vector<LPCOLLISIONEVENT>& coEvents, vector<LPCOLLISIONEVENT>& coEventsResult, float& min_tx, float& min_ty, float& nx, float& ny, float& rdx, float& rdy);
	virtual void Render();

public:
	CDRAP();
	virtual void SetState(int state);
};