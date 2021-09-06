#include "framework.h"
#include "grunt.h"
#include "effect.h"
grunt::grunt(bool isLaser, bool isStair, Vec2 checkPoint)
	:_isLaser(isLaser), _isStair(isStair), _checkPoint(checkPoint)
{
	IMAGE->addImage("followMark", "images/enemy/enemy_follow.bmp", 16, 20, true);
	IMAGE->addFrameImage("grunt_all", "images/enemy/grunt_all.bmp", 896 * 2, 504 * 2, 16, 12, true);
	IMAGE->addFrameImage("grunt_all_bw", "images/enemy/grunt_all_bw.bmp", 896 * 2, 504 * 2, 16, 12, true);
	IMAGE->addFrameImage("grunt_all_laserdie", "images/enemy/grunt_all_laserdie.bmp", 896 * 2, 504 * 2, 16, 12, true);
	IMAGE->addFrameImage("blood", "images/effect/blood.bmp", 800, 158, 10, 2, true);
	IMAGE->addFrameImage("bloodremain", "images/effect/bloodRemain.bmp", 432, 288, 3, 2, true);
}

grunt::~grunt()
{
}

void grunt::addEnemy(float x, float y, FOWARD foward, Vec2 patrolpoint)
{
	tagEnemy newEnemy;
	newEnemy.x = x;
	newEnemy.y = y;
	newEnemy.z = ZUNIT;
	newEnemy.speed = 3.f;
	newEnemy.img = IMAGE->findImage("grunt_all");
	newEnemy.bwimg = IMAGE->findImage("grunt_all_bw");
	newEnemy.foward = foward;
	newEnemy.ani = ANIMATION->addNoneKeyAnimation("grunt_all", 0, 7, 10, false, true);
	newEnemy.bloodAni = ANIMATION->addNoneKeyAnimation("blood", 0, 9, 10, false, false);
	newEnemy.bloodAni->stop();
	newEnemy.col = COLLISION->addCollider(
		Vec2(newEnemy.x + newEnemy.img->getFrameWidth() / 2, newEnemy.y + newEnemy.img->getFrameHeight() * 3 / 5),
		Vec2(newEnemy.img->getFrameWidth() / 3, newEnemy.img->getFrameHeight() * 4 / 5),
		COLLIDER_TYPE::ENEMY_UNIT, ZCOL3);
	newEnemy.searchCol = COLLISION->addCollider(
		Vec2(newEnemy.col->getPos().x, newEnemy.col->getBottom() - 8),
		Vec2(WINSIZEX, 1), COLLIDER_TYPE::SEARCH, ZCOL1);
	newEnemy.bottom = 0;
	newEnemy.isDeath = false;
	newEnemy.state = ENEMYSTATE::IDLE;
	newEnemy.count = 0;
	newEnemy.patterncount = 0;
	newEnemy.haveToChangeAni = true;
	newEnemy.findPlayer = false;
	newEnemy.deatheffect = nullptr;
	newEnemy.ignoreBlack = false;
	newEnemy.deathSpeed = 0;
	newEnemy.isLand = false;
	newEnemy.bloodOn = false;
	newEnemy.patrolpointX = patrolpoint;
	_vEnemy.push_back(newEnemy);
}

void grunt::release()
{
	for (_viEnemy = _vEnemy.begin(); _viEnemy != _vEnemy.end(); )
	{
		COLLISION->erase(_viEnemy->col);
		//COLLISION->erase(_viEnemy->searchCol);
		_viEnemy = _vEnemy.erase(_viEnemy);
	}
}

void grunt::update()
{
	for (_viEnemy = _vEnemy.begin(); _viEnemy != _vEnemy.end(); ++_viEnemy)
	{
		setCollider();
		giveFrame();
		deathCheck();
		findPlayer();
		deathMove();
		getgravity();
		move();
	}
}

