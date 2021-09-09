#pragma once
#include "gameNode.h"
#include "bossNormalBullet.h"
#include "verlaser.h"
#include "sweeplaser.h"
#include "horiLaser.h"
#include "dashAtk.h"
#include "circleBomb.h"
class bossBulletManater : public gameNode
{
private:
	bossNormalBullet* _normal;
	verlaser* _verLaser;
	sweeplaser* _sweepLaser;
	horiLaser* _holLaser;
	dashAtk* _dashAtk;
	circleBomb* _cirBomb;
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
	horiLaser* getholLaser() { return _holLaser; }
	dashAtk* getDashAtk() {	return _dashAtk; }
	circleBomb* getCirBomb() {	return _cirBomb; }

};
