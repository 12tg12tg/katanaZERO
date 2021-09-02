#pragma once
#include "Cmap.h"
class textMap3 : public Cmap
{
private:
	image* _addforslow;
public:
	textMap3();
	~textMap3();

	HRESULT init();
	void release();
	void update();
	void render();
};

