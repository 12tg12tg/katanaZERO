#include "framework.h"
#include "player.h"

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
	_maxSpeed = 7.f;
	_state = PLAYERSTATE::IDLE;
	_oldstate = PLAYERSTATE::IDLE;
	_foward = FOWARD::RIGHT;
	_oldfoward = FOWARD::RIGHT;
	_frameCount = 0;
	//_isFowardChange = false;
	//_isStateChange = false;
	return S_OK;
}

void player::imageInit()
{
	IMAGE->addFrameImage("player_IDLE", "images/player/player_idle.bmp", 396 * PLAYERSIZEUP, 35 * PLAYERSIZEUP, 11, 1, true);
	IMAGE->addFrameImage("player_IDLE_to_RUN", "images/player/player_idle_to_Run.bmp", 176 * PLAYERSIZEUP, 32 * PLAYERSIZEUP, 4, 1, true);
	IMAGE->addFrameImage("player_RUN", "images/player/player_run.bmp", 440 * PLAYERSIZEUP, 32 * PLAYERSIZEUP, 10, 1, true);
	IMAGE->addFrameImage("player_RUN_to_IDLE", "images/player/player_run_to_idle.bmp", 260 * PLAYERSIZEUP, 36 * PLAYERSIZEUP, 5, 1, true);
	IMAGE->addFrameImage("player_JUMP", "images/player/player_jump.bmp", 128 * PLAYERSIZEUP, 42 * PLAYERSIZEUP, 4, 1, true);
	IMAGE->addFrameImage("player_ROLL", "images/player/player_roll.bmp", 336 * PLAYERSIZEUP, 33 * PLAYERSIZEUP, 7, 1, true);
	IMAGE->addFrameImage("player_PRECROUCH", "images/player/player_precrouch.bmp", 72 * PLAYERSIZEUP, 40 * PLAYERSIZEUP, 2, 1, true);
	IMAGE->addFrameImage("player_POSTCROUCH", "images/player/player_postcrouch.bmp", 72 * PLAYERSIZEUP, 40 * PLAYERSIZEUP, 2, 1, true);

	IMAGE->addFrameImage("player_ALL1", "images/player/player_all1.bmp", 572*2, 672*2, 11, 16, true);
}

void player::release()
{
}

void player::update()
{
	move();
	giveFrame();
}

void player::render()
{
	if (_isDebug) ZORDER->ZorderRectangle(_rc, ZCOL3);
	ZORDER->ZorderAniRender(_img, ZUNIT, _rc.bottom, _x, _y, _ani);

	TCHAR str[128];
	wsprintf(str, "state : %d", _oldstate);
	ZORDER->UITextOut(str, ZUIFIRST, 100, 0, RGB(0, 0, 0));
}

void player::move()
{
	//좌우
	if (INPUT->isStayKeyDown('A') && INPUT->isStayKeyDown('D')) {
		if (_oldstate != PLAYERSTATE::IDLE) {
			_oldstate = _state;
			_state = PLAYERSTATE::IDLE;
			_frameCount = 0;
		}
		_speed = 0.f;
	}
	else if (INPUT->isStayKeyDown('A') && _state != PLAYERSTATE::ROLL){
		if (_oldstate != PLAYERSTATE::RUN || _foward != FOWARD::LEFT) {
			_oldstate = _state;
			_state = PLAYERSTATE::RUN;
			_frameCount = 0;
		}
		_foward = FOWARD::LEFT;
		_speed += _maxSpeed / 20;
		if (_speed > _maxSpeed) _speed = _maxSpeed;
		_x -= _speed;
		/*상자*/
	}
	else if (INPUT->isStayKeyDown('D') && _state != PLAYERSTATE::ROLL) {
		if (_oldstate != PLAYERSTATE::RUN || _foward != FOWARD::RIGHT) {
			_oldstate = _state;
			_state = PLAYERSTATE::RUN;
			_frameCount = 0;
		}
		_foward = FOWARD::RIGHT;
		_speed += _maxSpeed / 20;
		if (_speed > _maxSpeed) _speed = _maxSpeed;
		_x += _speed;
		/*상자*/
	}
	//정지
	if (_state != PLAYERSTATE::ROLL && (INPUT->isOnceKeyUp('D') || INPUT->isOnceKeyUp('A'))){
		if (_oldstate != PLAYERSTATE::IDLE) {
			_oldstate = _state;
			_state = PLAYERSTATE::IDLE;
			_frameCount = 0;
		}
		_speed = 0.f;
	}

	//점프, 구르기
	if (INPUT->isStayKeyDown('W')) {
		
	}
	else if (INPUT->isStayKeyDown('S') && _state != PLAYERSTATE::ROLL) {
		if (_state == PLAYERSTATE::IDLE) {
			_oldstate = _state;
			_frameCount = 0;
			_state = PLAYERSTATE::CROUCH;
		}
		else if (_state == PLAYERSTATE::RUN) {
			_oldstate = _state;
			_frameCount = 0;
			_state = PLAYERSTATE::ROLL;
		}
	}
	//정지
	if (_state == PLAYERSTATE::CROUCH && INPUT->isOnceKeyUp('S')) {
		if (_oldstate != PLAYERSTATE::IDLE) {
			_oldstate = _state;
			_state = PLAYERSTATE::IDLE;
			_frameCount = 0;
		}
	}
}

