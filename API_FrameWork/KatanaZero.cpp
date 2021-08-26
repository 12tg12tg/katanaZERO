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

    dropFrame();
}

void KatanaZero::render()
{
    PLAYER->render();



    TCHAR str[128];
    _stprintf(str, "¹è¼Ó : %.1f", TIME->getGameTimeRate());
    ZORDER->UITextOut(str, ZUIFIRST, 0, 0, RGB(0, 0, 0));

    ZORDER->ZorderTotalRender(getMemDC());
    ZORDER->ZorderUITotalRender(getMemDC());
}

void KatanaZero::dropFrame()
{
    if (INPUT->isStayKeyDown(VK_LSHIFT)) {
        TIME->setGameTimeRate(TIME->getGameTimeRate() - 0.05f);
        if (TIME->getGameTimeRate() < 0.3f) {
            TIME->setGameTimeRate(0.3f);
        }
    }
    if (INPUT->isOnceKeyUp(VK_LSHIFT)) {
        TIME->setGameTimeRate(1.0f);
    }
}
