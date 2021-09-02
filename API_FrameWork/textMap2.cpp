#include "framework.h"
#include "textMap2.h"

textMap2::textMap2()
{
	_map = IMAGE->addImage("map2", "images/map/second.bmp", 2148, 768);
	_bwmap = IMAGE->addImage("map2_bw", "images/map/second_bw.bmp", 2148, 768);
	_colmap = IMAGE->addImage("map2_col", "images/map/second_col.bmp", 2148, 768, true);
	_map_front = _bwmap_front = nullptr;
	_startPt = Vec2(4, 399);
	_nextScene = "Å×½ºÆ®¸Ê3";
}

textMap2::~textMap2()
{
}

HRESULT textMap2::init()
{
	_goal = COLLISION->addCollider(Vec2(2143, 385), Vec2(19, 221), COLLIDER_TYPE::POTAL, ZCOL1);

	PLAYER->setX(_startPt.x);
	PLAYER->setY(_startPt.y);

	prevx = PLAYER->getCollider()->getPos().x;
	prevy = PLAYER->getCollider()->getPos().y;
	prevRc = PLAYER->getCollider()->getRect();

	CAMERA->init(PLAYER->getX(), PLAYER->getY(), _map->getWidth(), _map->getHeight(), 0, 0, WINSIZEX / 2, WINSIZEY / 2,
		WINSIZEX, WINSIZEY);
    return S_OK;
}

void textMap2::release()
{
}

void textMap2::update()
{
	coltoMap();
	_isClear = true;
	CheckClear();
	goalCol();

	//--------------------------------------------------
	PLAYER->update();
	ANIMATION->update();
	COLLISION->update();
	CAMERA->movePivot(PLAYER->getCollider()->getPos().x, PLAYER->getY());
	CAMERA->update();
	EFFECT->update();
}

void textMap2::render()
{
	//¸Ê
	ZORDER->ZorderRender(_map, ZFLOORMAP, 0, 0, 0);
	if (_isDebug) ZORDER->ZorderRender(_colmap, ZCOL1, 0, 0, 0);

	//--------------------------------------------------
	PLAYER->render();
	COLLISION->render();
	EFFECT->render();
}