void player::giveFrame()
{
	switch (_state)
	{
	case PLAYERSTATE::IDLE:
		if (_frameCount == 0) {
			switch (_oldstate)
			{
			case PLAYERSTATE::IDLE:
				if (_foward == FOWARD::RIGHT)
					ANIMATION->changeNonKeyAnimation(_ani, "player_ALL1", 0, 10, 11, false, true);
				else if (_foward == FOWARD::LEFT)
					ANIMATION->changeNonKeyAnimation(_ani, "player_ALL1", 98, 88, 11, false, true);
			case PLAYERSTATE::RUN:
				if (_foward == FOWARD::RIGHT)
					ANIMATION->changeNonKeyAnimation(_ani, "player_ALL1", 33, 37, 12, false, false);
				else if (_foward == FOWARD::LEFT)
					_ani = ANIMATION->addNoneKeyAnimation("player_ALL1", 131, 127, 12, false, false);
				break;
			case PLAYERSTATE::CROUCH:
			case PLAYERSTATE::ROLL:
				if (_foward == FOWARD::RIGHT)
					ANIMATION->changeNonKeyAnimation(_ani, "player_ALL1", 77, 78, 8, false, false);
				else if (_foward == FOWARD::LEFT)
					_ani = ANIMATION->addNoneKeyAnimation("player_ALL1", 175, 174, 8, false, false);
				break;
			case PLAYERSTATE::JUMP:
				break;
			case PLAYERSTATE::ATTACK:
				break;
			case PLAYERSTATE::DIE:
				break;
			}
			_oldstate = PLAYERSTATE::IDLE;
		}
		else if (!_ani->isPlay()) {
			if (_foward == FOWARD::RIGHT)
				ANIMATION->changeNonKeyAnimation(_ani, "player_ALL1", 0, 10, 11, false, true);
			else if (_foward == FOWARD::LEFT)
				ANIMATION->changeNonKeyAnimation(_ani, "player_ALL1", 98, 88, 11, false, true);
		}
		break;
	case PLAYERSTATE::RUN:
		if (_frameCount == 0) {
			if (_oldstate == PLAYERSTATE::IDLE) {
				_oldstate = PLAYERSTATE::RUN;
				if (_foward == FOWARD::RIGHT)
					ANIMATION->changeNonKeyAnimation(_ani, "player_ALL1", 11, 14, 12, false, false);
				else if (_foward == FOWARD::LEFT)
					ANIMATION->changeNonKeyAnimation(_ani, "player_ALL1", 109, 106, 12, false, false);
			}
			else {
				if (_foward == FOWARD::RIGHT)
					ANIMATION->changeNonKeyAnimation(_ani, "player_ALL1", 22, 31, 12, false, true);
				else if (_foward == FOWARD::LEFT)
					ANIMATION->changeNonKeyAnimation(_ani, "player_ALL1", 120, 111, 12, false, true);
			}
		}
		else if (!_ani->isPlay()) {
			if (_foward == FOWARD::RIGHT)
				ANIMATION->changeNonKeyAnimation(_ani, "player_ALL1", 22, 31, 12, false, true);
			else if (_foward == FOWARD::LEFT)
				ANIMATION->changeNonKeyAnimation(_ani, "player_ALL1", 120, 111, 12, false, true);
		}
		break;
	case PLAYERSTATE::CROUCH:
		if (_frameCount == 0) {
			_oldstate = PLAYERSTATE::CROUCH;
			if (_foward == FOWARD::RIGHT) {
				ANIMATION->changeNonKeyAnimation(_ani, "player_ALL1", 55, 56, 8, false, false);
			}
			else if (_foward == FOWARD::LEFT) {
				ANIMATION->changeNonKeyAnimation(_ani, "player_ALL1", 153, 152, 8, false, false);
			}
		}
		break;
	case PLAYERSTATE::ROLL:
		if (_frameCount == 0) {
			_oldstate = PLAYERSTATE::ROLL;
			if (_foward == FOWARD::RIGHT) {
				ANIMATION->changeNonKeyAnimation(_ani, "player_ALL1", 66, 72, 8, false, false);
			}
			else if (_foward == FOWARD::LEFT) {
				ANIMATION->changeNonKeyAnimation(_ani, "player_ALL1", 164, 158, 8, false, false);
			}
		}
		else if (!_ani->isPlay()) {
			_state = PLAYERSTATE::IDLE;
		}
		break;
	case PLAYERSTATE::JUMP:
		break;
	case PLAYERSTATE::WALK:
		break;
	case PLAYERSTATE::ATTACK:
		break;
	case PLAYERSTATE::DIE:
		break;
	}
	++_frameCount;
}

void player::stateUpdate()
{
	if (_speed > 0){
		_state = PLAYERSTATE::RUN;
	}
	else {
		_state = PLAYERSTATE::IDLE;
	}
}
