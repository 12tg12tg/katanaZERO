#include "framework.h"
#include "sweeplaser.h"
#include "playerstate.h"

sweeplaser::sweeplaser()
{
	IMAGE->addFrameImage("sweeplaser", "images/bullet/sweeplazer.bmp", 1000, 120, 1, 5, true);
}

sweeplaser::~sweeplaser()
{
}

void sweeplaser::fire(float x, float y, FOWARD foward) //left먼저제작.
{
	tagBullet newBul;
	newBul.img = IMAGE->findImage("sweeplaser");
	int arr[] = { 0, 1, 0, 1, 2, 3 };
	newBul.ani = ANIMATION->addNoneKeyAnimation("sweeplaser", arr, sizeof(arr)/sizeof(int), 100, false);
	newBul.x = x;
	newBul.y = y;
	newBul.bwimg = nullptr;
	newBul.foward = foward;
	if(foward == FOWARD::LEFT) newBul.angle = PI;
	else newBul.angle = PI2;
	newBul.z = ZEFFECT2;
	newBul.bottom = 0;
	newBul.speed = RADIAN(4.5);
	newBul.count = 0;
	newBul.alpha = 0;
	newBul.firstTime = true;
	newBul.secendTime = false;
	newBul.isOn = false;
	newBul.centerX = x + cosf(newBul.angle) * (newBul.img->getFrameWidth() / 2 + DISTANCE);
	newBul.centerY = y - sinf(newBul.angle) * (newBul.img->getFrameWidth() / 2 + DISTANCE);
	newBul.col = COLLISION->addCollider(Vec2(newBul.centerX, newBul.centerY),
		Vec2(newBul.img->getFrameWidth(), newBul.img->getFrameHeight()),
		COLLIDER_TYPE::BULLET_ENEMY, ZCOL3, true, newBul.angle);
	_vBullet.push_back(newBul);

	//CAMERA->setShake(7, 10, 1);
}

void sweeplaser::release()
{
	for (_viBullet = _vBullet.begin(); _viBullet != _vBullet.end(); )
	{
		COLLISION->erase(_viBullet->col);
		_viBullet = _vBullet.erase(_viBullet);
	}
}

void sweeplaser::update()
{
	//삭제되지않는업뎃
	for (_viBullet = _vBullet.begin(); _viBullet != _vBullet.end(); ++_viBullet)
	{
		//플레이어와 충돌시 충돌OFF
		if (_viBullet->col->isEnterThere(COLLIDER_TYPE::PLAYER_UNIT) && !PLAYER->IsGracePeriod()) {
			PLAYER->getFSM()->SetDeath(_viBullet->angle, 10);
			_viBullet->col->setCanCol(false);
			EFFECT->play("explosion", ZEFFECT2, PLAYER->getCollider()->getPos().x, PLAYER->getCollider()->getPos().y);
		}

		if (_viBullet->isOn) {


			//각바꾸기 & 출력위치바꾸기
			if(_viBullet->foward == FOWARD::LEFT) _viBullet->angle += _viBullet->speed * TIME->getGameTimeRate();
			else _viBullet->angle -= _viBullet->speed * TIME->getGameTimeRate();
			_viBullet->centerX = _viBullet->x + cosf(_viBullet->angle) * (_viBullet->img->getFrameWidth() / 2 + DISTANCE);
			_viBullet->centerY = _viBullet->y - sinf(_viBullet->angle) * (_viBullet->img->getFrameWidth() / 2 + DISTANCE);

			//콜라이더수정 - 각, 위치
			_viBullet->col->setAngle(_viBullet->angle);
			_viBullet->col->setPos(Vec2(_viBullet->centerX, _viBullet->centerY));
		}

		//애니끝나면 흰주흰주흰주
		if (_viBullet->firstTime && !_viBullet->ani->isPlay()) {
			int rnd = RND->getInt(3);
			if (rnd == 0) SOUND->play("sound_boss_huntresslaser_shot_1", 0.1f);
			else if (rnd == 1) SOUND->play("sound_boss_huntresslaser_shot_2", 0.1f);
			else SOUND->play("sound_boss_huntresslaser_shot_3", 0.1f);
			_viBullet->firstTime = false;
			_viBullet->secendTime = true;
			_viBullet->isOn = true;
			ANIMATION->changeNonKeyAnimation(_viBullet->ani, "sweeplaser", 3, 4, 8, false, true);
		}
	}


	//소멸확인(&콜라이더)
	for (_viBullet = _vBullet.begin(); _viBullet != _vBullet.end(); )
	{
		//사라지는애니
		if (_viBullet->foward == FOWARD::LEFT && _viBullet->angle > PI2 ||
			_viBullet->foward == FOWARD::RIGHT && _viBullet->angle < PI) {
			COLLISION->erase(_viBullet->col);
			_viBullet = _vBullet.erase(_viBullet);
		}
		else ++_viBullet;
	}
}

void sweeplaser::render()
{
	for (_viBullet = _vBullet.begin(); _viBullet != _vBullet.end(); ++_viBullet)
	{
		ZORDER->ZorderRotateFrameRender(_viBullet->img, _viBullet->z, _viBullet->bottom, _viBullet->centerX, _viBullet->centerY,
			_viBullet->angle, _viBullet->ani->getFrameX(), _viBullet->ani->getFrameY());
		ZORDER->SaveRotateFrameRender(_viBullet->img, _viBullet->bwimg, _viBullet->z, _viBullet->bottom, _viBullet->centerX, _viBullet->centerY,
			_viBullet->angle, _viBullet->ani->getFrameX(), _viBullet->ani->getFrameY());
	}
}
