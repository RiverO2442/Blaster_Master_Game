#pragma once

#include <d3dx9.h>
#include <d3d9.h>
#include <vector>
#include "Sprites.h"
#include "define.h"

#define TILE_WIDTH 32
#define TILE_HEIGHT 32

class Map
{
private:
	int** TileMap;
	vector<LPSPRITE> Tiles;
	LPDIRECT3DTEXTURE9 TileSet; //map1-1_bank.png
	int TotalColumnsOfMap, TotalRowsOfMap;
	int TotalColumnsOfTileSet, TotalRowsOfTileSet;
	int TotalTiles; //total tiles of tileset

public:
	Map(int _idTileSet, int _totalRowsTileSet, int _totalColumnsTileSet, int _totalRowsMap, int _totalColumnsMap, int _totalTiles);
	~Map();
	void Render(int playerX, int playerY);
	void Render();
	void ExtractTileFromTileSet();
	void LoadMap(LPCWSTR file_path);

	int GetMapHeight();
	int GetMapWidth();
};