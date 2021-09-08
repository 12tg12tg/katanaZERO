#include "framework.h"
#include "BossFSM.h"

//====================================================
void BossState::decideFoward()
{
	if (RecCenX(_boss->_col->getRect()) < RecCenX(PLAYER->getCollider()->getRect())) {
		_boss->_foward = FOWARD::RIGHT;
	}
	else _boss->_foward = FOWARD::LEFT;
}
//====================================================

BossFSM::BossFSM()
{
}

BossFSM::~BossFSM()
{
}

void BossFSM::update()
{
	m_pCurState->update();
}

void BossFSM::release()
{
	auto iter = m_vecState.begin();
	for (iter; iter != m_vecState.end(); ++iter)
	{
		SAFE_DELETE(*iter);
	}
}

void BossFSM::AddState(BossState* _pState, boss* _boss)
{
	_pState->SetFSM(this);
	_pState->SetBoss(_boss);
	m_vecState.push_back(_pState);
}

void BossFSM::ChangeState(BOSSSTATE _eType)
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

void BossFSM::SetState(BOSSSTATE _eType)
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

Boss_Idle::Boss_Idle()
{
	m_eState = BOSSSTATE::IDLE;
}

Boss_Idle::~Boss_Idle()
{
}

void Boss_Idle::init()
{
	_boss->_state = BOSSSTATE::IDLE;
	_boss->_isGracePeriod = false;
	decideFoward();
	//이전상태에따른 처리
	switch (m_pFSM->getPreState()->getThisState())
	{
	case BOSSSTATE::IDLE:
	case BOSSSTATE::SHOOT:
	case BOSSSTATE::RIFLE:
	case BOSSSTATE::ROLL_JUMP_SHOTGUN:
	case BOSSSTATE::DASH:
	case BOSSSTATE::SWEEP:
	case BOSSSTATE::VERTICALLASER:
	case BOSSSTATE::HURT:
	case BOSSSTATE::DEAD:
		if (_boss->_foward == FOWARD::RIGHT) ANIMATION->changeNonKeyAnimation(_boss->_ani, "headhunter_all", 0, 11, 11, false, true);
		else ANIMATION->changeNonKeyAnimation(_boss->_ani, "headhunter_all", 431, 420, 15, false, true);
		break;
	}
}

void Boss_Idle::update()
{

}

void Boss_Idle::release()
{
}

//====================================================

Boss_Shoot::Boss_Shoot()
{
	m_eState = BOSSSTATE::SHOOT;
}

Boss_Shoot::~Boss_Shoot()
{
}

void Boss_Shoot::init()
{
	_boss->_state = BOSSSTATE::SHOOT;
	_boss->_isGracePeriod = false;
	decideFoward();
	//이전상태에따른 처리
	switch (m_pFSM->getPreState()->getThisState())
	{
	case BOSSSTATE::IDLE:
	case BOSSSTATE::SHOOT:
	case BOSSSTATE::RIFLE:
	case BOSSSTATE::ROLL_JUMP_SHOTGUN:
	case BOSSSTATE::DASH:
	case BOSSSTATE::SWEEP:
	case BOSSSTATE::VERTICALLASER:
	case BOSSSTATE::HURT:
	case BOSSSTATE::DEAD:
		if (_boss->_foward == FOWARD::RIGHT) ANIMATION->changeNonKeyAnimation(_boss->_ani, "headhunter_all", 18, 24, 15, false, false);
		else ANIMATION->changeNonKeyAnimation(_boss->_ani, "headhunter_all", 449, 444, 15, false, false);
		break;
	}

	//고유멤버변수
	_shootNum = RND->getFromTo(2, 3);
	_shootCount = 0;
}

void Boss_Shoot::update()
{
	//애니매이션이 끝나는 시점에 다음 동작 실행.
	//shoot횟수를 채우거나 IDLE로 가거나
	if (!_boss->_ani->isPlay()) {
		if (_shootCount < _shootNum) {
			if (_boss->_foward == FOWARD::RIGHT) ANIMATION->changeNonKeyAnimation(_boss->_ani, "headhunter_all", 36, 43, 15, false, false);
			else ANIMATION->changeNonKeyAnimation(_boss->_ani, "headhunter_all", 467, 460, 15, false, false);
			/*원형폭탄fire*/
			++_shootCount;
		}
		else {
			m_pFSM->ChangeState(BOSSSTATE::IDLE);
		}
	}
}

