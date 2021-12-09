#include <iostream>
#include <fstream>

#include "PlayScene.h"
#include "Utils.h"
#include "Textures.h"
#include "Sprites.h"
#include "Portal.h"

using namespace std;

CPlayScene::CPlayScene(int id, LPCWSTR filePath) :
	CScene(id, filePath)
{
	key_handler = new CPlayScenceKeyHandler(this);
}

/*
	Load scene resources from scene file (textures, sprites, animations and objects)
	See scene1.txt, scene2.txt for detail format specification
*/

#define SCENE_SECTION_UNKNOWN -1
#define SCENE_SECTION_TEXTURES 2
#define SCENE_SECTION_SPRITES 3
#define SCENE_SECTION_ANIMATIONS 4
#define SCENE_SECTION_ANIMATION_SETS	5
#define SCENE_SECTION_OBJECTS	6
#define SCENE_SECTION_MAP	7
#define SCENE_SECTION_QUADTREE	8
#define SCENE_SECTION_SETTING	9

#define OBJECT_TYPE_TANK_BODY	0
#define OBJECT_TYPE_TANK_PART	100
#define OBJECT_TYPE_BRICK	1
#define OBJECT_TYPE_CTANKBULLET	2


#define OBJECT_TYPE_PORTAL	50

#define MAX_SCENE_LINE 1024

void CPlayScene::Load()
{
	DebugOut(L"[INFO] Start loading scene resources from : %s \n", sceneFilePath);

	ifstream f;
	f.open(sceneFilePath);

	// current resource section flag
	int section = SCENE_SECTION_UNKNOWN;

	char str[MAX_SCENE_LINE];
	while (f.getline(str, MAX_SCENE_LINE))
	{
		string line(str);

		if (line[0] == '#') continue;	// skip comment lines	
		if (line == "[MAP]") { section = SCENE_SECTION_MAP; continue; }
		if (line == "[TEXTURES]") { section = SCENE_SECTION_TEXTURES; continue; }
		if (line == "[SPRITES]") {
			section = SCENE_SECTION_SPRITES; continue;
		}
		if (line == "[ANIMATIONS]") {
			section = SCENE_SECTION_ANIMATIONS; continue;
		}
		if (line == "[MAP]") { section = SCENE_SECTION_MAP; continue; }
		if (line == "[ANIMATION_SETS]") {
			section = SCENE_SECTION_ANIMATION_SETS; continue;
		}
		if (line == "[OBJECTS]") {
			section = SCENE_SECTION_OBJECTS; continue;
		}
		if (line == "[QUADTREE]") {
			section = SCENE_SECTION_QUADTREE; continue;
		}
		if (line == "[SETTING]") {
			section = SCENE_SECTION_SETTING; continue;
		}
		if (line[0] == '[') { section = SCENE_SECTION_UNKNOWN; continue; }

		//
		// data section
		//
		switch (section)
		{
		case SCENE_SECTION_MAP: _ParseSection_MAP(line); break;
		case SCENE_SECTION_SETTING: _ParseSection_SETTING(line); break;
		case SCENE_SECTION_TEXTURES: _ParseSection_TEXTURES(line); break;
		case SCENE_SECTION_SPRITES: _ParseSection_SPRITES(line); break;
		case SCENE_SECTION_ANIMATIONS: _ParseSection_ANIMATIONS(line); break;
		case SCENE_SECTION_ANIMATION_SETS: _ParseSection_ANIMATION_SETS(line); break;
		case SCENE_SECTION_OBJECTS: _ParseSection_OBJECTS(line); break;
		case SCENE_SECTION_QUADTREE: _ParseSection_QUADTREE(line); break;
		}
	}

	f.close();

	CTextures::GetInstance()->Add(ID_TEX_BBOX, L"textures\\bbox.png", D3DCOLOR_XRGB(255, 255, 255));

	DebugOut(L"[INFO] Done loading scene resources %s\n", sceneFilePath);
}

void CPlayScene::_ParseSection_QUADTREE(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 1) return;

	wstring file_path = ToWSTR(tokens[0]);
	if (quadtree == NULL)
		quadtree = new CQuadTree(file_path.c_str());
}

void CPlayScene::_ParseSection_SETTING(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 1) return;

	setMapheight(int(atoi(tokens[0].c_str())));

	CGame::GetInstance()->GetCurrentScene()->setMapheight(int(atoi(tokens[0].c_str())));

	DebugOut(L"Y: la %d  \n", getMapheight());

}

void CPlayScene::_ParseSection_TEXTURES(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 5) return; // skip invalid lines

	int texID = atoi(tokens[0].c_str());
	wstring path = ToWSTR(tokens[1]);
	int R = atoi(tokens[2].c_str());
	int G = atoi(tokens[3].c_str());
	int B = atoi(tokens[4].c_str());

	CTextures::GetInstance()->Add(texID, path.c_str(), D3DCOLOR_XRGB(R, G, B));
}

