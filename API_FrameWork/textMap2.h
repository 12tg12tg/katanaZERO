#pragma once
#include "Cmap.h"
class textMap2 : public Cmap
{
private:

public:
	textMap2();
	~textMap2();

	HRESULT init();
	void release();
	void update();
	void render();

};

