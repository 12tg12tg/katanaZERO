#pragma once
#include "Cbullet.h"
class verlaser : public Cbullet
{
private:


public:
	verlaser();
	~verlaser();

	void fire(float x, float y);
	void release();
	void update();
	void render();

};