void CPlayScene::_ParseSection_SPRITES(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 6) return; // skip invalid lines

	int ID = atoi(tokens[0].c_str());
	int l = atoi(tokens[1].c_str());
	int t = atoi(tokens[2].c_str());
	int r = atoi(tokens[3].c_str());
	int b = atoi(tokens[4].c_str());
	int texID = atoi(tokens[5].c_str());

	LPDIRECT3DTEXTURE9 tex = CTextures::GetInstance()->Get(texID);
	if (tex == NULL)
	{
		DebugOut(L"[ERROR] Texture ID %d not found!\n", texID);
		return;
	}

	CSprites::GetInstance()->Add(ID, l, t, r, b, tex);
}

void CPlayScene::_ParseSection_ANIMATIONS(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 3) return; // skip invalid lines - an animation must at least has 1 frame and 1 frame time

	//DebugOut(L"--> %s\n",ToWSTR(line).c_str());

	LPANIMATION ani = new CAnimation();

	int ani_id = atoi(tokens[0].c_str());
	for (int i = 1; i < tokens.size(); i += 2)	// why i+=2 ?  sprite_id | frame_time  
	{
		int sprite_id = atoi(tokens[i].c_str());
		int frame_time = atoi(tokens[i + 1].c_str());
		ani->Add(sprite_id, frame_time);
	}

	CAnimations::GetInstance()->Add(ani_id, ani);
}

void CPlayScene::_ParseSection_ANIMATION_SETS(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 2) return; // skip invalid lines - an animation set must at least id and one animation id

	int ani_set_id = atoi(tokens[0].c_str());

	LPANIMATION_SET s = new CAnimationSet();

	CAnimations* animations = CAnimations::GetInstance();

	for (int i = 1; i < tokens.size(); i++)
	{
		int ani_id = atoi(tokens[i].c_str());

		LPANIMATION ani = animations->Get(ani_id);
		s->push_back(ani);
	}

	CAnimationSets::GetInstance()->Add(ani_set_id, s);
}

/*
	Parse a line in section [OBJECTS]
*/
void CPlayScene::_ParseSection_OBJECTS(string line)
{
	vector<string> tokens = split(line);

	//DebugOut(L"--> %s\n",ToWSTR(line).c_str());

	if (tokens.size() < 3) return; // skip invalid lines - an object set must have at least id, x, y

	int object_type = atoi(tokens[0].c_str());
	float x = atof(tokens[1].c_str());
	float y = atof(tokens[2].c_str());

	int ani_set_id = atoi(tokens[3].c_str());

	CAnimationSets* animation_sets = CAnimationSets::GetInstance();

	CGameObject* obj = NULL;

	switch (object_type)
	{
	case OBJECT_TYPE_TANK_BODY:
		if (player != NULL)
		{
			DebugOut(L"[ERROR] TANK_BODY object was created before!\n");
			return;
		}
		obj = new CTANK_BODY(x, getMapheight() - y);

		player = (CTANK_BODY*)obj;

		DebugOut(L"[INFO] Player object created!\n");

		break;
	case OBJECT_TYPE_BRICK: obj = new CBrick(); break;
	case OBJECT_TYPE_CTANKBULLET: obj = new CTANKBULLET(); break;
		
	case OBJECT_TYPE_TANK_PART:
	{
		float part = atof(tokens[4].c_str());
		obj = new TankParts(part);
	}
	break;
	case OBJECT_TYPE_PORTAL:
	{
		float r = atof(tokens[4].c_str());
		float b = atof(tokens[5].c_str());
		int scene_id = atoi(tokens[6].c_str());
		obj = new CPortal(x, y, r, b, scene_id);
	}
	break;
	
	default:
		DebugOut(L"[ERR] Invalid object type: %d\n", object_type);
		return;
	}

	// General object setup


	LPANIMATION_SET ani_set = animation_sets->Get(ani_set_id);

	if (obj != NULL )
	{
		if(object_type != OBJECT_TYPE_TANK_BODY)
		obj->SetPosition(x, getMapheight() - y);
		obj->SetAnimationSet(ani_set);
		obj->SetOrigin(x, y, obj->GetState());
		obj->SetisOriginObj(true);
		objects.push_back(obj);
	}
}

void CPlayScene::_ParseSection_MAP(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 7) return; // skip invalid lines

	int idTileSet = atoi(tokens[0].c_str());
	int totalRowsTileSet = atoi(tokens[1].c_str());
	int totalColumnsTileSet = atoi(tokens[2].c_str());
	int totalRowsMap = atoi(tokens[3].c_str());
	int totalColumnsMap = atoi(tokens[4].c_str());
	int totalTiles = atoi(tokens[5].c_str());

	setMapheight(int(atoi(tokens[3].c_str())) * 32);

	CGame::GetInstance()->GetCurrentScene()->setMapheight(int(atoi(tokens[3].c_str())) * 32);

	wstring file_path = ToWSTR(tokens[6]);

	map = new Map(idTileSet, totalRowsTileSet, totalColumnsTileSet, totalRowsMap, totalColumnsMap, totalTiles);
	map->LoadMap(file_path.c_str());
	map->ExtractTileFromTileSet();
}

