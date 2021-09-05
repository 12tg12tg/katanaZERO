#pragma once
#include "gameNode.h"
#include "Cbullet.h"
class laser :public Cbullet
{
private:

	bool _laserOn;
public:
	laser();
	~laser();

	void fire(float x, float y, float length);
	void release();
	void update();
	void render();

	void laserOnOff();
	bool getlaserOn() { return _laserOn; };
};

