#pragma once
#include "Cmap.h"
#include "laser.h"
class textMap2 : public Cmap
{
private:
	laser* _laser;
	RECT _butRgn;
	animation* _butAni;
	bool _canTurnOnOff;



public:
	textMap2();
	~textMap2();

	HRESULT init();
	void release();
	void update();
	void render();

};

