#include "framework.h"
#include "textMap.h"
#include "PlayerState.h"

textMap::textMap()
{
	_map = IMAGE->addImage("map1", "images/map/first.bmp", 1715, 768);
	_bwmap = IMAGE->addImage("map1_bw", "images/map/first_bw.bmp", 1715, 768);
	_colmap = IMAGE->addImage("map1_col", "images/map/first_col2.bmp", 1715, 768, true);
	_map_front = _bwmap_front = nullptr;
	_startPt = Vec2(4.5, 539);
	_timelimit = 2000;
	_nextScene = "테스트맵2";
}

textMap::~textMap()
{
}

HRESULT textMap::init()
{
	if (!SOUND->isPlaySound("song_bunker_2")) {
		SOUND->play("song_bunker_2", 0.1f);
	}
	Cmap::init();
	_goal = COLLISION->addCollider(Vec2(1712, 237), Vec2(19, 221), COLLIDER_TYPE::POTAL, ZCOL1);
	PLAYER->setX(_startPt.x);
	PLAYER->setY(_startPt.y);

	prevx = PLAYER->getCollider()->getPos().x;
	prevy = PLAYER->getCollider()->getPos().y;
	prevRc = PLAYER->getCollider()->getRect();

	CAMERA->init(PLAYER->getX(), PLAYER->getY(), _map->getWidth(), _map->getHeight(), 0, 0, WINSIZEX / 2, WINSIZEY / 2,
		WINSIZEX, WINSIZEY);

	_door = new door;
	_door->addEnemy(441, 225);

	_grunt = new grunt(false, true, Vec2(714, 569-17), Vec2(1015-37, 265));
	_grunt->addEnemy(756, 265, FOWARD::RIGHT, Vec2(583, 1039));
	//_grunt->addEnemy(1500, 254, FOWARD::LEFT, Vec2(1040, 1500));
	_grunt->addEnemy(450, 265, FOWARD::LEFT, Vec2(0, 0));
	_grunt->addEnemy(200, 265, FOWARD::RIGHT, Vec2(0, 0));

	_timeCount = _timelimit;
	_isClear = false;
    return S_OK;
}

void textMap::release()
{
	COLLISION->erase(_goal);
	_door->release();
	_grunt->release();
	SAFE_DELETE(_door);
	SAFE_DELETE(_grunt);
}

void textMap::update()
{
	//타이머
	timeCheck();
	//충돌
	coltoMap();
	//적충돌
	coltoMapEnemy(_grunt);



	CheckClear();
	goalCol();
	_door->update();
	_grunt->update();
    //--------------------------------------------------
	PLAYER->update();
	ANIMATION->update();
	COLLISION->update();
	POINT lerp;
	lerp.x = CAMERA->getPivotX() * (1 - 0.08) + PLAYER->getCollider()->getPos().x * 0.08;
	lerp.y = CAMERA->getPivotY() * (1 - 0.08) + PLAYER->getY() * 0.08;
	CAMERA->movePivot(lerp.x, lerp.y);
	CAMERA->update();
	EFFECT->update();
}

void textMap::render()
{
	//맵
	ZORDER->ZorderRender(_map, ZFLOORMAP, 0, 0, 0);
	if(_isDebug) ZORDER->ZorderRender(_colmap, ZCOL1, 0, 0, 0);
	_door->render();
	_grunt->render();


	//--------------------------------------------------
	PLAYER->render();
	COLLISION->render();
	EFFECT->render();
}

void textMap::CheckClear()
{
	if (_grunt->checkEverybodyDie()) {
		if(!_isClear) SOUND->play("go", 0.1f);
		_isClear = true;
	}
}

