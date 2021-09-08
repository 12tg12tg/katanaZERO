#pragma once
#include "boss.h"
//====================================================
class BossState;
class BossFSM
{
private:
	vector<BossState*> m_vecState;
	BossState* m_pCurState;
	BossState* m_pPreState;
public:
	BossFSM();
	~BossFSM();

	void update();
	void release();

public:
	void AddState(BossState* _pState, boss* _boss);
	void ChangeState(BOSSSTATE _eType);
	void SetState(BOSSSTATE _eType);
	BossState* getState() { return m_pCurState; }
	BossState* getPreState() { return m_pPreState; }
};
//====================================================
class BossState
{
protected:
	boss* _boss;
	BossFSM* m_pFSM;
	BOSSSTATE m_eState;
public:
	BossState()	{};
	~BossState() {};

	virtual void init() = 0;
	virtual void update() = 0;
	virtual void release() = 0;
	virtual void decideFoward();

	BOSSSTATE getThisState() { return m_eState; }
	void SetFSM(BossFSM* _pFSM) { m_pFSM = _pFSM; }
	void SetBoss(boss* boss) { _boss = boss; }
};
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
class Boss_Idle : public BossState
{
private:
	
public:
	Boss_Idle();
	~Boss_Idle();

	virtual void init();
	virtual void update();
	virtual void release();
};
//====================================================
class Boss_Shoot : public BossState
{
private:
	int _shootNum;
	int _shootCount;
public:
	Boss_Shoot();
	~Boss_Shoot();

	virtual void init();
	virtual void update();
	virtual void release();
};
//====================================================
class Boss_Rifle : public BossState
{
private:
	bool _readyForShoot;
	float _count;
public:
	Boss_Rifle();
	~Boss_Rifle();

	virtual void init();
	virtual void update();
	virtual void release();
};
//====================================================
class Boss_RollJumpShot : public BossState
{
private:
	bool _afterRoll;
	bool _afterJump;
	bool _afterwallGrap;
	float _jumpAngle;
	
	float _jumpPower;
	float _gravity;
	float _maxGravity;
	float _shotCount;
	float _angle;
	int _shotNum;

	float _wallGrapCount;
public:
	Boss_RollJumpShot();
	~Boss_RollJumpShot();

	virtual void init();
	virtual void update();
	virtual void release();
};
//====================================================
class Boss_Dash : public BossState
{
private:
	bool _afterReady;
	float _dashCount;

	bool _changeAni;
public:
	Boss_Dash();
	~Boss_Dash();

	virtual void init();
	virtual void update();
	virtual void release();
};
//====================================================
class Boss_Sweep : public BossState
{
private:
	bool _afterReady;
	bool _afterShot;
	bool _aftermove;
	float _firstterm;

	bool _secondReady;
	bool _secondShot;
	bool _seconddisapear;
	float _secondterm;
public:
	Boss_Sweep();
	~Boss_Sweep();

	virtual void init();
	virtual void update();
	virtual void release();
};
//====================================================
class Boss_verLaser : public BossState
{
private:
	bool _afterShot;
	int _shootNum;
	bool _verShotDone;
	bool _sweepShot;
	bool _isshot;
	bool _afterAll;
public:
	Boss_verLaser();
	~Boss_verLaser();

	virtual void init();
	virtual void update();
	virtual void release();
};
//====================================================
class Boss_Hurt : public BossState
{
private:
	bool _afterHurt;

	float _jumpPower;
	float _gravity;
	float _maxGravity;

	float _stunGauge;
	bool _stunOver;
public:
	Boss_Hurt();
	~Boss_Hurt();

	virtual void init();
	virtual void update();
	virtual void release();
};
//====================================================
class Boss_Dead : public BossState
{
private:
	bool _afterDead;
	bool _afterCut;
	float _deadCount;

	float _jumpPower;
	float _gravity;
	float _maxGravity;
public:
	Boss_Dead();
	~Boss_Dead();

	virtual void init();
	virtual void update();
	virtual void release();
};