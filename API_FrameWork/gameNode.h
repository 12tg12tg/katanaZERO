#pragma once
#include "image.h"
static image* _backBuffer = IMAGE->addImage("backBuffer", WINSIZEX, WINSIZEY);
class gameNode
{
private:
	HDC _hdc;
	bool _managerInit;
	RECT _rt;						//Ŭ���̾�Ʈũ��
	bool _cliSizeChange = false;	//Ŭ��ũ��ٲ�.

protected:
	bool _isDebug;
public:
	gameNode();
	virtual ~gameNode();

	virtual HRESULT init();
	virtual HRESULT init(bool managerInit);
	virtual void release();
	virtual void update(); 
	virtual void render();

	image* getBackBuffer()const { return _backBuffer; }
	HDC getHDC()const { return _hdc; }
	HDC getMemDC()const { return _backBuffer->getMemDC(); }

	RECT getClRect() { return _rt; }	//����â�� �簢�� ��������(0, 0, ����, ����)
	bool getCliChange() { return _cliSizeChange; }
	void setCliChange(bool isChange) { _cliSizeChange = isChange; }

	LRESULT MainProc(HWND hWnd, UINT imessage, WPARAM wParam, LPARAM lParam);

	void setIsDebug(bool isDebug) { _isDebug = isDebug; }
};

