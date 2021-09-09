#include "framework.h"
#include "bossNormalBullet.h"
#include "playerstate.h"
bossNormalBullet::bossNormalBullet()
{
	IMAGE->addImage("normalbullet", "images/bullet/boss_normal.bmp", 2, 2);
}

bossNormalBullet::~bossNormalBullet()
{
}

void bossNormalBullet::fire(float x, float y, float angle)
{
	tagBullet newBul;
	newBul.img = IMAGE->findImage("normalbullet");
	newBul.bwimg = nullptr;
	newBul.foward = FOWARD::NONE;
	newBul.angle = angle;
	newBul.col = COLLISION->addCollider(Vec2(x + 1, y + 1), Vec2(2, 2), COLLIDER_TYPE::BULLET_ENEMY, ZCOL3);
	newBul.x = x;
	newBul.y = y;
	newBul.z = ZUNIT;
	newBul.bottom = 0;
	newBul.speed = 10.f;
	newBul.count = 0;
	newBul.alpha = 0;
	newBul.firstTime = true;
	_vBullet.push_back(newBul);
}

void bossNormalBullet::fire2(float x, float y, float angle)
{
	tagBullet newBul;
	newBul.img = IMAGE->findImage("normalbullet");
	newBul.bwimg = nullptr;
	newBul.foward = FOWARD::NONE;
	newBul.angle = angle;
	newBul.col = COLLISION->addCollider(Vec2(x + 1, y + 1), Vec2(2, 2), COLLIDER_TYPE::BULLET_PLAYER, ZCOL3);
	newBul.x = x;
	newBul.y = y;
	newBul.z = ZUNIT;
	newBul.bottom = 0;
	newBul.speed = 15.f;
	newBul.count = 0;
	newBul.alpha = 0;
	newBul.firstTime = true;
	_vReflect.push_back(newBul);
}

void bossNormalBullet::release()
{
	for (_viBullet = _vBullet.begin(); _viBullet != _vBullet.end(); )
	{
		COLLISION->erase(_viBullet->col);
		_viBullet = _vBullet.erase(_viBullet);
	}

	for (_viReflect = _vReflect.begin(); _viReflect != _vReflect.end(); )
	{
		COLLISION->erase(_viReflect->col);
		_viReflect = _vBullet.erase(_viReflect);
	}
}

void bossNormalBullet::update()
{
	updateforEnemyBullet();
	updateforPlayerBullet();
}

void bossNormalBullet::render()
{
	renderforEnemyBullet();
	renderforPlayerBullet();
}

void bossNormalBullet::updateforEnemyBullet()
{
	//삭제되지않는업뎃
	for (_viBullet = _vBullet.begin(); _viBullet != _vBullet.end(); ++_viBullet)
	{
		//변화량계산
		float xchange = cosf(_viBullet->angle) * _viBullet->speed * TIME->getGameTimeRate();
		float ychange = sinf(_viBullet->angle) * _viBullet->speed * TIME->getGameTimeRate();

		//잔상생성
		bulletSpec newspec;
		newspec.x = _viBullet->x + xchange/4;
		newspec.y = _viBullet->y - ychange/3;
		newspec.alpha = 200;
		_viBullet->specList.push_back(newspec);
		newspec.x = _viBullet->x + xchange*2/4;
		newspec.y = _viBullet->y - ychange*2/4;
		newspec.alpha = 200;
		_viBullet->specList.push_back(newspec);
		newspec.x = _viBullet->x + xchange*3/4;
		newspec.y = _viBullet->y - ychange*3/4;
		newspec.alpha = 200;
		_viBullet->specList.push_back(newspec);
		newspec.x = _viBullet->x + xchange;
		newspec.y = _viBullet->y - ychange;
		newspec.alpha = 200;
		_viBullet->specList.push_back(newspec);

		//이동
		_viBullet->x += xchange;
		_viBullet->y -= ychange;

		//콜라이더수정
		_viBullet->col->setPos(Vec2(_viBullet->x + 1, _viBullet->y + 1));
	}

	//삭제하는업뎃
	for (_viBullet = _vBullet.begin(); _viBullet != _vBullet.end(); )
	{
		if (_viBullet->col->getType() == COLLIDER_TYPE::BULLET_ENEMY) {
			//반사확인
			if (_viBullet->col->isEnterThere(COLLIDER_TYPE::BULLET_PLAYER)) {
				COLLISION->erase(_viBullet->col);
				Vec2 clickPt = Vec2(CAMERA->getRelativeMouse().x, CAMERA->getRelativeMouse().y);
				float angle = PLAYER->getCollider()->getPos().AngleTo(clickPt);
				this->fire2(_viBullet->x, _viBullet->y, angle);
				_viBullet = _vBullet.erase(_viBullet);
			}
			else ++_viBullet;
		}
	}

	for (_viBullet = _vBullet.begin(); _viBullet != _vBullet.end();)
	{
		if (_viBullet->col->getType() == COLLIDER_TYPE::BULLET_ENEMY) {
			//플레이어와 충돌
			if (_viBullet->col->isEnterThere(COLLIDER_TYPE::PLAYER_UNIT) && !PLAYER->IsGracePeriod()) {
				PLAYER->getFSM()->SetDeath(_viBullet->angle, 10);
				COLLISION->erase(_viBullet->col);
				_viBullet = _vBullet.erase(_viBullet);
			}
			else ++_viBullet;
		}
	}

	//소멸확인(&콜라이더)
	for (_viBullet = _vBullet.begin(); _viBullet != _vBullet.end(); )
	{
		//y가 617 넘어가거나, x가 172 1196 넘기면 삭제
		if (_viBullet->x < 172 || _viBullet->x >1194 || _viBullet->y > 615 || _viBullet->y < 0) {
			COLLISION->erase(_viBullet->col);
			_viBullet = _vBullet.erase(_viBullet);
		}
		else ++_viBullet;
	}
}

