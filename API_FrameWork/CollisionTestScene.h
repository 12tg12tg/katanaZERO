#pragma once
#include "gameNode.h"
#include "fan.h"
class CollisionTestScene : public gameNode
{
private:
	int count;
	float _angle;
	Collider* _rotateCol;
	vector<Collider*> _vCollider;
	vector<Collider*>::iterator _viCollider;

	fan* _fan;
public:
	CollisionTestScene();
	~CollisionTestScene();

	HRESULT init();
	void update();
	void release();
	void render();
};

