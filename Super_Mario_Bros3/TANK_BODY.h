#pragma once
#include "GameObject.h"

#define TANK_BODY_WALKING_SPEED		0.15f
//0.1f
#define TANK_BODY_JUMP_SPEED_Y		0.5f
#define TANK_BODY_JUMP_DEFLECT_SPEED 0.2f
#define TANK_BODY_GRAVITY			0.002f
#define TANK_BODY_DIE_DEFLECT_SPEED	 0.5f

#define TANK_BODY_STATE_IDLE			0
#define TANK_BODY_STATE_WALKING_RIGHT	100
#define TANK_BODY_STATE_WALKING_LEFT	200
#define TANK_BODY_STATE_WALKING_UP	500
#define TANK_BODY_STATE_WALKING_DOWN	600
#define TANK_BODY_STATE_JUMP			300
#define TANK_BODY_STATE_DIE				400

#define TANK_BODY_ANI_BIG_IDLE_RIGHT		0
#define TANK_BODY_ANI_BIG_IDLE_LEFT			1
#define TANK_BODY_ANI_SMALL_IDLE_RIGHT		2
#define TANK_BODY_ANI_SMALL_IDLE_LEFT			3

#define TANK_BODY_ANI_BIG_WALKING_RIGHT			4
#define TANK_BODY_ANI_BIG_WALKING_LEFT			5
#define TANK_BODY_ANI_SMALL_WALKING_RIGHT		6
#define TANK_BODY_ANI_SMALL_WALKING_LEFT		7

#define TANK_BODY_ANI_DIE				8

#define	TANK_BODY_LEVEL_SMALL	1
#define	TANK_BODY_LEVEL_BIG		2

#define TANK_BODY_BIG_BBOX_WIDTH  8
#define TANK_BODY_BIG_BBOX_HEIGHT 6

#define TANK_BODY_SMALL_BBOX_WIDTH  13
#define TANK_BODY_SMALL_BBOX_HEIGHT 15

#define TANK_BODY_UNTOUCHABLE_TIME 5000



class CTANK_BODY : public CGameObject
{
	int level;
	int untouchable;
	DWORD untouchable_start;

	float start_x;			// initial position of TANK_BODY at scene
	float start_y;
public:
	CTANK_BODY(float x = 0.0f, float y = 0.0f);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects = NULL);
	virtual void Render();

	void SetState(int state);
	void SetLevel(int l) { level = l; }
	void StartUntouchable() { untouchable = 1; untouchable_start = GetTickCount(); }

	void Reset();

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
};