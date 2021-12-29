#pragma once

#include <unordered_map>

#include <Windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include "Camera.h"
#include "PlayScene.h"


#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>

#include "Scene.h"

using namespace std;

#define KEYBOARD_BUFFER_SIZE 1024

class CGame
{
	static CGame* __instance;
	HWND hWnd;									// Window handle

	LPDIRECT3D9 d3d = NULL;						// Direct3D handle
	LPDIRECT3DDEVICE9 d3ddv = NULL;				// Direct3D device object

	LPDIRECT3DSURFACE9 backBuffer = NULL;
	LPD3DXSPRITE spriteHandler = NULL;			// Sprite helper library to help us draw 2D image on the screen 

	LPDIRECTINPUT8       di;		// The DirectInput object         
	LPDIRECTINPUTDEVICE8 didv;		// The keyboard device 

	BYTE  keyStates[256];			// DirectInput keyboard state buffer 
	DIDEVICEOBJECTDATA keyEvents[KEYBOARD_BUFFER_SIZE];		// Buffered keyboard data

	LPKEYEVENTHANDLER keyHandler;


	CCamera Camera;
	int screen_width;
	int screen_height;

	float camX = 0.0f;
	float camY = 0.0f;

	bool filming = false;

	int startX = 0, startY = 0, endX = 1000, endY = 1000, MapX = 2020, MapY = 2020;

	int heath = 800;
	int attack = 100;

	unordered_map<int, LPSCENE> scenes;
	int current_scene;

	void _ParseSection_SETTINGS(string line);
	void _ParseSection_SCENES(string line);

public:

	int Getattack()
	{
		return attack;
	}
	void setattack(int value)
	{
		attack = value;
	}
	int Getheath()
	{
		return heath;
	}
	void setheath(int value)
	{
		heath = value;
	}
	bool GetFilming()
	{
		return filming;
	}
	void setFilming(bool value)
	{
		filming = value;
	}
	void setMap(int x, int y) 
	{
		MapX = x;
		MapY = y;
	}
	void SetStartEnd(int sX, int sY, int eX, int eY)
	{
		startX = sX;
		startY = sY;
		endX = eX;
		endY = eY;
	}
	int GetMapX()
	{
		return MapX;
	}
	int GetMapY()
	{
		return MapY;
	}
	int GetstartX()
	{
		return startX;
	}
	int GetstartY()
	{
		return startY;
	}
	int GetendX()
	{
		return endX;
	}
	int GetendY()
	{
		return endY;
	}
	void InitKeyboard();
	void SetKeyHandler(LPKEYEVENTHANDLER handler) { keyHandler = handler; }
	void Init(HWND hWnd);
	void Draw(float x, float y, LPDIRECT3DTEXTURE9 texture, int left, int top, int right, int bottom, int alpha = 255, bool abs = false);

	int IsKeyDown(int KeyCode);
	void ProcessKeyboard();
	HWND GetCurrenthWnd() { return hWnd; }
	void Load(LPCWSTR gameFile);
	LPSCENE GetCurrentScene() { return scenes[current_scene]; }
	void SwitchScene(int scene_id);

	int GetScreenWidth() { return screen_width; }
	int GetScreenHeight() { return screen_height; }

	static void SweptAABB(
		float ml,			// move left 
		float mt,			// move top
		float mr,			// move right 
		float mb,			// move bottom
		float dx,			// 
		float dy,			// 
		float sl,			// static left
		float st,
		float sr,
		float sb,
		float& t,
		float& nx,
		float& ny);

	LPDIRECT3DDEVICE9 GetDirect3DDevice() { return this->d3ddv; }
	LPDIRECT3DSURFACE9 GetBackBuffer() { return backBuffer; }
	LPD3DXSPRITE GetSpriteHandler() { return this->spriteHandler; }

	static CGame* GetInstance();


	void SetCamPos(float x, float y) { Camera.SetCamPos(x, y); }

	int GetCamX()
	{
		return camX;
	}

	int GetCamY()
	{
		return camY;
	}

	CCamera GetCam()
	{
		return Camera;
	}


	~CGame();
};


