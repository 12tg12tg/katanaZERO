#include "framework.h"
#include "titleScene.h"

titleScene::titleScene()
{
    _bg = IMAGE->addImage("title_bg", "images/ui/title_bg.bmp", 1365, 1535);
    IMAGE->addImage("title_neon_bg", "images/ui/title_neon.bmp", 1365, 1535);
    _gr = IMAGE->addImage("title_grass", "images/ui/title_grass.bmp", 1365, 256, true);
    _pl = IMAGE->addFrameImage("title_plant", "images/ui/title_plant.bmp", 1408, 6732, 1, 12, true);
}

titleScene::~titleScene()
{
}

HRESULT titleScene::init()
{
    if (!SOUND->isPlaySound("bgm_title")) {
        SOUND->play("bgm_title", 0.1f);
    }
    _grassani = ANIMATION->addNoneKeyAnimation("title_plant", 10, false, true);
    _offsetTop = 0;
    _offsetBottom = WINSIZEY + _bg->getHeight() - WINSIZEY;
    _neonOn = false;
    _fortextNeon = false;

    _pressBox = RectMakeCenter(WINSIZEX / 2, WINSIZEY / 2 + 170, 175, 100);
    _count = 0;

    _fadeOffset = 0;
    _nextScene = false;
    return S_OK;
}

void titleScene::release()
{
    SOUND->stop("bgm_title");
}

void titleScene::update()
{
    if (!_nextScene && !_neonOn) {
        if (_offsetTop > -767) {
            _offsetTop -= 5;
            _offsetBottom -= 5;
        }
        else {
            _neonOn = true;
            _bg = IMAGE->findImage("title_neon_bg");
        }
    }
    else if (_neonOn){
        _count++;
        if (_count % 5 == 0) {
            _fortextNeon = !_fortextNeon;
        }
        if (INPUT->isAnyKeyDown() || INPUT->isOnceKeyDown(VK_LBUTTON) || INPUT->isOnceKeyDown(VK_RBUTTON)) {
            _neonOn = false;
            _nextScene = true;
            SOUND->play("beep", 0.5f);
        }
    }
    else if (_nextScene) {
        _fadeOffset+=5;
        _offsetTop -= 5;
        _offsetBottom -= 5;
        if (_fadeOffset >= WINSIZEY) {
            MAIN->changeMainState(MAINSTATE::INGAME);
            SCENE->changeScene("테스트맵1");
        }
    }



    //===============================
    ANIMATION->update();
}

void titleScene::render()
{
    ZORDER->UIRender(_bg, ZUIFIRST, 0, 0, _offsetTop);
    ZORDER->UIRender(_gr, ZUISECOND, 0, 0, _offsetBottom -256);
    ZORDER->UIAniRender(_pl, ZUITHIRD, 0, 0, _offsetBottom - 561, _grassani);
    if (_neonOn) {
        string str = "아무 버튼이나 눌러 시작";
   
        if (_fortextNeon) {
            ZORDER->UIDrawText(str, ZUIFIRTH, _pressBox,
                CreateFont(20, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET,
                    0, 0, 0, VARIABLE_PITCH | FF_ROMAN, TEXT("소야논8")),
                RGB(64, 180, 219), DT_LEFT | DT_VCENTER);
        }
        else {
            ZORDER->UIDrawText(str, ZUIFIRTH, _pressBox,
                CreateFont(20, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET,
                    0, 0, 0, VARIABLE_PITCH | FF_ROMAN, TEXT("소야논8")),
                RGB(138, 207, 242), DT_LEFT | DT_VCENTER);
        }
    }
    if(_nextScene) ZORDER->UIRender(IMAGE->findImage("fadeImg"), ZUISECOND, 0, 0, WINSIZEY - _fadeOffset);
}
