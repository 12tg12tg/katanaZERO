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
	CAMERA->FadeInit(20, FADE_LEFT_IN);
	CAMERA->FadeStart();
	_timeOver = false;
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
	color = GetPixel(_colmap->getMemDC(), rc.right + 2, y);
	int rr = GetRValue(color);
	int rg = GetGValue(color);
	int rb = GetBValue(color);
	color = GetPixel(_colmap->getMemDC(), rc.left - 2, y);
	int lr = GetRValue(color);
	int lg = GetGValue(color);
	int lb = GetBValue(color);
	if ((lr == 255 && lg == 200 && lb == 0) || (rr == 255 && rg == 200 && rb == 0)) {
		PLAYER->setColYello(true);
	}
	else {
		PLAYER->setColYello(false);
	}

	//�����¿��� ������ ���ʿ� ����� �ִٸ�
	if (PLAYER->getState() == PLAYERSTATE::WALLSLIDE) {
		color = GetPixel(_colmap->getMemDC(), rc.right + 10, y);
		rr = GetRValue(color);
		rg = GetGValue(color);
		rb = GetBValue(color);
		color = GetPixel(_colmap->getMemDC(), rc.left - 10, y);
		lr = GetRValue(color);
		lg = GetGValue(color);
		lb = GetBValue(color);
		if ((lr == 255 && lg == 200 && lb == 0) || (rr == 255 && rg == 200 && rb == 0)) {
			PLAYER->setStillWall(true);
		}
		else {
			PLAYER->setStillWall(false);
		}
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

void Cmap::coltoMapEnemy(enemy* enemy)
{
	auto& vec = enemy->getVector();
	auto iter = vec.begin();
	for (iter; iter != vec.end(); ++iter)
	{
		bool isRcol = false;
		bool isLcol = false;
		bool isUcol = false;
		int r;
		int g;
		int b;
		COLORREF color;

		//������
		color = GetPixel(_colmap->getMemDC(), iter->col->getRect().right + 1, iter->col->getPos().y);
		r = GetRValue(color);
		g = GetGValue(color);
		b = GetBValue(color);

		if (!(r == 255 && g == 0 && b == 255) && !(r == 0 && g == 0 && b == 0))
		{
				isRcol = true;
				iter->isRightCol = true;
				//TCHAR str[128];
				//_stprintf(str, "�������浹��");
				//ZORDER->ZorderTextOut(str, 10, 760, 850, RGB(0, 0, 0));
				//Sleep(50);
		}
		else {
			iter->isRightCol = false;
		}

		//����
		color = GetPixel(_colmap->getMemDC(), iter->col->getRect().left - 1, iter->col->getPos().y);
		r = GetRValue(color);
		g = GetGValue(color);
		b = GetBValue(color);

		if (!(r == 255 && g == 0 && b == 255) && !(r == 0 && g == 0 && b == 0))
		{
			isLcol = true;
			iter->isLeftCol = true;
			//TCHAR str[128];
			//_stprintf(str, "�������浹��");
			//ZORDER->ZorderTextOut(str, 10, 760, 850, RGB(0, 0, 0));
			//Sleep(50);
		}
		else {
			iter->isLeftCol = false;
		}

		//��
		color = GetPixel(_colmap->getMemDC(), iter->col->getPos().x, iter->col->getRect().top - 1);

		r = GetRValue(color);
		g = GetGValue(color);
		b = GetBValue(color);

		if (!(r == 255 && g == 0 && b == 255) && !(r == 0 && g == 0 && b == 0))
		{
			isUcol = true;
			iter->isUpCol = true;
			//TCHAR str[128];
			//_stprintf(str, "���浹��");
			//ZORDER->ZorderTextOut(str, 10, 760, 850 + 40, RGB(0, 0, 0));
			//Sleep(50);
		}
		else {
			iter->isUpCol = false;
		}

		//�Ʒ�
		color = GetPixel(_colmap->getMemDC(), iter->col->getPos().x, iter->col->getRect().bottom + 5);

		r = GetRValue(color);
		g = GetGValue(color);
		b = GetBValue(color);

		if (!iter->isLand) {
			if (!(r == 255 && g == 0 && b == 255))
			{
				if ((r == 0 && g == 0 && b == 0) && iter->ignoreBlack) break;
				isUcol = true;
				iter->isLand = true;
				//TCHAR str[128];
				//_stprintf(str, "���浹��");
				//ZORDER->ZorderTextOut(str, 10, 760, 850 + 40, RGB(0, 0, 0));
				//Sleep(50);
			}
			else {
				iter->isLand = false;
			}
		}
	}

		////���Ϳ� �� - ��, �Ʒ�, ��, ��
		////if (prevx == PLAYER->getCollider()->getPos().x &&
		////	prevy == PLAYER->getCollider()->getPos().y) return;
		//int x, y, r, g, b;
		//COLORREF color;
		//RECT rc;

		//bool isRcol = false;
		//bool isLcol = false;
		//bool isUcol = false;

		//x = iter->col->getPos().x;
		//y = iter->col->getPos().y;
		//rc = iter->col->getRect();

		////������
		//for (int i = iter->col->getRect().right - 1; i < iter->col->getRect().right + 10; i++)
		//{
		//	COLORREF color = GetPixel(_colmap->getMemDC(), i, iter->col->getPos().y);

		//	int r = GetRValue(color);
		//	int g = GetGValue(color);
		//	int b = GetBValue(color);

		//	if (!(r == 255 && g == 0 && b == 255) && !(r == 0 && g == 0 && b == 0))
		//	{
		//		iter->x = iter->x - 1;
		//		isRcol = true;
		//		iter->isRightCol = true;
		//		//TCHAR str[128];
		//		//_stprintf(str, "�������浹��");
		//		//ZORDER->ZorderTextOut(str, 10, 760, 850, RGB(0, 0, 0));
		//		//Sleep(50);
		//	}
		//	else {
		//		iter->isRightCol = false;
		//	}
		//}
		////����
		//for (int i = iter->col->getRect().left - 1; i > iter->col->getRect().left - 10; i--)
		//{
		//	COLORREF  color = GetPixel(_colmap->getMemDC(), i, iter->col->getPos().y);

		//	int r = GetRValue(color);
		//	int g = GetGValue(color);
		//	int b = GetBValue(color);

		//	if (!(r == 255 && g == 0 && b == 255) && !(r == 0 && g == 0 && b == 0))
		//	{
		//		iter->x = iter->x + 1;
		//		isLcol = true;
		//		iter->isLeftCol = true;
		//		//TCHAR str[128];
		//		//_stprintf(str, "�����浹��");
		//		//ZORDER->ZorderTextOut(str, 10, 760, 850+20, RGB(0, 0, 0));
		//		//Sleep(50);
		//	}
		//	else {
		//		iter->isLeftCol = false;
		//	}
		//}
		////��
		//for (int i = iter->col->getRect().top + 1; i < iter->col->getRect().top + 10; i++)
		//{
		//	COLORREF  color = GetPixel(_colmap->getMemDC(), iter->col->getPos().x, i);

		//	int r = GetRValue(color);
		//	int g = GetGValue(color);
		//	int b = GetBValue(color);

		//	if (!(r == 255 && g == 0 && b == 255) && !(r == 0 && g == 0 && b == 0))
		//	{
		//		iter->y = iter->y + 2;
		//		isUcol = true;
		//		iter->isUpCol = true;
		//		//TCHAR str[128];
		//		//_stprintf(str, "���浹��");
		//		//ZORDER->ZorderTextOut(str, 10, 760, 850 + 40, RGB(0, 0, 0));
		//		//Sleep(50);
		//	}
		//	else {
		//		iter->isUpCol = false;
		//	}

		//}
		////�Ʒ�
		//for (int i = iter->col->getRect().bottom - 1; i > iter->col->getRect().bottom - 10; i--)
		//{
		//	COLORREF  color = GetPixel(_colmap->getMemDC(), iter->col->getPos().x, i);

		//	int r = GetRValue(color);
		//	int g = GetGValue(color);
		//	int b = GetBValue(color);

		//	if ((!isUcol) && !(r == 255 && g == 0 && b == 255))
		//	{
		//		if ((r == 0 && g == 0 && b == 0) && iter->ignoreBlack) break;
		//		int tempY = iter->col->getRect().bottom;
		//		//do {
		//		//	tempY--;
		//		//	//TCHAR str[128];
		//		//	//_stprintf(str, "�Ʒ��浹��");
		//		//	////Sleep(50);
		//		//	//ZORDER->ZorderTextOut(str, 10, 760, 850 + 60, RGB(0, 0, 0));
		//		//} while (GetPixel(_colmap->getMemDC(), iter->col->getPos().x, tempY) != RGB(255, 0, 255));
		//		//iter->y = iter->y - iter->col->getRect().bottom - tempY;
		//		iter->isLand = true;
		//		iter->ignoreBlack = false;
		//		break;
		//	}

		//}
}

void Cmap::CheckClear()
{
}

void Cmap::goalCol()
{
	if(_isClear && _goal->isColEnter()){
		COLLISION->erase(_goal);
		CAMERA->FadeInit(70, FADEKIND::FADE_OUT);
		CAMERA->FadeStart();
		MAIN->changeMainState(MAINSTATE::REPLAY);
		//SCENE->changeScene(_nextScene);		
		/*�̷����ϸ� �ǰڱ�... -> Ŭ���� ���÷��� -> ��Ŭ���� ��ü����*/
	}
}

void Cmap::timeCheck()
{
	//Ÿ�̸�
	_timeCount -= 1 * TIME->getGameTimeRate();
	MAIN->getUIlink()->setMapTimer(_timeCount, _timelimit);
	if (!_timeOver && _timeCount < 0) {
		_timeOver = true;
		PLAYER->getFSM()->ChangeState(PLAYERSTATE::DEAD);
	}
}
