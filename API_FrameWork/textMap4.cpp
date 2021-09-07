#include "framework.h"
#include "textMap4.h"

textMap4::textMap4()
{
	_map = IMAGE->addImage("map4", "images/map/boss.bmp", 1365, 784);
	_bwmap = IMAGE->addImage("map4_bw", "images/map/boss_bw.bmp", 1365, 784);
	_colmap = IMAGE->addImage("map4_col", "images/map/boss_col.bmp", 1365, 784, true);
	_map_front = _bwmap_front = nullptr;
	_startPt = Vec2(328, 520);
	_timelimit = 6000;
	_nextScene = "엔딩";
}

textMap4::~textMap4()
{
}

HRESULT textMap4::init()
{
	Cmap::init();
	_goal = nullptr;
	PLAYER->setX(_startPt.x);
	PLAYER->setY(_startPt.y);

	prevx = PLAYER->getCollider()->getPos().x;
	prevy = PLAYER->getCollider()->getPos().y;
	prevRc = PLAYER->getCollider()->getRect();

	CAMERA->init(PLAYER->getX(), PLAYER->getY(), _map->getWidth(), _map->getHeight(), 0, 0, WINSIZEX / 2, WINSIZEY / 2,
		WINSIZEX, WINSIZEY);

	_timeCount = _timelimit;
	_isClear = false;
	return S_OK;
}

void textMap4::release()
{
}

void textMap4::update()
{
	//타이머
	timeCheck();
	//충돌
	coltoMap();



	CheckClear();
	//--------------------------------------------------
	PLAYER->update();
	ANIMATION->update();
	COLLISION->update();
	//POINT lerp;
	//lerp.x = CAMERA->getPivotX() * (1 - 0.08) + PLAYER->getCollider()->getPos().x * 0.08;
	//lerp.y = CAMERA->getPivotY() * (1 - 0.08) + PLAYER->getY() * 0.08;
	//CAMERA->movePivot(lerp.x, lerp.y);
	CAMERA->update();
	EFFECT->update();
}

void textMap4::render()
{
	//맵
	ZORDER->ZorderRender(_map, ZFLOORMAP, 0, 0, 0);
	if (_isDebug) ZORDER->ZorderRender(_colmap, ZCOL1, 0, 0, 0);


	//--------------------------------------------------
	PLAYER->render();
	COLLISION->render();
	EFFECT->render();
}

void textMap4::CheckClear()
{

}