void grunt::render()
{
	for (_viEnemy = _vEnemy.begin(); _viEnemy != _vEnemy.end(); ++_viEnemy)
	{
		ZORDER->ZorderAniRender(_viEnemy->img, _viEnemy->z, _viEnemy->bottom, _viEnemy->x, _viEnemy->y, _viEnemy->ani);
		ZORDER->SaveAniRender(_viEnemy->img, _viEnemy->bwimg, _viEnemy->z, _viEnemy->bottom, _viEnemy->x, _viEnemy->y, _viEnemy->ani);
		if (_viEnemy->findPlayer && !_viEnemy->isDeath) {
			ZORDER->ZorderRender(IMAGE->findImage("followMark"), _viEnemy->z,
				_viEnemy->col->getRect().top, _viEnemy->col->getPos().x,
				_viEnemy->col->getRect().top - 30);
		}
		if (_viEnemy->isDeath) {
			if (_viEnemy->bloodAni->isPlay()) {
				if(_viEnemy->foward == FOWARD::LEFT) ZORDER->ZorderAniRender(IMAGE->findImage("blood"), ZEFFECT2, 0, _viEnemy->col->getPos().x + 10, _viEnemy->col->getPos().y - IMAGE->findImage("blood")->getFrameHeight(), _viEnemy->bloodAni);
				else ZORDER->ZorderAniRender(IMAGE->findImage("blood"), ZEFFECT2, 0, _viEnemy->col->getPos().x - IMAGE->findImage("blood")->getFrameWidth(), _viEnemy->col->getPos().y - IMAGE->findImage("blood")->getFrameHeight(), _viEnemy->bloodAni);
			}
			if ((_viEnemy->bloodAni->getFrameX() == 4 && _viEnemy->bloodAni->getFrameY() == 0) ||
				(_viEnemy->bloodAni->getFrameX() == 5 && _viEnemy->bloodAni->getFrameY() == 1)) {
				_viEnemy->bloodOn = true;
				_viEnemy->bloodRemainIndex = RND->getInt(3);
			}
		}
		if (_viEnemy->bloodOn) {
			if (_viEnemy->foward == FOWARD::RIGHT)
				ZORDER->ZorderFrameRender(IMAGE->findImage("bloodremain"), ZEFFECT1, 0, _viEnemy->col->getPos().x - IMAGE->findImage("blood")->getFrameWidth() - 36, _viEnemy->col->getPos().y - IMAGE->findImage("bloodremain")->getFrameHeight(), _viEnemy->bloodRemainIndex, 0);
			if (_viEnemy->foward == FOWARD::LEFT)
				ZORDER->ZorderFrameRender(IMAGE->findImage("bloodremain"), ZEFFECT1, 0, _viEnemy->col->getPos().x + 10, _viEnemy->col->getPos().y - IMAGE->findImage("bloodremain")->getFrameHeight(), _viEnemy->bloodRemainIndex, 1);
		}
	}
}

void grunt::move()
{
	++_viEnemy->patterncount;
	switch (_viEnemy->state)
	{
	case ENEMYSTATE::IDLE:
		if (_viEnemy->patterncount > 100) {
			_viEnemy->state = ENEMYSTATE::FIND;
			_viEnemy->patterncount = 0;
		}
		break;
	case ENEMYSTATE::FIND:
		if (!_viEnemy->findPlayer) {
			//�����̹߽߰�
			//0. ������ΰ� 0, 0�̶�� �׳� IDLE ����.
			//1. �����糡�� ��򰡿� �������ִٸ� �ݴ����� ��ǥ�� ����.
			//1+. �ƴ϶�� ���� ª������ ����.
			//2. �������� ���⼳�� �� WALK����.
			//3. ���������� �ٽ� ���̵��.
			if (_viEnemy->patrolpointX == Vec2(0, 0)) {
				_viEnemy->state = ENEMYSTATE::IDLE;
			}
			else if(_viEnemy->x == _viEnemy->patrolpointX.x)	{
				_viEnemy->destinationX = _viEnemy->patrolpointX.y;
				_viEnemy->foward = FOWARD::RIGHT;
				_viEnemy->state = ENEMYSTATE::WALK;
				_viEnemy->haveToChangeAni = true;
			}
			else if (_viEnemy->x == _viEnemy->patrolpointX.y) {
				_viEnemy->destinationX = _viEnemy->patrolpointX.x;
				_viEnemy->foward = FOWARD::LEFT;
				_viEnemy->state = ENEMYSTATE::WALK;
				_viEnemy->haveToChangeAni = true;
			}
			else {
				if ((abs(_viEnemy->x - _viEnemy->patrolpointX.x) >
					abs(_viEnemy->x - _viEnemy->patrolpointX.y)))
				{
					_viEnemy->destinationX = _viEnemy->patrolpointX.y;
					_viEnemy->foward = FOWARD::RIGHT;
				}
				else {
					_viEnemy->destinationX = _viEnemy->patrolpointX.x;
					_viEnemy->foward = FOWARD::LEFT;
				}						
				_viEnemy->state = ENEMYSTATE::WALK;
				_viEnemy->haveToChangeAni = true;
			}
		}
		else {
			//�߽߰� ���� �̱���.





		}
		break;
	case ENEMYSTATE::WALK:
		if (_viEnemy->foward == FOWARD::LEFT) {
			//�������������ٸ� �ڼ�
			if (_viEnemy->x - _viEnemy->patrolpointX.x < _viEnemy->speed) {
				_viEnemy->x = _viEnemy->patrolpointX.x;
			}
			//�ƴ϶�� �ӵ���ŭ �̵�.
			else _viEnemy->x -= _viEnemy->speed;
		}
		else {
			if (_viEnemy->patrolpointX.y - _viEnemy->x < _viEnemy->speed) {
				_viEnemy->x = _viEnemy->patrolpointX.y;
			}
			else _viEnemy->x += _viEnemy->speed;
		}
		if (_viEnemy->x == _viEnemy->patrolpointX.x || _viEnemy->x == _viEnemy->patrolpointX.y) {
			_viEnemy->state = ENEMYSTATE::IDLE;
			_viEnemy->haveToChangeAni = true;
			_viEnemy->patterncount = 0;
		}
		break;
	case ENEMYSTATE::ATTACK:
		break;
	case ENEMYSTATE::RUN:
		break;
	case ENEMYSTATE::TURN:
		break;
	case ENEMYSTATE::DEAD:
		break;
	}

}

