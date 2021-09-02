#include "framework.h"
#include "textMap.h"
#include "PlayerState.h"
textMap::textMap()
{
	_map = IMAGE->addImage("map1", "images/map/first.bmp", 1733, 768);
	_bwmap = IMAGE->addImage("map1_bw", "images/map/first_bw.bmp", 1733, 768);
	_colmap = IMAGE->addImage("map1_col", "images/map/first_col2.bmp", 1733, 768, true);
	_map_front = _bwmap_front = nullptr;
	_startPt = Vec2(4.5, 539);
	_nextScene = "Å×½ºÆ®¸Ê2";
	COLLISION->CollisionCheck(COLLIDER_TYPE::PLAYER_UNIT, COLLIDER_TYPE::POTAL);
}

textMap::~textMap()
{
}

HRESULT textMap::init()
{
	_goal = COLLISION->addCollider(Vec2(1730, 237), Vec2(19, 221), COLLIDER_TYPE::POTAL, ZCOL1);
	PLAYER->setX(_startPt.x);
	PLAYER->setY(_startPt.y);

	prevx = PLAYER->getCollider()->getPos().x;
	prevy = PLAYER->getCollider()->getPos().y;
	prevRc = PLAYER->getCollider()->getRect();

	CAMERA->init(PLAYER->getX(), PLAYER->getY(), _map->getWidth(), _map->getHeight(), 0, 0, WINSIZEX / 2, WINSIZEY / 2,
		WINSIZEX, WINSIZEY);
    return S_OK;
}

void textMap::release()
{
}

void textMap::update()
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

void textMap::render()
{
	//¸Ê
	ZORDER->ZorderRender(_map, ZFLOORMAP, 0, 0, 0);
	if(_isDebug) ZORDER->ZorderRender(_colmap, ZCOL1, 0, 0, 0);

	//--------------------------------------------------
	PLAYER->render();
	COLLISION->render();
	EFFECT->render();
}

