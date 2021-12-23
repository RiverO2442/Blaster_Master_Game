#include "NoCollisionObject.h"
#include"Game.h"
#include"Scene.h"

CNoCollisionObject::CNoCollisionObject()
{
}

void CNoCollisionObject::Render()
{
		animation_set->at(0)->Render(x, y);
		//RenderBoundingBox();
}
void CNoCollisionObject::GetBoundingBox(float& l, float& t, float& r, float& b)
{
}