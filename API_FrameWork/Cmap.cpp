#include "framework.h"
#include "Cmap.h"
#include "PlayerState.h"
Cmap::Cmap()
{
    _isClear = false;
}

Cmap::~Cmap()
{
}

HRESULT Cmap::init()
{
    return S_OK;
}

void Cmap::release()
{
}

void Cmap::update()
{
}

void Cmap::render()
{
}

void Cmap::coltoMap()
{
	//�÷��̾�� �� - ��, �Ʒ�, ��, �� - ����, ����, �Ķ�, ����Ÿ
	if (prevx == PLAYER->getCollider()->getPos().x &&
		prevy == PLAYER->getCollider()->getPos().y) return;
	int x, y, r, g, b;
	COLORREF color;
	RECT rc;

	bool isRcol = false;
	bool isLcol = false;
	bool isUcol = false;

	x = PLAYER->getCollider()->getPos().x;
	y = PLAYER->getCollider()->getPos().y;
	rc = PLAYER->getCollider()->getRect();

	//������
	for (int i = PLAYER->getCollider()->getRect().right - 1; i < PLAYER->getCollider()->getRect().right + 10; i++)
	{
		COLORREF  color = GetPixel(_colmap->getMemDC(), i, PLAYER->getCollider()->getPos().y);

		int r = GetRValue(color);
		int g = GetGValue(color);
		int b = GetBValue(color);

		if (!(r == 255 && g == 0 && b == 255) && !(r == 0 && g == 0 && b == 0))
		{
			PLAYER->setX(PLAYER->getX() - 1);
			isRcol = true;

			//TCHAR str[128];
			//_stprintf(str, "�������浹��");
			//ZORDER->ZorderTextOut(str, 10, 760, 850, RGB(0, 0, 0));
			//Sleep(50);
		}
	}
	//����
	for (int i = PLAYER->getCollider()->getRect().left - 1; i > PLAYER->getCollider()->getRect().left - 10; i--)
	{
		COLORREF  color = GetPixel(_colmap->getMemDC(), i, PLAYER->getCollider()->getPos().y);

		int r = GetRValue(color);
		int g = GetGValue(color);
		int b = GetBValue(color);

		if (!(r == 255 && g == 0 && b == 255) && !(r == 0 && g == 0 && b == 0))
		{
			PLAYER->setX(PLAYER->getX() + 1);
			isLcol = true;

			//TCHAR str[128];
			//_stprintf(str, "�����浹��");
			//ZORDER->ZorderTextOut(str, 10, 760, 850+20, RGB(0, 0, 0));
			//Sleep(50);
		}
	}
	//��
	for (int i = PLAYER->getCollider()->getRect().top + 1; i < PLAYER->getCollider()->getRect().top + 10; i++)
	{
		COLORREF  color = GetPixel(_colmap->getMemDC(), PLAYER->getCollider()->getPos().x, i);

		int r = GetRValue(color);
		int g = GetGValue(color);
		int b = GetBValue(color);

		if (!(r == 255 && g == 0 && b == 255) && !(r == 0 && g == 0 && b == 0))
		{
			PLAYER->setY(PLAYER->getY() + 2);
			isUcol = true;

			//TCHAR str[128];
			//_stprintf(str, "���浹��");
			//ZORDER->ZorderTextOut(str, 10, 760, 850 + 40, RGB(0, 0, 0));
			//Sleep(50);
		}

	}
	//�Ʒ�
	for (int i = PLAYER->getCollider()->getRect().bottom - 1; i > PLAYER->getCollider()->getRect().bottom - 10; i--)
	{
		COLORREF  color = GetPixel(_colmap->getMemDC(), PLAYER->getCollider()->getPos().x, i);

		int r = GetRValue(color);
		int g = GetGValue(color);
		int b = GetBValue(color);

		if ((!isUcol) && !(r == 255 && g == 0 && b == 255))
		{
			if ((r == 0 && g == 0 && b == 0) && PLAYER->getIgnoreBlack()) break;
			int tempY = PLAYER->getCollider()->getRect().bottom;
			do {
				tempY--;

				//TCHAR str[128];
				//_stprintf(str, "�Ʒ��浹��");
				////Sleep(50);
				//ZORDER->ZorderTextOut(str, 10, 760, 850 + 60, RGB(0, 0, 0));
			} while (GetPixel(_colmap->getMemDC(), PLAYER->getCollider()->getPos().x, tempY) != RGB(255, 0, 255));
			PLAYER->setY(PLAYER->getY() - (PLAYER->getCollider()->getRect().bottom - tempY));
			PLAYER->setIsLand(true);
			PLAYER->setIgnoreBlack(false);
			break;
		}

	}

	//������ ���ʿ� ����� �ִٸ�
	color = GetPixel(_colmap->getMemDC(), rc.right + 1, y);
	int rr = GetRValue(color);
	int rg = GetGValue(color);
	int rb = GetBValue(color);
	color = GetPixel(_colmap->getMemDC(), rc.left - 1, y);
	int lr = GetRValue(color);
	int lg = GetGValue(color);
	int lb = GetBValue(color);
	if ((lr == 255 && lg == 200 && lb == 0) || (rr == 255 && rg == 200 && rb == 0)) {
		PLAYER->setColYello(true);
	}
	else {
		PLAYER->setColYello(false);
	}


	//�Ʒ��� �������ִٸ�
	color = GetPixel(_colmap->getMemDC(), x, rc.bottom + 1);
	r = GetRValue(color);
	g = GetGValue(color);
	b = GetBValue(color);
	if (r == 0 && g == 0 && b == 0) {
		PLAYER->setColBlack(true);
	}
	else {
		PLAYER->setColBlack(false);
	}
	//IDLE && �Ʒ�����Ÿ -> FALL
	if ((PLAYER->getState() == PLAYERSTATE::IDLE || PLAYER->getState() == PLAYERSTATE::RUN || PLAYER->getState() == PLAYERSTATE::ROLL) && r == 255 && g == 0 && b == 255) {
		PLAYER->getFSM()->ChangeState(PLAYERSTATE::FALL);
	}

	prevx = x;
	prevy = y;
}

void Cmap::CheckClear()
{

}

void Cmap::goalCol()
{
	if(_isClear && _goal->isColEnter()){
		COLLISION->erase(_goal);
		SCENE->changeScene(_nextScene);		
	}
}
