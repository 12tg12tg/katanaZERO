#pragma once
#include "gameNode.h"
class fan : public gameNode
{
private:
	Collider* _fanCol;
	image* _img;
	image* _img_bw;
	image* _img_slow;
	animation* _ani;
	animation* _slowAni;
	Vec2 _imgPos;
	bool _slowIng;

public:
	fan();
	~fan();

	HRESULT init(float x, float y);
	void release();
	void update();
	void render();
	void makeArr();

	bool canPass();
	Collider* getCollider() { return _fanCol; }
};

