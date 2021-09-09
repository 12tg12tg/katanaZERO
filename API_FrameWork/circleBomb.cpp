#include "framework.h"
#include "circleBomb.h"

circleBomb::circleBomb()
{
	IMAGE->addFrameImage("circleBomb", "images/bullet/boss_mine.bmp", 40, 20, 2, 1, true);
	IMAGE->addFrameImage("mind_circle", "images/bullet/mind_circle.bmp", 548, 274, 2, 1, true);
	_explo = new explosion;
}

circleBomb::~circleBomb()
{
}

void circleBomb::fire(float x, float y, FOWARD foward)
{
	if(_explo == nullptr) {
		_explo = new explosion;
	}

	tagBullet newBul;
	newBul.img = IMAGE->findImage("circleBomb");
	newBul.bwimg = nullptr;

	int arr[] = { 1 };
	newBul.ani = ANIMATION->addNoneKeyAnimation("circleBomb", arr, sizeof(arr) / sizeof(int), 10, false);

	newBul.foward = foward;
	if (foward == FOWARD::LEFT) newBul.angle = RND->getFloatFromTo(PI * 3 / 4, PI * 5 / 4);
	else {
		newBul.angle = RND->getFloatFromTo(PI * 5 / 4, PI * 7 / 4);
		if (newBul.angle > PI2) newBul.angle - PI2;
	}
	newBul.x = x;
	newBul.y = y;
	newBul.col = COLLISION->addCollider(Vec2(x + newBul.img->getFrameWidth()/2, y + newBul.img->getFrameHeight() / 2), Vec2(newBul.img->getFrameWidth(), newBul.img->getFrameHeight()), COLLIDER_TYPE::BULLET_ENEMY, ZCOL3);
	newBul.z = ZUNIT;
	newBul.bottom = 0;
	newBul.speed = RND->getFloatFromTo(20.f, 30.f);	
	newBul.count = 0;
	newBul.alpha = 0;
	newBul.firstTime = true;
	newBul.secendTime = false;
	newBul.thirdTime = false;
	_vBullet.push_back(newBul);
}

void circleBomb::fire2(float x, float y)
{
	tagBullet newBul;
	newBul.img = IMAGE->findImage("circleBomb");
	newBul.bwimg = nullptr;

	int arr[] = { 1 };
	newBul.ani = ANIMATION->addNoneKeyAnimation("circleBomb", arr, sizeof(arr) / sizeof(int), 10, false);

	newBul.angle = UTIL::getAngle(PLAYER->getCollider()->getPos().x, PLAYER->getCollider()->getPos().y,
		x+newBul.img->getFrameWidth()/2, y+newBul.img->getFrameHeight()/2);

	newBul.x = x;
	newBul.y = y;
	newBul.col = COLLISION->addCollider(Vec2(x + newBul.img->getFrameWidth() / 2, y + newBul.img->getFrameHeight() / 2), Vec2(newBul.img->getFrameWidth(), newBul.img->getFrameHeight()), COLLIDER_TYPE::BULLET_PLAYER, ZCOL3);
	newBul.z = ZUNIT;
	newBul.bottom = 0;
	newBul.speed = 20.f;
	newBul.count = 0;
	newBul.alpha = 0;
	newBul.firstTime = true;
	newBul.secendTime = false;
	newBul.thirdTime = false;
	_reflect.push_back(newBul);
}

void circleBomb::release()
{
	for (_viBullet = _vBullet.begin(); _viBullet != _vBullet.end(); )
	{
		COLLISION->erase(_viBullet->col);
		_viBullet = _vBullet.erase(_viBullet);
	}
	_explo->release();
	SAFE_DELETE(_explo);
}

void circleBomb::update()
{
	updateForEnemy();
	updateForPlayer();
	_explo->update();
}

void circleBomb::render()
{
	renderForEnemy();
	renderForPlayer();
	_explo->render();
}

