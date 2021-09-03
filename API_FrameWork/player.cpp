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

	if(!_col) _col = COLLISION->addCollider({ _x + 62, _y + 40 },
		{ _img->getFrameWidth() * 1 / 5.f, _img->getFrameHeight() * 7 / 10.f },
		COLLIDER_TYPE::PLAYER_UNIT, ZCOL3);
	_ani = ANIMATION->addNoneKeyAnimation("player_ALL1", 0, 10, 11, false, true);
	_alpha = 0;
	_speed = 0.f;
	_maxSpeed = 9.f;
	_frameCount = 0;

	_attDash = false;
	_viglitch = _vglitch.begin();
	_specCount = 0.f;
	_specOn = false;
	_idleCount = 30.1f;
	_isLand = true;
	_colBlack = false;

	_foward = FOWARD::RIGHT;

	stateInit();
	soundInit();
	return S_OK;
}

void player::release()
{
	_FSM->release();
	SAFE_DELETE(_FSM);
}

void player::update()
{
	_FSM->update();	//이미지프레임 상태패턴 - 이동.

	setCollider();	//콜박스이동.

	//상태테스트
	if (INPUT->isOnceKeyDown('J')) _FSM->ChangeState(PLAYERSTATE::DOORBREAK);
	if (INPUT->isOnceKeyDown('K')) _FSM->ChangeState(PLAYERSTATE::DEAD);
	if (INPUT->isOnceKeyDown('R')) _FSM->ChangeState(PLAYERSTATE::HURTCOVER);


	//잔상생성
	makeSpectrum();
}

void player::render()
{
	_bottom = _col->getRect().bottom;
	//switch (MAIN->getMainState())
	//{
	//case MAINSTATE::INGAME:
	//	//애니랜더 - 슬로일때와 평범할때
	//	if (MAIN->getIsSlow()) {
	//		ZORDER->ZorderAniRender(_img, _z - 0.1, _bottom, _x, _y, _ani);
	//		if (_alpha < 200) _alpha += 5;
	//		ZORDER->ZorderAniAlphaRender(IMAGE->findImage("player_neon1_ALL1"), _z, _bottom, _x, _y, _ani, _alpha);
	//	}
	//	else {
	//		if (_alpha > 0) _alpha -= 5;
	//		ZORDER->ZorderAniRender(_img, _z, _bottom, _x, _y, _ani);
	//		ZORDER->ZorderAniAlphaRender(IMAGE->findImage("player_neon1_ALL1"), _z, _bottom, _x, _y, _ani, _alpha);
	//	}
	//	break;
	//case MAINSTATE::REPLAY:
	//	//프레임랜더 - 리플랜더(클리어후다시보기) - 흑백
	//	ZORDER->ZorderFrameRender(IMAGE->findImage("player_bw_ALL1"), _z, _bottom, _x, _y, _frameX, _frameY);
	//	break;
	//case MAINSTATE::ROLLBACK:
	//	//프레임랜더 - 되감기랜더(사망후역재생)
	//	ZORDER->ZorderFrameRender(_img, _z, _bottom, _x, _y, _frameX, _frameY);
	//	break;
	//case MAINSTATE::PAUSE:
	//	break;
	//case MAINSTATE::NONE:
	//	break;
	//}
	
	//애니랜더 - 슬로일때 알파값증가
	if (MAIN->getIsSlow() && _alpha < 200) _alpha += 5;
	else if (!MAIN->getIsSlow() && _alpha > 0) _alpha -= 5;
	//평범과 네온 인쇄
	ZORDER->ZorderAniRender(_img, _z, _bottom, _x, _y, _ani);
	if(_alpha > 0) ZORDER->ZorderAniAlphaRender(IMAGE->findImage("player_neon1_ALL1"), _z+1, _bottom, _x, _y, _ani, _alpha);
	
	//세이브저장
	ZORDER->SaveAniRender(_img, IMAGE->findImage("player_bw_ALL1"), _z, _bottom, _x, _y, _ani);

	//잔상
	for (_ispectrum = _spectrum.begin(); _ispectrum != _spectrum.end(); )
	{
		ZORDER->ZorderAlphaFrameRender(*_viglitch, _z - 1, _bottom, _ispectrum->pos.x, _ispectrum->pos.y,
			_ispectrum->frame.x, _ispectrum->frame.y, _ispectrum->alpha);
		++_viglitch;
		if (_viglitch == _vglitch.end()) _viglitch = _vglitch.begin();

		if (_ispectrum->alpha <= 100)
			_ispectrum = _spectrum.erase(_ispectrum);
		else {
			_ispectrum->alpha -= 10;
			++_ispectrum;
		}
	}
	



	/*테스트*/
	//TCHAR str[128];
	//_stprintf_s(str, "ani정보 : %d", (int)_alpha);
	//ZORDER->UITextOut(str, ZUIFIRST, 300, 300, MINT);
	//_stprintf_s(str, "ani pos : %d, %d", _ani->getFramePos().x, _ani->getFramePos().y);
	//ZORDER->UITextOut(str, ZUIFIRST, 300, 300+20, MINT);

}

