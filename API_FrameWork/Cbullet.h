#pragma once
#include "gameNode.h"
class Cbullet : public gameNode
{
protected:
	vector<tagBullet> _vBullet;
	vector<tagBullet>::iterator _viBullet;

public:
	Cbullet();
	~Cbullet();

	void fire(float x, float y);
	void release();
	void update();
	void render();

	vector<tagBullet> getVBullet() { return _vBullet; }
};

