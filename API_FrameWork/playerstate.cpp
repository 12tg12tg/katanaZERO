#include "framework.h"
#include "PlayerState.h"

//====================================================
void PlayerState::jump()
{
	if (INPUT->isOnceKeyDown('W')) {
		m_pFSM->ChangeState(PLAYERSTATE::JUMP);
	}
}

void PlayerState::attack()
{
}
//====================================================
PlayerFSM::PlayerFSM(){}

PlayerFSM::~PlayerFSM(){}

void PlayerFSM::update()
{
	m_pCurState->update();
}

void PlayerFSM::AddState(PlayerState* _pState)
{
	_pState->SetFSM(this);
	m_vecState.push_back(_pState);
}

void PlayerFSM::ChangeState(PLAYERSTATE _eType)
{
	m_pPreState = m_pCurState;
	m_pCurState->release();

	for (UINT i = 0; i < m_vecState.size(); ++i)
	{
		if (_eType == m_vecState[i]->getThisState())
		{
			m_pCurState = m_vecState[i];
			m_pCurState->init();
			return;
		}
	}
}

void PlayerFSM::SetState(PLAYERSTATE _eType)
{
	for (UINT i = 0; i < m_vecState.size(); ++i)
	{
		if (_eType == m_vecState[i]->getThisState())
		{
			m_pCurState = m_vecState[i];
			m_pPreState = m_vecState[i];
			m_pCurState->init();
			return;
		}
	}
}
//====================================================
//////////////////////////////////////////////////////
//////////////////////////////////////////////////////
//////////////////////////////////////////////////////
//////////////////////////////////////////////////////
//////////////////////////////////////////////////////
//////////////////////////////////////////////////////
//////////////////////////////////////////////////////
//////////////////////////////////////////////////////
//////////////////////////////////////////////////////
//////////////////////////////////////////////////////
//====================================================

Player_Idle::Player_Idle()
{
	m_eState = PLAYERSTATE::IDLE;
	PLAYER->setState(PLAYERSTATE::IDLE);
}

Player_Idle::~Player_Idle()
{
}

void Player_Idle::init()
{
	_accel = 0;
	switch (m_pFSM->getPreState()->getThisState())
	{
	case PLAYERSTATE::IDLE:
		if (PLAYER->getFoward() == FOWARD::RIGHT)
			ANIMATION->changeNonKeyAnimation(PLAYER->getAni(), "player_ALL1", 0, 10, 11, false, true);
		else if (PLAYER->getFoward() == FOWARD::LEFT)
			ANIMATION->changeNonKeyAnimation(PLAYER->getAni(), "player_ALL1", 106, 96, 11, false, true);
		break;
	case PLAYERSTATE::CROUCH:
	case PLAYERSTATE::ROLL:
		if (PLAYER->getFoward() == FOWARD::RIGHT)
			ANIMATION->changeNonKeyAnimation(PLAYER->getAni(), "player_ALL1", 84, 85, 10, false, false);
		else if (PLAYER->getFoward() == FOWARD::LEFT)
			ANIMATION->changeNonKeyAnimation(PLAYER->getAni(), "player_ALL1", 190, 189, 10, false, false);
		break;
	case PLAYERSTATE::RUN:
		if (PLAYER->getFoward() == FOWARD::RIGHT)
			ANIMATION->changeNonKeyAnimation(PLAYER->getAni(), "player_ALL1", 36, 40, 12, false, false);
		else if (PLAYER->getFoward() == FOWARD::LEFT)
			ANIMATION->changeNonKeyAnimation(PLAYER->getAni(), "player_ALL1", 142, 138, 12, false, false);
		break;
	case PLAYERSTATE::JUMP:
	case PLAYERSTATE::ATTACK:
	case PLAYERSTATE::FALL:
	case PLAYERSTATE::DOORBREAK:
	case PLAYERSTATE::HURTCOVER:
	case PLAYERSTATE::FLIP:
	case PLAYERSTATE::WALLSLIDE:
	case PLAYERSTATE::DEAD:
		if (PLAYER->getFoward() == FOWARD::RIGHT)
			ANIMATION->changeNonKeyAnimation(PLAYER->getAni(), "player_ALL1", 0, 10, 11, false, true);
		else if (PLAYER->getFoward() == FOWARD::LEFT)
			ANIMATION->changeNonKeyAnimation(PLAYER->getAni(), "player_ALL1", 106, 96, 11, false, true);
		break;
	}
}

