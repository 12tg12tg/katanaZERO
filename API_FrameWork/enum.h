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

enum class PLAYERSTATE	//īŸ������
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

enum class ENEMYSTATE
{
	IDLE,
	FIND,
	ATTACK,
	RUN,
	WALK,
	TURN,
	DEAD
};

enum class COLLIDER_TYPE
{
	PLAYER_UNIT,
	BULLET_PLAYER,
	ENEMY_UNIT,
	BULLET_ENEMY,
	POTAL,
	LASER,
	DOOR,
	SEARCH,

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

enum class CHASEPOINT
{
	UPSTAIR,
	UPPOINT,
	ONSTAIR,
	DOWNPOINT,
	DOWNSTAIR
};

enum class BOSSSTATE
{
	IDLE,
	SHOOT,
	RIFLE,
	ROLL_JUMP_SHOTGUN,
	DASH,
	SWEEP,
	VERTICALLASER,
	HURT,
	DEAD
};