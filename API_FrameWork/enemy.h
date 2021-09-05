#pragma once
#include "gameNode.h"
class enemy : public gameNode
{
protected:
	vector<tagEnemy> _vEnemy;
	vector<tagEnemy>::iterator _viEnemy;
public:
	enemy();
	~enemy();

	virtual void release();
	virtual void update();
	virtual void render();

	virtual void addEnemy(float x, float y);

	vector<tagEnemy> getVector() { return _vEnemy; }
};

