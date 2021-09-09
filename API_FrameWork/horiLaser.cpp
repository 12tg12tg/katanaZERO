#include "framework.h"
#include "horiLaser.h"
#include "playerstate.h"
horiLaser::horiLaser()
{
	IMAGE->addFrameImage("horizonlaser", "images/bullet/horizonlazer.bmp", 1000, 240, 1, 10, true);
}

horiLaser::~horiLaser()
{
}

void horiLaser::fire(float x, float y, FOWARD foward)
{
	tagBullet newBul;
	newBul.img = IMAGE->findImage("horizonlaser");

	if (foward == FOWARD::LEFT) {
		int arr[] = { 0 + 5, 1 + 5, 0 + 5, 1 + 5, 0 + 5, 1 + 5, 0 + 5, 1 + 5, 0 + 5, 1 + 5, 0 + 5, 1 + 5, 2 + 5, 3 + 5 };
		newBul.ani = ANIMATION->addNoneKeyAnimation("horizonlaser", arr, sizeof(arr) / sizeof(int), 10, false);
	}
	else {
		int arr[] = { 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 2, 3 };
		newBul.ani = ANIMATION->addNoneKeyAnimation("horizonlaser", arr, sizeof(arr) / sizeof(int), 10, false);
	}

	if (foward == FOWARD::RIGHT) newBul.x = x;
	else newBul.x = x - newBul.img->getFrameWidth();
	newBul.y = y;

	newBul.bwimg = nullptr;
	newBul.z = ZEFFECT2;
	newBul.bottom = 0;
	newBul.count = 0.f;

	newBul.firstTime = true;
	newBul.secendTime = false;
	newBul.isOn = false;
	newBul.col = COLLISION->addCollider(Vec2(newBul.x + newBul.img->getFrameWidth()/2, newBul.y + newBul.img->getFrameHeight() / 2),
		Vec2(newBul.img->getFrameWidth(), newBul.img->getFrameHeight()),
		COLLIDER_TYPE::BULLET_ENEMY, ZCOL3);
	newBul.col->setCanCol(false);
	_vBullet.push_back(newBul);
}

void horiLaser::release()
{
}

void horiLaser::update()
{
	//삭제되지않는업뎃
	for (_viBullet = _vBullet.begin(); _viBullet != _vBullet.end(); ++_viBullet)
	{
		//플레이어와 충돌시 충돌OFF
		if (_viBullet->col->isEnterThere(COLLIDER_TYPE::PLAYER_UNIT) && !PLAYER->IsGracePeriod()) {
			PLAYER->getFSM()->SetDeath(PI, 10);
			_viBullet->col->setCanCol(false);
		}

		//애니끝나면 흰주흰주흰주
		if (_viBullet->firstTime && !_viBullet->ani->isPlay()) {
			_viBullet->firstTime = false;
			_viBullet->secendTime = true;
			_viBullet->isOn = true;
			_viBullet->col->setCanCol(true);
			if (_viBullet->foward == FOWARD::LEFT) {
				int arr[] = { 4 + 5, 1 + 5, 0 + 5 };
				ANIMATION->changeNonKeyAnimation(_viBullet->ani, "horizonlaser", arr, sizeof(arr) / sizeof(int), 10, false);
			}
			else {
				int arr[] = { 4, 1, 0 };
				ANIMATION->changeNonKeyAnimation(_viBullet->ani, "horizonlaser", arr, sizeof(arr) / sizeof(int), 10, false);
			}
			CAMERA->setShake(10, 15, 1);
		}

		////시간증가
		//_viBullet->count += 1 * TIME->getGameTimeRate();
	}


	//소멸확인(&콜라이더)
	for (_viBullet = _vBullet.begin(); _viBullet != _vBullet.end(); )
	{
		//사라지는애니
		//if (_viBullet->count > 150) {
		//	COLLISION->erase(_viBullet->col);
		//	_viBullet = _vBullet.erase(_viBullet);
		//}
		if (!_viBullet->firstTime && !_viBullet->ani->isPlay()) {
			COLLISION->erase(_viBullet->col);
			_viBullet = _vBullet.erase(_viBullet);
		}
		else ++_viBullet;
	}
}

void horiLaser::render()
{
	for (_viBullet = _vBullet.begin(); _viBullet != _vBullet.end(); ++_viBullet)
	{
		ZORDER->ZorderAniRender(_viBullet->img, _viBullet->z, _viBullet->bottom, _viBullet->x, _viBullet->y, _viBullet->ani);
		ZORDER->SaveAniRender(_viBullet->img, _viBullet->bwimg, _viBullet->z, _viBullet->bottom, _viBullet->x, _viBullet->y, _viBullet->ani);
	}
}
