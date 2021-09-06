#include "framework.h"
#include "laser.h"

laser::laser()
{
	_laserOn = true;
	IMAGE->addImage("레이저파편", "images/particle/yello1px.bmp", 2, 2);
	SOUND->addSound("laser_on", "sound/laser/lasers_on.wav", false, false);
	SOUND->addSound("laser_off", "sound/laser/lasers_off.wav", false, false);
	SOUND->addSound("laser_explosion", "sound/laser/sound_laser_explosion.wav", false, false);
	SOUND->addSound("laser_blast", "sound/laser/sound_laserblast.wav", false, false);
}

laser::~laser()
{
}

void laser::fire(float x, float y, float length)
{
	tagBullet newlaser;
	newlaser.img = IMAGE->addFrameImage("laser", "images/bullet/laser.bmp", 30, 491, 6, 1, true);
	newlaser.bwimg = IMAGE->addFrameImage("laser_bw", "images/bullet/laser_bw.bmp", 30, 491, 6, 1, true);
	newlaser.ani = ANIMATION->addNoneKeyAnimation("laser", 0, 3, 15, false, true);
	newlaser.x = x;
	newlaser.y = y;
	newlaser.z = ZUNIT;
	newlaser.bottom = y + length;
	newlaser.length = length;
	newlaser.angle = 0;
	newlaser.speed = 0;
	newlaser.isOn = true;
	newlaser.count = 0;
	newlaser.alpha = 0;
	newlaser.col = COLLISION->addCollider(Vec2(x+2, y+length/2), Vec2(2, length), COLLIDER_TYPE::LASER, ZUNIT);
	_vBullet.push_back(newlaser);
}

void laser::release()
{
	for (_viBullet = _vBullet.begin(); _viBullet != _vBullet.end(); )
	{
		COLLISION->erase(_viBullet->col);
		_viBullet = _vBullet.erase(_viBullet);
	}
}

void laser::update()
{
	for (_viBullet = _vBullet.begin(); _viBullet != _vBullet.end(); ++_viBullet)
	{
		//레이저 온오프설정
		if (!_laserOn) {
			_viBullet->col->setCanCol(false);
		}
		else {
			_viBullet->col->setCanCol(true);
		}


		//충돌시 애니변경 - 플레이어
		//if (_viBullet->col->isColEnter() && _viBullet->col->isThere(COLLIDER_TYPE::PLAYER_UNIT)) {
		if (_viBullet->col->isEnterThere(COLLIDER_TYPE::PLAYER_UNIT)) {
			ANIMATION->changeNonKeyAnimation(_viBullet->ani, "laser", 4, 5, 15, true, false);
			if (!(PLAYER->getState() == PLAYERSTATE::ROLL || PLAYER->getState() == PLAYERSTATE::FLIP)) {
				SOUND->play("laser_blast", 0.3f);
				PLAYER->setLaserDie(true);
			}
			else
				SOUND->play("laser_explosion", 0.3f);
			if(!CAMERA->getIsShake()) CAMERA->setShake(10, 5, 1);
		}
		//충돌시 애니변경 - 적
		//if (_viBullet->col->isColEnter() && _viBullet->col->isThere(COLLIDER_TYPE::ENEMY_UNIT)) {
		if (_viBullet->col->isEnterThere(COLLIDER_TYPE::ENEMY_UNIT)) {
			ANIMATION->changeNonKeyAnimation(_viBullet->ani, "laser", 4, 5, 15, true, false);
			SOUND->play("laser_blast", 0.3f);
			//적에서 레이저에닿앗는지 확인할것.
		}

		//비충돌시 원래애니로.
		if (!_viBullet->ani->isPlay()) {
			ANIMATION->changeNonKeyAnimation(_viBullet->ani, "laser", 0, 3, 15, false, true);
		}
	}
}

void laser::render()
{
	if (!_laserOn) return;
	for (_viBullet = _vBullet.begin(); _viBullet != _vBullet.end(); ++_viBullet)
	{
		ZORDER->ZorderFrameRender(_viBullet->img, _viBullet->z, _viBullet->bottom, _viBullet->x, _viBullet->y,
			_viBullet->ani->getFrameX(), _viBullet->ani->getFrameY(), 0, 0, _viBullet->img->getFrameWidth(), _viBullet->length);
		ZORDER->SaveFrameRender(_viBullet->img, _viBullet->bwimg, _viBullet->z, _viBullet->bottom, _viBullet->x, _viBullet->y,
			_viBullet->ani->getFrameX(), _viBullet->ani->getFrameY(), 0, 0, _viBullet->img->getFrameWidth(), _viBullet->length);
	}
}

void laser::laserOnOff()
{
	if (_laserOn) SOUND->play("laser_off", 0.3f);
	else SOUND->play("laser_on", 0.3f);
	_laserOn = !_laserOn;
}
