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

enum class COLLIDER_TYPE
{
	PLAYER_UNIT,
	BULLET_PLAYER,
	ENEMY_UNIT,
	BULLET_ENEMY,

	END = 32,
};

enum class MAINSTATE
{
	TITLE,
	LOADING,
	INGAME,
	REPLAY,
	ROLLBACK,
	PAUSE,
	NONE
};