#include <fstream>
#include <iostream>
#include "NoRenderObj.h"
#include "Textures.h"

NoRenderObj::NoRenderObj(int _totalRowsMap, int _totalColumnsMap)
{
	this->TotalRowsOfMap = _totalRowsMap;
	this->TotalColumnsOfMap = _totalColumnsMap;
}

NoRenderObj::~NoRenderObj()
{
	if (TileMap)
	{
		for (int i = 0; i < TotalRowsOfMap; i++)
		{
			delete TileMap[i];
		}
		delete TileMap;
		TileMap = nullptr;
	}
}

void NoRenderObj::Render(vector<LPGAMEOBJECT>& listObjects)
{
	CAnimationSets* animation_sets = CAnimationSets::GetInstance();
	LPANIMATION_SET ani_set = animation_sets->Get(2);
	CGameObject* obj = NULL;
	for (int r = 0; r < TotalRowsOfMap; r++)
		for (int c = 0; c < TotalColumnsOfMap; c++)
		{
			if (TileMap[r][c] == 1) {
				obj = new CBrick();
				obj->SetPosition(16*c, 16*r);
				obj->SetAnimationSet(ani_set);
				obj->SetOrigin(16 * c, 16 * r, obj->GetState());
				listObjects.push_back(obj);
			}
		}
}

void NoRenderObj::LoadNoRenderObj(LPCWSTR path)
{
	ifstream f;

	f.open(path);

	// init tilemap
	this->TileMap = new int* [TotalRowsOfMap];
	for (int i = 0; i < TotalRowsOfMap; i++)
	{
		TileMap[i] = new int[TotalColumnsOfMap];
		for (int j = 0; j < TotalColumnsOfMap; j++)
		{
			f >> TileMap[i][j];
		}
	}
	f.close();
}