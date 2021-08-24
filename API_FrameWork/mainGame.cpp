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
	gameNode::init(true);	//�ʿ��ѵ����Ҵ�

	_KZmain = new KatanaZero;
	_KZmain->init();

	return S_OK;
}

void mainGame::release()
{
	gameNode::release();	//����


	SAFE_DELETE(_KZmain);
}

void mainGame::update()
{
	gameNode::update();


	_KZmain->update();





	setIsDebug();
	setFPSprint();
}

void mainGame::render(/*HDC hdc*/)	//�׸��׸��°�
{
	PatBlt(_finalBuffer->getMemDC(), 0, 0, _finalBuffer->getWidth(), _finalBuffer->getHeight(), BLACKNESS);
	PatBlt(getMemDC(), 0, 0, WINSIZEX, WINSIZEY, WHITENESS);
	RECT rc = checkGameSize();
	//========================================================
						/*������ ����*/
	//if(_isDebug) RectangleMake(getMemDC(), { WINSIZEX / 2 - 50, WINSIZEY / 2 - 50, WINSIZEX / 2 + 50, WINSIZEY / 2 + 50 });	//�׽�Ʈ�߽ɻ簢��


	_KZmain->render();







	//========================================================
	//���۹��� ������ finalBuffer�� �׸���. (getmemdc + fps����)
	this->getBackBuffer()->stretchRender(_finalBuffer->getMemDC(), RecCenX(rc), RecCenY(rc), RecWidth(rc), RecHeight(rc));
	if (_showFPS) TIME->render(_finalBuffer->getMemDC());
	//hdc�� ����
	_finalBuffer->render(getHDC());
}

RECT mainGame::checkGameSize()
{
	//���� ������â�� ������ Ȯ��. GAMESIZE���� �ݿ�.
	RECT rc = this->getClRect();
	float wid = rc.right;
	float hei = rc.bottom;
	float widOverHei = (float)GAMESIZEX / GAMESIZEY;
	float heiOverWid = (float)GAMESIZEY / GAMESIZEX;
	
	float w, h;
	//���� ���ο� ���� ���ΰ� ���� â�� ���κ��� ũ�ٸ�, ���� ���ο� �����.
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
	//MoveWindow(gethwnd(), left, top, right, bottom, TRUE);    // listbox�� size�� change
	return { left, top, right, bottom };

}

