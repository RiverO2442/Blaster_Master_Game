#pragma once
#include "Game.h"
#include "Textures.h"
#include "Scene.h"
#include "GameObject.h"
#include "Brick.h"
#include "SOPHIA.h"
#include "Eye.h"
#include "Koopas.h"
#include "Map.h"
#include "CTANKWHEELS.h"
#include "MapObj.h"
#include "CLaserGuard.h"
#include "CBallCarry.h"
#include "CBallbot.h"
#include "CDRAP.h"
#include "CGX680.h"
#include "CGX680S.h"
#include "CSTUKA.h"
#include "Eyelet.h"
#include "Interrupt.h"
#include "CTANKBULLET.h"
#include "CEvenType1.h"
#include "CINTERRUPT_BULLET.h"
#include "CREDWORM.h"
#include "TANKBODY.h"
#include "TANKTURRET.h"
#include "EFFECT.h"
#include "JASON.h"
#include "CBOOM.h"
#include "NoCollisionObject.h"

#include "Utils.h"
#include "Game.h"
#include <iostream>
#include <fstream>
#include "Utils.h"
#include "Textures.h"
#include "Sprites.h"
#include "Portal.h"

#define QUADTREE_SECTION_SETTINGS	1
#define QUADTREE_SECTION_OBJECTS	2
#define MAX_QUADTREE_LINE 1024

class CQuadTree
{
	bool isLeaf = false;
	float x = 0;
	float y = 0;
	float cellWidth = 0;
	float  cellHeight = 0;
	CQuadTree* BrachTL = NULL;
	CQuadTree* BrachTR = NULL;
	CQuadTree* BrachBL = NULL;
	CQuadTree* BrachBR = NULL;
	MapObj* obj;
	vector<LPGAMEOBJECT> listObjects;

	void _ParseSection_SETTINGS(string line);
	void _ParseSection_OBJECTS(string line);
	void _ParseSection_MapObj(string line);
public:
	CQuadTree(float cellWidth, float cellHeight, float x, float y);
	CQuadTree(LPCWSTR filePath);
	void GetObjects(vector<LPGAMEOBJECT>& listObject, int playerX, int playerY);
	void Load(LPCWSTR filePath);
	void Unload();
	void Add(CGameObject* obj)
	{
		listObjects.push_back(obj);
	}
	int getVollunm(){
		return listObjects.size();
	}
	void Render();
	void Plit();
	void ObjectPlit(CQuadTree* brach);
	bool inRange(float ox, float oy, float x, float y, float width, float height);
	void Pop(vector<LPGAMEOBJECT>& listObject, int CamX, int CamY);

};

class CPlayScene : public CScene
{
protected:
	CSOPHIA* player;				// A play scene has to have player, right? 
	JASON* player2;
	vector<LPGAMEOBJECT> objects;
	int mapHeight;
	Map* map;
	CQuadTree* quadtree;
	vector<CEvenType1*> InterruptBulletMng ;
	vector<CEvenType1*> WormSpamMng;
	vector<CEvenType1*> KaboomMng;
	vector<CEvenType1*> BoomCarryMng;

	void _ParseSection_TEXTURES(string line);
	void _ParseSection_SPRITES(string line);
	void _ParseSection_ANIMATIONS(string line);
	void _ParseSection_ANIMATION_SETS(string line);
	void _ParseSection_OBJECTS(string line);
	void _ParseSection_MAP(string line);
	void _ParseSection_QUADTREE(string line);
	void _ParseSection_SETTING(string line);
public:
	CPlayScene(int id, LPCWSTR filePath);

	virtual void Load();
	virtual void Update(DWORD dt);
	virtual void Render();
	virtual void Unload();

	bool IsInUseArea(float Ox, float Oy);
	bool IsInside(float Ox, float Oy, float xRange, float yRange, float tx, float ty);

	CSOPHIA* GetPlayer() { return player; }
	JASON* GetPlayer2() { return player2; }

	void setMapheight(int height)
	{
		mapHeight = height;
	}

	int getMapheight()
	{
		return mapHeight;
	}
	/////////////////BoomCarryMng
	void AddBoomCarryMng(float x, float y)
	{
		CEvenType1* obj = new CEvenType1(x, y);
		this->BoomCarryMng.push_back(obj);
	}
	void CheckStackBoomCarryMng()
	{
		if (BoomCarryMng.at(0)->getCEventStack() < 3)
		{
			BoomCarryMng.at(0)->setCEventStack(BoomCarryMng.at(0)->getCEventStack() + 1);
		}
		else 
		{
			DeleteBoomCarryMng();
		}
	}
	CEvenType1* GetBoomCarryMng()
	{
		return BoomCarryMng.at(0);
	}
	bool CheckBoomCarryMng()
	{
		if (BoomCarryMng.size() != 0)
			return true;
		return false;
	}
	void DeleteBoomCarryMng()
	{
		this->BoomCarryMng.erase(BoomCarryMng.begin());
	}
	/////////////////KaboomMng
	void AddKaboomMng(float x, float y)
	{
		CEvenType1* obj = new CEvenType1(x, y);
		this->KaboomMng.push_back(obj);
	}
	CEvenType1* GetKaboomMng()
	{
		return KaboomMng.at(0);
	}
	bool CheckKaboomMng()
	{
		if (KaboomMng.size() != 0)
			return true;
		return false;
	}
	void DeleteKaboomMng()
	{
		this->KaboomMng.erase(KaboomMng.begin());
	}
	/////////////////InterruptBulletMng
	void AddInterruptBulletMng(float x, float y)
	{
		CEvenType1* obj = new CEvenType1(x, y);
		this->InterruptBulletMng.push_back(obj);
	}
	CEvenType1* GetInterruptBulletMng()
	{
		return InterruptBulletMng.at(0);
	}
	bool CheckInterruptBulletMng()
	{
		if (InterruptBulletMng.size() != 0)
			return true;
		return false;
	}
	void DeleteInterruptBulletMng()
	{
		this->InterruptBulletMng.erase(InterruptBulletMng.begin());
	}
	//////////////////////////WormSpamMng
	void AddWormSpamMng(float x, float y)
	{
		CEvenType1* obj = new CEvenType1(x, y);
		this->WormSpamMng.push_back(obj);
	}
	CEvenType1* GetWormSpamMng()
	{
		return WormSpamMng.at(0);
	}
	bool CheckWormSpamMng()
	{
		if (WormSpamMng.size() != 0)
			return true;
		return false;
	}
	void DeleteWormSpamMng()
	{
		this->WormSpamMng.erase(WormSpamMng.begin());
	}
	///////////////////////////////
	//friend class CPlayScenceKeyHandler;
};

class CPlayScenceKeyHandler : public CScenceKeyHandler
{
public:
	virtual void KeyState(BYTE* states);
	virtual void OnKeyDown(int KeyCode);
	virtual void OnKeyUp(int KeyCode);
	CPlayScenceKeyHandler(CScene* s) :CScenceKeyHandler(s) {};
};

