#pragma once
#include "gameNode.h"
class textMap : public gameNode
{
private:
	image* _map;
	image* _colmap;
public:
	textMap();
	~textMap();

	HRESULT init();
	void release();
	void update();
	void render();

	image* getColmap() { return _colmap; }
};

