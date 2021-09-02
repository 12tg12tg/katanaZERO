#include "framework.h"
#include "KatanaZero.h"
#include "PlayerState.h"
KatanaZero::KatanaZero()
{
    _cameraBuffer = IMAGE->addImage("cameraBuffer", WINSIZEX*2, WINSIZEY*2);
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

    sceneInit();
    collisionInit();

    _state = MAINSTATE::INGAME;
    _caretaker = new Caretaker;
    _caretaker->init();

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
    //외부에서 상태를 변경했는가
    if (MAIN->getChangeState() != MAINSTATE::NONE) {
        _state = MAIN->getChangeState();
        MAIN->changeMainState(MAINSTATE::NONE);
    }
    //전역에 상태정보 입력
    MAIN->setIsSlow(_isSlow);
    MAIN->setMainState(_state);

    //상태에맞는 update
    switch (_state)
    {
    case MAINSTATE::TITLE:
        break;
    case MAINSTATE::LOADING:
        break;
    case MAINSTATE::INGAME:
    {
        dropFrame();            //슬로우기능
        m_ui->update();
        SCENE->update();
        _caretaker->snapshot();
        if (INPUT->isOnceKeyDown('N')) {        /*연출테스트*/
            _state = MAINSTATE::REPLAY;
        }
        if (INPUT->isOnceKeyDown('M')) {
            _state = MAINSTATE::ROLLBACK;
        }
    }
        break;
    case MAINSTATE::REPLAY:
    {
        //m_ui->update();
        _caretaker->replay();
        CAMERA->update();
        if (_caretaker->getReplayDone()) {
            _caretaker->setReplayDone(false);
            _caretaker->allVectorClear();
            _state = MAINSTATE::INGAME;
        }
    }
        break;
    case MAINSTATE::ROLLBACK:
    {
        //m_ui->update();
        _caretaker->rollback();
        CAMERA->update();
        if (_caretaker->getRollbackDone()) {
            _caretaker->setRollbackDone(false);
            _caretaker->allVectorClear();
            _state = MAINSTATE::INGAME;
            PLAYER->getFSM()->ChangeState(PLAYERSTATE::IDLE);
            SCENE->curScene()->init();
        }
    }
        break;
    case MAINSTATE::PAUSE:
        break;
    case MAINSTATE::NONE:
        break;
    }
    SOUND->update();
}