void Boss_Shoot::release()
{
}

//====================================================

Boss_Rifle::Boss_Rifle()
{
	m_eState = BOSSSTATE::RIFLE;
}

Boss_Rifle::~Boss_Rifle()
{
}

void Boss_Rifle::init()
{
	_boss->_state = BOSSSTATE::RIFLE;
	_boss->_isGracePeriod = false;
	decideFoward();
	//이전상태에따른 처리
	switch (m_pFSM->getPreState()->getThisState())
	{
	case BOSSSTATE::IDLE:
	case BOSSSTATE::SHOOT:
	case BOSSSTATE::RIFLE:
	case BOSSSTATE::ROLL_JUMP_SHOTGUN:
	case BOSSSTATE::DASH:
	case BOSSSTATE::SWEEP:
	case BOSSSTATE::VERTICALLASER:
	case BOSSSTATE::HURT:
	case BOSSSTATE::DEAD:
		if (_boss->_foward == FOWARD::RIGHT) ANIMATION->changeNonKeyAnimation(_boss->_ani, "headhunter_all", 54, 61, 11, false, false);
		else ANIMATION->changeNonKeyAnimation(_boss->_ani, "headhunter_all", 485, 478, 11, false, false);
		break;
	}

	//고유멤버변수
	_readyForShoot = false;
	_count = 0.f;
}

void Boss_Rifle::update()
{
	//애니매이션이 끝나는 시점에 다음 동작 실행.
	if (!_boss->_ani->isPlay()) {
		if (!_readyForShoot) {
			_readyForShoot = true;
			/*레이저fire*/

		}
		else {
			/*레이저발사딜레이 55 count*/
			_count += 1 * TIME->getGameTimeRate();
			if (_count > 55) {
				//IDLE로
				m_pFSM->ChangeState(BOSSSTATE::IDLE);
			}
		}
	}
}

void Boss_Rifle::release()
{
}

//====================================================

Boss_RollJumpShot::Boss_RollJumpShot()
{
	m_eState = BOSSSTATE::ROLL_JUMP_SHOTGUN;
}

Boss_RollJumpShot::~Boss_RollJumpShot()
{
}

void Boss_RollJumpShot::init()
{
	_boss->_state = BOSSSTATE::ROLL_JUMP_SHOTGUN;
	_boss->_isGracePeriod = true;
	decideFoward();
	//이전상태에따른 처리
	switch (m_pFSM->getPreState()->getThisState())
	{
	case BOSSSTATE::IDLE:
	case BOSSSTATE::SHOOT:
	case BOSSSTATE::RIFLE:
	case BOSSSTATE::ROLL_JUMP_SHOTGUN:
	case BOSSSTATE::DASH:
	case BOSSSTATE::SWEEP:
	case BOSSSTATE::VERTICALLASER:
	case BOSSSTATE::HURT:
	case BOSSSTATE::DEAD:
		if (_boss->_foward == FOWARD::RIGHT) ANIMATION->changeNonKeyAnimation(_boss->_ani, "headhunter_all", 72, 78, 15, false, false);
		else ANIMATION->changeNonKeyAnimation(_boss->_ani, "headhunter_all", 503, 497, 15, false, false);
		break;
	}
	//고유멤버변수
	_afterRoll = false;
	_afterJump = false;
	_afterwallGrap = false;
	_jumpAngle = 0.f;
	_wallGrapCount = 0.f;

	_jumpPower = 13.f;
	_gravity = 0.4f;
	_maxGravity = 8.f;
	_shotCount = 0;
	_angle = PI+PI/3;
	_shotNum = 0;
}

