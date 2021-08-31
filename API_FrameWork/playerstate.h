#pragma once
#include "player.h"
//====================================================
class PlayerState;
class PlayerFSM
{
private:
	vector<PlayerState*> m_vecState;
	PlayerState* m_pCurState;
	PlayerState* m_pPreState;
public:
	PlayerFSM();
	~PlayerFSM();

	void update();

public:
	void AddState(PlayerState* _pState);
	void ChangeState(PLAYERSTATE _eType);
	void SetState(PLAYERSTATE _eType);
	PlayerState* getState() { return m_pCurState; }
	PlayerState* getPreState() { return m_pPreState; }
};
//====================================================
class PlayerState
{
protected:
	PlayerFSM* m_pFSM;
	PLAYERSTATE m_eState;
public:
	PlayerState() {};
	~PlayerState() {};

	virtual void init() = 0;
	virtual void update() = 0;
	virtual void release() = 0;

	//모든상태공통접근 - 점프/공격
	void jump();
	void attack();

	PLAYERSTATE getThisState() { return m_eState; }
	void SetFSM(PlayerFSM* _pFSM) { m_pFSM = _pFSM; }
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
class Player_Idle : public PlayerState
{
private:
	float _accel;
public:
	Player_Idle();
	~Player_Idle();

	virtual void init();
	virtual void update();
	virtual void release();
};
//====================================================
class Player_Run : public PlayerState
{
private:
	float _accel;
	float _sountCount;
public:
	Player_Run();
	~Player_Run();

	virtual void init();
	virtual void update();
	virtual void release();
};
//====================================================
class Player_Roll : public PlayerState
{
private:

public:
	Player_Roll();
	~Player_Roll();

	virtual void init();
	virtual void update();
	virtual void release();
};
//====================================================
class Player_Crouch : public PlayerState
{
private:
	float _accel;
public:
	Player_Crouch();
	~Player_Crouch();

	virtual void init();
	virtual void update();
	virtual void release();
};
//====================================================
class Player_Jump : public PlayerState
{
private:
	float _count;
	float _accelA;
	float _accelB;
	float _startX;
	float _startY;
	float _graphX;
	float _maxJumpPower;
	float _maxDashPower;
public:
	Player_Jump();
	~Player_Jump();

	virtual void init();
	virtual void update();
	virtual void release();
};
//====================================================
class Player_Fall : public PlayerState
{
private:
	int _count;
	float _gravity;
	float _maxGravity;
	float _accelB;
	float _maxDashPower;
	bool _onGravity;
	float _delay;
public:
	Player_Fall();
	~Player_Fall();

	virtual void init();
	virtual void update();
	virtual void release();
};
//====================================================
class Player_Attack : public PlayerState
{
private:
	float _attAngle;
	float _accel;
	float _maxSpeed;
	bool _effectOn;
	float _effectCount;
	effect* _slashEff;
public:
	Player_Attack();
	~Player_Attack();

	virtual void init();
	virtual void update();
	virtual void release();
};
//====================================================
class Player_Doorbreak : public PlayerState
{
private:

public:
	Player_Doorbreak();
	~Player_Doorbreak();

	virtual void init();
	virtual void update();
	virtual void release();
};
//====================================================
class Player_Dead : public PlayerState
{
private:

public:
	Player_Dead();
	~Player_Dead();

	virtual void init();
	virtual void update();
	virtual void release();
};
//====================================================
class Player_HurtCover : public PlayerState
{
private:

public:
	Player_HurtCover();
	~Player_HurtCover();

	virtual void init();
	virtual void update();
	virtual void release();
};
//====================================================
class Player_Flip : public PlayerState
{
private:
	float _angle;
public:
	Player_Flip();
	~Player_Flip();

	virtual void init();
	virtual void update();
	virtual void release();
};
//====================================================
class Player_WallSlide : public PlayerState
{
private:

public:
	Player_WallSlide();
	~Player_WallSlide();

	virtual void init();
	virtual void update();
	virtual void release();
};