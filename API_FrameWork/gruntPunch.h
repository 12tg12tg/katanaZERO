#pragma once
#include "Cbullet.h"
class gruntPunch : public Cbullet
{
private:

public:
	gruntPunch();
	~gruntPunch();

	void fire(float x, float y, FOWARD foward);
	void release();
	void update();
	void render();
};

