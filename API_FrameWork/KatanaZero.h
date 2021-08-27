#pragma once
#include "gameNode.h"
#include "UI.h"
class KatanaZero : public gameNode
{
private:
	UI* m_ui;


	BYTE _slowAlpha;
	bool _isSlow;

	RECT m_debugRc;				//������ؽ�Ʈ ����� �簢��
	bool _isDebug;
public:
	KatanaZero();
	~KatanaZero();

	HRESULT init();
	void release();
	void update();
	void render();

	void dropFrame();
	void setIsDebug(bool isDebug)
	{
		_isDebug = isDebug;
		PLAYER->setIsDebug(isDebug);

	}
};