void Player_Idle::update()
{
	//�ִϸ��̼����
	if (!PLAYER->getAni()->isPlay()) {
		if (PLAYER->getFoward() == FOWARD::RIGHT)
			ANIMATION->changeNonKeyAnimation(PLAYER->getAni(), "player_ALL1", 0, 10, 11, false, true);
		else if (PLAYER->getFoward() == FOWARD::LEFT)
			ANIMATION->changeNonKeyAnimation(PLAYER->getAni(), "player_ALL1", 106, 96, 11, false, true);
	}
	//���� ���¿����� �ణ�̵�
	float speed, maxspeed;
	speed = PLAYER->getSpeed();
	maxspeed = PLAYER->getMaxSpeed();
	if (speed > 0 && m_pFSM->getPreState()->getThisState() == PLAYERSTATE::RUN){
		if (PLAYER->getFoward() == FOWARD::RIGHT)
			PLAYER->setX(PLAYER->getX() + PLAYER->getSpeed() * TIME->getGameTimeRate());
		else if (PLAYER->getFoward() == FOWARD::LEFT)
			PLAYER->setX(PLAYER->getX() - PLAYER->getSpeed() * TIME->getGameTimeRate());
		_accel += maxspeed / 80;
		speed -= _accel;
		if (speed < 0) speed = 0.f;
		PLAYER->setSpeed(speed);
	}
	if (speed > 0 && m_pFSM->getPreState()->getThisState() == PLAYERSTATE::ROLL) {
		if (PLAYER->getFoward() == FOWARD::RIGHT)
			PLAYER->setX(PLAYER->getX() + PLAYER->getSpeed() * TIME->getGameTimeRate());
		else if (PLAYER->getFoward() == FOWARD::LEFT)
			PLAYER->setX(PLAYER->getX() - PLAYER->getSpeed() * TIME->getGameTimeRate());
		_accel += maxspeed / 20;
		speed -= _accel;
		if (speed < 0) speed = 0.f;
		PLAYER->setSpeed(speed);
	}
	//Ű�Է�
	//�¿�
	if (INPUT->isStayKeyDown('A') && INPUT->isStayKeyDown('D')) {
		PLAYER->setSpeed(0.f);
	}
	else if (INPUT->isStayKeyDown('A')) {
		PLAYER->setFoward(FOWARD::LEFT);
		m_pFSM->ChangeState(PLAYERSTATE::RUN);
	}
	else if (INPUT->isStayKeyDown('D')) {
		PLAYER->setFoward(FOWARD::RIGHT);
		m_pFSM->ChangeState(PLAYERSTATE::RUN);
	}
	//�Ʒ�
	if (INPUT->isStayKeyDown('S')) {
		m_pFSM->ChangeState(PLAYERSTATE::CROUCH);
	}
	//����
	this->jump();
}

void Player_Idle::release()
{
}
//====================================================

Player_Run::Player_Run()
{
	m_eState = PLAYERSTATE::RUN;
	PLAYER->setState(PLAYERSTATE::RUN);
}

Player_Run::~Player_Run()
{
}