void circleBomb::updateForEnemy()
{
	for (_viBullet = _vBullet.begin(); _viBullet != _vBullet.end(); ++_viBullet)
	{
		if (_viBullet->firstTime) {
			//벽충돌 각변경
			if (_viBullet->x < 169 || _viBullet->x > 1210) {
				_viBullet->angle = PI - _viBullet->angle;

			}
			if (_viBullet->y < 231 || _viBullet->y >620) {
				_viBullet->angle = PI2 - _viBullet->angle;

			}

			//이동 및 스피드감소
			_viBullet->x += cosf(_viBullet->angle) * _viBullet->speed * TIME->getGameTimeRate();
			_viBullet->y -= sinf(_viBullet->angle) * _viBullet->speed * TIME->getGameTimeRate();

			_viBullet->speed -= 0.3 * TIME->getGameTimeRate();
			if (_viBullet->speed < 0) _viBullet->speed = 0.f;

			//콜라이더재조정
			_viBullet->col->setPos(Vec2(_viBullet->x + _viBullet->img->getFrameWidth() / 2, _viBullet->y + _viBullet->img->getFrameHeight() / 2));

			//스피드 0이면
			if (_viBullet->speed == 0) {
				_viBullet->firstTime = false;
				_viBullet->secendTime = true;
				ANIMATION->changeNonKeyAnimation(_viBullet->ani, "circleBomb", 0, 1, 2, false, true);
			}
		}
		//카운트시작
		else if (_viBullet->secendTime) {
			_viBullet->count += 1 * TIME->getGameTimeRate();
			if (_viBullet->count > 100) {
				_viBullet->count = 0;
				_viBullet->secendTime = false;
				_viBullet->thirdTime = true;

				ANIMATION->changeNonKeyAnimation(_viBullet->ani, "circleBomb", 0, 1, 10, false, true);
			}
		}
		else  if (_viBullet->thirdTime) {
			_viBullet->count += 1 * TIME->getGameTimeRate();

		}
	}

	//소멸
	for (_viBullet = _vBullet.begin(); _viBullet != _vBullet.end(); )
	{
		//일정시간후 소멸
		if (_viBullet->thirdTime && _viBullet->count > 80) {
			//폭발발사
			int num = RND->getFromTo(8, 10);
			for (size_t i = 0; i < num; i++) {
				_explo->fire(_viBullet->col->getPos().x, _viBullet->col->getPos().y, COLLIDER_TYPE::BULLET_ENEMY);
			}
			//소멸
			COLLISION->erase(_viBullet->col);
			_viBullet = _vBullet.erase(_viBullet);
			continue;
		}
		//플레이어검기와 소멸 후 소멸
		else if (_viBullet->firstTime && _viBullet->col->isEnterThere(COLLIDER_TYPE::BULLET_PLAYER)) {
			COLLISION->erase(_viBullet->col);
			this->fire2(_viBullet->x, _viBullet->y);
			_viBullet = _vBullet.erase(_viBullet);
		}
		else ++_viBullet;
	}

}

