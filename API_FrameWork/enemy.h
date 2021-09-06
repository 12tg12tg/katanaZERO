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


	virtual void addEnemy(float x, float y);
	virtual void release();
	virtual void update();
	virtual void render();

	virtual void move();
	virtual void giveFrame();
	virtual void deathCheck();
	virtual void findPlayer();
	virtual void setCollider();
	virtual void getgravity();

	vector<tagEnemy>& getVector() { return _vEnemy; }
};