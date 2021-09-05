#include "framework.h"
#include "playerSlash.h"

playerSlash::playerSlash()
{
}

playerSlash::~playerSlash()
{
}

void playerSlash::fire(float x, float y, float angle)
{
	tagBullet newBullet;
	newBullet.x = x;
	newBullet.y = y;
	newBullet.z = ZUNIT;
	newBullet.angle = angle;
	newBullet.speed = 0;
	newBullet.isOn = true;
	newBullet.count = 0;
	newBullet.alpha = 0;
	newBullet.col = COLLISION->addCollider(Vec2(x, y), Vec2(105, 80), COLLIDER_TYPE::BULLET_PLAYER, ZUNIT,
		true, angle);	
	_vBullet.push_back(newBullet);
}

void playerSlash::release()
{
	for (_viBullet = _vBullet.begin(); _viBullet != _vBullet.end(); )
	{
		COLLISION->erase(_viBullet->col);
		_viBullet = _vBullet.erase(_viBullet);
	}
}

void playerSlash::update()
{
	for (_viBullet = _vBullet.begin(); _viBullet != _vBullet.end(); )
	{
		float centerX = PLAYER->getCollider()->getPos().x + 52 * cosf(_viBullet->angle);
		float centerY = PLAYER->getCollider()->getPos().y - 52 * sinf(_viBullet->angle);
		_viBullet->col->setPos(Vec2(centerX, centerY));

		_viBullet->count += 1 * TIME->getGameTimeRate();
		if (_viBullet->count > 10) {
			COLLISION->erase(_viBullet->col);
			_viBullet=_vBullet.erase(_viBullet);
		}
		else {
			++_viBullet;
		}
	}
}

void playerSlash::render()
{
}
