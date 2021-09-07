#pragma once
#include "Cmap.h"
#include "fan.h"
#include "laser.h"
#include "grunt.h"
class textMap3 : public Cmap
{
private:
	image* _addforslow;

	fan* _fan;
	laser* _laser;
	grunt* _grunt;
public:
	textMap3();
	~textMap3();

	HRESULT init();
	void release();
	void update();
	void render();
	virtual void CheckClear();

	void checkPlayerDie();
};

