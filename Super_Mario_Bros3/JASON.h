#pragma once
#include "GameObject.h"
#include "DF.h"

class JASON : public CGameObject
{
	int weapon = 1;
	int untouchable;

	int pre_ani = 0;

	DWORD firing_start;
	DWORD untouchable_start;

	bool isFiring = 0;
	bool isAlreadyFired = 0;

public:
	JASON(float x = 0.0f, float y = 0.0f);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects = NULL);
	virtual void Render();
	void CalcPotentialCollisions(vector<LPGAMEOBJECT>* coObjects, vector<LPCOLLISIONEVENT>& coEvents);

	void SetState(int state);
	int getWeapon() { return weapon; }
	void SwitchWeapon() { weapon = -weapon; }
	void StartUntouchable() { untouchable = 1; untouchable_start = GetTickCount(); }
	void StartFiring() {if(firing_start == 0) firing_start = GetTickCount(); }
	void SetisIsFiring(int time) { firing_start = time; }

	void Reset();

	void SetisFiring(bool value) { isFiring = value; }
	bool GetisFiring() { return isFiring; }
	int GetPre_ani() { return pre_ani; }
	void SetisAlreadyFired(bool value) { isAlreadyFired = value; }
	bool GetisAlreadyFired() { return isAlreadyFired; }

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
};