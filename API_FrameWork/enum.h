#pragma once
//===============================================
//					enum
//===============================================
enum class FOWARD
{
	LEFT,
	RIGHT,
	UP,
	DOWN,
	LEFTUP,
	RIGHTUP,
	LEFTDOWN,
	RIGHTDOWN,
	NONE
};

enum class PLAYERSTATE	//카타나제로
{
	IDLE,
	RUN,
	CROUCH,
	ROLL,
	JUMP,
	FALL,
	ATTACK,
	DOORBREAK,
	HURTCOVER,
	FLIP,
	WALLSLIDE,
	DEAD
};