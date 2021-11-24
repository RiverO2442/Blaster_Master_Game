#pragma once
class CCamera
{
	float cam_x = 0.0f;
	float cam_y = 0.0f;

public:

	void SetCamPos(float x, float y) { cam_x = x; cam_y = y; }

	int GetCamX()
	{
		return cam_x;
	}
	int GetCamY()
	{
		return cam_y;
	}
};

