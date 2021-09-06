#include "framework.h"
#include "player.h"
#include "PlayerState.h"
#include "playerSlash.h"
player::player()
{
}

player::~player()
{
}

HRESULT player::init()
{
	_slash = new playerSlash;
	imageInit();

	_z = ZPLAYER;
	_x = WINSIZEX / 2;
	_y = WINSIZEY / 2;
	_img = IMAGE->findImage("player_ALL1");

	if (!_col) _col = COLLISION->addCollider({ _x + 62, _y + 40 },
		{ _img->getFrameWidth() * 1 / 5.f, _img->getFrameHeight() * 7 / 10.f },
		COLLIDER_TYPE::PLAYER_UNIT, ZCOL3);
	_ani = ANIMATION->addNoneKeyAnimation("player_ALL1", 0, 10, 11, false, true);
	
	_foward = FOWARD::RIGHT;
	stateInit();
	soundInit();

	reInit();

	return S_OK;
}

void player::release()
{
	_FSM->release();
	_slash->release();
	SAFE_DELETE(_slash);
	SAFE_DELETE(_FSM);
}

void player::update()
{
	if (!_isLaserDie) {
		_FSM->update();	//�̹��������� �������� - �̵�.
		_slash->update();

		setCollider();	//�ݹڽ��̵�.

		//�ܻ����
		makeSpectrum();
	}
	else {
		laserDie();	//�����������
	}
	//�����׽�Ʈ
	//if (INPUT->isOnceKeyDown('J')) _FSM->ChangeState(PLAYERSTATE::DOORBREAK);
	//if (INPUT->isOnceKeyDown('K')) _FSM->ChangeState(PLAYERSTATE::DEAD);
	//if (INPUT->isOnceKeyDown('R')) _FSM->ChangeState(PLAYERSTATE::HURTCOVER);



}

void player::render()
{
	_bottom = _col->getRect().bottom;
	//switch (MAIN->getMainState())
	//{
	//case MAINSTATE::INGAME:
	//	//�ִϷ��� - �����϶��� ����Ҷ�
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
	//	//�����ӷ��� - ���÷���(Ŭ�����Ĵٽú���) - ���
	//	ZORDER->ZorderFrameRender(IMAGE->findImage("player_bw_ALL1"), _z, _bottom, _x, _y, _frameX, _frameY);
	//	break;
	//case MAINSTATE::ROLLBACK:
	//	//�����ӷ��� - �ǰ��ⷣ��(����Ŀ����)
	//	ZORDER->ZorderFrameRender(_img, _z, _bottom, _x, _y, _frameX, _frameY);
	//	break;
	//case MAINSTATE::PAUSE:
	//	break;
	//case MAINSTATE::NONE:
	//	break;
	//}
	if (!_isLaserDie) {
		//�ִϷ��� - �����϶� ���İ�����
		if (MAIN->getIsSlow() && _alpha < 200) _alpha += 5;
		else if (!MAIN->getIsSlow() && _alpha > 0) _alpha -= 5;
		//����� �׿� �μ�
		ZORDER->ZorderAniRender(_img, _z, _bottom, _x, _y, _ani);
		if (_alpha > 0) ZORDER->ZorderAniAlphaRender(IMAGE->findImage("player_neon1_ALL1"), _z + 1, _bottom, _x, _y, _ani, _alpha);

		//���̺�����
		ZORDER->SaveAniRender(_img, IMAGE->findImage("player_bw_ALL1"), _z, _bottom, _x, _y, _ani);

		//�ܻ�
		for (_ispectrum = _spectrum.begin(); _ispectrum != _spectrum.end(); )
		{
			ZORDER->ZorderAlphaFrameRender(*_viglitch, ZGLITCH, _bottom, _ispectrum->pos.x, _ispectrum->pos.y,
				_ispectrum->frame.x, _ispectrum->frame.y, _ispectrum->alpha);
			++_viglitch;
			if (_viglitch == _vglitch.end()) _viglitch = _vglitch.begin();

			if (_ispectrum->alpha <= 80)
				_ispectrum = _spectrum.erase(_ispectrum);
			else {
				_ispectrum->alpha -= 15;
				++_ispectrum;
			}
		}
	}
	else {
		EFFECT->addParticle("����������", ZEFFECT2, _col->getPos().x+RND->getInt(30)-15, _y + _laserX, 10, RND->getFloatFromTo(PI_8, PI_8 * 7), 20, 5, false);
		EFFECT->addParticle("����������", ZEFFECT2, _col->getPos().x+RND->getInt(30)-15, _y + _laserX, 10, RND->getFloatFromTo(PI_8, PI_8 * 7), 20, 5, false);
		EFFECT->addParticle("����������", ZEFFECT2, _col->getPos().x+RND->getInt(30)-15, _y + _laserX, 10, RND->getFloatFromTo(PI_8, PI_8 * 7), 20, 5, false);
		ZORDER->ZorderFrameRender(IMAGE->findImage("player_laserDie"), _z, _bottom, _x, _y + _laserX, _ani->getFrameX(),
			_ani->getFrameY(), 0, _laserX, _img->getFrameWidth(), _img->getFrameHeight() - _laserX);
		ZORDER->SaveFrameRender(_img, IMAGE->findImage("player_bw_ALL1"), _z, _bottom, _x, _y + _laserX, _ani->getFrameX(),
			_ani->getFrameY(), 0, _laserX, _img->getFrameWidth(), _img->getFrameHeight() - _laserX);
	}

	/*�׽�Ʈ*/
	//TCHAR str[128];
	//_stprintf_s(str, "ani���� : %d", (int)_alpha);
	//ZORDER->UITextOut(str, ZUIFIRST, 300, 300, MINT);
	//_stprintf_s(str, "ani pos : %d, %d", _ani->getFramePos().x, _ani->getFramePos().y);
	//ZORDER->UITextOut(str, ZUIFIRST, 300, 300+20, MINT);

}

