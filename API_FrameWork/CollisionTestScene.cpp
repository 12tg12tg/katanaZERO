#include "framework.h"
#include "CollisionTestScene.h"

CollisionTestScene::CollisionTestScene()
{
}

CollisionTestScene::~CollisionTestScene()
{
}

HRESULT CollisionTestScene::init()
{
	COLLISION->CollisionCheck(COLLIDER_TYPE::PLAYER_UNIT, COLLIDER_TYPE::BULLET_ENEMY);
	_angle = 0;
	_rotateCol = COLLISION->addCollider({WINSIZEX/2, WINSIZEY/2 }, { 100, 50 }, COLLIDER_TYPE::BULLET_ENEMY,
		ZCOL3, true, _angle);
	_fan = new fan;
	_fan->init(WINSIZEX, WINSIZEY);
	return S_OK;
}

void CollisionTestScene::update()
{
	//�˻���
	count++;
	if (count % 100 == 0) {
		Collider* temp;
		temp = COLLISION->addCollider({ RND->getFloat(WINSIZEX), 0 }, { 50, 50 }, COLLIDER_TYPE::BULLET_ENEMY,
			ZCOL3);
		_vCollider.push_back(temp);
	}
	//ȸ��ü��������
	if (INPUT->isStayKeyDown(VK_RIGHT)) {
		_angle += 0.1;
	}
	if (INPUT->isStayKeyDown(VK_LEFT)) {
		_angle -= 0.1;
	}
	_rotateCol->setAngle(_angle);

	//����ġ����
	for (_viCollider = _vCollider.begin(); _viCollider != _vCollider.end(); )
	{
		(*_viCollider)->setPos((*_viCollider)->getPos().moveY(5));

		if ((*_viCollider)->getPos().y > WINSIZEY / 2+100 ||
			(*_viCollider)->isColEnter()) {
			COLLISION->erase(*_viCollider);
			_viCollider=_vCollider.erase(_viCollider);
		}
		else {
			++_viCollider;
		}
	}
	

	PLAYER->update();
	ANIMATION->update();
	COLLISION->update();
	CAMERA->movePivot(PLAYER->getCollider()->getPos().x, PLAYER->getY());
	CAMERA->update();
	EFFECT->update();
}

void CollisionTestScene::release()
{
}

void CollisionTestScene::render()
{
	_fan->render();
	PLAYER->render();
	COLLISION->render();
	EFFECT->render();
}
