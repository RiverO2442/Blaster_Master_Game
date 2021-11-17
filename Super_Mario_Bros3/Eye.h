#pragma once
#pragma once
#include "GameObject.h"
#include "Brick.h"
#include "algorithm"

#define EYE_WALKING_SPEED 0.05f;

#define EYE_BBOX_WIDTH 16
#define EYE_BBOX_HEIGHT 15
#define EYE_BBOX_HEIGHT_DIE 9

#define EYE_STATE_WALKING_RIGHT 1000
#define EYE_STATE_WALKING_LEFT 1001
#define EYE_STATE_WALKING_UP 1002
#define EYE_STATE_WALKING_DOWN 1003

#define EYE_STATE_DIE 200
#define EYE_STATE_WALKING_UP 300

#define EYE_ANI_WALKING 0
#define EYE_ANI_DIE 1
#define MARIO_GRAVITY			0.002f

#define TOP_LIMIT 84
#define BOTTOM_LIMIT 134
#define LEFT_LIMIT 0
#define RIGHT_LIMIT	448

class CGoomba : public CGameObject
{
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void CalcPotentialCollisions(vector<LPGAMEOBJECT>* coObjects, vector<LPCOLLISIONEVENT>& coEvents);
	void FilterCollision(vector<LPCOLLISIONEVENT>& coEvents, vector<LPCOLLISIONEVENT>& coEventsResult, float& min_tx, float& min_ty, float& nx, float& ny, float& rdx, float& rdy);
	virtual void Render();

public:
	CGoomba();
	virtual void SetState(int state);
};