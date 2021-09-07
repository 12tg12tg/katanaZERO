#include "framework.h"
#include "grunt.h"
#include "effect.h"
grunt::grunt(bool isLaser, bool isStair, Vec2 downPoint, Vec2 upPoint)
	:_isLaser(isLaser), _isStair(isStair), _DownPoint(downPoint), _UpPoint(upPoint)
{
	//����ǥ
	IMAGE->addImage("followMark", "images/enemy/enemy_follow.bmp", 16, 20, true);
	//�̹���
	IMAGE->addFrameImage("grunt_all", "images/enemy/grunt_all.bmp", 896 * 2, 504 * 2, 16, 12, true);
	IMAGE->addFrameImage("grunt_all_bw", "images/enemy/grunt_all_bw.bmp", 896 * 2, 504 * 2, 16, 12, true);
	IMAGE->addFrameImage("grunt_all_laserdie", "images/enemy/grunt_all_laserdie.bmp", 896 * 2, 504 * 2, 16, 12, true);
	IMAGE->addFrameImage("blood", "images/effect/blood.bmp", 800, 158, 10, 2, true);
	IMAGE->addFrameImage("blood_bw", "images/effect/blood_bw.bmp", 800, 158, 10, 2, true);
	IMAGE->addFrameImage("bloodremain", "images/effect/bloodRemain.bmp", 432, 288, 3, 2, true);
	IMAGE->addFrameImage("bloodremain_bw", "images/effect/bloodRemain_bw.bmp", 432, 288, 3, 2, true);

	//����̵� �� ���
	_goDownAngle = _UpPoint.AngleTo(_DownPoint);

	//��ġ
	_gP = new gruntPunch;
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
	newEnemy.isLand = true;
	newEnemy.bloodOn = false;
	newEnemy.patrolpointX = patrolpoint;
	newEnemy.destinationY = 0;
	newEnemy.angle = 0;
	newEnemy.laserDie = false;
	newEnemy.laserDieover = false;
	newEnemy.laserX = 0;
	newEnemy.attackDelay = 0;
	_vEnemy.push_back(newEnemy);
}

void grunt::release()
{
	for (_viEnemy = _vEnemy.begin(); _viEnemy != _vEnemy.end(); )
	{
		COLLISION->erase(_viEnemy->col);
		COLLISION->erase(_viEnemy->searchCol);
		_viEnemy = _vEnemy.erase(_viEnemy);
	}
	_gP->release();
}

void grunt::update()
{
	for (_viEnemy = _vEnemy.begin(); _viEnemy != _vEnemy.end(); ++_viEnemy)
	{
		if (!_viEnemy->laserDie) {
			setCollider();
			giveFrame();
			deathCheck();
			findPlayer();
			deathMove();
			getgravity();
			move();
			doorCollision();
			laserCollision();
			attack();
		}
		else {
			laserDie();
		}

		//if (INPUT->isStayKeyDown(VK_RIGHT)) {
		//	_viEnemy->x += 3;
		//}
		//if (INPUT->isStayKeyDown(VK_LEFT)) {
		//	_viEnemy->x -= 3;
		//}
	}
	_gP->update();
}

