#pragma once
#include "Cbullet.h"
class playerSlash : public Cbullet
{
private:

public:
	playerSlash();
	~playerSlash();

	void fire(float centerx, float centery, float angle);
	void release();
	void update();
	void render();

};

