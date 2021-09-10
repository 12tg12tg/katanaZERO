#pragma once
#include "gameNode.h"
class conversation :public gameNode
{
private:

public:
	conversation();
	~conversation();

	HRESULT init();
	void release();
	void update();
	void render();

};

