#pragma once

#include <Windows.h>
#include <d3dx9.h>
#include <vector>

#include "Sprites.h"
#include "Animations.h"

#define DIE_PULL 0.2f
#define STATE_DIE 200
#define STATE_OVER 2000
#define STATE_IDLE 100

using namespace std;

#define ID_TEX_BBOX -100		// special texture to draw object bounding box

class CGameObject;
typedef CGameObject* LPGAMEOBJECT;

struct CCollisionEvent;
typedef CCollisionEvent* LPCOLLISIONEVENT;
struct CCollisionEvent
{
	LPGAMEOBJECT obj;
	float t, nx, ny;

	float dx, dy;		// *RELATIVE* movement distance between this object and obj

	CCollisionEvent(float t, float nx, float ny, float dx = 0, float dy = 0, LPGAMEOBJECT obj = NULL)
	{
		this->t = t;
		this->nx = nx;
		this->ny = ny;
		this->dx = dx;
		this->dy = dy;
		this->obj = obj;
	}

	static bool compare(const LPCOLLISIONEVENT& a, LPCOLLISIONEVENT& b)
	{
		return a->t < b->t;
	}
};


class CGameObject
{
public:

	float x;
	float y;

	float dx;	// dx = vx*dt
	float dy;	// dy = vy*dt

	float vx;
	float vy;

	int nx;

	int state;

	DWORD dt;

	LPANIMATION_SET animation_set;

	bool isAlive = true;

	bool Actived = false;

	float origin_x, origin_y;

	int origin_state;

	bool isOriginObj = false;

	int heath = 500;

	bool spammed = false;

public:

	bool Getspammed()
	{
		return spammed;
	}
	void setspammed(bool value)
	{
		spammed = value;
	}
	int Getheath()
	{
		return heath;
	}
	void setheath(int value)
	{
		heath = value;
		if (heath < 0)
			heath = 0;
		if (heath == 0)
			SetState(STATE_DIE);
	}
	void SetPosition(float x, float y) { this->x = x, this->y = y; }
	void SetSpeed(float vx = 0, float vy = 0) { this->vx = vx, this->vy = vy; }
	void GetPosition(float& x, float& y) { x = this->x; y = this->y; }
	float GetPositionX() {
		return x;
	}
	float GetPositionY() {
		return y;
	}
	void GetSpeed(float& vx, float& vy) { vx = this->vx; vy = this->vy; }

	int GetState() { if (this) return this->state; }

	void RenderBoundingBox();

	void SetAnimationSet(LPANIMATION_SET ani_set) { animation_set = ani_set; }

	LPCOLLISIONEVENT SweptAABBEx(LPGAMEOBJECT coO);
	void CalcPotentialCollisions(vector<LPGAMEOBJECT>* coObjects, vector<LPCOLLISIONEVENT>& coEvents);
	void FilterCollision(
		vector<LPCOLLISIONEVENT>& coEvents,
		vector<LPCOLLISIONEVENT>& coEventsResult,
		float& min_tx,
		float& min_ty,
		float& nx,
		float& ny,
		float& rdx,
		float& rdy);

	CGameObject();

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom) = 0;
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	virtual void Render() = 0;
	virtual void SetState(int state) { this->state = state; }


	~CGameObject();

	void SetisAlive(bool value)
	{
		isAlive = value;
	}
	bool GetisAlive()
	{
		return isAlive;
	}
	void SetisOriginObj(bool value)
	{
		isOriginObj = value;
	}
	bool GetisOriginObj()
	{
		return isOriginObj;
	}
	void reset()
	{
		SetState(origin_state);
		x = origin_x;
		y = origin_y;
	}

	void GetOriginLocation(float& x, float& y)
	{
		x = origin_x;
		y = origin_y;
	}

	void SetActive(bool value)
	{
		Actived = value;
	}

	bool GetActive()
	{
		return	Actived;
	}

	void SetOrigin(float x, float y, int state) { this->origin_x = x, this->origin_y = y; this->origin_state = state; }
};

