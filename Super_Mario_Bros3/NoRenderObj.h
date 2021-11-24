#pragma once

#include <d3dx9.h>
#include <d3d9.h>
#include <vector>
#include "Brick.h"

#define TILE_WIDTH 32
#define TILE_HEIGHT 32

#define IN_USE_WIDTH 330
#define IN_USE_HEIGHT 300
#define CAM_X_BONUS 50

class NoRenderObj
{
private:
	int** TileMap;
	LPDIRECT3DTEXTURE9 TileSet; //map1-1_bank.png
	int TotalColumnsOfMap, TotalRowsOfMap;

public:
	NoRenderObj(int _totalRowsMap, int _totalColumnsMap);
	~NoRenderObj();
	void Render(vector<LPGAMEOBJECT>& listObjects);
	void LoadNoRenderObj(LPCWSTR file_path);
};