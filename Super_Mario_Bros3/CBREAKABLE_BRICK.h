#pragma once
#include "GameObject.h"

#define BREAKABLE_BRICK_BBOX_WIDTH  16
#define BREAKABLE_BRICK_BBOX_HEIGHT 16

class CBREAKABLE_BRICK : public CGameObject
{
public:
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
};