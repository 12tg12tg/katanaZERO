#include "framework.h"
#include "camera.h"

camera::camera()
{
	_cameraRect = { 0,0,0,0 };
	_pivotX = 0.f;
	_pivotY = 0.f;
	_maxX = 0.f;
	_maxY = 0.f;
	_minX = 0.f;
	_minY = 0.f;
	_distanceX = 0.f;
	_distanceY = 0.f;
	_isShake = false;
}

camera::~camera()
{
}

HRESULT camera::init(float pivotX, float pivotY, float maxX, float maxY, float minX, float minY, float disX, float disY)
{
	_maxX = maxX;
	_maxY = maxY;
	_minX = minX;
	_minY = minY;
	_pivotX = pivotX;
	_pivotY = pivotY;
	_distanceX = disX;
	_distanceY = disY;
	_absDistanceX = disX;
	_absDistanceY = disY;

	_cameraSizeX = WINSIZEX;
	_cameraSizeY = WINSIZEY;

	_cameraRect = RectMake(_pivotX - disX, _pivotY - disY, _cameraSizeX, _cameraSizeY);

	_state = CAMERASTATE::FOLLOWPIVOT;

	return S_OK;
}

HRESULT camera::init(float pivotX, float pivotY, float maxX, float maxY, float minX, float minY, float disX, float disY, float sizeX, float sizeY)
{
	_maxX = maxX;
	_maxY = maxY;
	_minX = minX;
	_minY = minY;
	_pivotX = pivotX;
	_pivotY = pivotY;
	_distanceX = disX;
	_distanceY = disY;
	_absDistanceX = disX;
	_absDistanceY = disY;

	_cameraSizeX = sizeX;
	_cameraSizeY = sizeY;

	_cameraRect = RectMake(_pivotX - disX, _pivotY - disY, _cameraSizeX, _cameraSizeY);

	_state = CAMERASTATE::FOLLOWPIVOT;

	return S_OK;
}

void camera::update()
{
	switch (_state)
	{
	case CAMERASTATE::FOLLOWPIVOT:
		//moveToPivot을 통해 갱신 받은 좌표로 사각형을 일단 재구성.
		_pivotX = _moveToPivot.x;
		_pivotY = _moveToPivot.y;
		_cameraRect = RectMake(_pivotX - _absDistanceX, _pivotY - _absDistanceY, _cameraSizeX, _cameraSizeY);
		_distanceX = _absDistanceX;
		_distanceY = _absDistanceY;
		_savePivot.x = _pivotX;
		_savePivot.y = _pivotY;

		//카메라가 공간의 최대값이나 최소값을 넘어섰을경우엔, 사각형 변 재설정.
		if (_cameraRect.left <= _minX)
		{
			_cameraRect.left = _minX;
			_cameraRect.right = _minX + _cameraSizeX;
			_distanceX = _pivotX - _cameraRect.left;
		}
		if (_cameraRect.right >= _maxX)
		{
			_cameraRect.right = _maxX;
			_cameraRect.left = _maxX - _cameraSizeX;
			_distanceX = _pivotX - _cameraRect.left;
		}
		if (_cameraRect.top <= _minY)
		{
			_cameraRect.top = _minY;
			_cameraRect.bottom = _minY + _cameraSizeY;
			_distanceY = _pivotY - _cameraRect.top;

		}
		if (_cameraRect.bottom >= _maxY)
		{
			_cameraRect.bottom = _maxY;
			_cameraRect.top = _maxY - _cameraSizeY;
			_distanceY = _pivotY - _cameraRect.top;
		}
		break;


	case CAMERASTATE::CHANGEPIVOT:
		float angle;
		float _x;
		float _y;

		angle = UTIL::getAngle(_pivotX, _pivotY, _changePivotX, _changePivotY);
		if (_pivotX == _changePivotX && _pivotY == _changePivotY)
		{
			_state = CAMERASTATE::FOLLOWPIVOT;
			break;
		}
		_x = _pivotX;
		_y = _pivotY;
		_savePivot.x = _pivotX;
		_savePivot.y = _pivotY;
		_x += cosf(angle) * _changeSpeed;
		_y -= sinf(angle) * _changeSpeed;
		_cameraRect = RectMake(_x - _distanceX, _y - _distanceY, _cameraSizeX, _cameraSizeY);

		_pivotX = _x;
		_pivotY = _y;

		//pivot 거리가 충분히 가까워졌다면 그냥 자석
		if (UTIL::getDistance(_x, _y, _changePivotX, _changePivotY)
			< 50)
		{
			//_pivotX = _changePivotX;
			//_pivotY = _changePivotY;
			////pivotX = _pivotX;
			////pivotY = _pivotY;
			_pivotX = _savePivot.x;
			_pivotY = _savePivot.y;
			_state = CAMERASTATE::FOLLOWPIVOT;
		}
		break;


	case CAMERASTATE::SHAKE:
		_shakeTime.current++;
		if (_shakeTime.current > _shakeTime.max)
		{
			_isShake = false;
			_cameraRect = RectMake(_savePivot.x - _distanceX, _savePivot.y - _distanceY, _cameraSizeX, _cameraSizeY);
			_state = CAMERASTATE::FOLLOWPIVOT;
		}
		if (_shakeTime.current % _shakeTime.cool == 0)
		{
			_shakePivot.x = RND->getFromTo(_savePivot.x - _shakePower, _savePivot.x + _shakePower);
			_shakePivot.y = RND->getFromTo(_savePivot.y - _shakePower, _savePivot.y + _shakePower);
			_cameraRect = RectMake(_shakePivot.x - _distanceX, _shakePivot.y - _distanceY, _cameraSizeX, _cameraSizeY);
		}
		break;
	}


	if (_isShake == true)
	{

	}
}
void camera::release()
{
}