void Player_Run::init()
{
	//�ӵ�����
	if (m_pFSM->getPreState()->getThisState() == PLAYERSTATE::ROLL) {
	}
	else {
		PLAYER->setSpeed(0.f);
	}
	//���ӵ��ʱ�ȭ
	_accel = 0.f;
	//���⼳��
	if (INPUT->isStayKeyDown('A')) PLAYER->setFoward(FOWARD::LEFT);
	if (INPUT->isStayKeyDown('D')) PLAYER->setFoward(FOWARD::RIGHT);
	//�ִϸ��̼� �ο�
	switch (m_pFSM->getPreState()->getThisState())
	{
	case PLAYERSTATE::IDLE:
		if (PLAYER->getFoward() == FOWARD::RIGHT)
			ANIMATION->changeNonKeyAnimation(PLAYER->getAni(), "player_ALL1", 12, 15, 12, false, false);
		else if (PLAYER->getFoward() == FOWARD::LEFT)
			ANIMATION->changeNonKeyAnimation(PLAYER->getAni(), "player_ALL1", 118, 115, 12, false, false);
		break;
	case PLAYERSTATE::CROUCH:
	case PLAYERSTATE::ROLL:
	case PLAYERSTATE::RUN:
	case PLAYERSTATE::JUMP:
	case PLAYERSTATE::ATTACK:
	case PLAYERSTATE::FALL:
	case PLAYERSTATE::DOORBREAK:
	case PLAYERSTATE::HURTCOVER:
	case PLAYERSTATE::FLIP:
	case PLAYERSTATE::WALLSLIDE:
	case PLAYERSTATE::DEAD:
		if (PLAYER->getFoward() == FOWARD::RIGHT)
			ANIMATION->changeNonKeyAnimation(PLAYER->getAni(), "player_ALL1", 24, 33, 14, false, true);
		else if (PLAYER->getFoward() == FOWARD::LEFT)
			ANIMATION->changeNonKeyAnimation(PLAYER->getAni(), "player_ALL1", 130, 121, 14, false, true);
		break;
	}
}

void Player_Run::update()
{
	//�ִ����
	if (!PLAYER->getAni()->isPlay()) {
		if (PLAYER->getFoward() == FOWARD::RIGHT)
			ANIMATION->changeNonKeyAnimation(PLAYER->getAni(), "player_ALL1", 24, 33, 14, false, true);
		else if (PLAYER->getFoward() == FOWARD::LEFT)
			ANIMATION->changeNonKeyAnimation(PLAYER->getAni(), "player_ALL1", 130, 121, 14, false, true);
	}
	//�̵�
	float speed, maxspeed;
	speed = PLAYER->getSpeed();
	maxspeed = PLAYER->getMaxSpeed();

	if (PLAYER->getFoward() == FOWARD::RIGHT)
		PLAYER->setX(PLAYER->getX() + PLAYER->getSpeed() * TIME->getGameTimeRate());
	else if (PLAYER->getFoward() == FOWARD::LEFT)
		PLAYER->setX(PLAYER->getX() - PLAYER->getSpeed() * TIME->getGameTimeRate());
	_accel += maxspeed / 80;
	speed += _accel;
	if (speed > maxspeed) speed = maxspeed;
	PLAYER->setSpeed(speed);

	//Ű�Է�
	if (INPUT->isStayKeyDown('A') && INPUT->isStayKeyDown('D')) {
		m_pFSM->ChangeState(PLAYERSTATE::IDLE);
	}
	if (INPUT->isOnceKeyUp('D') || INPUT->isOnceKeyUp('A')) {
		m_pFSM->ChangeState(PLAYERSTATE::IDLE);
	}
	if (INPUT->isOnceKeyDown('S')){
		m_pFSM->ChangeState(PLAYERSTATE::ROLL);
	}
	//����
	this->jump();
}

void Player_Run::release()
{
}
//====================================================

Player_Roll::Player_Roll()
{
	m_eState = PLAYERSTATE::ROLL;
	PLAYER->setState(PLAYERSTATE::ROLL);
}

Player_Roll::~Player_Roll()
{
}

void Player_Roll::init()
{
	PLAYER->setSpeed(12.f);
	if (PLAYER->getFoward() == FOWARD::RIGHT) {
		ANIMATION->changeNonKeyAnimation(PLAYER->getAni(), "player_ALL1", 72, 78, 16, false, false);
	}
	else if (PLAYER->getFoward() == FOWARD::LEFT) {
		ANIMATION->changeNonKeyAnimation(PLAYER->getAni(), "player_ALL1", 178, 172, 16, false, false);
	}
}

void Player_Roll::update()
{
	//�̵�
	if (PLAYER->getFoward() == FOWARD::RIGHT)
		PLAYER->setX(PLAYER->getX() + PLAYER->getSpeed() * TIME->getGameTimeRate());
	else if (PLAYER->getFoward() == FOWARD::LEFT)
		PLAYER->setX(PLAYER->getX() - PLAYER->getSpeed() * TIME->getGameTimeRate());

	//������ ������
	if (!PLAYER->getAni()->isPlay()) {
		if (INPUT->isStayKeyDown('D')|| INPUT->isStayKeyDown('A')) {
			m_pFSM->ChangeState(PLAYERSTATE::RUN);
		}
		else if(INPUT->isStayKeyDown('S')){
			m_pFSM->ChangeState(PLAYERSTATE::CROUCH);
		}
		else {
			m_pFSM->ChangeState(PLAYERSTATE::IDLE);
		}
	}
	//����
	this->jump();
}

