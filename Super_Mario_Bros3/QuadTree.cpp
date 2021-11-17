#include "PlayScene.h"
#define OBJECT_TYPE_BRICK	1
#define OBJECT_TYPE_GOOMBA	2
#define OBJECT_TYPE_KOOPAS	3

#define OBJECT_TYPE_PORTAL	50

#define MAX_SCENE_LINE 1024

CQuadTree::CQuadTree(LPCWSTR filePath)
{
	Load(filePath);
}
CQuadTree::CQuadTree(float cellWidth, float cellHeight, float x, float y) {
	this->cellHeight = cellHeight;
	this->cellWidth = cellWidth;
	this->x = x;
	this->y = y;
}

void CQuadTree::Plit() {
	if (listObjects.size() == 0)
		return;
	CGame* game = CGame::GetInstance();
	if (cellHeight > game->GetScreenHeight() / 2 || cellWidth > game->GetScreenWidth() / 2)
	{
		BrachTL = new CQuadTree(cellWidth / 2, cellHeight / 2, x, y);
		BrachTR = new CQuadTree(cellWidth / 2, cellHeight / 2, x + cellWidth / 2, y);
		BrachBL = new CQuadTree(cellWidth / 2, cellHeight / 2, x, y + cellHeight / 2);
		BrachBR = new CQuadTree(cellWidth / 2, cellHeight / 2, x + cellWidth / 2, y + cellHeight / 2);
		ObjectPlit(BrachTL);
		ObjectPlit(BrachTR);
		ObjectPlit(BrachBL);
		ObjectPlit(BrachBR);
		BrachTL->Plit();
		BrachTR->Plit();
		BrachBL->Plit();
		BrachBR->Plit();
		listObjects.clear();
		return;
	}
	isLeaf = true;
	DebugOut(L"LEAF : %d %f %f %f %f \n", listObjects.size(), cellWidth, cellHeight, x, y);
	return;
}

bool CQuadTree::inRange(float ox, float oy, float x, float y, float width, float height)
{
	if (x <= ox && ox <= x + width && y <= oy && oy <= y + height)
		return true;
	return false;
}
void CQuadTree::ObjectPlit(CQuadTree* brach)
{
	int SL = listObjects.size();
	for (int i = 0; i < SL; i++)
	{
		if (brach->inRange(listObjects[i]->GetPositionX(), listObjects[i]->GetPositionY(), brach->x, brach->y, brach->cellWidth, brach->cellHeight))
		{
			brach->Add(listObjects[i]);
		}

	}
}

void CQuadTree::_ParseSection_SETTINGS(string line)
{
	vector<string> tokens = split(line);
	DebugOut(L"--> %s\n", ToWSTR(line).c_str());

	if (tokens.size() < 2) return; // skip invalid lines

	cellWidth = atoi(tokens[0].c_str());
	cellHeight = atoi(tokens[1].c_str());
}

void CQuadTree::_ParseSection_OBJECTS(string line)
{
	vector<string> tokens = split(line);
	DebugOut(L"--> %s\n", ToWSTR(line).c_str());

	CAnimationSets* animation_sets = CAnimationSets::GetInstance();

	if (tokens.size() < 4) return; // skip invalid lines

	int x = atoi(tokens[1].c_str());
	int y = atoi(tokens[2].c_str());

	/*int cellX = atoi(tokens[5].c_str());
	int cellY = atoi(tokens[6].c_str());*/

	int object_type = atoi(tokens[0].c_str());

	int ani_set_id = atoi(tokens[3].c_str());

	/*int renderLayer = atoi(tokens[4].c_str());*/

	CGameObject* obj = NULL;

	switch (object_type)
	{
	case OBJECT_TYPE_GOOMBA: obj = new CGoomba(); break;
	case OBJECT_TYPE_BRICK: obj = new CBrick(); break;
	case OBJECT_TYPE_KOOPAS: obj = new CKoopas(); break;
	default:
		DebugOut(L"[ERR] Invalid object type: %d\n", object_type);
		return;
	}

	// General object setup
	LPANIMATION_SET ani_set = animation_sets->Get(ani_set_id);
	if (obj != NULL)
	{
		int add = 0;
		obj->SetPosition((float)x, (float)y);
		obj->SetAnimationSet(ani_set);
		obj->SetOrigin((float)x, (float)y, obj->GetState());
		/*obj->SetrenderLayer(renderLayer);*/
		Add(obj);
	}

}

