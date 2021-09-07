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

	virtual HRESULT init();
	virtual void release();
	virtual void update();
	virtual void render();
	virtual void CheckClear();

};

