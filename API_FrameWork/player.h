#pragma once
#include "singleton.h"
#define PLAYERSIZEUP 2
class PlayerFSM;
class player : public Singleton<player>
{
private:
	PlayerFSM* _FSM;		//상태패턴
public:
	/*메멘토취소*/
	////메멘토패턴 inner클래스
	//class Memento
	//{
	//private:
	//	friend class player;
	//	friend class Caretaker;
	//	image* _sImg;
	//	float _sZ;
	//	float _sBottom;
	//	float _sX, _sY;
	//	int _sFrameX;
	//	int _sFrameY;
	//	Memento(image* img, float z, float bottom, float x, float y, int frameX, int frameY)
	//		:_sImg(img), _sZ(z), _sBottom(bottom), _sX(x), _sY(y), _sFrameX(frameX), _sFrameY(frameY)
	//	{}
	//};
private:
	float _x, _y, _z;
	float _bottom;
	float _centerX, _centerY;
	RECT _rc;
	Collider* _col;
	image* _img;
	animation* _ani;
	int _frameCount;
	BYTE _alpha;

	float _speed;
	float _maxSpeed;
	FOWARD _foward;

	PLAYERSTATE _state;

	bool _attDash;
public:
	player();
	~player();

	HRESULT init();
	void release();
	void update();
	void render();

	//초기화
	void imageInit();
	void stateInit();
	void soundInit();

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
	Collider* getCollider() { return _col; }
	void setFoward(FOWARD foward) { _foward = foward; }
	void setState(PLAYERSTATE state) { _state = state; }
	bool getAttDash() { return _attDash; }
	void setAttDash(bool attDash) { _attDash = attDash; }
public:
	////메멘토패턴 저장,복구
	//Memento save()const { return Memento(_img, _z, _col->getRect().bottom, _x, _y, _ani->getFrameX(), _ani->getFrameY()); }
	//void restore(const Memento& m)
	//{
	//	_img = m._sImg;
	//	_z = m._sZ;
	//	_bottom = m._sBottom;
	//	_x = m._sX;
	//	_y = m._sY;
	//	_frameX = m._sFrameX;
	//	_frameY = m._sFrameY;
	//}
};

