#pragma once
#include "gameNode.h"
#include "bossNormalBullet.h"
#include "verlaser.h"
#include "sweeplaser.h"
class bossBulletManater : public gameNode
{
private:
	bossNormalBullet* _normal;
	verlaser* _verLaser;
	sweeplaser* _sweepLaser;
public:
	bossBulletManater();
	~bossBulletManater();

	HRESULT init();
	void release();
	void update();
	void render();

	bossNormalBullet* getNormalBullet() { return _normal; }
	verlaser* getVerLaser() { return _verLaser; }
	sweeplaser* getSweepLaser() { return _sweepLaser; }

};

