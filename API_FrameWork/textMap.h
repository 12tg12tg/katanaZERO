#pragma once
#include "Cmap.h"
#include "door.h"
class textMap : public Cmap
{
private:
	door* _door;
public:
	textMap();
	~textMap();

	HRESULT init();
	void release();
	void update();
	void render();


};