void grunt::render()
{
	for (_viEnemy = _vEnemy.begin(); _viEnemy != _vEnemy.end(); ++_viEnemy)
	{
		if (!_viEnemy->laserDie) {
			//�Ϲݻ���
			ZORDER->ZorderAniRender(_viEnemy->img, _viEnemy->z, _viEnemy->bottom, _viEnemy->x, _viEnemy->y, _viEnemy->ani);
			ZORDER->SaveAniRender(_viEnemy->img, _viEnemy->bwimg, _viEnemy->z, _viEnemy->bottom, _viEnemy->x, _viEnemy->y, _viEnemy->ani);
			if (_viEnemy->findPlayer && !_viEnemy->isDeath) {
				ZORDER->ZorderRender(IMAGE->findImage("followMark"), _viEnemy->z,
					_viEnemy->col->getRect().top, _viEnemy->col->getPos().x,
					_viEnemy->col->getRect().top - 30);
			}
			//�׾�����
			if (_viEnemy->isDeath) {
				if (_viEnemy->bloodAni->isPlay()) {
					if (_viEnemy->foward == FOWARD::LEFT) {
						ZORDER->ZorderAniRender(IMAGE->findImage("blood"), ZEFFECT2, 0, _viEnemy->col->getPos().x + 10, _viEnemy->col->getPos().y - IMAGE->findImage("blood")->getFrameHeight(), _viEnemy->bloodAni);
						ZORDER->SaveAniRender(IMAGE->findImage("blood"), IMAGE->findImage("blood_bw"), ZEFFECT2, 0, _viEnemy->col->getPos().x + 10, _viEnemy->col->getPos().y - IMAGE->findImage("blood")->getFrameHeight(), _viEnemy->bloodAni);
					}
					else {
						ZORDER->ZorderAniRender(IMAGE->findImage("blood"), ZEFFECT2, 0, _viEnemy->col->getPos().x - IMAGE->findImage("blood")->getFrameWidth(), _viEnemy->col->getPos().y - IMAGE->findImage("blood")->getFrameHeight(), _viEnemy->bloodAni);
						ZORDER->SaveAniRender(IMAGE->findImage("blood"), IMAGE->findImage("blood_bw"), ZEFFECT2, 0, _viEnemy->col->getPos().x - IMAGE->findImage("blood")->getFrameWidth(), _viEnemy->col->getPos().y - IMAGE->findImage("blood")->getFrameHeight(), _viEnemy->bloodAni);
					}
				}
				if ((_viEnemy->bloodAni->getFrameX() == 4 && _viEnemy->bloodAni->getFrameY() == 0) ||
					(_viEnemy->bloodAni->getFrameX() == 5 && _viEnemy->bloodAni->getFrameY() == 1)) {
					_viEnemy->bloodOn = true;
					_viEnemy->bloodRemainIndex = RND->getInt(3);
				}
			}
			//��ü�ǰ���
			if (_viEnemy->bloodOn) {
				if (_viEnemy->foward == FOWARD::RIGHT) {
					ZORDER->ZorderFrameRender(IMAGE->findImage("bloodremain"), ZEFFECT1, 0, _viEnemy->col->getPos().x - IMAGE->findImage("blood")->getFrameWidth() - 36, _viEnemy->col->getPos().y - IMAGE->findImage("bloodremain")->getFrameHeight(), _viEnemy->bloodRemainIndex, 0);
					ZORDER->SaveFrameRender(IMAGE->findImage("bloodremain"), IMAGE->findImage("bloodremain_bw"), ZEFFECT1, 0, _viEnemy->col->getPos().x - IMAGE->findImage("blood")->getFrameWidth() - 36, _viEnemy->col->getPos().y - IMAGE->findImage("bloodremain")->getFrameHeight(), _viEnemy->bloodRemainIndex, 0);
				}
				if (_viEnemy->foward == FOWARD::LEFT) {
					ZORDER->ZorderFrameRender(IMAGE->findImage("bloodremain"), ZEFFECT1, 0, _viEnemy->col->getPos().x + 10, _viEnemy->col->getPos().y - IMAGE->findImage("bloodremain")->getFrameHeight(), _viEnemy->bloodRemainIndex, 1);
					ZORDER->SaveFrameRender(IMAGE->findImage("bloodremain"), IMAGE->findImage("bloodremain_bw"), ZEFFECT1, 0, _viEnemy->col->getPos().x + 10, _viEnemy->col->getPos().y - IMAGE->findImage("bloodremain")->getFrameHeight(), _viEnemy->bloodRemainIndex, 1);
				}
			}
		}
		else {
			if (!_viEnemy->laserDieover) {
				EFFECT->addParticle("����������", ZEFFECT2, _viEnemy->col->getPos().x + RND->getInt(30) - 15, _viEnemy->y + _viEnemy->laserX, 10, RND->getFloatFromTo(PI_8, PI_8 * 7), 20, 5, false);
				EFFECT->addParticle("����������", ZEFFECT2, _viEnemy->col->getPos().x + RND->getInt(30) - 15, _viEnemy->y + _viEnemy->laserX, 10, RND->getFloatFromTo(PI_8, PI_8 * 7), 20, 5, false);
				EFFECT->addParticle("����������", ZEFFECT2, _viEnemy->col->getPos().x + RND->getInt(30) - 15, _viEnemy->y + _viEnemy->laserX, 10, RND->getFloatFromTo(PI_8, PI_8 * 7), 20, 5, false);
				ZORDER->ZorderFrameRender(IMAGE->findImage("grunt_all_laserdie"), _viEnemy->z, _viEnemy->bottom, _viEnemy->x, _viEnemy->y + _viEnemy->laserX, _viEnemy->ani->getFrameX(),
					_viEnemy->ani->getFrameY(), 0, _viEnemy->laserX, _viEnemy->img->getFrameWidth(), _viEnemy->img->getFrameHeight() - _viEnemy->laserX);
				ZORDER->SaveFrameRender(IMAGE->findImage("grunt_all_laserdie"), IMAGE->findImage("grunt_all_bw"), _viEnemy->z, _viEnemy->bottom, _viEnemy->x, _viEnemy->y + _viEnemy->laserX, _viEnemy->ani->getFrameX(),
					_viEnemy->ani->getFrameY(), 0, _viEnemy->laserX, _viEnemy->img->getFrameWidth(), _viEnemy->img->getFrameHeight() - _viEnemy->laserX);
			}
		}
	}
	_gP->render();
	/*�����*/
	//string str;
	//for (size_t i = 0; i < _vEnemy.size(); i++)
	//{
	//	DWORD id = _vEnemy[i].col->getID();
	//	auto temp = _vEnemy[i].col->getOthers();	
	//	str += "("+to_string(id)+", "+ to_string(temp.size()) + ") /";
	//}
	//
	//ZORDER->UITextOut(str, ZMAXLAYER, 500, 500, RGB(255,255, 255));
	//else ZORDER->UITextOut("�÷��̾�� ��", ZMAXLAYER, 500, 500, RGB(255, 255, 255));
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
			//�߽߰� �ٷ� IDLE�� �ò���
			//1. �÷��̾ �����ִ��� �Ʒ��ִ��� �Ǵ�.(���÷� �Ǵ�)
			//2. �������� �ִٸ�, �÷��̾��� ��ġ�� ��ǥ�λ�� ������ RUN. (��ǥ�� ���÷� ����)
			//3. �Ʒ����� �ִٸ�, checkPoint�� ��ǥ�� RUN.
			//4. checkPoint�� ���ٸ�, (X�ȼ��浹�� Ÿ�� ���ʾƷ��� �̵�.X) (üũ����Ʈ �ΰ��� �̿��� ������ �̵�)
			//5. ��Ÿ������� ���Դٸ� ������ ����. ( ���÷� �Ǵ�)
			checkRoute();



		}
		break;
	case ENEMYSTATE::WALK:
		if (_viEnemy->foward == FOWARD::LEFT) {
			//�������������ٸ� �ڼ�
			if (_viEnemy->x - _viEnemy->patrolpointX.x < _viEnemy->speed) {
				_viEnemy->x = _viEnemy->patrolpointX.x;
			}
			//�ƴ϶�� �ӵ���ŭ �̵�.
			else _viEnemy->x -= _viEnemy->speed * TIME->getGameTimeRate();
		}
		else {
			if (_viEnemy->patrolpointX.y - _viEnemy->x < _viEnemy->speed ) {
				_viEnemy->x = _viEnemy->patrolpointX.y;
			}
			else _viEnemy->x += _viEnemy->speed * TIME->getGameTimeRate();
		}
		if (_viEnemy->x == _viEnemy->patrolpointX.x || _viEnemy->x == _viEnemy->patrolpointX.y) {
			_viEnemy->state = ENEMYSTATE::IDLE;
			_viEnemy->haveToChangeAni = true;
			_viEnemy->patterncount = 0;
		}
		break;
	case ENEMYSTATE::ATTACK:
	{
		if (!_viEnemy->ani->isPlay()) {
			_viEnemy->state = ENEMYSTATE::IDLE;
			_viEnemy->patterncount = 0;
			_viEnemy->haveToChangeAni = true;
		}
	}
		break;
	case ENEMYSTATE::RUN:
		if (_viEnemy->destinationY == 0) {
			//���� �̵�.
			//�����ٸ� �ڼ�
			if (abs(_viEnemy->destinationX - _viEnemy->x) < _viEnemy->speed) {
				_viEnemy->x = _viEnemy->destinationX;
				_viEnemy->state = ENEMYSTATE::IDLE;
				_viEnemy->haveToChangeAni = true;
			}
			//�ƴ϶�� �ӵ���ŭ �̵�.
			else {
				if (_viEnemy->foward == FOWARD::LEFT)
					_viEnemy->x -= _viEnemy->speed * TIME->getGameTimeRate();
				else
					_viEnemy->x += _viEnemy->speed * TIME->getGameTimeRate();
			}
		}
		else {
			//�����������̵�.
			//�����ٸ� �ڼ�
			float tempX = _viEnemy->x + cosf(_viEnemy->angle) * _viEnemy->speed;
			float tempY = _viEnemy->y - sinf(_viEnemy->angle) * _viEnemy->speed;
			if (UTIL::getDistance(tempX, tempY, _viEnemy->destinationX, _viEnemy->destinationY) <= _viEnemy->speed) {
				_viEnemy->x = _viEnemy->destinationX * TIME->getGameTimeRate();
				_viEnemy->y = _viEnemy->destinationY * TIME->getGameTimeRate();
				_viEnemy->state = ENEMYSTATE::IDLE;
				_viEnemy->haveToChangeAni = true;
			}
			else {
				_viEnemy->x = tempX;
				_viEnemy->y = tempY;
			}
		}

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
		//_viEnemy->col->setCanCol(false);
		_viEnemy->searchCol->setCanCol(false);
		COLLISION->erase(_viEnemy->searchCol);
	}
}

