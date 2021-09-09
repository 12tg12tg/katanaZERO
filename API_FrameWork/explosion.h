#pragma once
#include "Cbullet.h"
class explosion :public Cbullet
{
private:

public:
	explosion();
	~explosion();

	void fire(float centerX, float centerY, COLLIDER_TYPE type);
	void release();
	void update();
	void render();

};

