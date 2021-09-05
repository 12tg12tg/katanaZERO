#pragma once
#include "Cmap.h"
#include "fan.h"
#include "laser.h"
class textMap3 : public Cmap
{
private:
	image* _addforslow;

	fan* _fan;
	laser* _laser;
public:
	textMap3();
	~textMap3();

	HRESULT init();
	void release();
	void update();
	void render();

	void checkPlayerDie();
};

