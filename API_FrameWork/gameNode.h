#pragma once
#include "image.h"
static image* _backBuffer = IMAGE->addImage("backBuffer", WINSIZEX, WINSIZEY);
class gameNode
{
private:
	HDC _hdc;
	bool _managerInit;
	RECT _rt;						//클라이언트크기
	bool _cliSizeChange = false;	//클라크기바뀜.

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

	RECT getClRect() { return _rt; }	//현재창의 사각형 가져오기(0, 0, 가로, 세로)
	bool getCliChange() { return _cliSizeChange; }
	void setCliChange(bool isChange) { _cliSizeChange = isChange; }

	LRESULT MainProc(HWND hWnd, UINT imessage, WPARAM wParam, LPARAM lParam);

	void setIsDebug(bool isDebug) { _isDebug = isDebug; }
};

