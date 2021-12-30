#pragma once
#include "GameObject.h"

#define THORN_BRICK_BBOX_WIDTH  16
#define THORN_BRICK_BBOX_HEIGHT 16

class THORN : public CGameObject
{
	DWORD reset_start = 0;
public:
	void StartReset() {if(reset_start == 0) reset_start = (DWORD)GetTickCount64(); }
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
};