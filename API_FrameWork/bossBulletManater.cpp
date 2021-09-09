#include "framework.h"
#include "bossBulletManater.h"

bossBulletManater::bossBulletManater()
{
}

bossBulletManater::~bossBulletManater()
{
}

HRESULT bossBulletManater::init()
{
    _normal = new bossNormalBullet;
    _verLaser = new verlaser;
    _sweepLaser = new sweeplaser;
    _holLaser = new horiLaser;
    _dashAtk = new dashAtk;
    _cirBomb = new circleBomb;
    return S_OK;
}

void bossBulletManater::release()
{
    _normal->release();
    _verLaser->release();
    _sweepLaser->release();
    _holLaser->release();
    _dashAtk->release();
    _cirBomb->release();
    SAFE_DELETE(_normal);
    SAFE_DELETE(_verLaser);
    SAFE_DELETE(_sweepLaser);
    SAFE_DELETE(_holLaser);
    SAFE_DELETE(_dashAtk);
    SAFE_DELETE(_cirBomb);
}

void bossBulletManater::update()
{
    _normal->update();
    _verLaser->update();
    _sweepLaser->update();
    _holLaser->update();
    _dashAtk->update();
    _cirBomb->update();
}

void bossBulletManater::render()
{
    _normal->render();
    _verLaser->render();
    _sweepLaser->render();
    _holLaser->render();
    _dashAtk->render();
    _cirBomb->render();
}
