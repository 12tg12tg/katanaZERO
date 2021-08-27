#include "framework.h"
#include "progressBar.h"

progressBar::progressBar()
{
}

progressBar::~progressBar()
{
}

HRESULT progressBar::init(char* frontImage, char* backImage, float x, float y, int width, int height, bool isUi, bool isAlpha, BYTE alpha)
{
	_x = x;
	_y = y;

	_rcProgress = RectMake(x, y, width, height);
	_progressBarFront = new image;
	_progressBarFront->init(frontImage, x, y, width, height, true, RGB(255, 0, 255));

	_progressBarBack = new image;
	_progressBarBack->init(backImage, x, y, width, height, true, RGB(255, 0, 255));

	_width = _progressBarFront->getWidth();

	_isUi = isUi;

	_isAlpha = isAlpha;
	_alpha = alpha;

	return S_OK;
}

HRESULT progressBar::initOnlyFront(char* frontImage, float x, float y, int width, int height, bool isUi, bool isAlpha, BYTE alpha)
{
	_x = x;
	_y = y;

	_rcProgress = RectMake(x, y, width, height);
	_progressBarFront = new image;
	_progressBarFront->init(frontImage, x, y, width, height, true, RGB(255, 0, 255));

	_progressBarBack = nullptr;

	_width = _progressBarFront->getWidth();

	_isUi = isUi;

	_isAlpha = isAlpha;
	_alpha = alpha;

	return S_OK;
}

void progressBar::release()
{
	SAFE_DELETE(_progressBarFront);
	SAFE_DELETE(_progressBarBack);
}

void progressBar::update()
{
	_rcProgress = RectMakeCenter(_x + _progressBarFront->getWidth() / 2,
		_y + _progressBarFront->getHeight() / 2,
		_progressBarFront->getWidth(), _progressBarFront->getHeight());
}

void progressBar::render(float z)
{
	if (_isUi) {
		if (!_isAlpha) {
			if(_progressBarBack) ZORDER->UIRender(_progressBarBack, z - 0.001, 0, _rcProgress.left, _y, 0, 0, _progressBarBack->getWidth(), _progressBarBack->getHeight());
			ZORDER->UIRender(_progressBarFront, z, 0, _rcProgress.left, _y, 0, 0, _width, _progressBarFront->getHeight());
		}
		else {
			if (_progressBarBack) ZORDER->UIAlphaRender(_progressBarBack, z - 0.001, 0, _rcProgress.left, _y, 0, 0, _progressBarBack->getWidth(), _progressBarBack->getHeight(), _alpha);
			ZORDER->UIAlphaRender(_progressBarFront, z, 0, _rcProgress.left, _y, 0, 0, _width, _progressBarFront->getHeight(), _alpha);
		}
	}
	else {
		if (!_isAlpha) {
			if (_progressBarBack) ZORDER->ZorderRender(_progressBarBack, z-0.001, 0, _rcProgress.left, _y, 0, 0, _progressBarBack->getWidth(), _progressBarBack->getHeight());
			ZORDER->ZorderRender(_progressBarFront, z, 1, _rcProgress.left, _y, 0, 0, _width, _progressBarFront->getHeight());
		}
		else {
			if (_progressBarBack) ZORDER->ZorderAlphaRender(_progressBarBack, z - 0.001, 0, _rcProgress.left, _y, 0, 0, _progressBarBack->getWidth(), _progressBarBack->getHeight(), _alpha);
			ZORDER->ZorderAlphaRender(_progressBarFront, z, 1, _rcProgress.left, _y, 0, 0, _width, _progressBarFront->getHeight(), _alpha);
		}
	}
}

void progressBar::setGauge(float currentGauge, float maxGauge, BYTE alpha)
{
	_width = (currentGauge / maxGauge) * _progressBarFront->getWidth();
	_alpha = alpha;
}
