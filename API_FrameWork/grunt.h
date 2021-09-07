#pragma once
#include "enemy.h"
#include "gruntPunch.h"
class grunt : public enemy
{
private:
	bool _isLaser;
	bool _isStair;
	Vec2 _DownPoint;
	Vec2 _UpPoint;

	float _goDownAngle;

	bool playerisDownside;

	float maxgravity=8.f;

	gruntPunch* _gP;
public:
	grunt(bool isLaser, bool isStair = false, Vec2 downPoint = { 0, 0 }, Vec2 upPoint = { 0, 0 });
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
	void checkRoute();
	void doorCollision();
	void laserCollision();
	void laserDie();
	void attack();

	virtual void setCollider();
};