void KatanaZero::render()
{
    PatBlt(_cameraBuffer->getMemDC(), 0, 0, _cameraBuffer->getWidth(), _cameraBuffer->getHeight(), BLACKNESS);
    //--------------------------------------------------------------------------
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
        str += "Player x, y: " + to_string(PLAYER->getX()) + ", " + to_string(PLAYER->getY()) + "\n";
        
        ZORDER->UIDrawText(str, ZUISECOND, m_debugRc,
            CreateFont(15, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET,
                0, 0, 0, VARIABLE_PITCH | FF_ROMAN, TEXT("굴림")),
                RGB(0, 0, 0), DT_LEFT | DT_VCENTER); 
        //----------------------------------------------
		//SetPixel(getMemDC(), WINSIZEX / 2 - 1, 246, RGB(0, 0, 0));
		//SetPixel(getMemDC(), WINSIZEX / 2, 246, RGB(0, 0, 0));
		//SetPixel(getMemDC(), WINSIZEX / 2 + 1, 246, RGB(0, 0, 0));

		//SetPixel(getMemDC(), WINSIZEX / 2 - 1, 343, RGB(0, 0, 0));
		//SetPixel(getMemDC(), WINSIZEX / 2, 343, RGB(0, 0, 0));
		//SetPixel(getMemDC(), WINSIZEX / 2 + 1, 343, RGB(0, 0, 0));

		//SetPixel(getMemDC(), WINSIZEX / 2, WINSIZEY / 2 + 50 - 1, RGB(0, 0, 0));
		//SetPixel(getMemDC(), WINSIZEX / 2, WINSIZEY / 2 + 50, RGB(0, 0, 0));
		//SetPixel(getMemDC(), WINSIZEX / 2, WINSIZEY / 2 + 50 + 1, RGB(0, 0, 0));

		//SetPixel(getMemDC(), 976, WINSIZEY / 2 + 50 - 1, RGB(0, 0, 0));
		//SetPixel(getMemDC(), 976, WINSIZEY / 2 + 50, RGB(0, 0, 0));
		//SetPixel(getMemDC(), 976, WINSIZEY / 2 + 50 + 1, RGB(0, 0, 0));
    }

    switch (_state)
    {
    case MAINSTATE::TITLE:
        break;
    case MAINSTATE::LOADING:
        break;
    case MAINSTATE::INGAME:
    {
        m_ui->render();
        SCENE->render();
        if(_isSlow || _slowAlpha > 0) ZORDER->ZorderAlphaRender(IMAGE->findImage("fadeImg"), ZSLOWFADE, 0, CAMERA->getRect().left, CAMERA->getRect().top, _slowAlpha);   //슬로우
    }
        break;
    case MAINSTATE::REPLAY:
    {
        //m_ui->render();
        image* bwmap = dynamic_cast<Cmap*>(SCENE->curScene())->getBwmap();
        image* bwmap_front = dynamic_cast<Cmap*>(SCENE->curScene())->getBwmap_front();
        ZORDER->ZorderRender(bwmap, ZFLOORMAP, 0, 0, 0);
        if(bwmap_front) ZORDER->ZorderRender(bwmap_front, ZABOVEMAP, 0, 0, 0);
    }
        break;
    case MAINSTATE::ROLLBACK:
    {
        //m_ui->render();
        image* map = dynamic_cast<Cmap*>(SCENE->curScene())->getMap();
        image* map_front = dynamic_cast<Cmap*>(SCENE->curScene())->getMap_front();
        ZORDER->ZorderRender(map, ZFLOORMAP, 0, 0, 0);
        if (map_front) ZORDER->ZorderRender(map_front, ZABOVEMAP, 0, 0, 0);
    }
        break;
    case MAINSTATE::PAUSE:
        break;
    case MAINSTATE::NONE:
        break;
    }
    //--------------------------------------------------------------------------
    ZORDER->ZorderTotalRender(_cameraBuffer->getMemDC());
    _cameraBuffer->render(getMemDC(), 0, 0, CAMERA->getRect().left, CAMERA->getRect().top, RecWidth(CAMERA->getRect()), RecHeight(CAMERA->getRect()));
    ZORDER->ZorderUITotalRender(getMemDC());
}

void KatanaZero::sceneInit()
{
    //------------------------------------------------------------------------------------------------
    _test = dynamic_cast<CollisionTestScene*>(SCENE->addScene("충돌테스트", new CollisionTestScene));
    _testmap1 = dynamic_cast<textMap*>(SCENE->addScene("테스트맵1", new textMap));
    _testmap2 = dynamic_cast<textMap2*>(SCENE->addScene("테스트맵2", new textMap2));
    _testmap3 = dynamic_cast<textMap3*>(SCENE->addScene("테스트맵3", new textMap3));
    SCENE->changeScene("테스트맵3");
    //------------------------------------------------------------------------------------------------
}

void KatanaZero::collisionInit()
{
    COLLISION->CollisionCheck(COLLIDER_TYPE::PLAYER_UNIT, COLLIDER_TYPE::POTAL);
    COLLISION->CollisionCheck(COLLIDER_TYPE::PLAYER_UNIT, COLLIDER_TYPE::BULLET_ENEMY);
    COLLISION->CollisionCheck(COLLIDER_TYPE::ENEMY_UNIT, COLLIDER_TYPE::BULLET_PLAYER);
    COLLISION->CollisionCheck(COLLIDER_TYPE::BULLET_PLAYER, COLLIDER_TYPE::BULLET_ENEMY);
}

void KatanaZero::dropFrame()
{
    if (PLAYER->getState() == PLAYERSTATE::DEAD) return;

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