void Player_Roll::release()
{
}

//====================================================

Player_Crouch::Player_Crouch()
{
	m_eState = PLAYERSTATE::CROUCH;
	PLAYER->setState(PLAYERSTATE::CROUCH);
}

Player_Crouch::~Player_Crouch()
{
}

void Player_Crouch::init()
{
	if (m_pFSM->getPreState()->getThisState() != PLAYERSTATE::ROLL) {
		if (PLAYER->getFoward() == FOWARD::RIGHT) {
			ANIMATION->changeNonKeyAnimation(PLAYER->getAni(), "player_ALL1", 60, 61, 8, false, false);
		}
		else if (PLAYER->getFoward() == FOWARD::LEFT) {
			ANIMATION->changeNonKeyAnimation(PLAYER->getAni(), "player_ALL1", 166, 165, 8, false, false);
		}
	}
	else {
		//��������¿��ٸ� �ٷ� �ɴ� ������.
		if (PLAYER->getFoward() == FOWARD::RIGHT) {
			int arr[] = { 61 };
			ANIMATION->changeNonKeyAnimation(PLAYER->getAni(), "player_ALL1", arr, sizeof(arr)/sizeof(int), 8, false);
		}
		else if (PLAYER->getFoward() == FOWARD::LEFT) {
			int arr[] = { 165 };
			ANIMATION->changeNonKeyAnimation(PLAYER->getAni(), "player_ALL1", arr, sizeof(arr)/sizeof(int), 8, false);
		}
	}
}

void Player_Crouch::update()
{
	float speed = PLAYER->getSpeed();
	float maxspeed = PLAYER->getMaxSpeed();
	//ROLL���� ������� �ܿ�����
	if (speed > 0 && m_pFSM->getPreState()->getThisState() == PLAYERSTATE::ROLL) {
		if (PLAYER->getFoward() == FOWARD::RIGHT)
			PLAYER->setX(PLAYER->getX() + PLAYER->getSpeed() * TIME->getGameTimeRate());
		else if (PLAYER->getFoward() == FOWARD::LEFT)
			PLAYER->setX(PLAYER->getX() - PLAYER->getSpeed() * TIME->getGameTimeRate());
		_accel += maxspeed / 20;
		speed -= _accel;
		if (speed < 0) speed = 0.f;
		PLAYER->setSpeed(speed);
	}
	//Ű�Է�
	if (INPUT->isOnceKeyUp('S')) {
		m_pFSM->ChangeState(PLAYERSTATE::IDLE);
	}
	if (INPUT->isOnceKeyDown('A')) {
		PLAYER->setFoward(FOWARD::LEFT);
		m_pFSM->ChangeState(PLAYERSTATE::ROLL);
	}
	if (INPUT->isOnceKeyDown('D')) {
		PLAYER->setFoward(FOWARD::RIGHT);
		m_pFSM->ChangeState(PLAYERSTATE::ROLL);
	}
	//����
	this->jump();
}

void Player_Crouch::release()
{
}

//====================================================

Player_Jump::Player_Jump()
{
	m_eState = PLAYERSTATE::JUMP;
	PLAYER->setState(PLAYERSTATE::JUMP);
	_jumpPower = 60;
	_maxJumpPower = 120;
	_maxDashPower = 9;
}

Player_Jump::~Player_Jump()
{
}

void Player_Jump::init()
{
	if (m_pFSM->getPreState()->getThisState() == PLAYERSTATE::RUN) {
		_accelB = PLAYER->getSpeed()/2;
	}
	else {
		_accelB = 0;
	}
	_count = 0;
	_accelA = 70;
	_graphX = 0;
	_startX = PLAYER->getX();
	_startY = PLAYER->getY();
	if (PLAYER->getFoward() == FOWARD::RIGHT) {
		ANIMATION->changeNonKeyAnimation(PLAYER->getAni(), "player_ALL1", 48, 51, 12, false, true);
	}
	else if (PLAYER->getFoward() == FOWARD::LEFT) {
		ANIMATION->changeNonKeyAnimation(PLAYER->getAni(), "player_ALL1", 154, 151, 12, false, true);
	}
}


