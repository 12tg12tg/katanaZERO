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
    return S_OK;
}

void bossBulletManater::release()
{
    _normal->release();
    _verLaser->release();
    _sweepLaser->release();
    SAFE_DELETE(_normal);
    SAFE_DELETE(_verLaser);
    SAFE_DELETE(_sweepLaser);
}

void bossBulletManater::update()
{
    _normal->update();
    _verLaser->update();
    _sweepLaser->update();
}

void bossBulletManater::render()
{
    _normal->render();
    _verLaser->render();
    _sweepLaser->render();
}