void grunt::giveFrame()
{
	if (!_viEnemy->haveToChangeAni) return;
	_viEnemy->haveToChangeAni = false;


	switch (_viEnemy->foward)
	{
	case FOWARD::UP:
	case FOWARD::LEFT:
	case FOWARD::LEFTUP:
	case FOWARD::LEFTDOWN:
		_viEnemy->foward = FOWARD::LEFT;
		switch (_viEnemy->state)
		{
		case ENEMYSTATE::IDLE:
			ANIMATION->changeNonKeyAnimation(_viEnemy->ani, "grunt_all", 111, 104, 10, false, true);
			break;
		case ENEMYSTATE::ATTACK:
			ANIMATION->changeNonKeyAnimation(_viEnemy->ani, "grunt_all", 191, 184, 10, false, false);
			break;
		case ENEMYSTATE::RUN:
			ANIMATION->changeNonKeyAnimation(_viEnemy->ani, "grunt_all", 127, 118, 10, false, true);
			break;
		case ENEMYSTATE::WALK:
			ANIMATION->changeNonKeyAnimation(_viEnemy->ani, "grunt_all", 159, 150, 10, false, true);
			break;
		case ENEMYSTATE::TURN:
			ANIMATION->changeNonKeyAnimation(_viEnemy->ani, "grunt_all", 143, 136, 10, false, false);
			break;
		case ENEMYSTATE::DEAD:
			ANIMATION->changeNonKeyAnimation(_viEnemy->ani, "grunt_all", 175, 160, 10, false, false);
			break;
		}
		break;

	case FOWARD::DOWN:
	case FOWARD::RIGHT:
	case FOWARD::RIGHTUP:
	case FOWARD::RIGHTDOWN:
		_viEnemy->foward = FOWARD::RIGHT;
		switch (_viEnemy->state)
		{
		case ENEMYSTATE::IDLE:
			ANIMATION->changeNonKeyAnimation(_viEnemy->ani, "grunt_all", 0, 7, 10, false, true);
			break;
		case ENEMYSTATE::TURN:
			ANIMATION->changeNonKeyAnimation(_viEnemy->ani, "grunt_all", 32, 39, 10, false, false);
			break;
		case ENEMYSTATE::RUN:
			ANIMATION->changeNonKeyAnimation(_viEnemy->ani, "grunt_all", 16, 25, 10, false, true);
			break;
		case ENEMYSTATE::WALK:
			ANIMATION->changeNonKeyAnimation(_viEnemy->ani, "grunt_all", 48, 57, 10, false, true);
			break;
		case ENEMYSTATE::ATTACK:
			ANIMATION->changeNonKeyAnimation(_viEnemy->ani, "grunt_all", 80, 87, 10, false, false);
			break;
		case ENEMYSTATE::DEAD:
			ANIMATION->changeNonKeyAnimation(_viEnemy->ani, "grunt_all", 64, 79, 10, false, false);
			break;
		}
		break;
	}
}