void Boss_RollJumpShot::update()
{
	//구르기상태일 때
	if (!_afterRoll) {
		//애니실행동안 이동
		if (_boss->_ani->isPlay()){
			if (_boss->_ani->getNowPlayIndex() >= 1) {
				if (_boss->_foward == FOWARD::RIGHT) _boss->_x += 7 * TIME->getGameTimeRate();
				else _boss->_x -= 7 * TIME->getGameTimeRate();
			}
		}
		else {
			//애니끝나면 점프준비 애니로 교체 & 무적해제
			decideFoward();
			_afterRoll = true;
			_boss->_isGracePeriod = false;
			if (_boss->_foward == FOWARD::RIGHT) ANIMATION->changeNonKeyAnimation(_boss->_ani, "headhunter_all", 144, 146, 11, false, false);
			else ANIMATION->changeNonKeyAnimation(_boss->_ani, "headhunter_all", 575, 573, 11, false, false);
			//바뀐 방향에따라 점프뛸각설정
			if (_boss->_foward == FOWARD::RIGHT) {
				_jumpAngle = UTIL::getAngle(_boss->_x, _boss->_y, 93, 340);
			}
			else {
				_jumpAngle = UTIL::getAngle(_boss->_x, _boss->_y, 1066, 340);
			}
		}
	}
	//점프상태일 때
	else if (!_afterJump) {
		if (!_boss->_ani->isPlay()) {
			//애니끝나면 점프애니로 교체하고 각도이동.
			if (_boss->_foward == FOWARD::RIGHT) {
				int arr[] = { 162 };
				ANIMATION->changeNonKeyAnimation(_boss->_ani, "headhunter_all", arr, sizeof(arr)/sizeof(int), 11, false);
			}
			else {
				int arr[] = { 593 };
				ANIMATION->changeNonKeyAnimation(_boss->_ani, "headhunter_all", arr, sizeof(arr) / sizeof(int), 11, false);
			}
			_boss->_x += cosf(_jumpAngle) * 90 * TIME->getGameTimeRate();
			_boss->_y -= sinf(_jumpAngle) * 90 * TIME->getGameTimeRate();
			//if (_jumpAngle > PI_2) _jumpAngle += 0.001;
			//else if(_jumpAngle < PI_2) _jumpAngle -= 0.001;
			//일정목표도착하면 벽잡기 애니로
			if (_boss->_y <= 340) {
				_afterJump = true;
				if (_boss->_foward == FOWARD::RIGHT) {
					ANIMATION->changeNonKeyAnimation(_boss->_ani, "headhunter_all", 234, 236, 11, false, false);
				}
				else {
					ANIMATION->changeNonKeyAnimation(_boss->_ani, "headhunter_all", 665, 663, 11, false, false);
				}
			}
		}
	}
	//벽잡기상태일 때,
	else if(!_afterwallGrap){
		_wallGrapCount += 1 * TIME->getGameTimeRate();
		if (_wallGrapCount > 50 && !_boss->_ani->isPlay()) {
			_afterwallGrap = true;
			//샷건애니로
			if (_boss->_foward == FOWARD::RIGHT) ANIMATION->changeNonKeyAnimation(_boss->_ani, "headhunter_all", 252, 258, 8, false, false);
			else ANIMATION->changeNonKeyAnimation(_boss->_ani, "headhunter_all", 683, 677, 8, false, false);
			//샷건뛸 점프각을 계산..
			if (_boss->_foward == FOWARD::RIGHT)_angle = PI + PI / 3;
			else _angle = PI2 - PI / 3;
		}
	}
	//샷건
	else {
		//이동
		if (_boss->_foward == FOWARD::RIGHT) _boss->_x += 7.f;
		else _boss->_x -= 7.f;

		if (_jumpPower > 0.f) {
			_boss->_y -= _jumpPower;
			_jumpPower -= _gravity;
			if (_jumpPower < 0.f) {
				_jumpPower = 0.f;
				_gravity = 0.f;
			}
		}
		else {
			_gravity += 0.2f;
			_boss->_y += _gravity;
			if (_gravity > _maxGravity) _gravity = _maxGravity;
			//땅에 닿으면 탈출
			if (_boss->_isLand) {
				m_pFSM->ChangeState(BOSSSTATE::IDLE);
			}		
		}
		//발사
		_shotCount += 1 * TIME->getGameTimeRate();
		if (_shotCount > 1 && _shotNum < 20) {
			_shotCount = 0;			
			_boss->_bm->getNormalBullet()->fire(_boss->_col->getPos().x, _boss->_col->getPos().y, _angle);
			if (_boss->_foward == FOWARD::RIGHT) _angle += RADIAN(5);
			else _angle -= RADIAN(5);
			_shotNum++;
		}
	}
}

void Boss_RollJumpShot::release()
{
}

//====================================================

Boss_Dash::Boss_Dash()
{
	m_eState = BOSSSTATE::DASH;
}

Boss_Dash::~Boss_Dash()
{
}

