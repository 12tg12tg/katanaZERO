#pragma once
#include "Cmap.h"
#include "boss.h"
class textMap4 : public Cmap
{
private:
	boss* _boss;

	int _retryNum;

	bool _clear;
	int _clearCount;
public:
	textMap4();
	~textMap4();

	virtual HRESULT init();
	virtual void release();
	virtual void update();
	virtual void render();
	virtual void CheckClear();

	void afterClearUpdate();
	void afterClearRender();
};

