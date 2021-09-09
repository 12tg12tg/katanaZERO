#pragma once
#include "vector2.h"
#define NOCOLCOLOR	RGB(20, 255, 20)
#define COLCOLOR	RGB(255, 20, 20)
class collisionManager;
class Collider;
struct ColEnter
{
	DWORD id;
	COLLIDER_TYPE type;
	Collider* col;

	ColEnter(DWORD id, COLLIDER_TYPE type, Collider* col)
		:id(id), type(type), col(col)
	{
	}
};
class Collider
{
private:
	static DWORD g_id;		//전체id
private:
	Vec2			_pos;			//위치
	Vec2			_size;			//크기
	float			_z;				//Z-layer
	DWORD			_id;			//고유id
	UINT			_color;			//충돌색
	COLLIDER_TYPE	_type;			//충돌체종류
	float			_isRotate;		//회전하는지
	float			_rotate;		//회전각

	bool			_isEnter;		//방금충돌
	vector<ColEnter> _enterVec;		//방금충돌
	bool			_isIng;			//충돌중
	bool			_isExit;		//충돌끝

	bool			_canCollision;	//충돌가능상태?
	bool			_isFirstCheck;	//충돌체크에 처음왔냐. 그렇다면 enter벡터 초기화.

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

	bool getCanCol() { return _canCollision; }
	void setCanCol(bool cancol) { _canCollision = cancol; }

	DWORD getID() { return _id; }
	COLLIDER_TYPE getType() { return _type; }
	RECT getRect() {
		RECT rc = RectMake(_pos.x - _size.x / 2, _pos.y - _size.y / 2, _size.x, _size.y);
		return rc;
	}
	float getBottom() { return _pos.y + _size.y / 2; }

	void eraseThisInEngerVec(DWORD id) {
		for (auto iter = _enterVec.begin(); iter != _enterVec.end(); ++iter) {
			if (iter->id == id) {
				_enterVec.erase(iter);
				return;
			}
		}
	}
	bool isColEnter() { return _isEnter; }
	bool isColIng() { return _isIng; }
	bool isColExit() { return _isExit; }

	bool isEnterThere(COLLIDER_TYPE type);
	bool isThere(COLLIDER_TYPE type);
	void enterVecClear() { _enterVec.clear(); }
	
	map<DWORD, Collider*>& getOthers() { return _others; }
	vector<ColEnter>& getEnters() { return _enterVec; }
	bool findOthers(DWORD id);

	friend collisionManager;
};

