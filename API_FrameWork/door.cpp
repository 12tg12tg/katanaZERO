#include "framework.h"
#include "door.h"
#include "PlayerState.h"
door::door()
{
}

door::~door()
{
}

void door::addEnemy(float x, float y)
{
	tagEnemy newDoor;
	newDoor.x = x;
	newDoor.y = y;
	newDoor.z = ZUNITBACK;
	newDoor.img = IMAGE->addFrameImage("door", "images/enemy/door.bmp", 3200, 127, 20, 1, true);
	newDoor.bwimg = IMAGE->addFrameImage("door_bw", "images/enemy/door_bw.bmp", 3200, 127, 20, 1, true);
	newDoor.col = COLLISION->addCollider(Vec2(x+newDoor.img->getFrameWidth()*21/30 + 7, y+ newDoor.img->getFrameHeight()/2 +1),
		Vec2(newDoor.img->getFrameWidth()*4/30, newDoor.img->getFrameHeight()), COLLIDER_TYPE::DOOR,
		ZCOL3);
	int arr[] = { 19 };
	newDoor.ani = ANIMATION->addNoneKeyAnimation("door", arr, sizeof(arr) / sizeof(int), 1, false);
	newDoor.bottom = y + newDoor.img->getFrameHeight();
	newDoor.isDeath = false;
	newDoor.state = ENEMYSTATE::IDLE;
	newDoor.count = 0;
	newDoor.isfirstTime = false;
	_vEnemy.push_back(newDoor);
}


void door::release()
{
	for (_viEnemy = _vEnemy.begin(); _viEnemy != _vEnemy.end(); )
	{
		COLLISION->erase(_viEnemy->col);
		_viEnemy = _vEnemy.erase(_viEnemy);
	}
}

void door::update()
{
	for (_viEnemy = _vEnemy.begin(); _viEnemy != _vEnemy.end(); ++_viEnemy)
	{
		switch (_viEnemy->state)
		{
		case ENEMYSTATE::IDLE:
			if (_viEnemy->col->isColIng() && _viEnemy->col->isThere(COLLIDER_TYPE::PLAYER_UNIT) &&
				PLAYER->getState()==PLAYERSTATE::RUN) {
				++_viEnemy->count;
				if (_viEnemy->count > 10) {
					_viEnemy->state = ENEMYSTATE::DEAD;
					PLAYER->getFSM()->ChangeState(PLAYERSTATE::DOORBREAK);
				}
			}
			else if (_viEnemy->col->isColEnter() && _viEnemy->col->isThere(COLLIDER_TYPE::BULLET_PLAYER)) {
				//사운드
				SOUND->play("doorbreak", 0.1f);
				_viEnemy->state = ENEMYSTATE::DEAD;
			}
			break;
		case ENEMYSTATE::FIND:
		case ENEMYSTATE::MOVE:
		case ENEMYSTATE::ATTACK:
			break;
		case ENEMYSTATE::DEAD:
			if (!_viEnemy->isfirstTime) {
				CAMERA->setShake(10, 10, 1);
				_viEnemy->isfirstTime = true;
				COLLISION->erase(_viEnemy->col);
				ANIMATION->changeNonKeyAnimation(_viEnemy->ani, "door", 18, 0, 15, false, false);
				/*fire*/
			}
			break;
		}

		//문충돌
		if (_viEnemy->state != ENEMYSTATE::DEAD && _viEnemy->col->isThere(COLLIDER_TYPE::PLAYER_UNIT)) {
			RECT temp;
			RECT door = _viEnemy->col->getRect();
			RECT player = PLAYER->getCollider()->getRect();
			if (IntersectRect(&temp, &door, &player)) {
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
					PLAYER->setX(door.right - (player.left - PLAYER->getX()));
				}
			}
			PLAYER->setCollider();
		}

	}
}

void door::render()
{
	for (_viEnemy = _vEnemy.begin(); _viEnemy != _vEnemy.end(); ++_viEnemy)
	{
		ZORDER->ZorderAniRender(_viEnemy->img, _viEnemy->z, _viEnemy->bottom, _viEnemy->x, _viEnemy->y, _viEnemy->ani);
		ZORDER->SaveAniRender(_viEnemy->img, _viEnemy->bwimg, _viEnemy->z, _viEnemy->bottom, _viEnemy->x, _viEnemy->y, _viEnemy->ani);
	}
}

