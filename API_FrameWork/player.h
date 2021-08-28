#pragma once
#include "singleton.h"
#define PLAYERSIZEUP 2
class PlayerFSM;
class player : public Singleton<player>
{
private:
	PlayerFSM* _FSM;
private:
	float _x, _y;
	float _centerX, _centerY;
	RECT _rc;
	Collider* _col;
	image* _img;
	animation* _ani;
	int _frameCount;

	float _speed;
	float _maxSpeed;
	FOWARD _foward;

	PLAYERSTATE _state;

	bool _isDebug;
public:
	player();
	~player();

	HRESULT init();
	void release();
	void update();
	void render();

	//√ ±‚»≠
	void imageInit();
	void stateInit();

	//getter & setter
	float getX() { return _x; }
	float getY() { return _y; }
	void setX(float x) { _x = x; }
	void setY(float y) { _y = y; }
	float getMaxSpeed() { return _maxSpeed; }
	float getSpeed() { return _speed; }
	void setSpeed(float speed) { _speed = speed; }
	image* getImage() { return _img; }
	animation* getAni() { return _ani; }
	PLAYERSTATE getState() { return _state; }
	FOWARD getFoward() { return _foward; }
	void setFoward(FOWARD foward) { _foward = foward; }
	void setState(PLAYERSTATE state) { _state = state; }
	void setIsDebug(bool isDebug) { _isDebug = isDebug; };
};

