#include "framework.h"
#include "mainGame.h"

mainGame::mainGame()
{
	_finalBuffer = IMAGE->addImage("finalbuffer", WINSIZEX * 2, WINSIZEY * 2);
}

mainGame::~mainGame()
{
}

HRESULT mainGame::init()
{
	gameNode::init(true);	//필요한동적할당
	fontInit();

	_KZmain = new KatanaZero;
	_KZmain->init();


	return S_OK;
}

void mainGame::release()
{
	gameNode::release();	//해제
	fontRelease();


	SAFE_DELETE(_KZmain);
}

void mainGame::update()
{
	gameNode::update();
	if (getCliChange()) {
		setCliChange(false);
		_clientGameRc = checkGameSize();
		CAMERA->setClientRect(_clientGameRc);
	}
	//========================================================
							/*업뎃 공간*/

	_KZmain->update();

	//이펙트실험
	//RECT rect = RectMakeCenter(WINSIZEX / 2, WINSIZEY / 2, 100, 100);
	//ZORDER->UIRectangle(rect, ZMAXLAYER);
	//if (INPUT->isOnceKeyDown(VK_SPACE)) {
	//	float angle = UTIL::getAngle(WINSIZEX / 2, WINSIZEY / 2, m_ptMouse.x, m_ptMouse.y);
	//	float startX = WINSIZEX / 2 + 1566 * cos((float)PI + angle);
	//	float startY = WINSIZEY / 2 - 1566 * sinf((float)PI + angle);
	//	//EFFECT->play("hitEffect", ZEFFECT2, startX, startY, angle, 150);
	//	EFFECT->play("hitEffect", ZEFFECT2, WINSIZEX/2, WINSIZEY/2, angle, 150);
	//}	


	setIsDebug();
	setFPSprint();
	//========================================================
}

void mainGame::render(/*HDC hdc*/)	//그림그리는곳
{
	PatBlt(_finalBuffer->getMemDC(), 0, 0, _finalBuffer->getWidth(), _finalBuffer->getHeight(), BLACKNESS);
	PatBlt(getMemDC(), 0, 0, WINSIZEX, WINSIZEY, BLACKNESS);
	//========================================================
						/*렌더링 공간*/

	_KZmain->render();


	//========================================================
	//백퍼버의 내용을 finalBuffer에 그린다. (getmemdc + fps정보)
	this->getBackBuffer()->stretchRender(_finalBuffer->getMemDC(), RecCenX(_clientGameRc), RecCenY(_clientGameRc), RecWidth(_clientGameRc), RecHeight(_clientGameRc));
	if (_showFPS) TIME->render(_finalBuffer->getMemDC());
	//hdc로 전송
	_finalBuffer->render(getHDC());
}


void mainGame::fontInit()
{
	AddFontResourceA("DungGeunMo.ttf");
	AddFontResourceA("소야논8.ttf");
	//AddFontResourceA("neodgm.ttf");
	//...
}

void mainGame::fontRelease()
{
	RemoveFontResourceA("DungGeunMo.ttf");
	RemoveFontResourceA("소야논8.ttf");
	//RemoveFontResourceA("neodgm.ttf");
	//...
}

RECT mainGame::checkGameSize()
{
	//현재 윈도우창의 사이즈 확인. GAMESIZE비율 반영.
	RECT rc = this->getClRect();
	float wid = rc.right;
	float hei = rc.bottom;
	float widOverHei = (float)GAMESIZEX / GAMESIZEY;
	float heiOverWid = (float)GAMESIZEY / GAMESIZEX;
	
	float w, h;
	//현재 가로에 맞춘 세로가 현재 창의 세로보다 크다면, 현재 세로에 맞춘다.
	(wid * heiOverWid > hei) ? w = hei * widOverHei, h = hei
		: w = wid, h = w * heiOverWid;

	int left, top, right, bottom;
	if (w < wid) {
		left = wid / 2 - w / 2;
		right = left + w;
		top = 0;
		bottom = h;
	}
	else {
		left = 0;
		right = w;
		top = hei / 2 - h / 2;
		bottom = top + h;
	}
	//MoveWindow(gethwnd(), left, top, right, bottom, TRUE);    // listbox의 size를 change
	return { left, top, right, bottom };

}