void circleBomb::updateForPlayer()
{
	for (_ireflect = _reflect.begin(); _ireflect != _reflect.end(); ++_ireflect)
	{
		if (_ireflect->firstTime) {
			//벽충돌 각변경
			if (_ireflect->x < 169 || _ireflect->x > 1210) {
				_ireflect->angle = PI - _ireflect->angle;

			}
			if (_ireflect->y < 231 || _ireflect->y >620) {
				_ireflect->angle = PI2 - _ireflect->angle;

			}

			//이동 및 스피드감소
			_ireflect->x += cosf(_ireflect->angle) * _ireflect->speed * TIME->getGameTimeRate();
			_ireflect->y -= sinf(_ireflect->angle) * _ireflect->speed * TIME->getGameTimeRate();

			_ireflect->speed -= 0.3 * TIME->getGameTimeRate();
			if (_ireflect->speed < 0) _ireflect->speed = 0.f;

			//콜라이더재조정
			_ireflect->col->setPos(Vec2(_ireflect->x + _ireflect->img->getFrameWidth() / 2, _ireflect->y + _ireflect->img->getFrameHeight() / 2));

			//시간재기
			_ireflect->count += 1 * TIME->getGameTimeRate();
		}
	}

	//소멸
	for (_ireflect = _reflect.begin(); _ireflect != _reflect.end(); )
	{
		//시간지나면 소멸
		if (_ireflect->count > 60) {
			//폭발발사
			int num = RND->getFromTo(8, 10);
			for (size_t i = 0; i < num; i++) {
				_explo->fire(_ireflect->col->getPos().x, _ireflect->col->getPos().y, COLLIDER_TYPE::BULLET_PLAYER);
			}
			//소멸
			COLLISION->erase(_ireflect->col);
			_ireflect = _reflect.erase(_ireflect);
			continue;
		}
		//보스와 충돌후 소멸
		else if (_ireflect->col->isEnterThere(COLLIDER_TYPE::ENEMY_UNIT)) {
			//폭발발사
			int num = RND->getFromTo(8, 10);
			for (size_t i = 0; i < num; i++) {
				_explo->fire(_ireflect->col->getPos().x, _ireflect->col->getPos().y, COLLIDER_TYPE::BULLET_PLAYER);
			}
			//소멸
			COLLISION->erase(_ireflect->col);
			_ireflect = _reflect.erase(_ireflect);
			continue;
		}
		else ++_ireflect;
	}

}

void circleBomb::renderForEnemy()
{
	for (_viBullet = _vBullet.begin(); _viBullet != _vBullet.end(); ++_viBullet)
	{
		ZORDER->ZorderAniRender(_viBullet->img, _viBullet->z, _viBullet->bottom, _viBullet->x, _viBullet->y, _viBullet->ani);
		ZORDER->SaveAniRender(_viBullet->img, _viBullet->bwimg, _viBullet->z, _viBullet->bottom, _viBullet->x, _viBullet->y, _viBullet->ani);

		if (!_viBullet->firstTime) {
			int circleframeX;
			circleframeX = (_viBullet->thirdTime) ? 1 : 0;
			ZORDER->ZorderFrameRender(IMAGE->findImage("mind_circle"), _viBullet->z, _viBullet->bottom,
				_viBullet->x + _viBullet->img->getFrameWidth() / 2 - IMAGE->findImage("mind_circle")->getFrameWidth() / 2,
				_viBullet->y + _viBullet->img->getFrameHeight() / 2 - IMAGE->findImage("mind_circle")->getFrameHeight() / 2,
				circleframeX, 0);
		}
	}
}

void circleBomb::renderForPlayer()
{
	for (_ireflect = _reflect.begin(); _ireflect != _reflect.end(); ++_ireflect)
	{
		ZORDER->ZorderAniRender(_ireflect->img, _ireflect->z, _ireflect->bottom, _ireflect->x, _ireflect->y, _ireflect->ani);
		ZORDER->SaveAniRender(_ireflect->img, _ireflect->bwimg, _ireflect->z, _ireflect->bottom, _ireflect->x, _ireflect->y, _ireflect->ani);

		if (!_ireflect->firstTime) {
			int circleframeX;
			circleframeX = (_ireflect->thirdTime) ? 1 : 0;
			ZORDER->ZorderFrameRender(IMAGE->findImage("mind_circle"), _ireflect->z, _ireflect->bottom,
				_ireflect->x + _ireflect->img->getFrameWidth() / 2 - IMAGE->findImage("mind_circle")->getFrameWidth() / 2,
				_ireflect->y + _ireflect->img->getFrameHeight() / 2 - IMAGE->findImage("mind_circle")->getFrameHeight() / 2,
				circleframeX, 0);
		}
	}
}
