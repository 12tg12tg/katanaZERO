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
    MAIN->setUIlink(m_ui);

    sceneInit();
    collisionInit();

    _replayDone = false;
    _RPcount = 0;
    _RPfadeIn = false;
    _firstFadeOut = false;
    _secondFadeIn = false;

    _state = MAINSTATE::INGAME;
    _caretaker = new Caretaker;
    _caretaker->init();

    _restartBox = IMAGE->addImage("restartbox", "images/ui/restartBox.bmp", 395, 240);
    _restartBox_over = IMAGE->addImage("restartbox_over", "images/ui/restartBox_over.bmp", 395, 240, true);
    _fortextNeon = false;
    _startRollback = false;
    _rollbackstartCount = 0;
    _restartTextRc = RectMakeCenter(WINSIZEX / 2 + 40, WINSIZEY / 2 + 60, 395 - 20, 240 - 20);

    m_debugRc = RectMake(1110, 79, 220, 200);
    _slowAlpha = 0;
    return S_OK;
}

void KatanaZero::release()
{
    PLAYER->release();
    PLAYER->releaseSingleton();
    MAIN->releaseSingleton();
    m_ui->release();
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

    //페이드업데이트 - 반드시 상태이전에 실행. 자연스럽게.
    CAMERA->FadeUpdate();


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
        SCENE->update();
        _caretaker->snapshot();
    }
        break;
    case MAINSTATE::REPLAY:
    {
        updateReplay();
    }
        break;
    case MAINSTATE::ROLLBACK:
    {
        restartBoxupdate();
        if (_startRollback) {
            //m_ui->update();
            _caretaker->rollback();
            CAMERA->update();
            if (_caretaker->getRollbackDone()) {
                _startRollback = false;
                _caretaker->setRollbackDone(false);
                _caretaker->allVectorClear();
                _state = MAINSTATE::INGAME;
                EFFECT->deleteParticle();
                MAIN->getUIlink()->reinit();
                PLAYER->reInit();
                PLAYER->getFSM()->ChangeState(PLAYERSTATE::IDLE);
                SCENE->curScene()->release();
                SCENE->curScene()->init();
            }
        }
    }
        break;
    case MAINSTATE::PAUSE:
        break;
    case MAINSTATE::NONE:
        break;
    }

    m_ui->update();
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
        showReplay();
    }
        break;


    case MAINSTATE::ROLLBACK:
    {
        restartBoxrender();
        if (_startRollback) {
            m_ui->render();
            image* map = dynamic_cast<Cmap*>(SCENE->curScene())->getMap();
            image* map_front = dynamic_cast<Cmap*>(SCENE->curScene())->getMap_front();
            ZORDER->ZorderRender(map, ZFLOORMAP, 0, 0, 0);
            if (map_front) ZORDER->ZorderRender(map_front, ZABOVEMAP, 0, 1365, 154);
        }
    }
        break;
    case MAINSTATE::PAUSE:
        break;
    case MAINSTATE::NONE:
        break;
    }

    CAMERA->FadeRender();
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
    _testmap4 = dynamic_cast<textMap4*>(SCENE->addScene("보스맵", new textMap4));
    SCENE->changeScene("테스트맵2");
    //------------------------------------------------------------------------------------------------
}

