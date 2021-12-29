#pragma once
#include "GameObject.h"
#include "DF.h"

class CSOPHIA : public CGameObject
{
	int level;
	int untouchable;
	DWORD firing_start;
	DWORD untouchable_start;

	bool isJumping = false;

	int dir = 0;

	bool isAimingUp;

	float start_x;			// initial position of SOPHIA at scene
	float start_y;

	bool isFiring = 0;
	bool isAlreadyFired = 0;

public:
	CSOPHIA(float x = 0.0f, float y = 0.0f);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects = NULL);
	virtual void Render();
	void CalcPotentialCollisions(vector<LPGAMEOBJECT>* coObjects, vector<LPCOLLISIONEVENT>& coEvents);

	void SetState(int state);
	void SetLevel(int l) { level = l; }
	void StartUntouchable() { untouchable = 1; untouchable_start = GetTickCount(); }
	void StartFiring() {if(firing_start == 0) firing_start = GetTickCount(); }
	void SetisIsFiring(int time) { firing_start = time; }

	void Reset();

	int GetNx() {
		return dir;
	}


	bool GetIsJumping() { return isJumping; }
	void SetIsJumping(bool value) { isJumping = value; }
	
	void SetisAimingUp(bool value) { isAimingUp = value; }
	bool GetisAimingUp() { return isAimingUp; }

	void SetisFiring(bool value) { isFiring = value; }
	bool GetisFiring() { return isFiring; }
	void SetisAlreadyFired(bool value) { isAlreadyFired = value; }
	bool GetisAlreadyFired() { return isAlreadyFired; }

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
};