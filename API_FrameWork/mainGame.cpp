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
	fontInit();

	_KZmain = new KatanaZero;
	_KZmain->init();


	return S_OK;
}

void mainGame::release()
{
	gameNode::release();	//����
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
							/*���� ����*/

	_KZmain->update();

	//����Ʈ����
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

void mainGame::render(/*HDC hdc*/)	//�׸��׸��°�
{
	PatBlt(_finalBuffer->getMemDC(), 0, 0, _finalBuffer->getWidth(), _finalBuffer->getHeight(), BLACKNESS);
	PatBlt(getMemDC(), 0, 0, WINSIZEX, WINSIZEY, BLACKNESS);
	//========================================================
						/*������ ����*/

	_KZmain->render();


	//========================================================
	//���۹��� ������ finalBuffer�� �׸���. (getmemdc + fps����)
	this->getBackBuffer()->stretchRender(_finalBuffer->getMemDC(), RecCenX(_clientGameRc), RecCenY(_clientGameRc), RecWidth(_clientGameRc), RecHeight(_clientGameRc));
	if (_showFPS) TIME->render(_finalBuffer->getMemDC());
	//hdc�� ����
	_finalBuffer->render(getHDC());
}


void mainGame::fontInit()
{
	AddFontResourceA("DungGeunMo.ttf");
	AddFontResourceA("�Ҿ߳�8.ttf");
	//AddFontResourceA("neodgm.ttf");
	//...
}

void mainGame::fontRelease()
{
	RemoveFontResourceA("DungGeunMo.ttf");
	RemoveFontResourceA("�Ҿ߳�8.ttf");
	//RemoveFontResourceA("neodgm.ttf");
	//...
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

