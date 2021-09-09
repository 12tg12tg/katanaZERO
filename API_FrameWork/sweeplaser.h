#pragma once
#include "Cbullet.h"
#define DISTANCE 60
class sweeplaser : public Cbullet
{
private:

public:
	sweeplaser();
	~sweeplaser();

	void fire(float x, float y, FOWARD foward);
	void release();
	void update();
	void render();

};