void grunt::deathCheck()
{
	if (_viEnemy->isDeath) return;
	//�÷��̾� �Ѿ˿� �׾��ٸ� ���ް� �浹ü���� ���º���
	if (_viEnemy->col->isColEnter() && _viEnemy->col->isThere(COLLIDER_TYPE::BULLET_PLAYER)) {
		_viEnemy->isDeath = true;
		_viEnemy->state = ENEMYSTATE::DEAD;
		_viEnemy->haveToChangeAni = true;
		_viEnemy->effectAngle = _viEnemy->deathAngle = PLAYER->getCollider()->getPos().AngleTo(_viEnemy->col->getPos());
		_viEnemy->foward = _viEnemy->col->getPos().whichFoward(PLAYER->getCollider()->getPos());
		float startX = _viEnemy->col->getPos().x + 1566 * cos((float)PI + _viEnemy->deathAngle);
		float startY = _viEnemy->col->getPos().y - 1566 * sinf((float)PI + _viEnemy->deathAngle);
		_viEnemy->deatheffect = EFFECT->play("hitEffect", ZEFFECT2, startX, startY, _viEnemy->deathAngle, 150);
		_viEnemy->deathSpeed = 15;

		if(_viEnemy->foward == FOWARD::UP||	_viEnemy->foward == FOWARD::LEFT||	_viEnemy->foward == FOWARD::LEFTUP||_viEnemy->foward == FOWARD::LEFTDOWN)
			ANIMATION->changeNonKeyAnimation(_viEnemy->bloodAni, "blood", 0, 9, 10, false, false);
		else
			ANIMATION->changeNonKeyAnimation(_viEnemy->bloodAni, "blood", 10, 19, 10, false, false);
		CAMERA->setShake(20, 10, 1);
		_viEnemy->col->setCanCol(false);
		_viEnemy->searchCol->setCanCol(false);
		COLLISION->erase(_viEnemy->searchCol);
	}
}

void grunt::findPlayer()
{
	
	//�߰��ߴ�! - ���������� �߰߻���
	if (!_viEnemy->findPlayer && _viEnemy->searchCol->isThere(COLLIDER_TYPE::PLAYER_UNIT))
	{
		//�浹�ϰ��ִ� �ݶ��̴��� ���� ����? (��, �������˻�)
		//_viEnemy->col->getOthers();


		//����ߴٸ�
		_viEnemy->findPlayer = true;
		_viEnemy->state = ENEMYSTATE::IDLE;

		if(PLAYER->getCollider()->getPos().x > _viEnemy->col->getPos().x)
			_viEnemy->foward = FOWARD::RIGHT;
		else
			_viEnemy->foward = FOWARD::LEFT;
		_viEnemy->haveToChangeAni = true;
		_viEnemy->patterncount = 0;
	}
}

void grunt::deathMove()
{
	if (_viEnemy->isDeath) {
		//����Ʈ�̵� & ������.*/
		if (_viEnemy->deatheffect != nullptr) {
			_viEnemy->deatheffect->offsetEffect(cosf(_viEnemy->effectAngle) * 150, -sinf(_viEnemy->effectAngle) * 150);
			if (!_viEnemy->deatheffect->getIsRunning()) _viEnemy->deatheffect = nullptr;		//�ڱ�����Ʈ������ Į���� ����
		}

		//xy�̵�
		_viEnemy->x = _viEnemy->x + cosf(_viEnemy->deathAngle) * _viEnemy->deathSpeed;
		if(!_viEnemy->isLand) _viEnemy->y = _viEnemy->y - sinf(_viEnemy->deathAngle) * _viEnemy->deathSpeed;
		_viEnemy->deathSpeed -= 0.3;
		if (_viEnemy->deathSpeed < 0) _viEnemy->deathSpeed = 0;
		//���浹�� ���� �ñ�¹�������
		if (_viEnemy->isLeftCol) {
			_viEnemy->deathAngle = PI - _viEnemy->deathAngle;
			_viEnemy->isLeftCol = false;
		}
		if (_viEnemy->isRightCol) {
			_viEnemy->deathAngle = PI - _viEnemy->deathAngle;
			_viEnemy->isRightCol = false;
		}
		if (_viEnemy->isUpCol) {
			_viEnemy->deathAngle = PI2 - _viEnemy->deathAngle;
			_viEnemy->isUpCol = false;
		}
	}
}

void grunt::getgravity()
{
	if (!_viEnemy->isLand) {
		_viEnemy->y = _viEnemy->y + 5;	//�߷�
	}
}

void grunt::setCollider()
{
	_viEnemy->col->setPos(Vec2(_viEnemy->x + _viEnemy->img->getFrameWidth() / 2, _viEnemy->y + _viEnemy->img->getFrameHeight() * 3 / 5));
	_viEnemy->searchCol->setPos(Vec2(_viEnemy->col->getPos().x, _viEnemy->col->getBottom() - 8));
}