bool CPlayScene::IsInUseArea(float Ox, float Oy)
{
	float CamX, CamY;

	CamX = (float)CGame::GetInstance()->GetCam().GetCamX();

	CamY = (float)CGame::GetInstance()->GetCam().GetCamY();

	if (((CamX - CAM_X_BONUS < Ox) && (Ox < CamX + IN_USE_WIDTH)) && ((CamY < Oy) && (Oy < CamY + IN_USE_HEIGHT)))
		return true;
	return false;
}

void CPlayScene::Update(DWORD dt)
{
	// We know that TANK_BODY is the first object in the list hence we won't add him into the colliable object list
	// TO-DO: This is a "dirty" way, need a more organized way 

	// skip the rest if scene was already unloaded (TANK_BODY::Update might trigger PlayScene::Unload)
	if (player == NULL) return;

	// Update camera to follow mario
	float cx, cy;

	player->GetPosition(cx, cy);

	cy = cy;

	/*DebugOut(L"Y: la %d %f  \n", CGame::GetInstance()->GetCurrentScene()->getMapheight(), cy);*/
	
	CGame* game = CGame::GetInstance();

	cx -= game->GetScreenWidth() / 2;
	cy -= game->GetScreenHeight() / 2;

	if (cx < 0)
	{
		cx = 0;
	}

	CGame::GetInstance()->SetCamPos(cx, cy);

	vector<LPGAMEOBJECT> coObjects;

	quadtree->GetObjects(objects, (int)cx, (int)cy);

	for (size_t i = 0; i < objects.size(); i++)
	{
		float Ox, Oy;
		objects[i]->GetPosition(Ox, Oy);
		if (!IsInUseArea(Ox, Oy) && !objects[i]->GetisOriginObj())
		{
			objects[i]->SetActive(false);
			objects.erase(objects.begin() + i);
			i--;
		}
	}

	for (size_t i = 1; i < objects.size(); i++)
	{
		coObjects.push_back(objects[i]);
	}

	for (size_t i = 0; i < objects.size(); i++)
	{
		objects[i]->Update(dt, &coObjects);
	}
}

void CPlayScene::Render()
{
	CGame* game = CGame::GetInstance();

	if (map)
	{
		this->map->Render();
	}
	for (int i = 0; i < objects.size(); i++)
		objects[i]->Render();
}

/*
	Unload current scene
*/
void CPlayScene::Unload()
{
	for (int i = 0; i < objects.size(); i++)
		delete objects[i];

	objects.clear();
	player = NULL;

	DebugOut(L"[INFO] Scene %s unloaded! \n", sceneFilePath);
}

void CPlayScenceKeyHandler::OnKeyDown(int KeyCode)
{
	//DebugOut(L"[INFO] KeyDown: %d\n", KeyCode);

	CTANK_BODY* mario = ((CPlayScene*)scence)->GetPlayer();
	switch (KeyCode)
	{
	case DIK_SPACE:
		mario->SetState(TANK_BODY_STATE_JUMP);
		break;
	case DIK_B:
		mario->Reset();
		break;
	case DIK_A:
		mario->SetisFiring(true);	
		break;
	}
}

void CPlayScenceKeyHandler::OnKeyUp(int KeyCode)
{
	CTANK_BODY* mario = ((CPlayScene*)scence)->GetPlayer();
		switch (KeyCode)
		{
		case DIK_A:
			mario->SetisFiring(false);
			break;
		}
}

void CPlayScenceKeyHandler::KeyState(BYTE* states)
{
	CGame* game = CGame::GetInstance();
	CTANK_BODY* mario = ((CPlayScene*)scence)->GetPlayer();

	// disable control key when TANK_BODY die 
	if (mario->GetState() == TANK_BODY_STATE_DIE) return;
	if (game->IsKeyDown(DIK_RIGHT))
		mario->SetState(TANK_BODY_STATE_WALKING_RIGHT);
	else if (game->IsKeyDown(DIK_LEFT))
		mario->SetState(TANK_BODY_STATE_WALKING_LEFT);
	else if (game->IsKeyDown(DIK_DOWN))
		mario->SetState(TANK_BODY_STATE_WALKING_DOWN);
	else if (game->IsKeyDown(DIK_UP))
		mario->SetState(TANK_BODY_STATE_WALKING_UP);
	else
		mario->SetState(TANK_BODY_STATE_IDLE);
}