void CQuadTree::Load(LPCWSTR filePath)
{
	DebugOut(L"[INFO] Start loading quadtree resources from : %s \n", filePath);

	ifstream f;
	f.open(filePath);

	// current resource section flag
	int section = 0;

	char str[MAX_GRID_LINE];
	while (f.getline(str, MAX_GRID_LINE))
	{
		string line(str);

		if (line[0] == '#') continue;	// skip comment lines	

		if (line == "[SETTINGS]") {
			section = GRID_SECTION_SETTINGS; continue;
		}
		if (line == "[OBJECTS]") {
			section = GRID_SECTION_OBJECTS; continue;
		}
		//
		// data section
		//
		switch (section)
		{
		case GRID_SECTION_SETTINGS: _ParseSection_SETTINGS(line); break;
		case GRID_SECTION_OBJECTS: _ParseSection_OBJECTS(line); break;
		}
	}

	f.close();
	Plit();
	DebugOut(L"[INFO] Done loading scene resources %s\n", filePath);
}

void CQuadTree::Render()
{
	for (int i = 0; i < listObjects.size(); i++)
		listObjects[i]->Render();
}

void CQuadTree::GetObjects(vector<LPGAMEOBJECT>& listObject, int CamX, int CamY)
{
	//listObject.clear();

	unsigned int  left, top, right, bottom;

	left = (CamX);

	right = (CamX + CGame::GetInstance()->GetScreenWidth());

	top = (CamY);

	bottom = (CamY + CGame::GetInstance()->GetScreenHeight());

	if (left > x + cellWidth || right < x || top > y + cellHeight || bottom < y)
		return;

	Pop(listObject, CamX, CamY);
}

void CQuadTree::Pop(vector<LPGAMEOBJECT>& listObject, int CamX, int CamY)
{
	unsigned int  left, top, right, bottom;

	left = (CamX);

	right = (CamX + CGame::GetInstance()->GetScreenWidth());

	top = (CamY);

	bottom = (CamY + CGame::GetInstance()->GetScreenHeight());

	if (this == NULL)
		return;
	if (isLeaf)
	{
		for (int i = 0; i < listObjects.size(); i++)
		{
			if (inRange(x + cellWidth, y + cellHeight, CamX, CamY, CGame::GetInstance()->GetScreenWidth(), CGame::GetInstance()->GetScreenHeight()))
			if (!listObjects[i]->GetActive())
			{
				float Ox, Oy;
				listObjects[i]->GetOriginLocation(Ox, Oy);
				if (!inRange(Ox, Oy, CamX, CamY, CGame::GetInstance()->GetScreenWidth(), CGame::GetInstance()->GetScreenHeight()))
					listObjects[i]->reset();
				listObject.push_back(listObjects[i]);
				listObjects[i]->SetActive(true);
			}
		}
		return;
	}

	BrachTL->Pop(listObject, CamX, CamY);
	BrachTR->Pop(listObject, CamX, CamY);
	BrachBL->Pop(listObject, CamX, CamY);
	BrachBR->Pop(listObject, CamX, CamY);
		
}

void CQuadTree::Unload()
{
	//if (this != nullptr)
	//	if (cells)
	//	{
	//		for (unsigned int i = 0; i < numCol; i++)
	//		{
	//			for (unsigned int j = 0; j < numRow; j++)
	//			{
	//				cells[i][j].Unload();
	//			}
	//		}
	//		delete cells;
	//		cells = NULL;
	//	}
}