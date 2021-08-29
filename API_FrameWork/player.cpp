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
	_z = ZUNIT;
	_x = WINSIZEX / 2;
	_y = WINSIZEY / 2;
	_img = IMAGE->findImage("player_ALL1");
	//_rc = RectMake(_x + _img->getFrameWidth() * 2 / 5, _y + _img->getFrameHeight() * 3 / 10, _img->getFrameWidth()* 1 / 5, _img->getFrameHeight() * 7 / 10);
	_col = COLLISION->addCollider({ _x + 62, _y + 40 },
		{ _img->getFrameWidth() * 1 / 5.f, _img->getFrameHeight() * 7 / 10.f },
		COLLIDER_TYPE::PLAYER_UNIT, ZCOL3);
	_ani = ANIMATION->addNoneKeyAnimation("player_ALL1", 0, 10, 11, false, true);
	_frameX = _frameY = 0;
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
	_col->setPos({ _x + 62, _y + 64 });

	//상태테스트
	if (INPUT->isOnceKeyDown('J')) _FSM->ChangeState(PLAYERSTATE::DOORBREAK);
	if (INPUT->isOnceKeyDown('K')) _FSM->ChangeState(PLAYERSTATE::DEAD);
	if (INPUT->isOnceKeyDown('R')) _FSM->ChangeState(PLAYERSTATE::HURTCOVER);
	if (INPUT->isOnceKeyDown('F')) _FSM->ChangeState(PLAYERSTATE::FLIP);
	if (INPUT->isOnceKeyDown('P')) _FSM->ChangeState(PLAYERSTATE::WALLSLIDE);
}

void player::render()
{
	_bottom = _col->getRect().bottom;
	switch (MAIN->getMainState())
	{
	case MAINSTATE::INGAME:
		//애니랜더
		ZORDER->ZorderAniRender(_img, _z, _bottom, _x, _y, _ani);
		break;
	case MAINSTATE::REPLAY:
		//프레임랜더 - 리플랜더(클리어후다시보기)
		ZORDER->ZorderFrameRender(_img, _z, _bottom, _x, _y, _frameX, _frameY);
		break;
	case MAINSTATE::ROLLBACK:
		//프레임랜더 - 되감기랜더(사망후역재생)
		ZORDER->ZorderFrameRender(_img, _z, _bottom, _x, _y, _frameX, _frameY);
		break;
	case MAINSTATE::PAUSE:
		break;
	case MAINSTATE::NONE:
		break;
	default:
		break;
	}

	TCHAR str[128];
	_stprintf_s(str, "ani정보 : %d, %d", _ani->getFrameX(), _ani->getFrameY());
	ZORDER->UITextOut(str, ZUIFIRST, 300, 300, MINT);
	_stprintf_s(str, "ani pos : %d, %d", _ani->getFramePos().x, _ani->getFramePos().y);
	ZORDER->UITextOut(str, ZUIFIRST, 300, 300+20, MINT);

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