void Player_Jump::update()
{
	//�¿� �Է� - ������ȯ
	if (INPUT->isStayKeyDown('A') && INPUT->isStayKeyDown('D')) {
	}
	else if (PLAYER->getFoward() == FOWARD::RIGHT && INPUT->isOnceKeyDown('A')) {
		_accelB = 0.f;
		PLAYER->setFoward(FOWARD::LEFT);
		ANIMATION->changeNonKeyAnimation(PLAYER->getAni(), "player_ALL1", 154, 151, 12, false, true);
	}
	else if (PLAYER->getFoward() == FOWARD::LEFT && INPUT->isOnceKeyDown('D')) {
		_accelB = 0.f;
		PLAYER->setFoward(FOWARD::RIGHT);
		ANIMATION->changeNonKeyAnimation(PLAYER->getAni(), "player_ALL1", 48, 51, 12, false, true);
	}

	//���������� - ����
	if (INPUT->isStayKeyDown('W')) {
		_accelA += 4;
		if (_accelA > _maxJumpPower)
			_accelA = _maxJumpPower;
	}
	//�뽬������ - ����
	if (PLAYER->getFoward() == FOWARD::RIGHT && INPUT->isStayKeyDown('D')) {
		_accelB += 1;
	}
	if (PLAYER->getFoward() == FOWARD::LEFT && INPUT->isStayKeyDown('A')) {
		_accelB += 1;
	}
	if (_accelB > _maxDashPower) _accelB = _maxDashPower;
	

	//���� - Y����, X����
	PLAYER->setY(_startY - sinf(_graphX) * _accelA);
	_graphX += PI / 32 * TIME->getGameTimeRate();
	if(PLAYER->getFoward() == FOWARD::RIGHT)
		PLAYER->setX(PLAYER->getX() + _accelB * TIME->getGameTimeRate());
	if (PLAYER->getFoward() == FOWARD::LEFT)
		PLAYER->setX(PLAYER->getX() - _accelB * TIME->getGameTimeRate());
	
	//����Ż������ - sin 90������ ���Ұų� �������΋H������(����)
	if (_count > 16) {
		PLAYER->setSpeed(_accelB);
		m_pFSM->ChangeState(PLAYERSTATE::FALL);
	}

	_count += 1* TIME->getGameTimeRate();
}

void Player_Jump::release()
{
}

//====================================================

Player_Fall::Player_Fall()
{
	m_eState = PLAYERSTATE::FALL;
	PLAYER->setState(PLAYERSTATE::FALL);
	_gravity = 0.f;
	_maxGravity = 10.f;
	_maxDashPower = 9.f;
}

Player_Fall::~Player_Fall()
{
}

void Player_Fall::init()
{
	_count = 0;
	_onGravity = true;
	switch (m_pFSM->getPreState()->getThisState())
	{
	case PLAYERSTATE::IDLE:
	case PLAYERSTATE::RUN:
	case PLAYERSTATE::CROUCH:
	case PLAYERSTATE::ROLL:
	case PLAYERSTATE::JUMP:
	case PLAYERSTATE::FALL:
	case PLAYERSTATE::ATTACK:
	case PLAYERSTATE::DOORBREAK:
	case PLAYERSTATE::HURTCOVER:
	case PLAYERSTATE::WALLSLIDE:
	case PLAYERSTATE::DEAD:
		_accelB = PLAYER->getSpeed();
		_gravity = 0.f;
		break;
	case PLAYERSTATE::FLIP:
		_accelB = 5;
		_gravity = 6.f;
		break;
	}
	//������
	if (PLAYER->getFoward() == FOWARD::RIGHT) {
		ANIMATION->changeNonKeyAnimation(PLAYER->getAni(), "player_ALL1", 192, 195, 12, false, true);
	}
	else if (PLAYER->getFoward() == FOWARD::LEFT) {
		ANIMATION->changeNonKeyAnimation(PLAYER->getAni(), "player_ALL1", 287, 284, 12, false, true);
	}
}

