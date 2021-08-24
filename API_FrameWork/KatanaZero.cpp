#include "framework.h"
#include "KatanaZero.h"

KatanaZero::KatanaZero()
{
}

KatanaZero::~KatanaZero()
{
}

HRESULT KatanaZero::init()
{
    PLAYER->init();

    return S_OK;
}

void KatanaZero::release()
{
    PLAYER->release();
    PLAYER->releaseSingleton();
}

void KatanaZero::update()
{
    PLAYER->update();
    ANIMATION->update();
}

void KatanaZero::render()
{
    PLAYER->render();
    ZORDER->ZorderTotalRender(getMemDC());
    ZORDER->ZorderUITotalRender(getMemDC());
}
