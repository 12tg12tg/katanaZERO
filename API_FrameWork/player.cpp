#include "framework.h"
#include "player.h"
#include "PlayerState.h"
player::player()
{
}

player::~player()
{
}

HRESULT player::init()
{
	imageInit();
	_x = WINSIZEX / 2;
	_y = WINSIZEY / 2;
	_img = IMAGE->findImage("player_ALL1");
	_rc = RectMake(_x + _img->getFrameWidth() * 2 / 5, _y + _img->getFrameHeight() * 1 / 5, _img->getFrameWidth()* 1 / 5, _img->getFrameHeight() * 4 / 5);
	_ani = ANIMATION->addNoneKeyAnimation("player_ALL1", 0, 10, 11, false, true);
	_speed = 0.f;
	_maxSpeed = 9.f;
	_frameCount = 0;
	stateInit();

	return S_OK;
}

void player::release()
{
	SAFE_DELETE(_FSM);
}

void player::update()
{
	_FSM->update();	//이미지프레임 상태패턴 - 이동.

	//테스트
	if (INPUT->isOnceKeyDown('J')) _FSM->ChangeState(PLAYERSTATE::DOORBREAK);
	if (INPUT->isOnceKeyDown('K')) _FSM->ChangeState(PLAYERSTATE::DEAD);
	if (INPUT->isOnceKeyDown('R')) _FSM->ChangeState(PLAYERSTATE::HURTCOVER);
	if (INPUT->isOnceKeyDown('F')) _FSM->ChangeState(PLAYERSTATE::FLIP);
	if (INPUT->isOnceKeyDown('P')) _FSM->ChangeState(PLAYERSTATE::WALLSLIDE);
}

void player::render()
{
	if (_isDebug) ZORDER->ZorderRectangle(_rc, ZCOL3);
	ZORDER->ZorderAniRender(_img, ZUNIT, _rc.bottom, _x, _y, _ani);

	TCHAR str[128];
	_stprintf_s(str, "speed : %.1f", _speed);
	ZORDER->UITextOut(str, ZUIFIRST, 100, 0, RGB(0, 0, 0));
}

void player::imageInit()
{
	IMAGE->addFrameImage("player_ALL1", "images/player/player_all1.bmp", 744 * 2, 1440 * 2, 12, 30, true);
}

void player::stateInit()
{
	_FSM = new PlayerFSM;
	_FSM->AddState(new Player_Idle);
	_FSM->AddState(new Player_Run);
	_FSM->AddState(new Player_Roll);
	_FSM->AddState(new Player_Crouch);
	_FSM->AddState(new Player_Jump);
	_FSM->AddState(new Player_Fall);
	_FSM->AddState(new Player_Attack);
	_FSM->AddState(new Player_Doorbreak);
	_FSM->AddState(new Player_Dead);
	_FSM->AddState(new Player_HurtCover);
	_FSM->AddState(new Player_Flip);
	_FSM->AddState(new Player_WallSlide);


	_FSM->SetState(PLAYERSTATE::IDLE);
}