#pragma once
#include "Cbullet.h"
class bossNormalBullet : public Cbullet
{
private:
	vector<tagBullet> _vReflect;
	vector<tagBullet>::iterator _viReflect;
public:
	bossNormalBullet();
	~bossNormalBullet();

	void fire(float x, float y, float angle);
	void fire2(float x, float y, float angle);
	void release();
	void update();
	void render();

	void updateforEnemyBullet();
	void updateforPlayerBullet();
	void renderforEnemyBullet();
	void renderforPlayerBullet();
};

