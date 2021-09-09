#include "framework.h"
#include "dashAtk.h"
#include "boss.h"
#include "playerstate.h"
dashAtk::dashAtk()
{
}

dashAtk::~dashAtk()
{
}

void dashAtk::fire(boss* boss)
{
	_boss = boss;

	tagBullet newBul;
	newBul.img = nullptr;
	newBul.bwimg = nullptr;
	newBul.foward = FOWARD::NONE;
	newBul.length = boss->_col->getSize().y;
	newBul.z = ZUNIT;
	newBul.bottom = 0;
	newBul.count = 0;
	newBul.firstTime = true;
	newBul.col = COLLISION->addCollider(boss->_col->getPos(),
		boss->_col->getSize(), COLLIDER_TYPE::BULLET_ENEMY, ZCOL3);
	_vBullet.push_back(newBul);
}

void dashAtk::release()
{
	for (_viBullet = _vBullet.begin(); _viBullet != _vBullet.end(); )
	{
		COLLISION->erase(_viBullet->col);
		_viBullet = _vBullet.erase(_viBullet);
	}
}

void dashAtk::update()
{
	for (_viBullet = _vBullet.begin(); _viBullet != _vBullet.end(); )
	{
		//카운트
		_viBullet->count++;

		//최대길이까지 길이증가
		if(_viBullet->length < MAXLEN){
			_viBullet->length += 10 * TIME->getGameTimeRate();
		}
		//위치다시잡기
		if (_boss->_foward == FOWARD::LEFT) {
			_viBullet->col->setSize(Vec2(_viBullet->length, _viBullet->col->getSize().y));
			_viBullet->col->setPos(Vec2(_boss->_col->getPos().x + _viBullet->length / 2, _boss->_col->getPos().y));
		}
		else {
			_viBullet->col->setSize(Vec2(_viBullet->length, _viBullet->col->getSize().y));
			_viBullet->col->setPos(Vec2(_boss->_col->getPos().x - _viBullet->length / 2, _boss->_col->getPos().y));
		}

		//플레이어와 충돌시 충돌OFF
		if (_viBullet->col->isEnterThere(COLLIDER_TYPE::PLAYER_UNIT) && !PLAYER->IsGracePeriod()) {
			float angle = (_boss->_foward == FOWARD::LEFT) ? PI : 0;
			PLAYER->getFSM()->SetDeath(angle, 10);
			_viBullet->col->setCanCol(false);
		}

		//소멸
		if (!_boss->_isdashAtk) {
			COLLISION->erase(_viBullet->col);
			_viBullet = _vBullet.erase(_viBullet);
		}
		else ++_viBullet;
	}
}

void dashAtk::render()
{
	//for (_viBullet = _vBullet.begin(); _viBullet != _vBullet.end(); ++_viBullet)
	//{
	//	출력
	//	ZORDER->ZorderRender(_viBullet->img, _viBullet->z, _viBullet->y + 2, _viBullet->x, _viBullet->y);
	//	ZORDER->SaveRender(_viBullet->img, _viBullet->bwimg, _viBullet->z, _viBullet->y + 2, _viBullet->x, _viBullet->y);
	//}
}
