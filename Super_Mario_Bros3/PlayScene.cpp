#include <iostream>
#include <fstream>

#include "PlayScene.h"


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
		if (line == "[MAPCAM]") {
			section = SCENE_SECTION_MAPCAM; continue;
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
		case SCENE_SECTION_MAPCAM: _ParseSection_MAPCAM(line); break;		
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

void CPlayScene::_ParseSection_MAPCAM(string line)
{
	vector<string> tokens = split(line);
	if (tokens.size() < 4) return;
	MapCamera* mCam = new MapCamera();
	mCam->SetCam(atoi(tokens[0].c_str()), atoi(tokens[1].c_str()), atoi(tokens[2].c_str()), atoi(tokens[3].c_str()));
	MapCam.push_back(mCam);
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
	case OBJECT_TYPE_SOPHIA:
		if (player != NULL)
		{
			DebugOut(L"[ERROR] SOPHIA object was created before!\n");
			return;
		}
		obj = new CSOPHIA(x, getMapheight() - y);

		player = (CSOPHIA*)obj;

		DebugOut(L"[INFO] Player object created!\n");

		break;
	case OBJECT_TYPE_JASON:
		if (player2 != NULL)
		{
			DebugOut(L"[ERROR] JASON object was created before!\n");
			return;
		}
		obj = new JASON(x, getMapheight() - y);

		player2 = (JASON*)obj;

		DebugOut(L"[INFO] Player object created!\n");

		break;
	case OBJECT_TYPE_MINI_JASON:
		if (player3 != NULL)
		{
			DebugOut(L"[ERROR] MINI_JASON object was created before!\n");
			return;
		}
		obj = new MINI_JASON(x, getMapheight() - y);

		player3 = (MINI_JASON*)obj;

		DebugOut(L"[INFO] Player object created!\n");

		break;
	case OBJECT_TYPE_BRICK: obj = new CBrick(); break;
	case OBJECT_TYPE_CBOOM: obj = new CBOOM(); break;
	case OBJECT_TYPE_CTANKBULLET: obj = new CTANKBULLET(); break;
	case OBJECT_TYPE_NoCollisionObject: obj = new CNoCollisionObject(); break;
	case OBJECT_TYPE_STATBAR: obj = new CSTATBAR(atoi(tokens[4].c_str())); break;
	case OBJECT_TYPE_ITEMS:
	{
		obj = new Items(0);
	}
	break;
	case OBJECT_TYPE_TANK_WHEEL:
	{
		float part = atof(tokens[4].c_str());
		obj = new TANKWHEEL(part);
	}
	break;
	case OBJECT_TYPE_CLASER_BULLET:
	{
		obj = new CLASER_BULLET();
	}
	break;
	case OBJECT_TYPE_TANK_BODY:
	{
		obj = new TANKBODY();
	}
	break;
	case OBJECT_TYPE_JASON_BULLET_1:
	{
		obj = new CWAVE_BULLET();
	}
	break;

	case OBJECT_TYPE_TANK_TURRET:
	{
		obj = new TANKTURRET();
	}
	break;
	case OBJECT_TYPE_TANKDOOR:
	{
		obj = new CTANKDOOR();
	}
	break;
	case OBJECT_TYPE_EFFECT:
	{
		float time = atof(tokens[4].c_str());
		obj = new EFFECT(time);
	}
	break;
	case OBJECT_TYPE_JASON_GRENADE:
	{
		obj = new CGRENADE();
	}
	break;
	case OBJECT_TYPE_THORN:
	{
		obj = new THORN();
	}
	break;
	case OBJECT_TYPE_BREAKABLE_BRICK:
	{
		obj = new CBREAKABLE_BRICK();
	}
	break;
	case OBJECT_TYPE_CGX_BULLET:
	{
		obj = new CGX_BULLET();
	}
	break;
	case OBJECT_TYPE_PORTAL:
	{
		float r = atof(tokens[4].c_str());
		float b = atof(tokens[5].c_str());
		int scene_id = atoi(tokens[6].c_str());
		int camState = atoi(tokens[7].c_str());
		obj = new CPortal(x, y, r, b, scene_id, camState);
	}
	break;
	case OBJECT_TYPE_CINTERCRUPT_BULLET: obj = new CINTERRUPT_BULLET(); break;
	case OBJECT_TYPE_RED_WORM: obj = new CREDWORM(); break;
		
	break;
	
	default:
		DebugOut(L"[ERR] Invalid object type: %d\n", object_type);
		return;
	}

	// General object setup


	LPANIMATION_SET ani_set = animation_sets->Get(ani_set_id);

	if (obj != NULL)
	{
		if (object_type == OBJECT_TYPE_NoCollisionObject || object_type == OBJECT_TYPE_STATBAR)
		{
			if(object_type != OBJECT_TYPE_STATBAR)
				obj->SetPosition(x, getMapheight() - y);
			else 
				obj->SetPosition(x, y);
			obj->SetAnimationSet(ani_set);
			secondLayer.push_back(obj);
			return;
		}
		if(object_type != OBJECT_TYPE_SOPHIA)
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

	if (((CamX < Ox) && (Ox < CamX + IN_USE_WIDTH + CAM_X_BONUS)) && ((CamY < Oy) && (Oy < CamY + IN_USE_HEIGHT)))
		return true;
	return false;
}

bool CPlayScene::IsInside(float Ox, float Oy, float xRange, float yRange, float tx, float ty)
{
	if (Ox <= tx && tx <= xRange && Oy <= ty && ty <= yRange)
		return true;
	return false;
}

void CPlayScene::Update(DWORD dt)
{
	CGame* game = CGame::GetInstance();

	if ((DWORD)GetTickCount64() - filming_start >= filming_duration && filming_start != 0)
	{
		filming_start = 0;
		game->setFilming(false);
	}

	if (MapCam.size() != 0 && camState < MapCam.size())
	{
		int sX, sY, eX, eY;

		MapCam.at(camState)->GetCam(sX, sY, eX, eY);

		game->SetStartEnd(sX, sY, eX, eY);
	}


	// We know that SOPHIA is the first object in the list hence we won't add him into the colliable object list
	// TO-DO: This is a "dirty" way, need a more organized way 

	// skip the rest if scene was already unloaded (SOPHIA::Update might trigger PlayScene::Unload)
	if (player == NULL && player2 == NULL && player3 == NULL) return;

	// Update camera to follow mario
	float cx, cy;

	if (player != NULL || player3 != NULL)
	{
		if (piloting)
		{
			player->GetPosition(cx, cy);
		}
		else
		{
			player3->GetPosition(cx, cy);
		}
	}
	else if(player2)
		player2->GetPosition(cx, cy);


	cy = cy;

	/*DebugOut(L"Y: la %d %f  \n", CGame::GetInstance()->GetCurrentScene()->getMapheight(), cy);*/
	
	

	cx -= game->GetScreenWidth() / 2;
	cy -= game->GetScreenHeight() / 2;

	if (!game->GetFilming())
	{
		if (cx + game->GetScreenWidth() > game->GetendX())
		{
			cx = game->GetendX() - game->GetScreenWidth();
		}
		if (cy + game->GetScreenHeight() > game->GetendY())
		{
			cy = game->GetendY() - game->GetScreenHeight();
		}
		if (cx < game->GetstartX())
		{
			cx = game->GetstartX();
		}
		if (cy < game->GetstartY())
		{
			cy = game->GetstartY();
		}
	}

	if (cx + game->GetScreenWidth() > game->GetMapX())
	{
		cx = game->GetMapX() - game->GetScreenWidth();
	}
	if (cy + game->GetScreenHeight() > game->GetMapY())
	{
		cy = game->GetMapY() - game->GetScreenHeight();
	}

	game->SetCamPos(cx, cy);

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

	if(player2)
	player2->Render();

	for (int i = 0; i < secondLayer.size(); i++)
		secondLayer[i]->Render();
	
}

/*
	Unload current scene
*/
void CPlayScene::Unload()
{
	objects.clear();

	MapCam.clear();

	secondLayer.clear();

	player = NULL;

	player2 = NULL;

	player3 = NULL;

	delete map;

	setCamState(0);

	map = nullptr;

	quadtree->Unload();

	quadtree = nullptr;

	delete quadtree;

	CGame* game = CGame::GetInstance();

	game->setFilming(false);

	DebugOut(L"[INFO] Scene %s unloaded! \n", sceneFilePath);
}

void CPlayScenceKeyHandler::OnKeyDown(int KeyCode)
{
	//DebugOut(L"[INFO] KeyDown: %d\n", KeyCode);
	CPlayScene* playscene = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene());

	if (playscene->GetPlayer() != NULL || playscene->GetPlayer3() != NULL)
	{
		if (playscene->getpiloting())
		{
			CSOPHIA* player = playscene->GetPlayer();
			switch (KeyCode)
			{
			case DIK_SPACE:
				if (!player->GetIsJumping())
				{
					player->SetState(SOPHIA_STATE_JUMP);
					player->SetIsJumping(true);
				}
				break;
			case DIK_B:
				player->Reset();
				break;
			case DIK_A:
				player->SetisFiring(true);
				break;
			case DIK_C:
				playscene->setCamState(playscene->getCamState() + 1);
				break;
			case DIK_UP:
				player->SetisAimingUp(true);
				break;
			}
		}
		else
		{
			MINI_JASON* player = playscene->GetPlayer3();
			switch (KeyCode)
			{
			case DIK_SPACE:
				if (!player->GetIsJumping())
				{
					player->SetState(SOPHIA_STATE_JUMP);
					player->SetIsJumping(true);
				}
				break;
			case DIK_B:
				player->Reset();
				break;
			case DIK_A:
				player->SetisFiring(true);
				break;
			case DIK_C:
				playscene->setCamState(playscene->getCamState() + 1);
				break;
			case DIK_UP:
				player->SetisAimingUp(true);
				break;
			}
		}
	}
	else {
		JASON* player = playscene->GetPlayer2();
		switch (KeyCode)
		{
		case DIK_B:
			player->Reset();
			break;
		case DIK_A:
			player->SetisFiring(true);
			break;
		}
	}
}

