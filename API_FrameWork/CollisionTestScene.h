#pragma once
#include "gameNode.h"
class CollisionTestScene : public gameNode
{
private:
	int count;
	float _angle;
	Collider* _rotateCol;
	vector<Collider*> _vCollider;
	vector<Collider*>::iterator _viCollider;
public:
	CollisionTestScene();
	~CollisionTestScene();

	HRESULT init();
	void update();
	void release();
	void render();
};