void Boss_Dash::init()
{
	_boss->_state = BOSSSTATE::DASH;
	_boss->_isGracePeriod = true;
	decideFoward();
	//이전상태에따른 처리
	switch (m_pFSM->getPreState()->getThisState())
	{
	case BOSSSTATE::IDLE:
	case BOSSSTATE::SHOOT:
	case BOSSSTATE::RIFLE:
	case BOSSSTATE::ROLL_JUMP_SHOTGUN:
	case BOSSSTATE::DASH:
	case BOSSSTATE::SWEEP:
	case BOSSSTATE::VERTICALLASER:
	case BOSSSTATE::HURT:
	case BOSSSTATE::DEAD:
		if (_boss->_foward == FOWARD::RIGHT) ANIMATION->changeNonKeyAnimation(_boss->_ani, "headhunter_all", 90, 97, 15, false, false);
		else ANIMATION->changeNonKeyAnimation(_boss->_ani, "headhunter_all", 521, 514, 15, false, false);
		break;
	}
	//고유멤버변수
	_afterReady = false;
	_dashCount = 0.f;
	_changeAni = false;
}

void Boss_Dash::update()
{
	if (!_afterReady) {
		if (!_boss->_ani->isPlay()) {
			_afterReady = true;
			if (_boss->_foward == FOWARD::RIGHT) {
				int arr[] = { 108 };
				ANIMATION->changeNonKeyAnimation(_boss->_ani, "headhunter_all", arr, sizeof(arr)/sizeof(int), 1, false);
			}
			else {
				int arr[] = { 539 };
				ANIMATION->changeNonKeyAnimation(_boss->_ani, "headhunter_all", arr, sizeof(arr) / sizeof(int), 1, false);
			}
		}
	}
	else {
		if (!_changeAni) {
			//슬로모드일때 애니변경
			if (MAIN->getIsSlow()) {
				if (_boss->_foward == FOWARD::RIGHT) {
					int arr[] = { 109 };
					ANIMATION->changeNonKeyAnimation(_boss->_ani, "headhunter_all", arr, sizeof(arr) / sizeof(int), 1, false);
				}
				else {
					int arr[] = { 538 };
					ANIMATION->changeNonKeyAnimation(_boss->_ani, "headhunter_all", arr, sizeof(arr) / sizeof(int), 1, false);
				}
			}
			else {
				if (_boss->_foward == FOWARD::RIGHT) {
					int arr[] = { 108 };
					ANIMATION->changeNonKeyAnimation(_boss->_ani, "headhunter_all", arr, sizeof(arr) / sizeof(int), 1, false);
				}
				else {
					int arr[] = { 539 };
					ANIMATION->changeNonKeyAnimation(_boss->_ani, "headhunter_all", arr, sizeof(arr) / sizeof(int), 1, false);
				}
			}
			_dashCount += 1 * TIME->getGameTimeRate();
			//애니변경
			if (_dashCount >= 20) {
				if (_boss->_foward == FOWARD::RIGHT) ANIMATION->changeNonKeyAnimation(_boss->_ani, "headhunter_all", 126, 135, 15, false, false);
				else ANIMATION->changeNonKeyAnimation(_boss->_ani, "headhunter_all", 557, 548, 15, false, false);
				_changeAni = true;
			}
		}
		//이동
		if (_boss->_foward == FOWARD::RIGHT) _boss->_x += 30 * TIME->getGameTimeRate();
		else _boss->_x -= 30 * TIME->getGameTimeRate();

		//발사
		/*fire - 시작지점부터 방향으로 일정길이까지 길어지는 투사체, 다 길어지면 이동.*/

		//탈출
		if (_boss->_isSideCol) {
			_boss->_isGracePeriod = false;
			m_pFSM->ChangeState(BOSSSTATE::IDLE);
		}
	}
}

void Boss_Dash::release()
{
}

//====================================================

Boss_Sweep::Boss_Sweep()
{
	m_eState = BOSSSTATE::SWEEP;
}

Boss_Sweep::~Boss_Sweep()
{
}

