#pragma once

#include "GameObject.h"
#include "algorithm"
#include "PlayScene.h"



#define CBOOM_STATE_IDLE	50
#define CBOOM_STATE_MOVE	100
#define	CBOOM_STATE_TOP_RIGHT	1
#define	CBOOM_STATE_TOP_LEFT	2
#define	CBOOM_STATE_BOTTOM_RIGHT	3
#define	CBOOM_STATE_BOTTOM_LEFT	0
#define CBOOM_GRAVITY		0.0008f

#define EXPLOSING_TIMING		2000
#define CBOOM_DISAPEAR_TIME_LIMIT		1000
#define CBOOM_DELAY_TIME_MOVING		1000
#define CBOOM_VX		0.1f
#define CBOOM_VY		0.5f

class CBOOM : public CGameObject
{
	int type;
	bool isUsed = false;
	DWORD timing_start = 0;
public:

	CBOOM();
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	virtual void SetState(int state);
	void StartTiming()
	{
		if (timing_start == 0)
			timing_start = (DWORD)GetTickCount64();
	}
	void SetIsUsed(bool isUsedBool)
	{
		isUsed = isUsedBool;
	}
	bool GetIsUsed()
	{
		return isUsed;
	}
	int GetType()
	{
		return type;
	}
	void SetType(int typeInt)
	{
		type = typeInt;
	}

};