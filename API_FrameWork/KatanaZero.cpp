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

    //------------------------------------------------------------------------------------------------
    _test = dynamic_cast<CollisionTestScene*>(SCENE->addScene("충돌테스트", new CollisionTestScene));
    SCENE->changeScene("충돌테스트");
    //------------------------------------------------------------------------------------------------


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

    dropFrame();    //슬로우기능
    //---------------------------
    SCENE->update();
    //---------------------------

    CAMERA->movePivot(PLAYER->getX(), PLAYER->getY());
    CAMERA->update();
}

void KatanaZero::render()
{
    if (_isDebug) {
        ZORDER->UIRectangleColor(m_debugRc, ZUIFIRST, MINT);



        Vec2 temp1(4, 5);
        Vec2 temp2(10, 0);
        temp1.Distance(temp2);
        Vec2 res = temp1.Project(temp2);

        string str = "";
        str = "배속 :" + to_string(TIME->getGameTimeRate()) + "\n";
        str += "알파 : " + to_string(_slowAlpha) + "\n";
        str += "마우스위치 : " + to_string(m_ptMouse.x) + ", " + to_string(m_ptMouse.y) + "\n";
        str += "클라내마우스 : " + to_string(CAMERA->getClientMouse().x) + ", " + to_string(CAMERA->getClientMouse().y) + "\n";
        str += "전체DC내부 마우스위치 : \n　　　　　　　" +
            to_string(CAMERA->getRelativeMouse().x) + ", " +
            to_string(CAMERA->getRelativeMouse().y) + "\n";
        str += "vectorTest : " + to_string(res.x)+", " + to_string(res.y) + "\n";
        //str += "vectorTest : " + to_string(temp1.Projection(temp2)) + "\n";
        
        ZORDER->UIDrawText(str, ZUISECOND, m_debugRc,
            CreateFont(15, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET,
                0, 0, 0, VARIABLE_PITCH | FF_ROMAN, TEXT("굴림")),
                RGB(0, 0, 0), DT_LEFT | DT_VCENTER); 
    }
   
    PLAYER->render();
    m_ui->render();






    //---------------------------
    SCENE->render();
    //---------------------------



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
