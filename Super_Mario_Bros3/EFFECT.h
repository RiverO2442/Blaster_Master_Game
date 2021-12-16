#pragma once
#include "GameObject.h"

#define BRICK_BBOX_WIDTH  16
#define BRICK_BBOX_HEIGHT 16

class EFFECT : public CGameObject
{
	bool active = false;
	float duration;
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom) {};
	DWORD switch_state = 0;
	void StartSwitch_state()
	{
		if (switch_state == 0)
		{
			switch_state = (DWORD)GetTickCount64();
		}
	}
public:

	EFFECT(float time);
	virtual void Render();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
};