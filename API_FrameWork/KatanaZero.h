#pragma once
#include "gameNode.h"
class KatanaZero : public gameNode
{
private:

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

