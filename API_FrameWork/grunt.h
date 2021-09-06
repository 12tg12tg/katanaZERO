#pragma once
#include "enemy.h"
class grunt : public enemy
{
private:
	bool _isLaser;
	bool _isStair;
	Vec2 _checkPoint;

	float maxgravity=8.f;
public:
	grunt(bool isLaser, bool isStair = false, Vec2 checkPoint = { 0, 0 });
	~grunt();

	virtual void addEnemy(float x, float y, FOWARD foward, Vec2 patrolpoint);
	virtual void release();
	virtual void update();
	virtual void render();

	virtual void move();
	virtual void giveFrame();
	virtual void deathCheck();
	virtual void findPlayer();
	virtual void deathMove();
	virtual void getgravity();

	virtual void setCollider();
};

