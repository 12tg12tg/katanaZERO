#include "framework.h"
#include "textMap3.h"

textMap3::textMap3()
{
	_map = IMAGE->addImage("map3", "images/map/third_back.bmp", 1863, 1189);
	_bwmap = IMAGE->addImage("map3_bw", "images/map/third_back_bw.bmp", 1863, 1189);
	_colmap = IMAGE->addImage("map3_col", "images/map/third_col.bmp", 1863, 1189, true);
	_map_front = IMAGE->addImage("map3_front", "images/map/third_front.bmp", 1863, 1189, true);
	_bwmap_front = IMAGE->addImage("map3_front_bw", "images/map/third_front_bw.bmp", 1863, 1189, true);
	_addforslow = IMAGE->addImage("map3_addforslow", "images/map/third_back_addforslow.bmp", 1863, 1189, true);
	_startPt = Vec2(42, 974);

	_nextScene = "º¸½º¸Ê";
}

textMap3::~textMap3()
{
}

HRESULT textMap3::init()
{
	_goal = COLLISION->addCollider(Vec2(1853, 265), Vec2(19, 221), COLLIDER_TYPE::POTAL, ZCOL1);

	PLAYER->setX(_startPt.x);
	PLAYER->setY(_startPt.y);

	prevx = PLAYER->getCollider()->getPos().x;
	prevy = PLAYER->getCollider()->getPos().y;
	prevRc = PLAYER->getCollider()->getRect();

	CAMERA->init(PLAYER->getX(), PLAYER->getY(), _map->getWidth(), _map->getHeight(), 0, 0, WINSIZEX / 2, WINSIZEY / 2,
		WINSIZEX, WINSIZEY);
	return S_OK;
}

void textMap3::release()
{
}

void textMap3::update()
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

void textMap3::render()
{
	//¸Ê
	//ZORDER->ZorderRender(_addforslow, ZSLOWFADE, 1, 0, 0);
	ZORDER->ZorderRender(_map, ZFLOORMAP, 0, 0, 0);
	ZORDER->ZorderRender(_map_front, ZABOVEMAP, 0, 0, 0);

	if (_isDebug) ZORDER->ZorderRender(_colmap, ZCOLMAP, 0, 0, 0);

	//--------------------------------------------------
	PLAYER->render();
	COLLISION->render();
	EFFECT->render();
}
