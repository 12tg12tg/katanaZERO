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

    return S_OK;
}

void bossBulletManater::release()
{
    _normal->release();
    SAFE_DELETE(_normal);
}

void bossBulletManater::update()
{
    _normal->update();
}

void bossBulletManater::render()
{
    _normal->render();
}