void player::reInit()
{
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

	_isLaserDie = false;
	_laserCount = 0;
	_laserX = 0;

	_colYello = false;
	_stillWall = false;

	_col->setCanCol(true);
	_foward = FOWARD::RIGHT;
}

void player::imageInit()
{
	//�⺻
	IMAGE->addFrameImage("player_ALL1", "images/player/player_all1.bmp", 744 * 2, 1440 * 2, 12, 30, true);
	IMAGE->addFrameImage("player_bw_ALL1", "images/player/player_bw_all.bmp", 744 * 2, 1440 * 2, 12, 30, true);
	IMAGE->addFrameImage("player_neon1_ALL1", "images/player/player_neon1_all.bmp", 744 * 2, 1440 * 2, 12, 30, true);
	
	//����
	IMAGE->addFrameImage("player_slash_left_bw", "images/effect/slash_left_bw.bmp", 630 * 2, 30 * 2, 6, 1, true);
	IMAGE->addFrameImage("player_slash_right_bw", "images/effect/slash_right_bw.bmp", 630 * 2, 30 * 2, 6, 1, true);
	EFFECT->addEffect("player_slash_left", "images/effect/slash_left.bmp", 630 * 2, 30 * 2, 105 * 2, 30 * 2, 20, 0.01666, 3, true, false, true, IMAGE->findImage("player_slash_left_bw"));
	EFFECT->addEffect("player_slash_right", "images/effect/slash_right.bmp", 630 * 2, 30 * 2, 105 * 2, 30 * 2, 20, 0.01666, 3, true, false, true, IMAGE->findImage("player_slash_right_bw"));

	//���������
	IMAGE->addFrameImage("player_laserDie", "images/player/player_all1_laserDie.bmp", 744 * 2, 1440 * 2, 12, 30, true);
	
	//�۸�ġ
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

	//��������
	IMAGE->addFrameImage("jumpcloud_bw", "images/effect/jumpcloud_bw.bmp", 128 * 2, 51 * 2, 4, 1, true);
	EFFECT->addEffect("jumpcloud", "images/effect/jumpcloud.bmp", 128 * 2, 51 * 2, 32 * 2, 51 * 2, 15, 0.01666, 1, false, false, true, IMAGE->findImage("jumpcloud_bw"));

	//�߸���
	IMAGE->addFrameImage("dust_left_bw", "images/effect/dust_left_bw.bmp", 133*2, 19*2, 7, 1, true);
	IMAGE->addFrameImage("dust_right_bw", "images/effect/dust_right_bw.bmp", 133*2, 19*2, 7, 1, true);
	EFFECT->addEffect("dust_left", "images/effect/dust_left.bmp", 133*2, 19 *2, 19 *2, 19 *2, 15, 0.01666, 4, false, false, true, IMAGE->findImage("dust_left_bw"));
	EFFECT->addEffect("dust_right", "images/effect/dust_right.bmp", 133 *2, 19 *2, 19 *2, 19 *2, 15, 0.01666, 4, false, false, true, IMAGE->findImage("dust_left_bw"));

	//��������
	IMAGE->addFrameImage("landcloud_bw", "images/effect/landcloud_bw.bmp", 350 * 2, 14 * 2, 7, 1, true);
	EFFECT->addEffect("landcloud", "images/effect/landcloud.bmp", 350 * 2, 14 * 2, 50 * 2, 14 * 2, 15, 0.01666, 1, false, false, true, IMAGE->findImage("landcloud_bw"));

	//������ϴ� ���� ����Ʈ
	EFFECT->addEffect("hitEffect", "images/effect/hiteffect.bmp", 395, 18, 395, 6, 4, 0.01666, 10, true, true, false);
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
	_FSM->AddState(new Player_Attack(_slash));
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

	//����Ʈ�� �����ܻ����� �ֱ�.
	if (_specOn) {
		_specCount += 1 * TIME->getGameTimeRate();
		if (_specCount > 1) {
			_specOn = false;
			_specCount = 0;
			tagGlitch temp;
			temp.alpha = 200;
			temp.frame = vector2(_ani->getFrameX(), _ani->getFrameY());
			temp.pos = vector2(_x, _y);
			_spectrum.push_back(temp);
		}
	}

}

void player::setCollider()
{
	_col->setPos({ _x + 62, _y + 64 });	//�ݹڽ��̵�.
	_rc = _col->getRect();
}

void player::laserDie()
{
	if (!_isLaserDie) return;
	_ani->stop();
	_col->setCanCol(false);
	++_laserX;
	if (_laserX == _img->getFrameHeight()) {
		MAIN->changeMainState(MAINSTATE::ROLLBACK);
	}
}
