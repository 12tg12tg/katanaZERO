#pragma once
#include "Cmap.h"
#include "fan.h"
class textMap3 : public Cmap
{
private:
	image* _addforslow;

	fan* _fan;
public:
	textMap3();
	~textMap3();

	HRESULT init();
	void release();
	void update();
	void render();

	void checkPlayerDie();
};

