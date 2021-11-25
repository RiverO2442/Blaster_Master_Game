#include <fstream>
#include <iostream>
#include"MapObj.h"
#include "Textures.h"

MapObj::MapObj(int _totalRowsMap, int _totalColumnsMap)
{
	this->TotalRowsOfMap = _totalRowsMap;
	this->TotalColumnsOfMap = _totalColumnsMap;
}

MapObj::~MapObj()
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

void MapObj::Render(vector<LPGAMEOBJECT>& listObjects)
{
	CAnimationSets* animation_sets = CAnimationSets::GetInstance();
	LPANIMATION_SET ani_set = animation_sets->Get(2);
	CGameObject* obj = NULL;
	for (int r = 0; r < TotalRowsOfMap; r++)
		for (int c = 0; c < TotalColumnsOfMap; c++)
		{
			if (TileMap[r][c] != 0) {
				obj = new CBrick();
				obj->SetPosition(16*c, 16*r - 16);
				obj->SetAnimationSet(ani_set);
				obj->SetOrigin(16 * c, (16 * r) - 16, obj->GetState());
				listObjects.push_back(obj);
			}
		}
}

void MapObj::LoadMapObj(LPCWSTR path)
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