#include "framework.h"
#include "explosion.h"
#include "effect.h"
#include "playerstate.h"
explosion::explosion()
{
}

explosion::~explosion()
{
}

void explosion::fire(float centerX, float centerY, COLLIDER_TYPE type)
{
	tagBullet newBul;
	newBul.z = ZUNIT;
	newBul.img = IMAGE->findImage("explosion");
	float rndAngle = RND->getFloat(PI2);
	float rndDis = RND->getFloat(81);
	int rndNumX = centerX + cosf(rndAngle) * rndDis;
	int rndNumY = centerY - sinf(rndAngle) * rndDis;
	newBul.effect = EFFECT->play("explosion", ZEFFECT2, rndNumX, rndNumY);
	newBul.count = 0;
	newBul.col = COLLISION->addCollider(Vec2(rndNumX, rndNumY), Vec2(newBul.img->getFrameWidth(), newBul.img->getFrameHeight()), type, ZCOL3);
	_vBullet.push_back(newBul);
}

void explosion::release()
{
	for (_viBullet = _vBullet.begin(); _viBullet != _vBullet.end(); )
	{
		COLLISION->erase(_viBullet->col);
		_viBullet = _vBullet.erase(_viBullet);
	}
}

void explosion::update()
{
	//Ãæµ¹Ã¼ ¼Ò¸ê
	for (_viBullet = _vBullet.begin(); _viBullet != _vBullet.end(); )
	{
		_viBullet->count += 1 * TIME->getGameTimeRate();
		if (_viBullet->count > 6) {
			COLLISION->erase(_viBullet->col);
			_viBullet = _vBullet.erase(_viBullet);
			continue;
		}
		else if (!PLAYER->IsGracePeriod() && _viBullet->col->getType() == COLLIDER_TYPE::BULLET_ENEMY && _viBullet->col->isEnterThere(COLLIDER_TYPE::PLAYER_UNIT)) {
			float angle = _viBullet->col->getPos().AngleTo(PLAYER->getCollider()->getPos());
			PLAYER->setIsGravePeriod(true);
			PLAYER->getFSM()->SetDeath(angle, 15);
			COLLISION->erase(_viBullet->col);
			_viBullet = _vBullet.erase(_viBullet);
			continue;
		}
		else ++_viBullet;
	}
}

void explosion::render()
{
}
