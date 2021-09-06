#pragma once
#include "Cbullet.h"
class gruntPunch : public Cbullet
{
private:

public:
	gruntPunch();
	~gruntPunch();

	void fire(float centerx, float centery, float angle);
	void release();
	void update();
	void render();
};

