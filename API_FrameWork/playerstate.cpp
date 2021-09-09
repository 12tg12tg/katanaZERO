#include "framework.h"
#include "PlayerState.h"
#include "effect.h"
#include "playerSlash.h"
//====================================================
void PlayerState::jump()
{
	if (INPUT->isOnceKeyDown('W')) {
		m_pFSM->ChangeState(PLAYERSTATE::JUMP);
	}
}

void PlayerState::attack()
{
	if (INPUT->isOnceKeyDown(VK_LBUTTON)) {
		m_pFSM->ChangeState(PLAYERSTATE::ATTACK);
	}
}
//====================================================
PlayerFSM::PlayerFSM() {}

PlayerFSM::~PlayerFSM(){}

void PlayerFSM::update()
{
	m_pCurState->update();
}

void PlayerFSM::release()
{
	auto iter = m_vecState.begin();
	for (iter; iter != m_vecState.end(); ++iter)
	{
		SAFE_DELETE(*iter);
	}
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
void PlayerFSM::SetDeath(float angle, float power)
{
	for (UINT i = 0; i < m_vecState.size(); ++i)
	{
		if (PLAYERSTATE::DEAD == m_vecState[i]->getThisState())
		{
			m_pCurState = m_vecState[i];
			m_pPreState = m_vecState[i];
			dynamic_cast<Player_Dead*>(m_pCurState)->init(angle, power);
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
}

Player_Idle::~Player_Idle()
{
}

void Player_Idle::init()
{
	PLAYER->setState(PLAYERSTATE::IDLE);
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
	case PLAYERSTATE::JUMP:
	case PLAYERSTATE::FALL:
		if (PLAYER->getFoward() == FOWARD::RIGHT)
			ANIMATION->changeNonKeyAnimation(PLAYER->getAni(), "player_ALL1", 36, 40, 12, false, false);
		else if (PLAYER->getFoward() == FOWARD::LEFT)
			ANIMATION->changeNonKeyAnimation(PLAYER->getAni(), "player_ALL1", 142, 138, 12, false, false);
		break;
	case PLAYERSTATE::ATTACK:
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
	if (speed > 0 && (m_pFSM->getPreState()->getThisState() == PLAYERSTATE::RUN
		||m_pFSM->getPreState()->getThisState() == PLAYERSTATE::FALL
		||m_pFSM->getPreState()->getThisState() == PLAYERSTATE::ROLL)){
		if (PLAYER->getFoward() == FOWARD::RIGHT)
			PLAYER->setX(PLAYER->getX() + PLAYER->getSpeed() * TIME->getGameTimeRate());
		else if (PLAYER->getFoward() == FOWARD::LEFT)
			PLAYER->setX(PLAYER->getX() - PLAYER->getSpeed() * TIME->getGameTimeRate());
		if (m_pFSM->getPreState()->getThisState() == PLAYERSTATE::RUN) {
			_accel += maxspeed / 80 * TIME->getGameTimeRate();
		}
		else if (m_pFSM->getPreState()->getThisState() == PLAYERSTATE::FALL) {
			_accel += maxspeed / 30 * TIME->getGameTimeRate();
		}
		else if (m_pFSM->getPreState()->getThisState() == PLAYERSTATE::ROLL) {
			_accel += maxspeed / 20 * TIME->getGameTimeRate();
		}
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
	//����
	this->attack();
}

void Player_Idle::release()
{
}
//====================================================

Player_Run::Player_Run()
{
	m_eState = PLAYERSTATE::RUN;
}

Player_Run::~Player_Run()
{
}

void Player_Run::init()
{
	PLAYER->setState(PLAYERSTATE::RUN);
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
	_sountCount = 0;
	_effectCount = 0;
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
	//�߸���
	++_effectCount;
	if (_effectCount%2==0 && _effectCount < 9) {
		float x;
		float bottom = PLAYER->getCollider()->getRect().bottom - 10;
		if (PLAYER->getFoward() == FOWARD::RIGHT) {
			x = PLAYER->getCollider()->getRect().left - 15;
			effect* temp = EFFECT->play("dust_right", ZEFFECT1, x + RND->getInt(20)-10,
				bottom + RND->getInt(30) - 15);
			_rdust.push_back(temp);
		}
		else if (PLAYER->getFoward() == FOWARD::LEFT) {
			x = PLAYER->getCollider()->getRect().right + 15;
			effect* temp = EFFECT->play("dust_left", ZEFFECT1, x + RND->getInt(20) - 10,
				bottom + RND->getInt(30) - 15);
			_ldust.push_back(temp);
		}
	}
	for (_idust = _rdust.begin(); _idust != _rdust.end(); ){
		if (!(*_idust)->getIsRunning()) {
			_idust = _rdust.erase(_idust);
			continue;
		}
		else {
			(*_idust)->offsetEffect(-2, 0);
			++_idust;
		}
	}
	for (_idust = _ldust.begin(); _idust != _ldust.end(); ) {
		if (!(*_idust)->getIsRunning()) {
			_idust = _ldust.erase(_idust);
			continue;
		}
		else {
			(*_idust)->offsetEffect(2, 0);
			++_idust;
		}
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
	//����
	this->attack();
	//����
	_sountCount += 1 * TIME->getGameTimeRate();
	if (_sountCount > 20) {
		_sountCount = 0;
		switch (RND->getInt(4))
		{
		case 0:
			SOUND->play("run1", 0.5f);
			break;
		case 1:
			SOUND->play("run2", 0.5f);
			break;
		case 2:
			SOUND->play("run3", 0.5f);
			break;
		case 3:
			SOUND->play("run4", 0.5f);
			break;
		}
	}
}

void Player_Run::release()
{
}
//====================================================

Player_Roll::Player_Roll()
{
	m_eState = PLAYERSTATE::ROLL;
}

Player_Roll::~Player_Roll()
{
}

void Player_Roll::init()
{
	PLAYER->setIsGravePeriod(true);
	PLAYER->setState(PLAYERSTATE::ROLL);
	PLAYER->setSpeed(8.f); //����10�̾����� ���ն��� ��ħ
	if (PLAYER->getFoward() == FOWARD::RIGHT) {
		ANIMATION->changeNonKeyAnimation(PLAYER->getAni(), "player_ALL1", 72, 78, 16, false, false);
	}
	else if (PLAYER->getFoward() == FOWARD::LEFT) {
		ANIMATION->changeNonKeyAnimation(PLAYER->getAni(), "player_ALL1", 178, 172, 16, false, false);
	}
	//����
	SOUND->play("roll", 0.1f);
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
		PLAYER->setIsGravePeriod(false);
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
	//����
	this->attack();
}

void Player_Roll::release()
{
}

//====================================================

Player_Crouch::Player_Crouch()
{
	m_eState = PLAYERSTATE::CROUCH;
}

Player_Crouch::~Player_Crouch()
{
}

void Player_Crouch::init()
{
	PLAYER->setState(PLAYERSTATE::CROUCH);
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
	if (INPUT->isStayKeyDown('S') && PLAYER->getColBlack()) {
		PLAYER->setIgnoreBlack(true);
		m_pFSM->ChangeState(PLAYERSTATE::FALL);
	}
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
	//����
	this->attack();
}

void Player_Crouch::release()
{
}

//====================================================

Player_Jump::Player_Jump()
{
	m_eState = PLAYERSTATE::JUMP;
	_maxDashPower = 10;
}

Player_Jump::~Player_Jump()
{
}

void Player_Jump::init()
{
	//���� �� bool - �������ص���.
	PLAYER->setIsLand(false);
	PLAYER->setState(PLAYERSTATE::JUMP);

	//���� �޸����϶� �ӵ� �������� - �������ص���.
	if (m_pFSM->getPreState()->getThisState() == PLAYERSTATE::RUN) {
		_accelB = PLAYER->getSpeed()/5;
	}
	else {
		_accelB = 0;
	}
	//�������� �� �ִ� ���� - �������ص���.
	_startY = PLAYER->getY();
	if (PLAYER->getFoward() == FOWARD::RIGHT) {
		ANIMATION->changeNonKeyAnimation(PLAYER->getAni(), "player_ALL1", 48, 51, 12, false, true);
	}
	else if (PLAYER->getFoward() == FOWARD::LEFT) {
		ANIMATION->changeNonKeyAnimation(PLAYER->getAni(), "player_ALL1", 154, 151, 12, false, true);
	}
	//���� - �������ص���.
	SOUND->play("jump", 0.1f);
	//����Ʈ - �������ص���.
	EFFECT->play("jumpcloud", ZEFFECT1, PLAYER->getCollider()->getPos().x,
		PLAYER->getCollider()->getBottom() - 52);

	//sin �׷������� ���� �κ� - �����ؾߵ�.
	_gravity = 0.5f;
	_jumpPower = 5.f;
	_count = 0.f;
	/* ������� */
	//�����Ŀ�����ŭ ���� ���.
	//��, �߷°���ŭ �����Ŀ� ����.
	//�����Ŀ��� 0�� ������, �߷¸� �ۿ�. - ���⼭ ���� FALL ���·� �����ָ� ��.
	//�߷��� 0���� ���� �����ϸ�, �ִ�ġ������ Ŀ��. - FALL ���.



}


void Player_Jump::update()
{
	//�¿� �Է� - ������ȯ - �������ص���.
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

	//���������� - ���� - ��
	_count += 1;
	if (_count < 7) {
		if (INPUT->isStayKeyDown('W')) {
			_jumpPower += 1.f;
		}
	}

	//�뽬������ - ���� - �������ص���.
	if (_count < 7) {
		if (PLAYER->getFoward() == FOWARD::RIGHT && INPUT->isStayKeyDown('D')) {
			_accelB += 1.f;
		}
		if (PLAYER->getFoward() == FOWARD::LEFT && INPUT->isStayKeyDown('A')) {
			_accelB += 1.f;
		}
	}
	if (_accelB > _maxDashPower) _accelB = _maxDashPower;
	//�¿� - X���� - �������ص���.
	if (PLAYER->getFoward() == FOWARD::RIGHT)
		PLAYER->setX(PLAYER->getX() + _accelB * TIME->getGameTimeRate());
	if (PLAYER->getFoward() == FOWARD::LEFT)
		PLAYER->setX(PLAYER->getX() - _accelB * TIME->getGameTimeRate());

	
	//���� - Y����, X����
	PLAYER->setY(PLAYER->getY() - _jumpPower * TIME->getGameTimeRate());
	_jumpPower -= _gravity * TIME->getGameTimeRate();
	
	//����Ż������ - �������� 0���϶�� - ��
	if (_jumpPower <= 0) {
		PLAYER->setSpeed(_accelB);
		m_pFSM->ChangeState(PLAYERSTATE::FALL);
	}

	//�����
	if (PLAYER->getColYello() &&
		((PLAYER->getFoward() == FOWARD::RIGHT && INPUT->isStayKeyDown('D')) ||
			(PLAYER->getFoward() == FOWARD::LEFT && INPUT->isStayKeyDown('A')) ||
			m_pFSM->getPreState()->getThisState() == PLAYERSTATE::FLIP)) {
		PLAYER->setAttDash(false);
		PLAYER->setSpeed(_jumpPower);
		m_pFSM->ChangeState(PLAYERSTATE::WALLSLIDE);
	}


	//���� - �������ص���.
	this->attack();
}

void Player_Jump::release()
{
}

//====================================================

Player_Fall::Player_Fall()
{
	m_eState = PLAYERSTATE::FALL;
	_gravity = 0.f;
	_maxGravity = 10.f;
	_maxDashPower = 8.f;
}

Player_Fall::~Player_Fall()
{
}

void Player_Fall::init()
{
	PLAYER->setIsLand(false);
	PLAYER->setState(PLAYERSTATE::FALL);
	_count = 0;
	_onGravity = true;
	_delay = 0;
	switch (m_pFSM->getPreState()->getThisState())
	{
	case PLAYERSTATE::IDLE:
	case PLAYERSTATE::RUN:
	case PLAYERSTATE::CROUCH:
	case PLAYERSTATE::ROLL:
	case PLAYERSTATE::JUMP:
	case PLAYERSTATE::FALL:
	case PLAYERSTATE::DOORBREAK:
	case PLAYERSTATE::HURTCOVER:
	case PLAYERSTATE::WALLSLIDE:
	case PLAYERSTATE::DEAD:
		_accelB = (PLAYER->getSpeed() > _maxDashPower)? _maxDashPower : PLAYER->getSpeed();
		_gravity = 2.f;
		break;
	case PLAYERSTATE::FLIP:
		_accelB = 5;
		_gravity = 6.f;
		break;
	case PLAYERSTATE::ATTACK:
		_accelB = PLAYER->getSpeed();
		if (_accelB > 3.7) _accelB = 3.7;
		if(PLAYER->getAttDash()) _gravity = 4.f;
		else  _gravity = 2.f;
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
	//�¿��̵��ӵ� ��������
	_accelB -= 0.05f;
	if (_accelB < 0)_accelB = 0;

	//��������
	if (INPUT->isOnceKeyDown('S') && !PLAYER->getIgnoreBlack()) {
		_onGravity = false;
		_gravity = 20;
	}

	//����ü���� Y����
	_delay += 1 * TIME->getGameTimeRate();
	if (_onGravity && _delay > 1) {
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

	//�������� - �����
	if (PLAYER->getColYello() && 
		((PLAYER->getFoward()==FOWARD::RIGHT && INPUT->isStayKeyDown('D')) ||
			(PLAYER->getFoward() == FOWARD::LEFT && INPUT->isStayKeyDown('A')) ||
			m_pFSM->getPreState()->getThisState()==PLAYERSTATE::FLIP)) {
		PLAYER->setAttDash(false);
		m_pFSM->ChangeState(PLAYERSTATE::WALLSLIDE);
	}


	//�������� -  �����浹bool ������ ����/*����*/ 
	if (PLAYER->getIsLand()) {
		PLAYER->setSpeed(_accelB);
		PLAYER->setAttDash(false);
		m_pFSM->ChangeState(PLAYERSTATE::IDLE);
		//����
		SOUND->play("land", 0.1f);
		//����Ʈ
		EFFECT->play("landcloud", ZEFFECT1, PLAYER->getCollider()->getPos().x,
			PLAYER->getCollider()->getBottom() - 16);
	}
	//����
	this->attack();
}

void Player_Fall::release()
{
}

//====================================================

Player_Attack::Player_Attack(playerSlash* sl) : _slash(sl)
{
	m_eState = PLAYERSTATE::ATTACK;
}

Player_Attack::~Player_Attack()
{
}

void Player_Attack::init()
{
	PLAYER->setIsLand(false);
	PLAYER->setState(PLAYERSTATE::ATTACK);
	//�������� ���� �ѹ��� ��Ҵ��� bool���� Ȯ���ؼ�
	//���������� �������� �ѹ��� �����ϰ�, ���� ������ �ݺ��Ǹ� ���������ʴ´�.
	Vec2 clickPt = Vec2(CAMERA->getRelativeMouse().x, CAMERA->getRelativeMouse().y);
	_attAngle = PLAYER->getCollider()->getPos().AngleTo(clickPt);
	_accel = 0;
	_maxSpeed = 12;
	_effectOn = false;
	_effectCount = 0;
	PLAYER->setSpeed(_maxSpeed);
	switch (PLAYER->getCollider()->getPos().whichFoward(clickPt))
	{
	case FOWARD::UP:
	case FOWARD::LEFT:
	case FOWARD::LEFTUP:
	case FOWARD::LEFTDOWN:
		PLAYER->setFoward(FOWARD::LEFT);
		ANIMATION->changeNonKeyAnimation(PLAYER->getAni(), "player_ALL1", 299, 293, 24, false, false);
		break;
	case FOWARD::NONE:
	case FOWARD::DOWN:
	case FOWARD::RIGHT:
	case FOWARD::RIGHTUP:
	case FOWARD::RIGHTDOWN:
		PLAYER->setFoward(FOWARD::RIGHT);
		ANIMATION->changeNonKeyAnimation(PLAYER->getAni(), "player_ALL1", 204, 210, 24, false, false);
		break;
	}
	//���� - �ݶ��̴��߰�
	_attackCenterX = PLAYER->getCollider()->getPos().x + 52 * cosf(_attAngle);
	_attackCenterY = PLAYER->getCollider()->getPos().y - 52 * sinf(_attAngle);
	_slash->fire(_attackCenterX, _attackCenterY, _attAngle);
	//����
	switch (RND->getInt(3))
	{
	case 0:
		SOUND->play("slash1", 0.1f);
		break;
	case 1:
		SOUND->play("slash2", 0.1f);
		break;
	case 2:
		SOUND->play("slash3", 0.1f);
		break;
	}
	if (m_pFSM->getPreState()->getThisState() == PLAYERSTATE::ROLL) {
		PLAYER->setIsGravePeriod(true);
		_GracePeriodCount = 0;
	}
}

void Player_Attack::update()
{
	if (m_pFSM->getPreState()->getThisState() == PLAYERSTATE::ROLL) {
		++_GracePeriodCount;
		if (_GracePeriodCount > 20) {
			PLAYER->setIsGravePeriod(false);
		}
	}
	//�ӵ����� - �ִ�ӵ����� accel��ŭ ����
	float speed;
	speed = PLAYER->getSpeed();
	_accel += _maxSpeed / 200 * TIME->getGameTimeRate();
	speed -= _accel * TIME->getGameTimeRate();
	if (speed < 0)	speed = 0.f;
	PLAYER->setSpeed(speed);
	//�ޱ۹������� �̵�
	PLAYER->setX(PLAYER->getX() + cosf(_attAngle) * PLAYER->getSpeed() * TIME->getGameTimeRate());
	if (!PLAYER->getAttDash()|| (_attAngle >PI && _attAngle < PI2)) {
		PLAYER->setY(PLAYER->getY() - sinf(_attAngle) * PLAYER->getSpeed() * TIME->getGameTimeRate());
	}
	//����Ʈ
	_effectCount += 1 * TIME->getGameTimeRate();
	if (_effectCount > 6 && !_effectOn) {
		_effectOn = true;
		if (PLAYER->getFoward() == FOWARD::RIGHT) {
			_slashEff = EFFECT->play("player_slash_right", ZEFFECT2, PLAYER->getCollider()->getPos().x, PLAYER->getCollider()->getPos().y, _attAngle);
		}
		else if (PLAYER->getFoward() == FOWARD::LEFT) {
			_slashEff = EFFECT->play("player_slash_left", ZEFFECT2, PLAYER->getCollider()->getPos().x, PLAYER->getCollider()->getPos().y, PI + _attAngle);
		}
	}
	if(_slashEff) _slashEff->moveEffect(PLAYER->getCollider()->getPos().x, PLAYER->getCollider()->getPos().y);
	//�����������
	if (!PLAYER->getAni()->isPlay()) {
		PLAYER->setAttDash(true);
		//�����̵��ӵ� ���� - ���ڱ� 3.7�� �����Ǵ��Ͼ���;
		PLAYER->setSpeed(abs(cosf(_attAngle) * PLAYER->getSpeed()));
		m_pFSM->ChangeState(PLAYERSTATE::FALL);
	}
}

void Player_Attack::release()
{
}

//====================================================

Player_Doorbreak::Player_Doorbreak()
{
	m_eState = PLAYERSTATE::DOORBREAK;
}

Player_Doorbreak::~Player_Doorbreak()
{
}

void Player_Doorbreak::init()
{
	PLAYER->setState(PLAYERSTATE::DOORBREAK);
	//������
	if (PLAYER->getFoward() == FOWARD::RIGHT) {
		ANIMATION->changeNonKeyAnimation(PLAYER->getAni(), "player_ALL1", 216, 225, 18, false, false);
	}
	else if (PLAYER->getFoward() == FOWARD::LEFT) {
		ANIMATION->changeNonKeyAnimation(PLAYER->getAni(), "player_ALL1", 311, 302, 18, false, false);
	}
	//����
	SOUND->play("doorbreak", 0.1f);
}

void Player_Doorbreak::update()
{
	if (!PLAYER->getAni()->isPlay())
		m_pFSM->ChangeState(PLAYERSTATE::IDLE);

	//����
	this->attack();
}

void Player_Doorbreak::release()
{
}

//====================================================

Player_Dead::Player_Dead()
{
	m_eState = PLAYERSTATE::DEAD;
}

Player_Dead::~Player_Dead()
{
}

void Player_Dead::init()
{
	_timeOver = true;
	PLAYER->setIsGravePeriod(true);
	PLAYER->setState(PLAYERSTATE::DEAD);
	//������
	if (PLAYER->getFoward() == FOWARD::RIGHT) {
		ANIMATION->changeNonKeyAnimation(PLAYER->getAni(), "player_ALL1", 228, 236, 5, false, false);
	}
	else if (PLAYER->getFoward() == FOWARD::LEFT) {
		ANIMATION->changeNonKeyAnimation(PLAYER->getAni(), "player_ALL1", 323, 315, 5, false, false);
	}
	CAMERA->setShake(5, 3, 1);
}

void Player_Dead::init(float angle, float power)
{
	_timeOver = false;
	PLAYER->setIsGravePeriod(true);
	_angle = angle;
	_speed = power;
	PLAYER->setState(PLAYERSTATE::DEAD);
	//������
	if (PLAYER->getFoward() == FOWARD::RIGHT) {
		ANIMATION->changeNonKeyAnimation(PLAYER->getAni(), "player_ALL1", 228, 236, 5, false, false);
	}
	else if (PLAYER->getFoward() == FOWARD::LEFT) {
		ANIMATION->changeNonKeyAnimation(PLAYER->getAni(), "player_ALL1", 323, 315, 5, false, false);
	}
	//����
	SOUND->play("dead", 0.1f);
	//����ũ
	CAMERA->setShake(10, 5, 1);
	//�ǰ�����Ʈ
	float startX = PLAYER->getCollider()->getPos().x + 1566 * cos((float)PI+_angle);
	float startY = PLAYER->getCollider()->getPos().y - 1566 * sinf((float)PI+_angle);
	_hitEffect = EFFECT->play("hitEffect", ZEFFECT2, startX, startY, _angle, 150);
}

void Player_Dead::update()
{
	//if (!PLAYER->getAni()->isPlay())
	//	m_pFSM->ChangeState(PLAYERSTATE::IDLE);
	if (!_timeOver) {
		//����Ʈ�̵�
		_hitEffect->offsetEffect(cosf(_angle) * 100, -sinf(_angle) * 100);


		//�̵�
		if (_speed != 0) {
			PLAYER->setX(PLAYER->getX() + cosf(_angle) * _speed);
			PLAYER->setY(PLAYER->getY() - sinf(_angle) * _speed);			//? ���� �ڻ����̾�����? �� ���̻�������?
			_speed -= 0.15;
		}
		if (_speed <= 0) {
			_speed = 0;
			PLAYER->setIsTimeOut(false);
			MAIN->changeMainState(MAINSTATE::ROLLBACK);
			/*�����������ž�.. -> Ŭ���� �ǰ��� -> ����*/
		}
		PLAYER->setY(PLAYER->getY() + 10);
	}
	else {
		PLAYER->setIsTimeOut(true);
		PLAYER->setY(PLAYER->getY() + 10);
		if (!PLAYER->getAni()->isPlay()) {
			MAIN->changeMainState(MAINSTATE::ROLLBACK);
		}
	}
}

void Player_Dead::release()
{
}

//====================================================

Player_HurtCover::Player_HurtCover()
{
	m_eState = PLAYERSTATE::HURTCOVER;
}

Player_HurtCover::~Player_HurtCover()
{
}

void Player_HurtCover::init()
{
	PLAYER->setState(PLAYERSTATE::HURTCOVER);
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
}

Player_Flip::~Player_Flip()
{
}

void Player_Flip::init()
{
	PLAYER->setState(PLAYERSTATE::FLIP);
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
	//�����
	if (PLAYER->getColYello())
		m_pFSM->ChangeState(PLAYERSTATE::WALLSLIDE);


	//����
	SOUND->play("walljump", 0.1f);
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
	//����
	this->attack();
}

void Player_Flip::release()
{
}

//====================================================

Player_WallSlide::Player_WallSlide()
{
	m_eState = PLAYERSTATE::WALLSLIDE;
}

Player_WallSlide::~Player_WallSlide()
{
}

void Player_WallSlide::init()
{
	PLAYER->setState(PLAYERSTATE::WALLSLIDE);
	//������
	if (PLAYER->getFoward() == FOWARD::RIGHT) {
		int arr[] = { 264 };
		ANIMATION->changeNonKeyAnimation(PLAYER->getAni(), "player_ALL1", arr, sizeof(arr)/sizeof(int), 12, false);
	}
	else if (PLAYER->getFoward() == FOWARD::LEFT) {
		int arr[] = { 359 };
		ANIMATION->changeNonKeyAnimation(PLAYER->getAni(), "player_ALL1", arr, sizeof(arr) / sizeof(int), 12, false);
	}
	
	//�����ӵ�
	if(m_pFSM->getPreState()->getThisState()==PLAYERSTATE::JUMP) _speed = PLAYER->getSpeed()*1.3;
	else if(m_pFSM->getPreState()->getThisState()==PLAYERSTATE::FALL) _speed = 0;
	_gravity = 0.f;
	_maxgravity = 5.f;
	_goUp = false;

	//����
	SOUND->play("grabwall", 0.1f);
}

void Player_WallSlide::update()
{
	//�̲�������
	if (_speed > 0) {
		_goUp = true;
		_speed -= _gravity;
	}
	if(_goUp && _speed < 0) {
		_speed = 0;
		_goUp = false;
		_gravity = 0.f;
	}

	//ZORDER->UITextOut(string("canWall : " + to_string(PLAYER->getStillWall())), ZMAXLAYER, 500, 500, RGB(255, 255, 255));
	//ZORDER->UITextOut(string("down : " + to_string(PLAYER->getIsLand())), ZMAXLAYER, 500, 520, RGB(255, 255, 255));


	//�߷°���
	_gravity += 0.04f;
	if (_gravity > _maxgravity) _gravity = _maxgravity;

	//�̵�
	if(_goUp) PLAYER->setY(PLAYER->getY() - _speed);
	else PLAYER->setY(PLAYER->getY() + _gravity);

	//������
	if (INPUT->isOnceKeyDown('W')) {
		m_pFSM->ChangeState(PLAYERSTATE::FLIP);
	}

	//Ż������
	bool isLand = PLAYER->getIsLand();
	bool stillWall = PLAYER->getStillWall();
	if (isLand || !stillWall) {
		m_pFSM->ChangeState(PLAYERSTATE::IDLE);
	}

	//����
	this->attack();
}

void Player_WallSlide::release()
{
}

//====================================================

//TCHAR str[128];
//_stprintf_s(str, "���� : %d", PLAYER->getFoward());
//ZORDER->UITextOut(str, ZUIFIRST, 0, 0, RGB(0, 0, 0));