void bossNormalBullet::updateforPlayerBullet()
{
	//삭제되지않는업뎃
	for (_viReflect = _vReflect.begin(); _viReflect != _vReflect.end(); ++_viReflect)
	{
		//변화량계산
		float xchange = cosf(_viReflect->angle) * _viReflect->speed * TIME->getGameTimeRate();
		float ychange = sinf(_viReflect->angle) * _viReflect->speed * TIME->getGameTimeRate();

		//잔상생성
		bulletSpec newspec;
		newspec.x = _viReflect->x + xchange / 4;
		newspec.y = _viReflect->y - ychange / 3;
		newspec.alpha = 200;
		_viReflect->specList.push_back(newspec);
		newspec.x = _viReflect->x + xchange * 2 / 4;
		newspec.y = _viReflect->y - ychange * 2 / 4;
		newspec.alpha = 200;
		_viReflect->specList.push_back(newspec);
		newspec.x = _viReflect->x + xchange * 3 / 4;
		newspec.y = _viReflect->y - ychange * 3 / 4;
		newspec.alpha = 200;
		_viReflect->specList.push_back(newspec);
		newspec.x = _viReflect->x + xchange;
		newspec.y = _viReflect->y - ychange;
		newspec.alpha = 200;
		_viReflect->specList.push_back(newspec);

		//이동
		_viReflect->x += xchange;
		_viReflect->y -= ychange;

		//콜라이더수정
		_viReflect->col->setPos(Vec2(_viReflect->x + 1, _viReflect->y + 1));
	}

	////삭제하는업뎃
	//for (_viReflect = _vReflect.begin(); _viReflect != _vReflect.end(); )
	//{
	//	//반사확인
	//	if (_viReflect->col->isThere(COLLIDER_TYPE::ENEMY_UNIT)) {
	//		COLLISION->erase(_viReflect->col);
	//		_viReflect = _vReflect.erase(_viReflect);
	//	}
	//	else ++_viReflect;
	//}

	//소멸확인(&콜라이더)
	for (_viReflect = _vReflect.begin(); _viReflect != _vReflect.end(); )
	{
		//y가 617 넘어가거나, x가 172 1196 넘기면 삭제
		if (_viReflect->x < 172 || _viReflect->x >1194 || _viReflect->y > 615 || _viReflect->y < 0) {
			COLLISION->erase(_viReflect->col);
			_viReflect = _vReflect.erase(_viReflect);
		}
		else ++_viReflect;
	}
}

void bossNormalBullet::renderforEnemyBullet()
{
	for (_viBullet = _vBullet.begin(); _viBullet != _vBullet.end(); ++_viBullet)
	{
		//출력
		ZORDER->ZorderRender(_viBullet->img, _viBullet->z, _viBullet->y + 2, _viBullet->x, _viBullet->y);
		ZORDER->SaveRender(_viBullet->img, _viBullet->bwimg, _viBullet->z, _viBullet->y + 2, _viBullet->x, _viBullet->y);

		//잔상출력
		auto& iter = _viBullet->specList.begin();
		for (iter; iter != _viBullet->specList.end(); )
		{
			ZORDER->ZorderAlphaRender(_viBullet->img, _viBullet->z, _viBullet->y + 2, iter->x, iter->y, iter->alpha);
			ZORDER->SaveAlphaRender(_viBullet->img, _viBullet->bwimg, _viBullet->z, _viBullet->y + 2, iter->x, iter->y, iter->alpha);
			iter->alpha -= 20;
			if (iter->alpha < 100) {
				iter = _viBullet->specList.erase(iter);
			}
			else ++iter;
		}
	}
}

void bossNormalBullet::renderforPlayerBullet()
{
	for (_viReflect = _vReflect.begin(); _viReflect != _vReflect.end(); ++_viReflect)
	{
		//출력
		ZORDER->ZorderRender(_viReflect->img, _viReflect->z, _viReflect->y + 2, _viReflect->x, _viReflect->y);
		ZORDER->SaveRender(_viReflect->img, _viReflect->bwimg, _viReflect->z, _viReflect->y + 2, _viReflect->x, _viReflect->y);

		//잔상출력
		auto& iter = _viReflect->specList.begin();
		for (iter; iter != _viReflect->specList.end(); )
		{
			ZORDER->ZorderAlphaRender(_viReflect->img, _viReflect->z, _viReflect->y + 2, iter->x, iter->y, iter->alpha);
			ZORDER->SaveAlphaRender(_viReflect->img, _viReflect->bwimg, _viReflect->z, _viReflect->y + 2, iter->x, iter->y, iter->alpha);
			iter->alpha -= 20;
			if (iter->alpha < 100) {
				iter = _viReflect->specList.erase(iter);
			}
			else ++iter;
		}
	}
}