void CPlayScenceKeyHandler::OnKeyUp(int KeyCode)
{
	CGame* game = CGame::GetInstance();
	CPlayScene* playscene = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene());
	if (KeyCode == DIK_Z && playscene->GetPlayer3())
	{
		playscene->setpiloting(!playscene->getpiloting());
		MINI_JASON* player = playscene->GetPlayer3();
		//if (!playscene->getpiloting())
		{
			player->SetState(SOPHIA_STATE_JUMP);
			player->SetIsJumping(true);
		}
	}
	if (playscene->GetPlayer() != NULL || playscene->GetPlayer3() != NULL)
	{
		if (playscene->getpiloting())
		{
			CSOPHIA* player = playscene->GetPlayer();
			switch (KeyCode)
			{
			case DIK_A:
				player->SetisFiring(false);
				break;
			case DIK_R:
				CGame::GetInstance()->SwitchScene(2);
				break;
			case DIK_H:
				CGame::GetInstance()->SwitchScene(1);
				break;
			case DIK_UP:
				player->SetisAimingUp(false);
				break;
			case DIK_V:
				game->setheath(game->Getheath() - 100);
				break;
			}
		}
		else
		{
			MINI_JASON* player = playscene->GetPlayer3();
			switch (KeyCode)
			{
			case DIK_A:
				player->SetisFiring(false);
				break;
			case DIK_R:
				CGame::GetInstance()->SwitchScene(2);
				break;
			case DIK_H:
				CGame::GetInstance()->SwitchScene(1);
				break;
			case DIK_UP:
				player->SetisAimingUp(false);
				break;
			case DIK_V:
				game->setheath(game->Getheath() - 100);
				break;
			}
		}
	}
	else {
		JASON* player = playscene->GetPlayer2();
		switch (KeyCode)
		{
		case DIK_A:
			player->SetisFiring(false);
			break;
		case DIK_R:
			CGame::GetInstance()->SwitchScene(2);
			break;
		case DIK_H:
			CGame::GetInstance()->SwitchScene(1);
			break;
		}
	}
		
}

