#pragma once

#include <d3dx9.h>
#include <d3d9.h>
#include <vector>
#include "Brick.h"

#define TILE_WIDTH 32
#define TILE_HEIGHT 32

#define IN_USE_WIDTH 330
#define IN_USE_HEIGHT 500
#define CAM_X_BONUS 300
class MapObj
{
private:
	int** TileMap;
	LPDIRECT3DTEXTURE9 TileSet; //map1-1_bank.png
	int TotalColumnsOfMap, TotalRowsOfMap;

public:
	MapObj(int _totalRowsMap, int _totalColumnsMap);
	~MapObj();
	void Render(vector<LPGAMEOBJECT>& listObjects);
	void LoadMapObj(LPCWSTR file_path);
};