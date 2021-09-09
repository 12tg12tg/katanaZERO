#pragma once
#include "Cbullet.h"
#include "explosion.h"
class circleBomb : public Cbullet
{
private:
	explosion* _explo;

	vector<tagBullet> _reflect;
	vector<tagBullet>::iterator _ireflect;
public:
	circleBomb();
	~circleBomb();

	void fire(float x, float y, FOWARD foward);
	void fire2(float x, float y);
	void release();
	void update();
	void render();

	void updateForEnemy();
	void updateForPlayer();
	void renderForEnemy();
	void renderForPlayer();
};

