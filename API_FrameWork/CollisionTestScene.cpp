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
	return S_OK;
}

void CollisionTestScene::update()
{
	//¶Ë»ı¼º
	count++;
	if (count % 100 == 0) {
		Collider* temp;
		temp = COLLISION->addCollider({ RND->getFloat(WINSIZEX), 0 }, { 50, 50 }, COLLIDER_TYPE::BULLET_ENEMY,
			ZCOL3);
		_vCollider.push_back(temp);
	}
	//È¸ÀüÃ¼°¢µµ¾÷µ«
	if (INPUT->isStayKeyDown(VK_RIGHT)) {
		_angle += 0.1;
	}
	if (INPUT->isStayKeyDown(VK_LEFT)) {
		_angle -= 0.1;
	}
	_rotateCol->setAngle(_angle);

	//¶ËÀ§Ä¡¾÷µ«
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
	
	//TCHAR str[128];
	//_stprintf_s(str, "Å×½ºÆ®¾Àº¤ÅÍÅ©±â : %d", _vCollider.size());
	//ZORDER->UITextOut(str, ZUIMOUSE, 300, 300, RGB(0, 0, 0));

	COLLISION->update();
}

void CollisionTestScene::release()
{
}

void CollisionTestScene::render()
{
	COLLISION->render();
}
