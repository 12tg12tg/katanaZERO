#pragma once
#include "gameNode.h"
#include "bossBulletManater.h"
class BossFSM;
class boss : public gameNode
{
public:
	BossFSM* _FSM;
	bossBulletManater* _bm;
public:
	float _x, _y, _z;
	float _bottom;
	image* _img;
	image* _bwimg;
	animation* _ani;
	Collider* _col;
	FOWARD _foward;
	float _speed;
	BOSSSTATE _state;

	int _hp;

	bool _isGracePeriod;
	float _effectAngle;
	effect* _hitEffect;

	bool _isSideCol;
	bool _isLand;
	bool _isCut;

	image* _headImg;
	animation* _headAni;
	float _headX;
	float _headY;
	float _headjumpPower;
	float _headgravity;
	float _headmaxGravity;
	float _headSpeed;
	float _headbreak;
	bool _headLand;
public:
	boss();
	~boss();

	HRESULT init(float x, float y);
	void release();
	void update();
	void render();
	void reInit();

	void stateInit();
	void soundInit();
	void imageInit();

	void setCollider();
	void setpos();
	void headFly();

	void checkHit();

	BossFSM* getFSM() { return _FSM; }


};

