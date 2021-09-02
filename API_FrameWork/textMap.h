#pragma once
#include "Cmap.h"
class textMap : public Cmap
{
private:

public:
	textMap();
	~textMap();

	HRESULT init();
	void release();
	void update();
	void render();


};

