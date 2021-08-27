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

	_KZmain = new KatanaZero;
	_KZmain->init();

	return S_OK;
}

void mainGame::release()
{
	gameNode::release();	//해제


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





	setIsDebug();
	setFPSprint();
	//========================================================
}

void mainGame::render(/*HDC hdc*/)	//그림그리는곳
{
	PatBlt(_finalBuffer->getMemDC(), 0, 0, _finalBuffer->getWidth(), _finalBuffer->getHeight(), BLACKNESS);
	PatBlt(getMemDC(), 0, 0, WINSIZEX, WINSIZEY, WHITENESS);
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

