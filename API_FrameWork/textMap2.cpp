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
	_timelimit = 2000;

	_butRgn = RectMake(1634, 364, 141, 129);
	IMAGE->addFrameImage("space", "images/ui/space.bmp", 60*2, 14*2, 2, 1, true);
	int arr[] = { 0, 1, 1 };
	_butAni = ANIMATION->addNoneKeyAnimation("space", arr, sizeof(arr)/sizeof(int), 2, true);
}

textMap2::~textMap2()
{
}

HRESULT textMap2::init()
{
	Cmap::init();
	_goal = COLLISION->addCollider(Vec2(2143, 385), Vec2(19, 221), COLLIDER_TYPE::POTAL, ZCOL1);

	PLAYER->setX(_startPt.x);
	PLAYER->setY(_startPt.y);

	prevx = PLAYER->getCollider()->getPos().x;
	prevy = PLAYER->getCollider()->getPos().y;
	prevRc = PLAYER->getCollider()->getRect();
	_canTurnOnOff = false;

	CAMERA->init(PLAYER->getX(), PLAYER->getY(), _map->getWidth(), _map->getHeight(), 0, 0, WINSIZEX / 2, WINSIZEY / 2,
		WINSIZEX, WINSIZEY);
	_timeCount = _timelimit;

	_laser = new laser;
	_laser->fire(326, 290, 206);
	_laser->fire(754, 290, 206);
	_laser->fire(812, 290, 206);
	_laser->fire(864, 290, 206);
	_laser->fire(1292, 290, 206);
	_laser->fire(1845, 290, 206);
	_laser->fire(1909, 290, 206);
	_laser->fire(1972, 290, 206);
	_laser->fire(2035, 290, 206);
	_laser->fire(2099, 290, 206);

    return S_OK;
}

void textMap2::release()
{
	_laser->release();
	SAFE_DELETE(_laser);
}

void textMap2::update()
{
	//Å¸ÀÌ¸Ó
	timeCheck();

	coltoMap();
	_isClear = true;
	CheckClear();
	goalCol();
	_laser->update();

	RECT temp;
	if (IntersectRect(&temp, &PLAYER->getCollider()->getRect(), &_butRgn)) {
		_canTurnOnOff = true;
		if (INPUT->isOnceKeyDown(VK_SPACE)) {
			_laser->laserOnOff();
		}
	}
	else {
		_canTurnOnOff = false;
	}

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

void textMap2::render()
{
	//¸Ê
	ZORDER->ZorderRender(_map, ZFLOORMAP, 0, 0, 0);
	if (_isDebug) ZORDER->ZorderRender(_colmap, ZCOL1, 0, 0, 0);
	_laser->render();
	if (_canTurnOnOff && _laser->getlaserOn()) {
		int dis = abs(UTIL::getDistance(PLAYER->getCollider()->getPos().x, PLAYER->getCollider()->getPos().y,
			RecCenX(_butRgn), RecCenY(_butRgn)));
		int alpha;
		if (dis < 30) alpha = 255;
		else if (dis > 100) alpha = 0;
		else alpha = 255 * (1 - (dis - 30) / 70.0f);
		ZORDER->ZorderAniAlphaRender(IMAGE->findImage("space"), ZMAXLAYER, _butRgn.bottom, RecCenX(_butRgn) - 25, _butRgn.top - 70, _butAni, alpha);
	}
	//--------------------------------------------------
	PLAYER->render();
	COLLISION->render();
	EFFECT->render();
}