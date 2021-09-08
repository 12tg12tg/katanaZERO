#pragma once
#include "Cmap.h"
#include "boss.h"
class textMap4 : public Cmap
{
private:
	boss* _boss;

	int _retryNum;
public:
	textMap4();
	~textMap4();

	virtual HRESULT init();
	virtual void release();
	virtual void update();
	virtual void render();
	virtual void CheckClear();
};

