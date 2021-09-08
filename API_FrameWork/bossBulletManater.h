#pragma once
#include "gameNode.h"
#include "bossNormalBullet.h"
class bossBulletManater : public gameNode
{
private:
	bossNormalBullet* _normal;
public:
	bossBulletManater();
	~bossBulletManater();

	HRESULT init();
	void release();
	void update();
	void render();

	bossNormalBullet* getNormalBullet() { return _normal; }
};

