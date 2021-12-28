#pragma once
class CCamera
{
	int cam_x = 0.0f;
	int cam_y = 0.0f;

public:

	void SetCamPos(int x, int y) { cam_x = x; cam_y = y; }

	int GetCamX()
	{
		return cam_x;
	}
	int GetCamY()
	{
		return cam_y;
	}
};