void Boss_Sweep::init()
{
	_boss->_state = BOSSSTATE::SWEEP;
	_boss->_isGracePeriod = false;
	//decideFoward();
	//이전상태에따른 처리
	switch (m_pFSM->getPreState()->getThisState())
	{
	case BOSSSTATE::IDLE:
	case BOSSSTATE::SHOOT:
	case BOSSSTATE::RIFLE:
	case BOSSSTATE::ROLL_JUMP_SHOTGUN:
	case BOSSSTATE::DASH:
	case BOSSSTATE::SWEEP:
	case BOSSSTATE::VERTICALLASER:
	case BOSSSTATE::HURT:
	case BOSSSTATE::DEAD:
		 ANIMATION->changeNonKeyAnimation(_boss->_ani, "headhunter_all", 629, 626, 15, false, false);
		_boss->_x = 127;
		_boss->_y = 184;
		//--------------------------
		//_boss->_x = 979, _y = 184
		//_boss->_x = 127, _y = 184
		//--------------------------
		break;
	}
	//고유멤버변수
	_afterReady = false;
	_afterShot = false;
	_aftermove = false;
	_secondReady = false;
	_secondShot = false;
	_seconddisapear = false;

	_firstterm = _secondterm = 0.f;
}

void Boss_Sweep::update()
{
	if (!_afterReady) {
		if (!_boss->_ani->isPlay()) {
			ANIMATION->changeNonKeyAnimation(_boss->_ani, "headhunter_all", 611, 594, 15, false, false);
			_afterReady = true;
		}
	}
	else if (!_afterShot) {
		if (!_boss->_ani->isPlay()) {
			ANIMATION->changeNonKeyAnimation(_boss->_ani, "headhunter_all", 647, 645, 15, false, false);
			_afterShot = true;	
			
		}
	}
	else if (!_aftermove) {
		if (!_boss->_ani->isPlay()) {
			_boss->_y = -100;		//화면밖으로
			_firstterm += 1 * TIME->getGameTimeRate();
			if (_firstterm > 40) {
				ANIMATION->changeNonKeyAnimation(_boss->_ani, "headhunter_all", 198, 201, 15, false, false);
				_boss->_x = 979;
				_boss->_y = 184;
				_aftermove = true;
			}
		}
	}
	else if (!_secondReady) {
		if (!_boss->_ani->isPlay()) {
			ANIMATION->changeNonKeyAnimation(_boss->_ani, "headhunter_all", 180, 197, 15, false, false);
			_secondReady = true;
		}
	}
	else if (!_seconddisapear) {
		if (!_boss->_ani->isPlay()) {
			ANIMATION->changeNonKeyAnimation(_boss->_ani, "headhunter_all", 216, 218, 15, false, false);
			_seconddisapear = true;
		}
	}
	else {
		if (!_boss->_ani->isPlay()) {
			_boss->_y = -100;		//화면밖으로
		}
		_secondterm += 1 * TIME->getGameTimeRate();
		if (_secondterm > 40) {
			_boss->_x = 880;
			_boss->_y = 523;
			m_pFSM->ChangeState(BOSSSTATE::IDLE);
		}
	}


}

void Boss_Sweep::release()
{
}

//====================================================

Boss_verLaser::Boss_verLaser()
{
	m_eState = BOSSSTATE::VERTICALLASER;
}

Boss_verLaser::~Boss_verLaser()
{
}

void Boss_verLaser::init()
{
	_boss->_state = BOSSSTATE::VERTICALLASER;
	_boss->_isGracePeriod = false;
	//decideFoward();
	//이전상태에따른 처리
	switch (m_pFSM->getPreState()->getThisState())
	{
	case BOSSSTATE::IDLE:
	case BOSSSTATE::SHOOT:
	case BOSSSTATE::RIFLE:
	case BOSSSTATE::ROLL_JUMP_SHOTGUN:
	case BOSSSTATE::DASH:
	case BOSSSTATE::SWEEP:
	case BOSSSTATE::VERTICALLASER:
	case BOSSSTATE::HURT:
	case BOSSSTATE::DEAD:
		ANIMATION->changeNonKeyAnimation(_boss->_ani, "headhunter_all", 288, 291, 15, false, false);
		_boss->_x = 124;// , 1024, 301, 823
		_boss->_y = 184;
		break;
	}
	//고유멤버변수
	_shootNum = 0;
	_verShotDone = false;
	_afterShot = false;
	_sweepShot = false;
	_isshot = false;
	_afterAll = false;
}

