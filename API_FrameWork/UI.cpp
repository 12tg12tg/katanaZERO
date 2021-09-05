#include "framework.h"
#include "UI.h"

UI::UI()
{
	IMAGE->addImage("배터리파편", "images/particle/mint1px.bmp", 2, 2);
}

UI::~UI()
{
}

HRESULT UI::init()
{
	_topUi = IMAGE->addImage("상단ui", "images/ui/hud_collapse.bmp",
		1365, 50, true, RGB(255, 0, 255));
	_cursor = IMAGE->addImage("커서", "images/ui/cursor.bmp", 25*2, 25*2, true);
	_battery = IMAGE->addFrameImage("배터리", "images/ui/battery.bmp", 121, 252, 1, 12, true);
	_batteryFrameY = 0;

	_timerPg = new progressBar;
	_timerPg->initOnlyFront("images/ui/timerProgress.bmp", 588, 6, 200, 24);

	_rightClick = IMAGE->addFrameImage("우클릭", "images/ui/right_click.bmp", 412, 77, 2, 1, true);
	_rightClickAni = ANIMATION->addNoneKeyAnimation("우클릭", 2, false, true);

	_rightClickCount = 0;
	_maxSlow = 580;
	_slowDelay = 150;
	_slowGauge = _maxSlow;
	_delay = 0;
	neonInit();
	return S_OK;
}

void UI::release()
{
	SAFE_DELETE(_timerPg);
}

void UI::update()
{
	neonUpdate();
	slowGauge();
}

void UI::render()
{
	if (MAIN->getMainState() == MAINSTATE::INGAME) {
		_rightClickCount = 0;
		//always
		ZORDER->UIRender(_topUi, ZUITOP, _topUi->getHeight(), 0, 0);
		ZORDER->UIRender(_cursor, ZUIMOUSE, CAMERA->getRelativeMouse().y,
			CAMERA->getClientMouse().x - _cursor->getWidth() / 2,
			CAMERA->getClientMouse().y - _cursor->getHeight() / 2);
		ZORDER->UIFrameRender(_battery, ZUITOP2, _topUi->getHeight(), 22, 14, 0, _batteryFrameY);
		_timerPg->render(ZUITOP2);
		//often
		neonRender();		
	}
	else if (MAIN->getMainState() == MAINSTATE::REPLAY) {
		_rightClickCount++;
		if (_rightClickCount > 80) {
			ZORDER->UIAniRender(_rightClick, ZUITOP, 0, 1022, 587, _rightClickAni);
			ANIMATION->update();
		}		
	}
}


void UI::neonInit()
{
	_topleft = IMAGE->addImage("좌측배터리자체알파", "images/ui/hud_collapse_part_left.bmp", 162, 50, true);
	_topmid = IMAGE->addImage("중단타이머알파필요", "images/ui/hud_collapse_part_mid.bmp", 810, 50, true);
	_topright = IMAGE->addImage("우측인벤알파필요", "images/ui/hud_collapse_part_right.bmp", 1365, 50, true);
	neonNum_left = RND->getInt(7);
	neonPeriod_left = RND->getInt(600);
	isNeon_left = true;
	neonCount_left = 0;
	neonNumCount_left = 0;
	neonNum_mid = RND->getInt(7);
	neonPeriod_mid = RND->getInt(600);
	isNeon_mid = true;
	neonCount_mid = 0;
	neonNumCount_mid = 0;
	neonNum_right = RND->getInt(7);
	neonPeriod_right = RND->getInt(600);
	isNeon_right = true;
	neonCount_right = 0;
	neonNumCount_right = 0;
}

void UI::neonUpdate()
{
	if (!isNeon_left) {
		neonNum_left = RND->getInt(10);
		neonPeriod_left = RND->getInt(500);
		neonCount_left = 0;
		neonNumCount_left = 0;
		isNeon_left = true;
	}
	else {
		neonCount_left++;
	}
	if (!isNeon_mid) {
		neonNum_mid = RND->getInt(10);
		neonPeriod_mid = RND->getInt(500);
		neonCount_mid = 0;
		neonNumCount_mid = 0;
		isNeon_mid = true;
	}
	else {
		neonCount_mid++;
	}
	if (!isNeon_right) {
		neonNum_right = RND->getInt(10);
		neonPeriod_right = RND->getInt(500);
		neonCount_right = 0;
		neonNumCount_right = 0;
		isNeon_right = true;
	}
	else {
		neonCount_right++;
	}
}

void UI::neonRender()
{
	if (isNeon_left && neonCount_left > neonPeriod_left) {
		if (isNeonOn_left) {
			ZORDER->UIRender(_topleft, ZUITOP2+1, _topleft->getHeight(), 0, 0);
			++neonNumCount_left;
			isNeonOn_left = false;
			if (neonNumCount_left > neonNum_left) isNeon_left = false;
		}
		else {
			isNeonOn_left = true;
		}
	}
	if (isNeon_mid && neonCount_mid > neonPeriod_mid) {
		if (isNeonOn_mid) {
			ZORDER->UIAlphaRender(_topmid, ZUITOP2 + 1, _topmid->getHeight(), 0, 0, 200);
			++neonNumCount_mid;
			isNeonOn_mid = false;
			if (neonNumCount_mid > neonNum_mid) isNeon_mid = false;
		}
		else {
			isNeonOn_mid = true;
		}
	}
	if (isNeon_right && neonCount_right > neonPeriod_right) {
		if (isNeonOn_right) {
			ZORDER->UIAlphaRender(_topright, ZUITOP2 + 1, _topright->getHeight(), 0, 0, 200);
			++neonNumCount_right;
			isNeonOn_right = false;
			if (neonNumCount_right > neonNum_right) isNeon_right = false;
		}
		else {
			isNeonOn_right = true;
		}
	}
	
}

void UI::slowGauge()
{
	if (MAIN->getIsSlow()) {
		--_slowGauge;
		EFFECT->addParticle("배터리파편", ZUIEFFECT, 14-RND->getInt(10), 25+RND->getInt(30)-15, 3, RND->getFloatFromTo(PI_8 * 5, PI_8*11), 15, 1, false, 255, true);
		EFFECT->addParticle("배터리파편", ZUIEFFECT, 160+RND->getInt(10), 25+RND->getInt(30)-15, 3, RND->getFloatFromTo(-PI_8 * 3, PI_8*3), 15, 1, false, 255, true);
		if (_slowGauge < 0) _slowGauge = 0;
	}
	else {
		_slowGauge += 0.5;
		if (_slowGauge > _maxSlow) _slowGauge = _maxSlow;
	}

	//없을때가 11, 만땅이 0
	float ratio = (float)_slowGauge / (float)_maxSlow;
	_batteryFrameY = 12 * ratio;
	if (_batteryFrameY == 12) _batteryFrameY = 11;
	_batteryFrameY = 11 - _batteryFrameY;

	if (_batteryFrameY == 11) {
		MAIN->setCantSlow(true);
	}
	if(MAIN->getCantSlow()){
		++_delay;
		if (_delay > _slowDelay) {
			_delay = 0;
			MAIN->setCantSlow(false);
		}
	}


}

void UI::slowReset()
{
	_slowGauge = _maxSlow;
	_delay = 0;
}

