
#pragma once
#include "GameObject.h"

#define ITEMS_BBOX_WIDTH  16
#define ITEMS_BBOX_HEIGHT 16

class Items : public CGameObject
{
	bool active = false;
	float type = 0;
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b) {
		l = x;
		t = y;
		r = x + ITEMS_BBOX_WIDTH;
		b = y + ITEMS_BBOX_HEIGHT;
	};
	DWORD switch_state = 0;
	void StartSwitch_state()
	{
		if (switch_state == 0)
		{
			switch_state = (DWORD)GetTickCount64();
		}
	}
public:
	Items(float time);
	virtual void Render();
	int getType()
	{
		return type;
	}
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
};
