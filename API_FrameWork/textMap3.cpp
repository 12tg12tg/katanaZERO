#include "framework.h"
#include "textMap3.h"
#include "PlayerState.h"
textMap3::textMap3()
{
	_map = IMAGE->addImage("map3", "images/map/third_back.bmp", 1863, 1189);
	_bwmap = IMAGE->addImage("map3_bw", "images/map/third_back_bw.bmp", 1863, 1189);
	_colmap = IMAGE->addImage("map3_col", "images/map/third_col.bmp", 1863, 1189, true);
	_map_front = IMAGE->addImage("map3_front", "images/map/third_front.bmp", 88, 222, true);
	_bwmap_front = IMAGE->addImage("map3_front_bw", "images/map/third_front_bw.bmp", 1863, 1189, true);
	_addforslow = IMAGE->addImage("map3_addforslow", "images/map/third_back_addforslow.bmp", 60, 222, true);
	_startPt = Vec2(42, 974);
	_timelimit = 4000;

	_nextScene = "º¸½º¸Ê";
}

textMap3::~textMap3()
{
}

HRESULT textMap3::init()
{
	Cmap::init();
	_goal = COLLISION->addCollider(Vec2(1853, 265), Vec2(19, 221), COLLIDER_TYPE::POTAL, ZCOL1);

	PLAYER->setX(_startPt.x);
	PLAYER->setY(_startPt.y);

	prevx = PLAYER->getCollider()->getPos().x;
	prevy = PLAYER->getCollider()->getPos().y;
	prevRc = PLAYER->getCollider()->getRect();

	CAMERA->init(PLAYER->getX(), PLAYER->getY(), _map->getWidth(), _map->getHeight(), 0, 0, WINSIZEX / 2, WINSIZEY / 2,
		WINSIZEX, WINSIZEY);
	_timeCount = _timelimit;

	_fan = new fan;
	_fan->init(1331, 167);
	_laser = new laser;
	_laser->fire(898, 170, 491);

	_grunt = new grunt(true, false, Vec2(0, 0));
	_grunt->addEnemy(773, 560+19, FOWARD::LEFT, Vec2(0, 0));
	_grunt->addEnemy(550, 560+19, FOWARD::RIGHT, Vec2(0, 0));

	//_grunt->addEnemy(600, 560+19, FOWARD::RIGHT, Vec2(0, 0));
	//_grunt->addEnemy(800, 560+19, FOWARD::LEFT, Vec2(0, 0));
	return S_OK;
}

void textMap3::release()
{
	COLLISION->erase(_goal);
	_fan->release();
	_laser->release();
	_grunt->release();
	SAFE_DELETE(_fan);
	SAFE_DELETE(_laser);
	SAFE_DELETE(_grunt);
}

void textMap3::update()
{
	//Å¸ÀÌ¸Ó
	timeCheck();

	coltoMap();
	//ÀûÃæµ¹
	coltoMapEnemy(_grunt);



	_isClear = true;
	CheckClear();
	goalCol();
	checkPlayerDie();
	_laser->update();
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

void textMap3::render()
{
	//¸Ê
	ZORDER->ZorderRender(_addforslow, ZSLOWFADE, 1, 1318, 154);
	ZORDER->ZorderRender(_map, ZFLOORMAP, 0, 0, 0);
	ZORDER->ZorderRender(_map_front, ZABOVEMAP, 0, 1365, 154);

	if (_isDebug) ZORDER->ZorderRender(_colmap, ZCOLMAP, 0, 0, 0);

	//¼­Å§·¹ÀÌÅÍ
	_fan->render();
	_laser->render();
	_grunt->render();

	//--------------------------------------------------
	PLAYER->render();
	COLLISION->render();
	EFFECT->render();
}

void textMap3::checkPlayerDie()
{
	if (_fan->getCollider()->isColIng() && !_fan->canPass() && PLAYER->getState() != PLAYERSTATE::DEAD) {
		float deathAngle = _fan->getCollider()->getPos().AngleTo(PLAYER->getCollider()->getPos());
		PLAYER->getFSM()->SetDeath(deathAngle, 20);
	}
}