void Player_Fall::update()
{
	//�¿� �Է� - ������ȯ
	if (INPUT->isStayKeyDown('A') && INPUT->isStayKeyDown('D')) {
	}
	else if (PLAYER->getFoward() == FOWARD::RIGHT && INPUT->isOnceKeyDown('A')) {
		_accelB = 0.f;
		PLAYER->setFoward(FOWARD::LEFT);
		ANIMATION->changeNonKeyAnimation(PLAYER->getAni(), "player_ALL1", 287, 284, 12, false, true);
	}
	else if (PLAYER->getFoward() == FOWARD::LEFT && INPUT->isOnceKeyDown('D')) {
		_accelB = 0.f;
		PLAYER->setFoward(FOWARD::RIGHT);
		ANIMATION->changeNonKeyAnimation(PLAYER->getAni(), "player_ALL1", 192, 195, 12, false, true);
	}

	//�뽬������ - ����
	if (PLAYER->getFoward() == FOWARD::RIGHT && INPUT->isStayKeyDown('D')) {
		_accelB += 1;
	}
	if (PLAYER->getFoward() == FOWARD::LEFT && INPUT->isStayKeyDown('A')) {
		_accelB += 1;
	}
	if (_accelB > _maxDashPower) _accelB = _maxDashPower;

	//��������
	if (INPUT->isOnceKeyDown('S')) {
		_onGravity = false;
		_gravity = 20;
	}

	//Y����
	if (_onGravity) {
		++_count;
		if (_count % 5 == 0) {
			_gravity += 3.f;
			if (_gravity > _maxGravity) _gravity = _maxGravity;
		}
	}
	PLAYER->setY(PLAYER->getY() + _gravity * TIME->getGameTimeRate());

	//X����
	if (PLAYER->getFoward() == FOWARD::RIGHT)
		PLAYER->setX(PLAYER->getX() + _accelB * TIME->getGameTimeRate());
	if (PLAYER->getFoward() == FOWARD::LEFT)
		PLAYER->setX(PLAYER->getX() - _accelB * TIME->getGameTimeRate());

	//�������� -  �����浹bool ������ ����/*����*/ 
	if(PLAYER->getY() > WINSIZEY/2)
		m_pFSM->ChangeState(PLAYERSTATE::IDLE);

}

void Player_Fall::release()
{
}

//====================================================

Player_Attack::Player_Attack()
{
	m_eState = PLAYERSTATE::ATTACK;
	PLAYER->setState(PLAYERSTATE::ATTACK);
}

Player_Attack::~Player_Attack()
{
}

void Player_Attack::init()
{

}

void Player_Attack::update()
{
}

void Player_Attack::release()
{
}

//====================================================

Player_Doorbreak::Player_Doorbreak()
{
	m_eState = PLAYERSTATE::DOORBREAK;
	PLAYER->setState(PLAYERSTATE::DOORBREAK);
}

Player_Doorbreak::~Player_Doorbreak()
{
}

void Player_Doorbreak::init()
{
	//������
	if (PLAYER->getFoward() == FOWARD::RIGHT) {
		ANIMATION->changeNonKeyAnimation(PLAYER->getAni(), "player_ALL1", 216, 225, 15, false, false);
	}
	else if (PLAYER->getFoward() == FOWARD::LEFT) {
		ANIMATION->changeNonKeyAnimation(PLAYER->getAni(), "player_ALL1", 311, 302, 15, false, false);
	}
}

void Player_Doorbreak::update()
{
	if (!PLAYER->getAni()->isPlay())
		m_pFSM->ChangeState(PLAYERSTATE::IDLE);
}

void Player_Doorbreak::release()
{
}

//====================================================

Player_Dead::Player_Dead()
{
	m_eState = PLAYERSTATE::DEAD;
	PLAYER->setState(PLAYERSTATE::DEAD);
}

Player_Dead::~Player_Dead()
{
}

void Player_Dead::init()
{
	//������
	if (PLAYER->getFoward() == FOWARD::RIGHT) {
		ANIMATION->changeNonKeyAnimation(PLAYER->getAni(), "player_ALL1", 228, 236, 12, false, false);
	}
	else if (PLAYER->getFoward() == FOWARD::LEFT) {
		ANIMATION->changeNonKeyAnimation(PLAYER->getAni(), "player_ALL1", 323, 315, 12, false, false);
	}
}

