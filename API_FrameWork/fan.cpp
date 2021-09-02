#include "framework.h"
#include "fan.h"

fan::fan()
{
	_img = IMAGE->addFrameImage("fan_nor", "images/bullet/fan.bmp", 990, 388, 11, 2, true);
	_img_slow = IMAGE->addFrameImage("fan_slow", "images/bullet/fan_slow.bmp", 990, 388, 11, 2, true);
	_img_bw = IMAGE->addFrameImage("fan_bw", "images/bullet/fan_bw.bmp", 990, 388, 11, 2, true);
}

fan::~fan()
{
}

HRESULT fan::init(float x, float y)
{
	int arr[22] = { 0, 5, 10, 15, 20, 3, 8, 13, 18, 1, 6, 11, 16, 21, 4, 9, 14, 19, 2, 7, 12, 17 };
	_ani = ANIMATION->addNoneKeyAnimation("fan_nor", arr, sizeof(arr)/sizeof(int), 54, true);
	_slowAni = ANIMATION->addNoneKeyAnimation("fan_nor", 54, false, true);
	_imgPos = Vec2(x, y);
	_fanCol = COLLISION->addCollider(Vec2(_imgPos.x + _img->getFrameWidth() / 2, _imgPos.y + _img->getFrameHeight() / 2),
		Vec2(19, 221), COLLIDER_TYPE::BULLET_ENEMY, ZCOL3, false);
	_slowIng = false;
	return S_OK;
}

void fan::release()
{
}

void fan::update()
{
	makeArr();
}

void fan::render()
{
	if (MAIN->getIsSlow()) {
		ZORDER->ZorderAniRender(_img_slow, ZUNIT, _fanCol->getBottom(), _imgPos.x, _imgPos.y, _slowAni);
	}
	else ZORDER->ZorderAniRender(_img, ZUNIT, _fanCol->getBottom(), _imgPos.x, _imgPos.y, _ani);
	ZORDER->SaveAniRender(_img, _img_bw, ZUNIT, _fanCol->getBottom(), _imgPos.x, _imgPos.y, _ani);
}

void fan::makeArr()
{
	if (_slowIng) {
		if (!MAIN->getIsSlow()) {
			_slowIng = false;
			int arr[22];
			int startIndex = _slowAni->getNowPlayIndex();
			for (size_t i = 0; i < 22; i++)
			{
				arr[i] = startIndex+=5;
				if (startIndex > 21) startIndex -= 22;				
			}
			ANIMATION->changeNonKeyAnimation(_ani, "fan_nor", arr, 22, 54, true);
		}
	}
	else {
		if (MAIN->getIsSlow()) {
			_slowIng = true;
			int arr[22];
			int startIndex = _ani->getNowPlayIndex();
			for (size_t i = 0; i < 22; i++)
			{
				arr[i] = startIndex++;
				if (startIndex > 21) startIndex -= 22;
			}
			ANIMATION->changeNonKeyAnimation(_slowAni, "fan_nor", arr, 22, 54, true);
		}
	}
}

bool fan::canPass()
{
	if (!MAIN->getIsSlow()) {
		return false;
		//if (PLAYER->getState() == PLAYERSTATE::ROLL) {
		//}
	}
	else {
		int nowIndex = _ani->getNowPlayIndex();
		if (nowIndex >= 14 && nowIndex <= 17) {
			return false;
		}
		return true;
	}
}
