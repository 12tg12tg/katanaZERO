#include "framework.h"
#include "verlaser.h"
#include "playerstate.h"
verlaser::verlaser()
{
	IMAGE->addFrameImage("verlaser", "images/bullet/boss_verlazer.bmp", 180, 750, 5, 1, true);
}

verlaser::~verlaser()
{
}

void verlaser::fire(float x, float y)
{
	tagBullet newBul;
	newBul.img = IMAGE->findImage("verlaser");
	newBul.ani = ANIMATION->addNoneKeyAnimation("verlaser", 3, 0, 11, false, false);
	newBul.x = x - newBul.img->getFrameWidth()/2;
	newBul.y = y;
	newBul.bwimg = nullptr;
	newBul.foward = FOWARD::NONE;
	newBul.col = COLLISION->addCollider(Vec2(newBul.x + newBul.img->getFrameWidth()/2, y+newBul.img->getFrameHeight()/2),
		Vec2(newBul.img->getFrameWidth(), newBul.img->getFrameHeight()),
		COLLIDER_TYPE::BULLET_ENEMY, ZCOL3);
	newBul.col->setCanCol(false);
	newBul.z = ZEFFECT2;
	newBul.bottom = 0;
	newBul.speed = 0.f;
	newBul.count = 0;
	newBul.alpha = 0;
	newBul.firstTime = true;
	newBul.secendTime = false;
	_vBullet.push_back(newBul);
}

void verlaser::release()
{
	for (_viBullet = _vBullet.begin(); _viBullet != _vBullet.end(); )
	{
		COLLISION->erase(_viBullet->col);
		_viBullet = _vBullet.erase(_viBullet);
	}
}

void verlaser::update()
{
	//삭제되지않는업뎃
	for (_viBullet = _vBullet.begin(); _viBullet != _vBullet.end(); ++_viBullet)
	{
		//처음으로 애니끝낫을때 충돌ON
		if (_viBullet->firstTime && !_viBullet->ani->isPlay()) {
			int rnd = RND->getInt(3);
			if (rnd == 0) SOUND->play("sound_boss_huntresslaser_shot_1", 0.1f);
			else if (rnd == 1) SOUND->play("sound_boss_huntresslaser_shot_2", 0.1f);
			else SOUND->play("sound_boss_huntresslaser_shot_3", 0.1f);

			CAMERA->setShake(7, 10, 1);
			_viBullet->col->setCanCol(true);
			_viBullet->firstTime = false;
			_viBullet->secendTime = true;
		}

		//플레이어와 충돌시 충돌OFF
		if (_viBullet->col->isEnterThere(COLLIDER_TYPE::PLAYER_UNIT) && !PLAYER->IsGracePeriod()) {
			PLAYER->getFSM()->SetDeath(_viBullet->angle, 10);
			COLLISION->erase(_viBullet->col);
		}

		//활성화후 시간증가.
		if (_viBullet->secendTime) {
			_viBullet->count += 1 * TIME->getGameTimeRate();
			if (_viBullet->count > 10) {
				_viBullet->secendTime = false;
				ANIMATION->changeNonKeyAnimation(_viBullet->ani, "verlaser", 0, 3, 11, false, false);
			}
		}
	}


	//소멸확인(&콜라이더)
	for (_viBullet = _vBullet.begin(); _viBullet != _vBullet.end(); )
	{
		//사라지는애니
		if (!_viBullet->firstTime && !_viBullet->secendTime && !_viBullet->ani->isPlay()){
			COLLISION->erase(_viBullet->col);
			_viBullet = _vBullet.erase(_viBullet);
		}
		else ++_viBullet;
	}
}

void verlaser::render()
{
	for (_viBullet = _vBullet.begin(); _viBullet != _vBullet.end(); ++_viBullet)
	{
		ZORDER->ZorderAniRender(_viBullet->img, _viBullet->z, _viBullet->bottom, _viBullet->x, _viBullet->y, _viBullet->ani);
		ZORDER->SaveAniRender(_viBullet->img, _viBullet->bwimg, _viBullet->z, _viBullet->bottom, _viBullet->x, _viBullet->y, _viBullet->ani);
	}
}