void CPlayScenceKeyHandler::KeyState(BYTE* states)
{
	CGame* game = CGame::GetInstance();
	CPlayScene* playscene = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene());
	if (playscene->GetPlayer() != NULL || playscene->GetPlayer3() != NULL)
	{
		if (playscene->getpiloting())
		{
			CSOPHIA* player = playscene->GetPlayer();
			if (player->GetState() == SOPHIA_STATE_DIE) return;
			if (game->IsKeyDown(DIK_RIGHT))
				player->SetState(SOPHIA_STATE_WALKING_RIGHT);
			else if (game->IsKeyDown(DIK_LEFT))
				player->SetState(SOPHIA_STATE_WALKING_LEFT);
			else
				player->SetState(SOPHIA_STATE_IDLE);
		}
		else
		{
			MINI_JASON* player = playscene->GetPlayer3();
			if (player->GetState() == SOPHIA_STATE_DIE) return;
			if (game->IsKeyDown(DIK_RIGHT))
				player->SetState(SOPHIA_STATE_WALKING_RIGHT);
			else if (game->IsKeyDown(DIK_LEFT))
				player->SetState(SOPHIA_STATE_WALKING_LEFT);
			else
				player->SetState(SOPHIA_STATE_IDLE);
		}
	}
	else {
		JASON* player = playscene->GetPlayer2();
		if (player->GetState() == SOPHIA_STATE_DIE) return;
		if (game->IsKeyDown(DIK_RIGHT))
			player->SetState(SOPHIA_STATE_WALKING_RIGHT);
		else if (game->IsKeyDown(DIK_LEFT))
			player->SetState(SOPHIA_STATE_WALKING_LEFT);
		else if (game->IsKeyDown(DIK_DOWN))
			player->SetState(SOPHIA_STATE_WALKING_DOWN);
		else if (game->IsKeyDown(DIK_UP))
			player->SetState(SOPHIA_STATE_WALKING_UP);
		else if (game->IsKeyDown(DIK_Q))
			player->SwitchWeapon();
		else
			player->SetState(SOPHIA_STATE_IDLE);
	}


}