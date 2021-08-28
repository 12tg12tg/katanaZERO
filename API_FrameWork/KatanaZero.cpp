#include "framework.h"
#include "KatanaZero.h"

KatanaZero::KatanaZero()
{
    IMAGE->addImage("fadeImg", WINSIZEX, WINSIZEY);
    SetBkMode(getMemDC(), TRANSPARENT);
    ShowCursor(false);
}

KatanaZero::~KatanaZero()
{
}

HRESULT KatanaZero::init()
{
    PLAYER->init();
    m_ui = new UI;
    m_ui->init();





    CAMERA->init(PLAYER->getX(), PLAYER->getY(), WINSIZEX*2, WINSIZEY*2, 0, 0, WINSIZEX / 2, WINSIZEY / 2,
        CAMERASIZEX, CAMERASIZEY);

    m_debugRc = RectMake(1110, 79, 220, 200);
    _slowAlpha = 0;
    return S_OK;
}

void KatanaZero::release()
{
    PLAYER->release();
    PLAYER->releaseSingleton();
    SAFE_DELETE(m_ui);
}

void KatanaZero::update()
{
    PLAYER->update();
    m_ui->update();
    ANIMATION->update();

    dropFrame();    //���ο���


    CAMERA->movePivot(PLAYER->getX(), PLAYER->getY());
    CAMERA->update();
}

void KatanaZero::render()
{
    if (_isDebug) {
        ZORDER->UIRectangleColor(m_debugRc, ZUIFIRST, MINT);

        string str = "";
        str = "��� :" + to_string(TIME->getGameTimeRate()) + "\n";
        str += "���� : " + to_string(_slowAlpha) + "\n";
        str += "���콺��ġ : " + to_string(m_ptMouse.x) + ", " + to_string(m_ptMouse.y) + "\n";
        str += "Ŭ�󳻸��콺 : " + to_string(CAMERA->getClientMouse().x) + ", " + to_string(CAMERA->getClientMouse().y) + "\n";
        str += "��üDC���� ���콺��ġ : \n��������������" +
            to_string(CAMERA->getRelativeMouse().x) + ", " +
            to_string(CAMERA->getRelativeMouse().y) + "\n";
        str += "getFloat = " + to_string(RND->getFloatFromTo(10,20)) + "\n";
        
        ZORDER->UIDrawText(str, ZUISECOND, m_debugRc,
            CreateFont(15, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET,
                0, 0, 0, VARIABLE_PITCH | FF_ROMAN, TEXT("����")),
                RGB(0, 0, 0), DT_LEFT | DT_VCENTER); 
    }
   
    PLAYER->render();
    m_ui->render();










    ZORDER->ZorderAlphaRender(IMAGE->findImage("fadeImg"), ZUNIT-0.5, 0, 0, 0, _slowAlpha);
    ZORDER->ZorderTotalRender(getMemDC());
    ZORDER->ZorderUITotalRender(getMemDC());

    //SetPixel(getMemDC(), 100, 100, RGB(0, 0, 0));
    //SetPixel(getMemDC(), 500, 500, RGB(0, 0, 0));
}

void KatanaZero::dropFrame()
{
    if (INPUT->isStayKeyDown(VK_LSHIFT)) {
        TIME->setGameTimeRate(TIME->getGameTimeRate() - 0.05f);
        if (TIME->getGameTimeRate() < 0.3f) {
            TIME->setGameTimeRate(0.3f);
        }
        _isSlow = true;
    }
    if (INPUT->isOnceKeyUp(VK_LSHIFT)) {
        TIME->setGameTimeRate(1.0f);
        _isSlow = false;
    }

    if (_isSlow) {
        _slowAlpha += 10;
        if (_slowAlpha > 200)_slowAlpha = 200;
    }
    else if(_slowAlpha > 0) {
		_slowAlpha -= 5;
		if (_slowAlpha < 0)_slowAlpha = 0;

    }
}