void Boss_verLaser::update()
{
	if (!_verShotDone) {
		if (!_afterShot) {
			if (!_boss->_ani->isPlay()) {
				/*fire*/
				++_shootNum;
				if (_shootNum == 4) {
					_verShotDone = true;
					_boss->_x = 574;
					ANIMATION->changeNonKeyAnimation(_boss->_ani, "headhunter_all", 628, 626, 15, false, false);
				}
				else {
					_afterShot = true;
					ANIMATION->changeNonKeyAnimation(_boss->_ani, "headhunter_all", 306, 310, 15, false, false);
				}
			}
		}
		else {
			if (!_boss->_ani->isPlay()) {
				_afterShot = false;
				if (_shootNum == 1) _boss->_x = 1024;
				else if(_shootNum == 2)_boss->_x = 301;
				else if(_shootNum == 3)_boss->_x = 823;
				ANIMATION->changeNonKeyAnimation(_boss->_ani, "headhunter_all", 288, 291, 15, false, false);
			}
		}
	}
	else {
		if (!_sweepShot) {
			if (!_boss->_ani->isPlay()) {
				_sweepShot = true;
				ANIMATION->changeNonKeyAnimation(_boss->_ani, "headhunter_all", 610, 594, 15, false, false);
			}
		}
		else if(!_afterAll){
			if (_boss->_ani->getNowPlayIndex() == 2 && !_isshot) {
				_isshot = true;
				/*fire-sweep*/
				OutputDebugString("Key Down!");
			}
			else if (!_boss->_ani->isPlay()) {
				ANIMATION->changeNonKeyAnimation(_boss->_ani, "headhunter_all", 647, 644, 15, false, false);
				_afterAll = true;
			}
		}
		else {
			_boss->_y = 523;
			m_pFSM->ChangeState(BOSSSTATE::IDLE);
		}
	}
}

void Boss_verLaser::release()
{
}

//====================================================

Boss_Hurt::Boss_Hurt()
{
	m_eState = BOSSSTATE::HURT;
}

Boss_Hurt::~Boss_Hurt()
{
}

void Boss_Hurt::init()
{
	_boss->_state = BOSSSTATE::HURT;
	_boss->_isGracePeriod = true;
	decideFoward();
	//이전상태에따른 처리
	switch (m_pFSM->getPreState()->getThisState())
	{
	case BOSSSTATE::IDLE:
	case BOSSSTATE::SHOOT:
	case BOSSSTATE::RIFLE:
	case BOSSSTATE::ROLL_JUMP_SHOTGUN:
	case BOSSSTATE::DASH:
	case BOSSSTATE::SWEEP:
	case BOSSSTATE::VERTICALLASER:
	case BOSSSTATE::HURT:
	case BOSSSTATE::DEAD:
		if (_boss->_foward == FOWARD::RIGHT) ANIMATION->changeNonKeyAnimation(_boss->_ani, "headhunter_all", 324, 333, 15, false, false);
		else ANIMATION->changeNonKeyAnimation(_boss->_ani, "headhunter_all", 755, 746, 15, false, false);
		break;
	}
	//고유멤버변수
	_afterHurt = false;
	_jumpPower = 5.f;
	_gravity = 0.4f;
	_maxGravity = 5.f;
	_stunGauge = 0.f;
	_stunOver = false;
}

void Boss_Hurt::update()
{
	if (!_afterHurt) {
		//이동
		if (_boss->_foward == FOWARD::RIGHT) _boss->_x -= 8.f * TIME->getGameTimeRate();
		else _boss->_x += 8.f * TIME->getGameTimeRate();
		
		if (_jumpPower > 0) {
			_boss->_y -= _jumpPower * TIME->getGameTimeRate();
			_jumpPower -= _gravity * TIME->getGameTimeRate();
			if (_jumpPower < 0) {
				_jumpPower = 0;
				_gravity = 0;
			}
		}
		else {
			_boss->_y += _gravity * TIME->getGameTimeRate();
			_gravity += 0.5f * TIME->getGameTimeRate();
			if (_gravity > _maxGravity) _gravity = _maxGravity;
		}

		//애니끝나면
		if (!_boss->_ani->isPlay() && _boss->_isLand) {
			_afterHurt = true;
		}
	}
	else {
		_stunGauge += 1 * TIME->getGameTimeRate();
		if (_stunGauge > 20 && !_stunOver) {
			if (_boss->_foward == FOWARD::RIGHT) ANIMATION->changeNonKeyAnimation(_boss->_ani, "headhunter_all", 342, 345, 15, false, false);
			else ANIMATION->changeNonKeyAnimation(_boss->_ani, "headhunter_all", 773, 770, 15, false, false);
			_stunOver = true;
		}
		if (_stunOver && !_boss->_ani->isPlay()) {
			_boss->_isGracePeriod = false;
			m_pFSM->ChangeState(BOSSSTATE::IDLE);
		}
	}
}

