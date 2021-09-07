#pragma once
#include "singleton.h"
#define PLAYERSIZEUP 2
class PlayerFSM;
class playerSlash;
class player : public Singleton<player>
{
private:
	PlayerFSM* _FSM;		//��������
public:
	/*�޸������*/
	////�޸������� innerŬ����
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

	//�̵�����
	bool _attDash;
	bool _isLand;
	bool _colBlack;
	bool _colYello;
	bool _stillWall;
	bool _ignoreBlack;

	//����
	playerSlash* _slash;

	//���������
	bool _isLaserDie;
	int _laserCount;
	int _laserX;

	//Ÿ�Ӿƿ�
	bool isTimeOut;

	//����
	bool _isGracePeriod;

	//�ܻ�
	bool _specOn;
	float _specCount;
	float _idleCount;
	list<tagGlitch> _spectrum;
	list<tagGlitch>::iterator _ispectrum;
	vector<image*> _vglitch;
	vector<image*>::iterator _viglitch;
public:
	player();
	~player();

	HRESULT init();
	void release();
	void update();
	void render();
	void reInit();

	//�ʱ�ȭ
	void imageInit();
	void stateInit();
	void soundInit();

	//�ܻ�
	void makeSpectrum();

	//�ݶ��̴�����
	void setCollider();

	//Laser �ǰݽ�
	void laserDie();


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
	bool getIsLand() { return _isLand; }
	void setIsLand(bool island) { _isLand = island; }
	bool getColBlack() { return _colBlack; }
	bool getColYello() { return _colYello; }
	void setColBlack(bool colBlack) { _colBlack = colBlack; }
	void setColYello(bool colyel) { _colYello = colyel; }
	void setStillWall(bool sw) { _stillWall = sw; }
	bool getStillWall() { return _stillWall; }
	bool getIgnoreBlack() { return _ignoreBlack; }
	void setIgnoreBlack(bool igblack) { _ignoreBlack = igblack; }
	PlayerFSM* getFSM() { return _FSM; }
	bool getLaserDie() {return _isLaserDie;	}
	void setLaserDie(bool ld) { _isLaserDie = ld; }
	playerSlash* getPlayerSlash() {	return _slash;	}
	bool IsGracePeriod() { return _isGracePeriod; }
	void setIsGravePeriod(bool set) { _isGracePeriod = set; }
	bool getIsTimeOut() {return isTimeOut;	}
	void setIsTimeOut(bool to) { isTimeOut = to; }
public:
	////�޸������� ����,����
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

