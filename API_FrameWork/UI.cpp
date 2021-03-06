#include "framework.h"
#include "UI.h"

UI::UI()
{
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

	neonInit();
	return S_OK;
}

void UI::release()
{
}

void UI::update()
{
	_timerPg->setGauge(100, 100);
	neonUpdate();
}

void UI::render()
{
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
		neonNum_left = RND->getInt(7);
		neonPeriod_left = RND->getInt(1200);
		neonCount_left = 0;
		neonNumCount_left = 0;
		isNeon_left = true;
	}
	else {
		neonCount_left++;
	}
	if (!isNeon_mid) {
		neonNum_mid = RND->getInt(7);
		neonPeriod_mid = RND->getInt(600);
		neonCount_mid = 0;
		neonNumCount_mid = 0;
		isNeon_mid = true;
	}
	else {
		neonCount_mid++;
	}
	if (!isNeon_right) {
		neonNum_right = RND->getInt(7);
		neonPeriod_right = RND->getInt(600);
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

