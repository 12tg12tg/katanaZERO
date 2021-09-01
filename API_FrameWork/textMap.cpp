#include "framework.h"
#include "textMap.h"

textMap::textMap()
{
	_map = IMAGE->addImage("map1", "images/map/first.bmp", 1733, 768);
	_colmap = IMAGE->addImage("map1", "images/map/first_col.bmp", 1733, 768, true);
}

textMap::~textMap()
{
}

HRESULT textMap::init()
{
	CAMERA->init(PLAYER->getX(), PLAYER->getY(), _map->getWidth(), _map->getHeight(), 0, 0, WINSIZEX / 2, WINSIZEY / 2,
		WINSIZEX, WINSIZEY);
    return S_OK;
}

void textMap::release()
{
}

void textMap::update()
{


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
	ZORDER->ZorderRender(_map, ZFLOORMAP, 0, 0, 0);


	//--------------------------------------------------
	PLAYER->render();
	COLLISION->render();
	EFFECT->render();
}
