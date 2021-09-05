#include "framework.h"
#include "effect.h"

effect::effect() : 
	_effectImage(nullptr),
	_effectAni(nullptr), 
	_isRunning(NULL),
	_x(0),
	_y(0),
	_alpha(255)
{
}

effect::~effect()
{
}

HRESULT effect::init(image* effectImage, int frameW, int frameH, int FPS, float elapsedTime, bool isSave, image* bwImg)
{
	//이펙트 이미지가 없을 때 리턴
	if (!effectImage) return E_FAIL;
	_effectImage = effectImage;
	_elapsedTime = elapsedTime;
	_isRunning = false;
	//이펙트 애니메이션이 없으면, 새로만들기
	if (!_effectAni) _effectAni = new animation;

	_frameW = frameW;
	_frameH = frameH;
	_effectAni->init(_effectImage->getWidth(), _effectImage->getHeight(), frameW, frameH);
	_effectAni->setDefPlayFrame();
	_effectAni->setFPS(FPS);

	_isRotate = false;
	_radian = 0;
	_isAlpha = false; 
	_alpha = 0;

	_isSave = isSave;
	_bwImage = bwImg;

	return S_OK;
}

HRESULT effect::initAlpha(image* effectImage, int frameW, int frameH, int FPS, float elapsedTime, bool isSave, image* bwImg)
{
	//이펙트 이미지가 없을 때 리턴
	if (!effectImage) return E_FAIL;
	_effectImage = effectImage;
	_elapsedTime = elapsedTime;
	_isRunning = false;
	//이펙트 애니메이션이 없으면, 새로만들기
	if (!_effectAni) _effectAni = new animation;

	_frameW = frameW;
	_frameH = frameH;
	_effectAni->init(_effectImage->getWidth(), _effectImage->getHeight(), frameW, frameH);
	_effectAni->setDefPlayFrame();
	_effectAni->setFPS(FPS);

	_isRotate = false;
	_radian = 0;
	_isAlpha = true;
	_alpha = 255;

	_isSave = isSave;
	_bwImage = bwImg;

	return S_OK;
}

HRESULT effect::initRotate(image* effectImage, int frameW, int frameH, int FPS, float elapsedTime, bool isSave, image* bwImg)
{
	//이펙트 이미지가 없을 때 리턴
	if (!effectImage) return E_FAIL;
	_effectImage = effectImage;
	_elapsedTime = elapsedTime;
	_isRunning = false;
	//이펙트 애니메이션이 없으면, 새로만들기
	if (!_effectAni)
		_effectAni = new animation;

	_frameW = frameW;
	_frameH = frameH;
	_effectAni->init(_effectImage->getWidth(), _effectImage->getHeight(), frameW, frameH);
	_effectAni->setDefPlayFrame();
	_effectAni->setFPS(FPS);

	_isRotate = true;
	_radian = 0;
	_isAlpha = false;
	_alpha = 0;

	_isSave = isSave;
	_bwImage = bwImg;

	return S_OK;
}

HRESULT effect::initRotateAlpha(image* effectImage, int frameW, int frameH, int FPS, float elapsedTime, bool isSave, image* bwImg)
{
	//이펙트 이미지가 없을 때 리턴
	if (!effectImage) return E_FAIL;
	_effectImage = effectImage;
	_elapsedTime = elapsedTime;
	_isRunning = false;
	//이펙트 애니메이션이 없으면, 새로만들기
	if (!_effectAni) _effectAni = new animation;

	_frameW = frameW;
	_frameH = frameH;
	_effectAni->init(_effectImage->getWidth(), _effectImage->getHeight(), frameW, frameH);
	_effectAni->setDefPlayFrame();
	_effectAni->setFPS(FPS);

	_isRotate = true;
	_radian = 0;
	_isAlpha = true;
	_alpha = 255;

	_isSave = isSave;
	_bwImage = bwImg;

	return S_OK;
}

void effect::release()
{
	SAFE_DELETE(_effectAni);
}

void effect::update()
{
	if (_isRunning)
	{
		_effectAni->frameUpdate(_elapsedTime);
	}
	//이펙트 애니메이션이 종료가 되면 멈추자.
	if (!_effectAni->isPlay())
		stopEffect();
}

void effect::render()
{
	if (_isRunning)
	{
		if (_isAlpha && _isRotate) {
			ZORDER->ZorderRotateAlphaFrameRender(_effectImage, _z, _y + _frameH, _x + (_effectAni->getFrameWidth() / 2), _y + (_effectAni->getFrameHeight() / 2), _radian, _effectAni->getFrameX(), _effectAni->getFrameY(), _alpha);
			if (_isSave) ZORDER->SaveRotateAlphaFrameRender(_effectImage, _bwImage, _z, _y + _frameH, _x + (_effectAni->getFrameWidth() / 2), _y + (_effectAni->getFrameHeight() / 2), _radian, _effectAni->getFrameX(), _effectAni->getFrameY(), _alpha);
		}
		else if (!_isAlpha && _isRotate) {
			ZORDER->ZorderRotateFrameRender(_effectImage, _z, _y + _frameH,	_x + (_effectAni->getFrameWidth() / 2),	_y + (_effectAni->getFrameHeight() / 2), _radian, _effectAni->getFrameX(), _effectAni->getFrameY());
			if (_isSave) ZORDER->SaveRotateFrameRender(_effectImage, _bwImage, _z, _y + _frameH, _x + (_effectAni->getFrameWidth() / 2), _y + (_effectAni->getFrameHeight() / 2), _radian, _effectAni->getFrameX(), _effectAni->getFrameY());
		}
		else if (_isAlpha && !_isRotate) {
			ZORDER->ZorderAniAlphaRender(_effectImage, _z, _y + _frameH, _x, _y, _effectAni, _alpha);
			if (_isSave) ZORDER->SaveAniAlphaRender(_effectImage, _bwImage, _z, _y + _frameH, _x, _y, _effectAni, _alpha);
		}
		else {
			ZORDER->ZorderAniRender(_effectImage, _z, _y + _frameH, _x, _y, _effectAni);
			if (_isSave) ZORDER->SaveAniRender(_effectImage, _bwImage, _z, _y + _frameH, _x, _y, _effectAni);
		}
	}
}
void effect::startEffect(float z, int x, int y, float radian, BYTE alpha)
{
	//위치 좌표를 센터 좌표로 만들기
	_x = x - (_effectAni->getFrameWidth() / 2);
	_y = y - (_effectAni->getFrameHeight() / 2);
	_z = z;

	if (_isAlpha) _alpha = alpha;
	if (_isRotate)_radian = radian;

	_isRunning = true;
	_effectAni->start();

}

void effect::moveEffect(float x, float y)
{
	_x = x - (_effectAni->getFrameWidth() / 2);
	_y = y - (_effectAni->getFrameHeight() / 2);
}

void effect::offsetEffect(float x, float y)
{
	_x += x;
	_y += y;
}

void effect::stopEffect()
{
	_isRunning = false;
}
