#pragma once
#include "Cbullet.h"
class horiLaser : public Cbullet
{
private:


public:
	horiLaser();
	~horiLaser();

	void fire(float x, float y, FOWARD foward);
	void release();
	void update();
	void render();

};

