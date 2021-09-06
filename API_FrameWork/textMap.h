#pragma once
#include "Cmap.h"
#include "door.h"
#include "grunt.h"
class textMap : public Cmap
{
private:
	door* _door;
	grunt* _grunt;
public:
	textMap();
	~textMap();

	HRESULT init();
	void release();
	void update();
	void render();


};

