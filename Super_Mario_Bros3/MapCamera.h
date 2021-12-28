#pragma once
#include "GameObject.h"
class MapCamera : public CGameObject
{
	int startX, startY, endX, endY;
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom) {};
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {};
	virtual void Render() {};
	void CalcPotentialCollisions(vector<LPGAMEOBJECT>* coObjects, vector<LPCOLLISIONEVENT>& coEvents) {};
public:
	MapCamera() {};
	void GetCam(int& sX, int& sY, int& eX, int& eY)
	{
		sX = startX;
		sY = startY;
		eX = endX;
		eY = endY;
	}
	void SetCam(int sX, int sY, int eX, int eY)
	{
		startX = sX;
		startY = sY;
		endX = eX;
		endY = eY;
	}
};

