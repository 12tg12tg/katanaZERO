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

    _state = MAINSTATE::INGAME;
    _caretaker = new Caretaker;
    _caretaker->init();

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
    MAIN->releaseSingleton();
    SAFE_DELETE(m_ui);
    SAFE_DELETE(_caretaker);

}

void KatanaZero::update()
{
    MAIN->setIsSlow(_isSlow);
    MAIN->setMainState(_state);

    switch (_state)
    {
    case MAINSTATE::TITLE:
        break;
    case MAINSTATE::LOADING:
        break;
    case MAINSTATE::INGAME:
    {
        dropFrame();            //슬로우기능
        PLAYER->update();
        m_ui->update();
        ANIMATION->update();
        SCENE->update();
        CAMERA->movePivot(PLAYER->getX(), PLAYER->getY());
        CAMERA->update();
        _caretaker->snapshot();

        if (INPUT->isOnceKeyDown(VK_LEFT)) {
            _state = MAINSTATE::REPLAY;
        }
        if (INPUT->isOnceKeyDown(VK_RIGHT)) {
            _state = MAINSTATE::ROLLBACK;
        }
    }
        break;
    case MAINSTATE::REPLAY:
    {
        m_ui->update();
        _caretaker->replay();
        if (_caretaker->getReplayDone()) {
            _caretaker->setReplayDone(false);
            _caretaker->allVectorClear();
            _state = MAINSTATE::INGAME;
        }
    }
        break;
    case MAINSTATE::ROLLBACK:
    {
        m_ui->update();
        _caretaker->rollback();
        if (_caretaker->getRollbackDone()) {
            _caretaker->setRollbackDone(false);
            _caretaker->allVectorClear();
            _state = MAINSTATE::INGAME;
        }
    }
        break;
    case MAINSTATE::PAUSE:
        break;
    case MAINSTATE::NONE:
        break;
    }
}

void KatanaZero::render()
{
    if (_isDebug) {
        ZORDER->UIRectangleColor(m_debugRc, ZUIFIRST, MINT);


        //----------------------------------------------
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
        //----------------------------------------------
    }

    switch (_state)
    {
    case MAINSTATE::TITLE:
        break;
    case MAINSTATE::LOADING:
        break;
    case MAINSTATE::INGAME:
    {
        PLAYER->render();
        m_ui->render();
        SCENE->render();
        ZORDER->ZorderAlphaRender(IMAGE->findImage("fadeImg"), ZUNIT - 0.5, 0, 0, 0, _slowAlpha);   //슬로우
    }
        break;
    case MAINSTATE::REPLAY:
    {
        PLAYER->render();
        m_ui->render();
        SCENE->render();
    }
        break;
    case MAINSTATE::ROLLBACK:
    {
        PLAYER->render();
        m_ui->render();
        SCENE->render();
    }
        break;
    case MAINSTATE::PAUSE:
        break;
    case MAINSTATE::NONE:
        break;
    }

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
