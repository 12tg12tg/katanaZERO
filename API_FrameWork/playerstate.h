#pragma once
class Player_Idle : public Cstate
{
private:

public:
	Player_Idle();
	~Player_Idle();

	virtual void update();
	virtual void Enter();
	virtual void Exit();
};