void Boss_Hurt::release()
{
}

//====================================================

Boss_Dead::Boss_Dead()
{
	m_eState = BOSSSTATE::DEAD;
}

Boss_Dead::~Boss_Dead()
{
}

void Boss_Dead::init()
{
	_boss->_state = BOSSSTATE::DEAD;
	_boss->_isGracePeriod = true;
	decideFoward();
	//이전상태에따른 처리
	switch (m_pFSM->getPreState()->getThisState())
	{
	case BOSSSTATE::IDLE:
	case BOSSSTATE::SHOOT:
	case BOSSSTATE::RIFLE:
	case BOSSSTATE::ROLL_JUMP_SHOTGUN:
	case BOSSSTATE::DASH:
	case BOSSSTATE::SWEEP:
	case BOSSSTATE::VERTICALLASER:
	case BOSSSTATE::HURT:
	case BOSSSTATE::DEAD:
		if (_boss->_foward == FOWARD::RIGHT) ANIMATION->changeNonKeyAnimation(_boss->_ani, "headhunter_all", 360, 367, 11, false, false);
		else ANIMATION->changeNonKeyAnimation(_boss->_ani, "headhunter_all", 791, 784, 11, false, false);
		break;
	}
	//고유멤버변수
	_afterDead = false;
	_afterCut = false;
	_deadCount = 0;
	_jumpPower = 5.f;
	_gravity = 0.4f;
	_maxGravity = 5.f;
}

void Boss_Dead::update()
{
	if (!_afterDead) {
		//이동
		if (_boss->_foward == FOWARD::RIGHT) _boss->_x -= 8.f * TIME->getGameTimeRate();
		else _boss->_x += 8.f * TIME->getGameTimeRate();

		if (_jumpPower > 0) {
			_boss->_y -= _jumpPower * TIME->getGameTimeRate();
			_jumpPower -= _gravity * TIME->getGameTimeRate();
			if (_jumpPower < 0) {
				_jumpPower = 0;
				_gravity = 0;
			}
		}
		else {
			_boss->_y += _gravity * TIME->getGameTimeRate();
			_gravity += 0.5f * TIME->getGameTimeRate();
			if (_gravity > _maxGravity) _gravity = _maxGravity;
		}

		//애니끝나면
		if (!_boss->_ani->isPlay()) {
			_afterDead = true;
		}
	}
	else if(!_afterCut){
		_deadCount += 1 * TIME->getGameTimeRate();
		if (_deadCount > 80) {
			if (!_boss->_ani->isPlay()) {
				_boss->_isGracePeriod = false;
				decideFoward();
				if (_boss->_foward == FOWARD::RIGHT) ANIMATION->changeNonKeyAnimation(_boss->_ani, "headhunter_all", 378, 395, 15, false, true);
				else ANIMATION->changeNonKeyAnimation(_boss->_ani, "headhunter_all", 809, 792, 15, false, true);
			}
			//이동 - 기어가기
			if (_boss->_foward == FOWARD::RIGHT) _boss->_x += 0.2f;
			else _boss->_x -= 0.2f;

			if (_boss->_isCut) {
				_afterCut = true;
				if (_boss->_foward == FOWARD::RIGHT) ANIMATION->changeNonKeyAnimation(_boss->_ani, "headhunter_all", 396, 401, 15, false, false);
				else ANIMATION->changeNonKeyAnimation(_boss->_ani, "headhunter_all", 827, 822, 15, false, false);

				decideFoward();
				if (_boss->_foward == FOWARD::LEFT) _boss->_headX = _boss->_x;
				else _boss->_headX = _boss->_x + _boss->_img->getFrameWidth()/2;
				_boss->_headY = _boss->_y;
				if (_boss->_foward == FOWARD::LEFT) ANIMATION->changeNonKeyAnimation(_boss->_headAni, "headhunter_head", 0, 15, 11, false, false);
				else ANIMATION->changeNonKeyAnimation(_boss->_headAni, "headhunter_head", 16, 31, 11, false, false);
			}
		}
	}
	else {

	}
}

void Boss_Dead::release()
{
}