void player::imageInit()
{
	IMAGE->addFrameImage("player_ALL1", "images/player/player_all1.bmp", 744 * 2, 1440 * 2, 12, 30, true);
	IMAGE->addFrameImage("player_bw_ALL1", "images/player/player_bw_all.bmp", 744 * 2, 1440 * 2, 12, 30, true);
	IMAGE->addFrameImage("player_neon1_ALL1", "images/player/player_neon1_all.bmp", 744 * 2, 1440 * 2, 12, 30, true);
	
	IMAGE->addFrameImage("player_slash_left_bw", "images/effect/slash_left_bw.bmp", 630 * 2, 30 * 2, 6, 1, true);
	IMAGE->addFrameImage("player_slash_right_bw", "images/effect/slash_right_bw.bmp", 630 * 2, 30 * 2, 6, 1, true);
	EFFECT->addEffect("player_slash_left", "images/effect/slash_left.bmp", 630 * 2, 30 * 2, 105 * 2, 30 * 2, 20, 0.01666, 3, true, false, true, IMAGE->findImage("player_slash_left_bw"));
	EFFECT->addEffect("player_slash_right", "images/effect/slash_right.bmp", 630 * 2, 30 * 2, 105 * 2, 30 * 2, 20, 0.01666, 3, true, false, true, IMAGE->findImage("player_slash_right_bw"));

	IMAGE->addFrameImage("player_glitch_pink", "images/player/player_all1_glitch_pink2.bmp", 744 * 2, 1440 * 2, 12, 30, true);
	IMAGE->addFrameImage("player_glitch_red", "images/player/player_all1_glitch_red2.bmp", 744 * 2, 1440 * 2, 12, 30, true);
	IMAGE->addFrameImage("player_glitch_green", "images/player/player_all1_glitch_green2.bmp", 744 * 2, 1440 * 2, 12, 30, true);
	IMAGE->addFrameImage("player_glitch_blue", "images/player/player_all1_glitch_blue2.bmp", 744 * 2, 1440 * 2, 12, 30, true);
	_vglitch.push_back(IMAGE->findImage("player_glitch_pink"));
	_vglitch.push_back(IMAGE->findImage("player_glitch_red"));
	//_vglitch.push_back(IMAGE->findImage("player_glitch_pink"));
	_vglitch.push_back(IMAGE->findImage("player_glitch_green"));
	_vglitch.push_back(IMAGE->findImage("player_neon1_ALL1"));
	_vglitch.push_back(IMAGE->findImage("player_glitch_blue"));
	//_vglitch.push_back(IMAGE->findImage("player_neon1_ALL1"));
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

void player::soundInit()
{
	SOUND->addSound("slash1", "sound/player/slash1.wav", false, false);
	SOUND->addSound("slash2", "sound/player/slash2.wav", false, false);
	SOUND->addSound("slash3", "sound/player/slash3.wav", false, false);
	SOUND->addSound("jump", "sound/player/jump.wav", false, false);
	SOUND->addSound("land", "sound/player/land.wav", false, false);
	SOUND->addSound("dead", "sound/player/dead.wav", false, false);
	SOUND->addSound("doorbreak", "sound/player/doorbreak.wav", false, false);
	SOUND->addSound("grabwall", "sound/player/grabwall.wav", false, false);
	SOUND->addSound("roll", "sound/player/roll.wav", false, false);
	SOUND->addSound("run1", "sound/player/run1.wav", false, false);
	SOUND->addSound("run2", "sound/player/run2.wav", false, false);
	SOUND->addSound("run3", "sound/player/run3.wav", false, false);
	SOUND->addSound("run4", "sound/player/run4.wav", false, false);
	SOUND->addSound("walljump", "sound/player/walljump1.wav", false, false);
}

void player::makeSpectrum()
{
	if (_state != PLAYERSTATE::IDLE) _idleCount = 0;
	if (!MAIN->getIsSlow()) {
		switch (_state)
		{
		case PLAYERSTATE::IDLE:
			_idleCount += 1 * TIME->getGameTimeRate();
			if (!_specOn && _idleCount < 30) {
				_specOn = true;
			}
			break;
		case PLAYERSTATE::ROLL:
		case PLAYERSTATE::ATTACK:
		case PLAYERSTATE::FLIP:
		case PLAYERSTATE::RUN:
		case PLAYERSTATE::FALL:
		case PLAYERSTATE::JUMP:
			_specOn = true;
			break;
		case PLAYERSTATE::DOORBREAK:
		case PLAYERSTATE::HURTCOVER:
		case PLAYERSTATE::WALLSLIDE:
		case PLAYERSTATE::DEAD:
			break;
		}
	}
	else {
		_specOn = true;
	}

	//리스트에 순간잔상정보 넣기.
	if (_specOn) {
		_specCount += 1 * TIME->getGameTimeRate();
		if (_specCount > 1) {
			_specOn = false;
			_specCount = 0;
			tagGlitch temp;
			temp.alpha = 180;
			temp.frame = vector2(_ani->getFrameX(), _ani->getFrameY());
			temp.pos = vector2(_x, _y);
			_spectrum.push_back(temp);
		}
	}

}

void player::setCollider()
{
	_col->setPos({ _x + 62, _y + 64 });	//콜박스이동.
}
