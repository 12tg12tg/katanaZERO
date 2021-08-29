#pragma once
#include "vector2.h"
#define NOCOLCOLOR	RGB(20, 255, 20)
#define COLCOLOR	RGB(255, 20, 20)
class collisionManager;
class Collider
{
private:
	static DWORD g_id;		//��üid
private:
	Vec2			_pos;			//��ġ
	Vec2			_size;			//ũ��
	float			_z;				//Z-layer
	DWORD			_id;			//����id
	UINT			_color;			//�浹��
	COLLIDER_TYPE	_type;			//�浹ü����
	float			_isRotate;		//ȸ���ϴ���
	float			_rotate;		//ȸ����

	bool			_isEnter;		//����浹
	bool			_isIng;			//�浹��
	bool			_isExit;		//�浹��

	map<DWORD, Collider*> _others;
public:
	Collider();
	~Collider();

	void init(vector2 pos, vector2 size, COLLIDER_TYPE type, float z, bool isRotate = false, float angle = 0);
	void render();

	vector2 getPos() { return _pos; }
	void setPos(const vector2& pos) { _pos = pos; }

	const float& getAngle() { return _rotate; }
	void setAngle(float angle) { _rotate = angle; }

	vector2 getSize() { return _size; }
	void setSize(const vector2& size) { _size = size; }

	DWORD getID() { return _id; }
	COLLIDER_TYPE getType() { return _type; }
	RECT getRect() {
		RECT rc = RectMake(_pos.x - _size.x / 2, _pos.y - _size.y / 2, _size.x, _size.y);
		return rc;
	}

	bool isColEnter() { return _isEnter;}
	bool isColIng() { return _isIng;}
	bool isColExit() { return _isExit;}
	
	map<DWORD, Collider*>& getOthers() { return _others; }

	friend collisionManager;
};

