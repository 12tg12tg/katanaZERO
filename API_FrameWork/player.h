#pragma once
#include "singleton.h"
#define PLAYERSIZEUP 2
enum class PLAYERSTATE
{
	IDLE,
	WALK,
	RUN,
	CROUCH,
	ROLL,
	JUMP,
	ATTACK,
	DIE
};

class player : public Singleton<player>
{
private:
	float _x, _y;
	RECT _rc;
	image* _img;
	animation* _ani;
	int _frameCount;

	float _speed;
	float _maxSpeed;
	FOWARD _foward;
	FOWARD _oldfoward;
	//bool _isFowardChange;

	PLAYERSTATE _state;
	PLAYERSTATE _oldstate;
	//bool _isStateChange;

	bool _isDebug;
public:
	player();
	~player();

	HRESULT init();
	void imageInit();
	void release();
	void update();
	void render();

	void giveFrame();
	void move();
	void stateUpdate();

	PLAYERSTATE getState() { return _state; }
	void setIsDebug(bool isDebug) { _isDebug = isDebug; };
};