void KatanaZero::collisionInit()
{
    COLLISION->CollisionCheck(COLLIDER_TYPE::PLAYER_UNIT, COLLIDER_TYPE::POTAL);
    COLLISION->CollisionCheck(COLLIDER_TYPE::PLAYER_UNIT, COLLIDER_TYPE::BULLET_ENEMY);
    COLLISION->CollisionCheck(COLLIDER_TYPE::ENEMY_UNIT, COLLIDER_TYPE::BULLET_PLAYER);
    COLLISION->CollisionCheck(COLLIDER_TYPE::ENEMY_UNIT, COLLIDER_TYPE::DOOR);
    COLLISION->CollisionCheck(COLLIDER_TYPE::BULLET_PLAYER, COLLIDER_TYPE::BULLET_ENEMY);
    COLLISION->CollisionCheck(COLLIDER_TYPE::BULLET_PLAYER, COLLIDER_TYPE::DOOR);
    COLLISION->CollisionCheck(COLLIDER_TYPE::PLAYER_UNIT, COLLIDER_TYPE::DOOR);
    COLLISION->CollisionCheck(COLLIDER_TYPE::PLAYER_UNIT, COLLIDER_TYPE::LASER);
    COLLISION->CollisionCheck(COLLIDER_TYPE::ENEMY_UNIT, COLLIDER_TYPE::LASER);
    COLLISION->CollisionCheck(COLLIDER_TYPE::PLAYER_UNIT, COLLIDER_TYPE::SEARCH);
    COLLISION->CollisionCheck(COLLIDER_TYPE::LASER, COLLIDER_TYPE::SEARCH);
    COLLISION->CollisionCheck(COLLIDER_TYPE::DOOR, COLLIDER_TYPE::SEARCH);
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
    if (INPUT->isOnceKeyUp(VK_LSHIFT) ||
        PLAYER->getState() == PLAYERSTATE::DEAD || PLAYER->getLaserDie() ||
        MAIN->getCantSlow())
    {
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

void KatanaZero::restartBoxupdate()
{
    if (!_startRollback) {
        if (_rollbackstartCount % 5 == 0) {
            _fortextNeon = !_fortextNeon;
        }
        _rollbackstartCount++;
        if (_rollbackstartCount > 20) {
            if (INPUT->isAnyKeyDown()||INPUT->isOnceKeyDown(VK_LBUTTON)||INPUT->isOnceKeyDown(VK_RBUTTON)) {
                _rollbackstartCount = 0;
                _startRollback = true;
                PLAYER->setIsTimeOut(false);
            }
        }
        SCENE->update();
    }
}

void KatanaZero::restartBoxrender()
{
    if (!_startRollback) {
        ZORDER->UIAlphaRender(_restartBox, ZUIFIRTH, 0, WINSIZEX / 2 - _restartBox->getWidth() / 2, WINSIZEY / 2 - _restartBox->getHeight() / 2, 180);
        ZORDER->UIAlphaRender(_restartBox_over, ZUIFIRTH, 1, WINSIZEX / 2 - _restartBox->getWidth() / 2, WINSIZEY / 2 - _restartBox->getHeight() / 2, 180);
        
        string str;
        if (PLAYER->getIsTimeOut()) str = "         전부 기억할 수 없어.\n      더 효율적으로 움직여야 해.\n\n      (아무 버튼이나 눌러 재시작)";
        else str = "                아니....\n           통하지 않을 거야.\n\n      (아무 버튼이나 눌러 재시작)";
        
        if (_fortextNeon) {
            ZORDER->UIDrawText(str, ZUIFIRTH, _restartTextRc,
                CreateFont(20, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET,
                    0, 0, 0, VARIABLE_PITCH | FF_ROMAN, TEXT("소야논8")),
                RGB(64, 180, 219), DT_LEFT | DT_VCENTER);
        }
        else {
            ZORDER->UIDrawText(str, ZUIFIRTH, _restartTextRc,
                CreateFont(20, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET,
                    0, 0, 0, VARIABLE_PITCH | FF_ROMAN, TEXT("소야논8")),
                RGB(138, 207, 242), DT_LEFT | DT_VCENTER);
        }

        m_ui->render();
        SCENE->render();
    }
}

void KatanaZero::showReplay()
{
    if (!_firstFadeOut) {
        m_ui->render();
        SCENE->render();
        //CAMERA->FadeRender();
        /*텍스트*/
        RECT txtRc = RectMake(555, 375, 290, 38);
        string str = "그래, 이렇게 하면 되겠지.";
        ZORDER->UIDrawText(str, ZUIFADE + 1, txtRc,
            CreateFont(30, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET,
                0, 0, 0, VARIABLE_PITCH | FF_ROMAN, TEXT("소야논8")),
            RGB(255, 255, 255), DT_LEFT | DT_VCENTER);
    }
    else if (!_secondFadeIn) {
        /*텍스트*/
        RECT txtRc = RectMake(555, 375, 290, 38);
        string str = "그래, 이렇게 하면 되겠지.";
        ZORDER->UIDrawText(str, ZUIFADE + 1, txtRc,
            CreateFont(30, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET,
                0, 0, 0, VARIABLE_PITCH | FF_ROMAN, TEXT("소야논8")),
            RGB(255, 255, 255), DT_LEFT | DT_VCENTER);
        if (!CAMERA->getFadeIsStart()) {
            ZORDER->UIAlphaRender(IMAGE->findImage("fadeImg"), ZUIFADE, 0, 0, 0, 255);
        }
        else {
            image* bwmap = dynamic_cast<Cmap*>(SCENE->curScene())->getBwmap();
            image* bwmap_front = dynamic_cast<Cmap*>(SCENE->curScene())->getBwmap_front();
            ZORDER->ZorderRender(bwmap, ZFLOORMAP, 0, 0, 0);
            if (bwmap_front) ZORDER->ZorderRender(bwmap_front, ZABOVEMAP, 0, 0, 0);
            //CAMERA->FadeRender();
        }
    }
    else if (!_replayDone) {
        m_ui->render();   //재생중. ui
        image* bwmap = dynamic_cast<Cmap*>(SCENE->curScene())->getBwmap();
        image* bwmap_front = dynamic_cast<Cmap*>(SCENE->curScene())->getBwmap_front();
        ZORDER->ZorderRender(bwmap, ZFLOORMAP, 0, 0, 0);
        if (bwmap_front) ZORDER->ZorderRender(bwmap_front, ZABOVEMAP, 0, 0, 0);
    }
    else {
        //m_ui->render();   //재생중. ui
        image* bwmap = dynamic_cast<Cmap*>(SCENE->curScene())->getBwmap();
        image* bwmap_front = dynamic_cast<Cmap*>(SCENE->curScene())->getBwmap_front();
        ZORDER->ZorderRender(bwmap, ZFLOORMAP, 0, 0, 0);
        if (bwmap_front) ZORDER->ZorderRender(bwmap_front, ZABOVEMAP, 0, 0, 0);
        //CAMERA->FadeRender();
    }
}

void KatanaZero::updateReplay()
{
    if (!_firstFadeOut) {
        //CAMERA->FadeUpdate();
        if (!CAMERA->getFadeIsStart()) {
            _firstFadeOut = true;
        }
    }
    else if (!_secondFadeIn) {
        _RPcount++;
        if (_RPcount == 80) {
            CAMERA->FadeInit(60, FADEKIND::FADE_IN);
            CAMERA->FadeStart();
        }
        else if (_RPcount > 80) {
            _caretaker->replay();
            CAMERA->update();
            //CAMERA->FadeUpdate();
            if (!CAMERA->getFadeIsStart()) {
                _secondFadeIn = true;
            }
        }
    }
    else if(!_replayDone){
        _caretaker->replay();       //되감기
        CAMERA->update();
        if (_caretaker->getReplayDone() || INPUT->isOnceKeyDown(VK_RBUTTON)) {
            _caretaker->setIsReplay(false);
            _caretaker->setReplayDone(false);
            _caretaker->allVectorClear();

            CAMERA->FadeInit(20, FADEKIND::FADE_LEFT_OUT);
            CAMERA->FadeStart();
            _replayDone = true;
        }
    }
    else {
        //CAMERA->FadeUpdate();
        if (!CAMERA->getFadeIsStart()) {
            _firstFadeOut = false;
            _secondFadeIn = false;
            _RPcount = 0;
            _replayDone = false;
            _state = MAINSTATE::INGAME;
            EFFECT->deleteParticle();
            MAIN->getUIlink()->reinit();
            PLAYER->reInit();
            SCENE->changeScene(dynamic_cast<Cmap*>(SCENE->curScene())->getNextSceneName());
        }
    }
}
