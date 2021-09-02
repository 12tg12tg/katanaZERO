#include "framework.h"
#include "Collider.h"

DWORD Collider::g_id = 0;

Collider::Collider()
	: _color(RGB(20, 255, 20))
	, _isEnter(false)
	, _isIng(false)
	, _isExit(false)
	, _isRotate(false)
	, _rotate(0)
{
}

Collider::~Collider()
{
}

void Collider::init(vector2 pos, vector2 size, COLLIDER_TYPE type, float z, bool isRotate, float angle)
{
	_pos = pos;
	_size = size;
	_type = type;
	_z = z;
	_isRotate = isRotate;
	_rotate = angle;
	_id = g_id++;
}

void Collider::render()
{
	if (_isIng || _isEnter) _color = COLCOLOR;
	else _color = NOCOLCOLOR;

	if (_isRotate)
		ZORDER->ZorderRectangleRotate(getRect(), _z, _rotate, _color);
	else
		ZORDER->ZorderRectangleColor(getRect(), _z, _color);
}

bool Collider::findOthers(DWORD id)
{
	auto iter = _others.find(id);
	if (iter == _others.end()) return false;
	else return true;
}