void grunt::findPlayer()
{
	if (_viEnemy->isDeath) return;
	//�߰��ߴ�! - ���������� �߰߻���
	if (!_viEnemy->findPlayer && _viEnemy->searchCol->isThere(COLLIDER_TYPE::PLAYER_UNIT))
	{
		//�浹�ϰ��ִ� �ݶ��̴��� ���� ����? (�� �˻�)
		FOWARD playerFoward;
		FOWARD doorFoward;
		float playerDis = 0;
		float otherMin = 10000;
		auto& vOthers = _viEnemy->searchCol->getOthers();
		auto viOthers = vOthers.begin();
		for (viOthers; viOthers != vOthers.end(); ++viOthers)
		{
			if (viOthers->second->getType() == COLLIDER_TYPE::PLAYER_UNIT) {
				playerDis = _viEnemy->searchCol->getPos().Distance(viOthers->second->getPos());
				playerFoward = _viEnemy->col->getPos().whichFoward(_viEnemy->searchCol->getPos());
				if (playerFoward == FOWARD::LEFT || playerFoward == FOWARD::LEFTDOWN || playerFoward == FOWARD::LEFTUP) {
					playerFoward = FOWARD::LEFT;
				}
				else {
					playerFoward = FOWARD::RIGHT;
				}
			}
		}
		for (viOthers = vOthers.begin(); viOthers != vOthers.end(); ++viOthers)
		{
			if (viOthers->second->getType() == COLLIDER_TYPE::DOOR) {
				doorFoward = _viEnemy->col->getPos().whichFoward(viOthers->second->getPos());
				if (doorFoward == FOWARD::LEFT || doorFoward == FOWARD::LEFTDOWN || doorFoward == FOWARD::LEFTUP) {
					doorFoward = FOWARD::LEFT;
				}
				else {
					doorFoward = FOWARD::RIGHT;
				}
				if (doorFoward != playerFoward) break;
				otherMin = _viEnemy->searchCol->getPos().Distance(viOthers->second->getPos());
			}
		}
		if (playerDis > otherMin) return;	//���հ�! �߰߾��Ѱ���!

		//����ߴٸ�
		_viEnemy->findPlayer = true;
		_viEnemy->speed = 4.f;
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

void grunt::checkRoute()
{
	if (_isStair) {
		//�߽߰� �ٷ� IDLE�� �ò���
		//1. �÷��̾ �����ִ��� �Ʒ��ִ��� �Ǵ�.(���÷� �Ǵ�)
		//2. �������� �ִٸ�, �÷��̾��� ��ġ�� ��ǥ�λ�� ������ RUN. (��ǥ�� ���÷� ����)
		//3. �Ʒ����� �ִٸ�, checkPoint�� ��ǥ�� RUN.
		//4. checkPoint�� ���ٸ�, (X�ȼ��浹�� Ÿ�� ���ʾƷ��� �̵�.X) (üũ����Ʈ �ΰ��� �̿��� ������ �̵�)
		//5. ��Ÿ������� ���Դٸ� ������ ����. ( ���÷� �Ǵ�)
		//	1 �÷��̾���ġ�Ǵ�
		float playerPosY = PLAYER->getY();
		if (playerPosY <= _UpPoint.y + 50) playerisDownside = false;
		else playerisDownside = true;

		//	2 �ڽ��� ���� �Ǵ�.
		if (Vec2(_viEnemy->x, _viEnemy->y) == _UpPoint) {
			_viEnemy->cpoint = CHASEPOINT::UPPOINT;
		}
		else if (_viEnemy->y == _UpPoint.y) {
			_viEnemy->cpoint = CHASEPOINT::UPSTAIR;
		}
		else if (Vec2(_viEnemy->x, _viEnemy->y) == _DownPoint) {
			_viEnemy->cpoint = CHASEPOINT::DOWNPOINT;
		}
		else if (_viEnemy->y == _DownPoint.y) {
			_viEnemy->cpoint = CHASEPOINT::DOWNSTAIR;
		}
		else {
			_viEnemy->cpoint = CHASEPOINT::ONSTAIR;
		}

		//	3 ���º��� ��ǥ���� ����
		switch (_viEnemy->cpoint)
		{
		case CHASEPOINT::UPSTAIR:
			if (playerisDownside) {
				_viEnemy->destinationX = _UpPoint.x;
				_viEnemy->destinationY = 0;
			}
			else {
				_viEnemy->destinationX = PLAYER->getX();
				_viEnemy->destinationY = 0;
			}
			break;
		case CHASEPOINT::UPPOINT:
			if (playerisDownside) {
				_viEnemy->destinationX = _DownPoint.x;
				_viEnemy->destinationY = _DownPoint.y;
				_viEnemy->angle = _goDownAngle;
			}
			else {
				_viEnemy->destinationX = PLAYER->getX();
				_viEnemy->destinationY = 0;
			}
			break;
		case CHASEPOINT::ONSTAIR:
			if (playerisDownside) {
				_viEnemy->destinationX = _DownPoint.x;
				_viEnemy->destinationY = _DownPoint.y;
				_viEnemy->angle = _goDownAngle;
			}
			else {
				_viEnemy->destinationX = _UpPoint.x;
				_viEnemy->destinationY = _UpPoint.y;
				_viEnemy->angle = _goDownAngle-PI;
			}
			break;
		case CHASEPOINT::DOWNPOINT:
			if (playerisDownside) {
				_viEnemy->destinationX = PLAYER->getX();
				_viEnemy->destinationY = 0;
			}
			else {
				_viEnemy->destinationX = _UpPoint.x;
				_viEnemy->destinationY = _UpPoint.y;
				_viEnemy->angle = _goDownAngle - PI;
			}
			break;
		case CHASEPOINT::DOWNSTAIR:
			if (playerisDownside) {
				_viEnemy->destinationX = PLAYER->getX();
				_viEnemy->destinationY = 0;
			}
			else {
				_viEnemy->destinationX = _DownPoint.x;
				_viEnemy->destinationY = 0;
			}
			break;
		}
		// 4 ���⼳��
		if (_viEnemy->destinationX > _viEnemy->x) {
			_viEnemy->foward = FOWARD::RIGHT;
		}
		else {
			_viEnemy->foward = FOWARD::LEFT;
		}

		//	5 ���� RUN����. RUN->FIND->RUN->FIND->... (��Ÿ����̸� ATTACK�鷵�ٰ� FIND)
		_viEnemy->state = ENEMYSTATE::RUN;
		_viEnemy->haveToChangeAni = true;
	}
	else {
		_viEnemy->destinationX = PLAYER->getCollider()->getPos().x;//��ܾ��¸ʿ����� �� �ߦi�°����� x������ٴ���	
		if (_viEnemy->destinationX > _viEnemy->x) {
			_viEnemy->foward = FOWARD::RIGHT;
		}
		else {
			_viEnemy->foward = FOWARD::LEFT;
		}
		_viEnemy->state = ENEMYSTATE::RUN;
		_viEnemy->haveToChangeAni = true;
	}
}

void grunt::setCollider()
{
	_viEnemy->col->setPos(Vec2(_viEnemy->x + _viEnemy->img->getFrameWidth() / 2, _viEnemy->y + _viEnemy->img->getFrameHeight() * 3 / 5));
	_viEnemy->searchCol->setPos(Vec2(_viEnemy->col->getPos().x, _viEnemy->col->getBottom() - 8));
}

void grunt::doorCollision()
{
	RECT door;
	if (_viEnemy->col->isThere(COLLIDER_TYPE::DOOR)) {
		auto& mTemp = _viEnemy->col->getOthers();
		auto iter = mTemp.begin();
		for (iter; iter != mTemp.end(); ++iter)
		{
			if (iter->second->getType() == COLLIDER_TYPE::DOOR) {
				door = iter->second->getRect();
			}
		}
	
		RECT temp;
		RECT enemyRc = _viEnemy->col->getRect();
		if (IntersectRect(&temp, &door, &enemyRc)) {
			int fromtop, frombottom, fromleft, fromright;
			int centerx, centery;
			int min;
			centerx = temp.left + (temp.right - temp.left) / 2;
			centery = temp.top + (temp.bottom - temp.top) / 2;
			fromtop = centery - door.top;
			frombottom = door.bottom - centery;
			fromleft = centerx - door.left;
			fromright = door.right - centerx;

			min = (fromtop >= frombottom) ? frombottom : fromtop;
			min = (min >= fromleft) ? fromleft : min;
			min = (min >= fromright) ? fromright : min;
			if (min == fromtop && min <= 10)
			{
				//PLAYER->setY(door.top - (player.bottom - player.top));
			}
			else if (min == frombottom)
			{
				//PLAYER->setY(door.bottom);
			}
			else if (min == fromleft)
			{
				//PLAYER->setX(door.left - (player.right - player.left));
			}
			else if (min == fromright)
			{
				_viEnemy->x = door.right - (enemyRc.left - _viEnemy->x);
			}
		}
	}
}

void grunt::laserCollision()
{
	//ZORDER->UITextOut("")
	if (_viEnemy->laserDie) return;
	if (_viEnemy->col->isThere(COLLIDER_TYPE::LASER)) {
		_viEnemy->laserDie = true;
	}
}

void grunt::laserDie()
{
	if (!_viEnemy->laserDie) return;
	_viEnemy->ani->stop();
	_viEnemy->col->setCanCol(false);
	++_viEnemy->laserX;
	if (_viEnemy->laserX == _viEnemy->img->getFrameHeight()) {
		_viEnemy->laserDieover = true;
	}
}

void grunt::attack()
{
	if (!(!_viEnemy->isDeath && !_viEnemy->laserDie && _viEnemy->findPlayer)) return;
	if (_viEnemy->state == ENEMYSTATE::ATTACK || _viEnemy->state == ENEMYSTATE::IDLE) return;
	Vec2 pos = _viEnemy->col->getPos();
	if (PLAYER->getCollider()->getPos().Distance(pos) < 100) {
		FOWARD foward = pos.whichFoward(PLAYER->getCollider()->getPos());
		if (foward == FOWARD::LEFT || foward == FOWARD::LEFTDOWN || foward == FOWARD::LEFTUP || foward == FOWARD::UP) foward = FOWARD::LEFT;
		else foward = FOWARD::RIGHT;
		_gP->fire(pos.x, pos.y, _viEnemy->foward);
		_viEnemy->foward = foward;
		_viEnemy->state = ENEMYSTATE::ATTACK;
		_viEnemy->haveToChangeAni = true;
		_viEnemy->attackDelay = 0;
	}
}