void Player_Dead::update()
{
	//if (!PLAYER->getAni()->isPlay())
	//	m_pFSM->ChangeState(PLAYERSTATE::IDLE);
}

void Player_Dead::release()
{
}

//====================================================

Player_HurtCover::Player_HurtCover()
{
	m_eState = PLAYERSTATE::HURTCOVER;
	PLAYER->setState(PLAYERSTATE::HURTCOVER);
}

Player_HurtCover::~Player_HurtCover()
{
}

void Player_HurtCover::init()
{
	//������
	if (PLAYER->getFoward() == FOWARD::RIGHT) {
		ANIMATION->changeNonKeyAnimation(PLAYER->getAni(), "player_ALL1", 240, 248, 12, false, false);
	}
	else if (PLAYER->getFoward() == FOWARD::LEFT) {
		ANIMATION->changeNonKeyAnimation(PLAYER->getAni(), "player_ALL1", 335, 327, 12, false, false);
	}
}

void Player_HurtCover::update()
{
	if (!PLAYER->getAni()->isPlay())
		m_pFSM->ChangeState(PLAYERSTATE::IDLE);
}

void Player_HurtCover::release()
{
}

//====================================================

Player_Flip::Player_Flip()
{
	m_eState = PLAYERSTATE::FLIP;
	PLAYER->setState(PLAYERSTATE::FLIP);
}

Player_Flip::~Player_Flip()
{
}

void Player_Flip::init()
{
	//������ȯ
	if (PLAYER->getFoward() == FOWARD::LEFT) {
		PLAYER->setFoward(FOWARD::RIGHT);
		_angle = PI / 6;
	}
	else if (PLAYER->getFoward() == FOWARD::RIGHT) {
		PLAYER->setFoward(FOWARD::LEFT);		
		_angle = PI * 5 / 6;
	}
	//������
	if (PLAYER->getFoward() == FOWARD::RIGHT) {
		ANIMATION->changeNonKeyAnimation(PLAYER->getAni(), "player_ALL1", 252, 262, 35, false, false);
	}
	else if (PLAYER->getFoward() == FOWARD::LEFT) {
		ANIMATION->changeNonKeyAnimation(PLAYER->getAni(), "player_ALL1", 347, 337, 35, false, false);
	}
}

void Player_Flip::update()
{
	//�̵�
	PLAYER->setX(PLAYER->getX() + cosf(_angle) * 12 * TIME->getGameTimeRate());
	PLAYER->setY(PLAYER->getY() - sinf(_angle) * 12 * TIME->getGameTimeRate());

	//������ �����
	if (!PLAYER->getAni()->isPlay()) {
		m_pFSM->ChangeState(PLAYERSTATE::FALL);
	}
}

void Player_Flip::release()
{
}

//====================================================

Player_WallSlide::Player_WallSlide()
{
	m_eState = PLAYERSTATE::WALLSLIDE;
	PLAYER->setState(PLAYERSTATE::WALLSLIDE);
}

Player_WallSlide::~Player_WallSlide()
{
}

void Player_WallSlide::init()
{
	//������
	if (PLAYER->getFoward() == FOWARD::RIGHT) {
		int arr[] = { 264 };
		ANIMATION->changeNonKeyAnimation(PLAYER->getAni(), "player_ALL1", arr, sizeof(arr)/sizeof(int), 12, false);
	}
	else if (PLAYER->getFoward() == FOWARD::LEFT) {
		int arr[] = { 359 };
		ANIMATION->changeNonKeyAnimation(PLAYER->getAni(), "player_ALL1", arr, sizeof(arr) / sizeof(int), 12, false);
	}
}

void Player_WallSlide::update()
{
	if (INPUT->isOnceKeyDown('W'))
		m_pFSM->ChangeState(PLAYERSTATE::FLIP);
}

void Player_WallSlide::release()
{
}

//====================================================

//TCHAR str[128];
//_stprintf_s(str, "���� : %d", PLAYER->getFoward());
//ZORDER->UITextOut(str, ZUIFIRST, 0, 0, RGB(0, 0, 0));