#include "framework.h"
#include "gruntPunch.h"
#include "playerstate.h"
gruntPunch::gruntPunch()
{
	IMAGE->addFrameImage("grunt_slash_left_bw", "images/effect/grunt_slash_left_bw.bmp", 256 * 2, 64 * 2, 4, 1, true);
	IMAGE->addFrameImage("grunt_slash_right_bw", "images/effect/grunt_slash_right_bw.bmp", 256 * 2, 64 * 2, 4, 1, true);
	EFFECT->addEffect("grunt_slash_left", "images/effect/grunt_slash_left.bmp", 256 * 2, 64 * 2, 64 * 2, 64 * 2, 20, 0.01666, 5, false, false, true, IMAGE->findImage("grunt_slash_left_bw"));
	EFFECT->addEffect("grunt_slash_right", "images/effect/grunt_slash_right.bmp", 256 * 2, 64 * 2, 64 * 2, 64 * 2, 20, 0.01666, 5, false, false, true, IMAGE->findImage("grunt_slash_right_bw"));
}

gruntPunch::~gruntPunch()
{
}

void gruntPunch::fire(float x, float y, FOWARD foward)
{
	tagBullet newBullet;
	newBullet.x = x;
	newBullet.y = y;
	newBullet.z = ZUNIT;
	newBullet.foward = foward;
	newBullet.speed = 0;
	newBullet.isOn = true;
	newBullet.count = 0;
	newBullet.alpha = 0;
	newBullet.firstTime = true;
	newBullet.col = COLLISION->addCollider(Vec2(x,
		y),
		Vec2(IMAGE->findImage("grunt_slash_left")->getFrameWidth(),
			IMAGE->findImage("grunt_slash_left")->getFrameHeight()*3/5),
		COLLIDER_TYPE::BULLET_ENEMY, ZUNIT);
	_vBullet.push_back(newBullet);
}

void gruntPunch::release()
{
	for (_viBullet = _vBullet.begin(); _viBullet != _vBullet.end(); )
	{
		if (_viBullet->col == nullptr) return;
		COLLISION->erase(_viBullet->col);
		_viBullet = _vBullet.erase(_viBullet);
	}
}

void gruntPunch::update()
{
	for (_viBullet = _vBullet.begin(); _viBullet != _vBullet.end(); )
	{
		//카운트증가
		_viBullet->count += 1 * TIME->getGameTimeRate();
		//이펙트
		if (_viBullet->firstTime && _viBullet->count > 20) {
			if (_viBullet->foward == FOWARD::LEFT) EFFECT->play("grunt_slash_left", ZUNIT, _viBullet->x, _viBullet->y-15);
			else EFFECT->play("grunt_slash_right", ZUNIT, _viBullet->x, _viBullet->y-15);
			_viBullet->firstTime = false;
		}
		//충돌판정
		if (_viBullet->count > 20) {
			if (_viBullet->col->isThere(COLLIDER_TYPE::PLAYER_UNIT) &&
				PLAYER->getState()!=PLAYERSTATE::ROLL) {
				float angle = _viBullet->col->getPos().AngleTo(PLAYER->getCollider()->getPos());
				PLAYER->getFSM()->SetDeath(angle, 10);
				PLAYER->getCollider()->setCanCol(false);
			}
		}
		if (_viBullet->count > 30) {
			COLLISION->erase(_viBullet->col);
			_viBullet = _vBullet.erase(_viBullet);
		}
		else {
			++_viBullet;
		}
	}
}

void gruntPunch::render()
{
}
