#pragma once
#include "Cmap.h"
class textMap4 : public Cmap
{
private:

public:
	textMap4();
	~textMap4();

	virtual HRESULT init();
	virtual void release();
	virtual void update();
	virtual void render();
	virtual void CheckClear();
};

