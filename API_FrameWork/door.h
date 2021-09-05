#pragma once
#include "enemy.h"
class door : public enemy
{
private:

public:
	door();
	~door();

	virtual void addEnemy(float x, float y);
	virtual void release();
	virtual void update();
	virtual void render();

};

