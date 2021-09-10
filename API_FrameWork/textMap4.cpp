#include "framework.h"
#include "textMap4.h"
#include "playerstate.h"
textMap4::textMap4()
{
	_map = IMAGE->addImage("map4", "images/map/boss.bmp", 1365, 784);
	_bwmap = IMAGE->addImage("map4_bw", "images/map/boss_bw.bmp", 1365, 784);
	_colmap = IMAGE->addImage("map4_col", "images/map/boss_col.bmp", 1365, 784, true);
	_map_front = _bwmap_front = nullptr;
	_startPt = Vec2(328, 526);
	_timelimit = 12000;
	_nextScene = "엔딩";
	_retryNum = -1;
}

textMap4::~textMap4()
{
}

HRESULT textMap4::init()
{
	if (!SOUND->isPlaySound("song_bossbattle")) {
		SOUND->play("song_bossbattle", 0.1f);
	}
	Cmap::init();
	_goal = nullptr;
	PLAYER->setX(_startPt.x);
	PLAYER->setY(_startPt.y);

	prevx = PLAYER->getCollider()->getPos().x;
	prevy = PLAYER->getCollider()->getPos().y;
	prevRc = PLAYER->getCollider()->getRect();

	CAMERA->init(PLAYER->getX(), PLAYER->getY(), _map->getWidth(), _map->getHeight(), 0, 0, WINSIZEX / 2, WINSIZEY / 2,
		WINSIZEX, WINSIZEY);

	_timeCount = _timelimit;
	_isClear = false;

	_boss = new boss;
	_boss->init(880, 523);

	_retryNum++;
	_clear = false;
	return S_OK;
}

void textMap4::release()
{
	_boss->release();
	SAFE_DELETE(_boss);
	SOUND->stop("bgm_ending");
}

void textMap4::update()
{
	//타이머
	timeCheck();
	//충돌
	coltoMap();

	_boss->update();

	CheckClear();
	afterClearUpdate();
	//--------------------------------------------------
	PLAYER->update();
	ANIMATION->update();
	COLLISION->update();
	POINT lerp;
	lerp.x = CAMERA->getPivotX() * (1 - 0.08) + PLAYER->getCollider()->getPos().x * 0.08;
	lerp.y = CAMERA->getPivotY() * (1 - 0.08) + PLAYER->getY() * 0.08;
	if (!_clear) CAMERA->movePivot(lerp.x, 513);
	CAMERA->update();
	EFFECT->update();
}

void textMap4::render()
{
	//맵
	ZORDER->ZorderRender(_map, ZFLOORMAP, 0, 0, 0);
	if (_isDebug) ZORDER->ZorderRender(_colmap, ZCOL1, 0, 0, 0);

	_boss->render();
	afterClearRender();
	//--------------------------------------------------
	PLAYER->render();
	COLLISION->render();
	EFFECT->render();
}

void textMap4::CheckClear()
{
	if (!_clear && _boss->_isCut) {
		_clear = true;
		PLAYER->setIsGravePeriod(true);
		CAMERA->FadeInit(255, FADEKIND::FADE_OUT);
	}
}

void textMap4::afterClearUpdate()
{
	if (_clear) {
		_clearCount++;
		if (!PLAYER->getAni()->isPlay()) {
			PLAYER->getFSM()->ChangeState(PLAYERSTATE::IDLE);
			if (PLAYER->getFoward() == FOWARD::RIGHT)
				ANIMATION->changeNonKeyAnimation(PLAYER->getAni(), "player_ALL1", 0, 10, 11, false, true);
			else if (PLAYER->getFoward() == FOWARD::LEFT)
				ANIMATION->changeNonKeyAnimation(PLAYER->getAni(), "player_ALL1", 106, 96, 11, false, true);
		}
		if (_clearCount == 250) {
			SOUND->stop("song_bossbattle");
			SOUND->play("bgm_ending", 0.1f);
			CAMERA->FadeStart();
			CAMERA->ChangePivot(CAMERA->getPivotX(), -WINSIZEY, 2);
		}
		if (_clearCount > 600) {
			if (INPUT->isAnyKeyDown() || INPUT->isOnceKeyDown(VK_LBUTTON) || INPUT->isOnceKeyDown(VK_RBUTTON)){
				MAIN->changeMainState(MAINSTATE::TITLE);
				SCENE->changeScene("타이틀");
				CAMERA->FadeInit(100, FADEKIND::FADE_IN);
				CAMERA->FadeStart();
			}
		}
	}
}

void textMap4::afterClearRender()
{
	if (!_clear) return;
	if (_clearCount > 250 && !CAMERA->getFadeIsStart()) {
		ZORDER->UIRender(IMAGE->findImage("fadeImg"), ZUIFADE, 2, 0, 0);
	}
	if (_clearCount > 505) {
		string str = "Katana ZERO";
		RECT temp = RectMakeCenter(WINSIZEX / 2, WINSIZEY / 2 - 170, 247, 100);
		ZORDER->UIDrawText(str, ZUIFADE+1, temp,
			CreateFont(50, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET,
				0, 0, 0, VARIABLE_PITCH | FF_ROMAN, TEXT("소야논8")),
			RGB(255, 255, 255), DT_LEFT | DT_VCENTER);
	}
	if (_clearCount > 600) {
		string str = "아무 버튼이나 눌러 타이틀로 돌아가기";
		RECT temp = RectMakeCenter(WINSIZEX / 2+110, WINSIZEY / 2, 550, 100);
		ZORDER->UIDrawText(str, ZUIFADE + 1, temp,
			CreateFont(25, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET,
				0, 0, 0, VARIABLE_PITCH | FF_ROMAN, TEXT("소야논8")),
			RGB(255, 255, 255), DT_LEFT | DT_VCENTER);

		str = "경일게임 아카데미 정예균";
		temp = RectMakeCenter(WINSIZEX / 2 + 590, WINSIZEY / 2 + 400, 247, 100);
		ZORDER->UIDrawText(str, ZUIFADE + 1, temp,
			CreateFont(20, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET,
				0, 0, 0, VARIABLE_PITCH | FF_ROMAN, TEXT("소야논8")),
			RGB(138, 207, 242), DT_LEFT | DT_VCENTER);
	}
}
