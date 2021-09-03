#pragma once
#include "gameNode.h"
class Cmap : public gameNode
{
protected:
	image* _map;
	image* _bwmap;
	image* _map_front;
	image* _bwmap_front;
	image* _colmap;

	vector2 _startPt;
	Collider* _goal;

	bool _isClear;
	string _nextScene;

	float prevx, prevy;
	RECT prevRc;
public:
	Cmap();
	~Cmap();

	virtual HRESULT init();
	virtual void release();
	virtual void update();
	virtual void render();

	virtual void coltoMap();
	virtual void CheckClear();
	virtual void goalCol();

	string getNextSceneName() { return _nextScene; }
	image* getColmap() { return _colmap; }
	image* getBwmap() { return _bwmap; }
	image* getMap() { return _map; }
	image* getBwmap_front() { return _bwmap_front; }
	image* getMap_front() { return _map_front; }
};