void camera::FadeInit(int time, FADEKIND fadeKind)
{
	IMAGE->addImage("fadeImg", WINSIZEX, WINSIZEY);
	_fadeInfo.minus = 255 / time;
	_fadeInfo.fadeKind = fadeKind;
	_fadeInfo.alpha = (fadeKind == FADE_IN) ? 255 : 0;
	_fadeInfo.isStart = false;
}

void camera::FadeStart()
{
	_fadeInfo.isStart = true;
	_fadeInfo.alpha = (_fadeInfo.fadeKind == FADE_IN) ? 255 : 0;
}

void camera::FadeUpdate()
{
	if (_fadeInfo.fadeKind == FADE_IN)
	{
		_fadeInfo.alpha -= _fadeInfo.minus;
		if (_fadeInfo.alpha <= 0) _fadeInfo.isStart = false;
	}
	else
	{
		_fadeInfo.alpha += _fadeInfo.minus;
		if (_fadeInfo.alpha >= 255) _fadeInfo.isStart = false;
	}
}

void camera::FadeRender(HDC hdc)
{
	if (_fadeInfo.isStart)
		ZORDER->UIAlphaRender(IMAGE->findImage("fadeImg"), ZUIFADE, 0, 0, 0, _fadeInfo.alpha);
}

void camera::movePivot(float x, float y)
{
	_moveToPivot.x = x;
	_moveToPivot.y = y;
}

void camera::ChangePivot(float x, float y, float speed)
{
	_state = CAMERASTATE::CHANGEPIVOT;
	_changePivotX = x;
	_changePivotY = y;
	_changeSpeed = speed;
}

void camera::setShake(float power, int time, int cool)
{
	_isShake = true;
	_shakePower = power;
	_shakeTime.current = 0;
	_shakeTime.max = time;
	_shakeTime.cool = cool;
	_state = CAMERASTATE::SHAKE;
}

int camera::getRelativeX(float x)
{
	return x - _cameraRect.left;
}

int camera::getRelativeY(float y)
{
	return y - _cameraRect.top;
}

POINT camera::getRelativePoint(POINT pt)
{
	pt.x = pt.x + _cameraRect.left;
	pt.y = pt.y + _cameraRect.top;
	return pt;
}

POINT camera::getRelativeMouse()
{
	POINT cameraMouse;
	POINT relativeMouse;
	relativeMouse.x = m_ptMouse.x - _clientGameRc.left;
	relativeMouse.y = m_ptMouse.y - _clientGameRc.top;
	float rate = (float)relativeMouse.x / RecWidth(_clientGameRc);
	cameraMouse.x = CAMERA->getRect().left + RecWidth(CAMERA->getRect()) * rate;
	rate = (float)relativeMouse.y / RecHeight(_clientGameRc);
	cameraMouse.y = CAMERA->getRect().top + RecHeight(CAMERA->getRect()) * rate;

	return cameraMouse;
}

POINT camera::getClientMouse()
{
	POINT relativeMouse;
	relativeMouse.x = m_ptMouse.x - _clientGameRc.left;
	relativeMouse.y = m_ptMouse.y - _clientGameRc.top;
	float rate = (float)relativeMouse.x / RecWidth(_clientGameRc);
	relativeMouse.x = RecWidth(CAMERA->getRect()) * rate;
	rate = (float)relativeMouse.y / RecHeight(_clientGameRc);
	relativeMouse.y = RecHeight(CAMERA->getRect()) * rate;

	return relativeMouse;
}

RECT camera::getRelativeRect(RECT rc)
{
	rc.left = rc.left - _cameraRect.left;
	rc.right = rc.right - _cameraRect.right;
	rc.top = rc.top - _cameraRect.top;
	rc.bottom = rc.bottom - _cameraRect.bottom;
	return rc;
}
