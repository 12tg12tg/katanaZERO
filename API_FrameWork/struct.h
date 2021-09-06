#pragma once
//===============================================
//					struct
//===============================================
struct tagGlitch
{
	vector2 pos;
	vector2 frame;
	BYTE alpha;
};

struct tagEnemy
{
	float x;
	float y;
	float z;
	float speed;
	image* img;
	image* bwimg;
	FOWARD foward;
	animation* ani;
	animation* bloodAni;
	Collider* col;
	Collider* searchCol;
	float bottom;
	bool isDeath;
	ENEMYSTATE state;
	float count;
	bool isfirstTime;
	int patterncount;
	bool haveToChangeAni;
	bool findPlayer;
	float deathAngle;
	float effectAngle;
	effect* deatheffect;
	bool ignoreBlack;
	float deathSpeed;
	bool isLand;
	bool isUpCol;
	bool isLeftCol;
	bool isRightCol;
	bool bloodOn;
	int bloodRemainIndex;
	Vec2 patrolpointX;
	int destinationX;
};

struct tagBullet
{
	image* img;
	image* bwimg;
	animation* ani;
	Collider* col;
	float x, y, z;
	float bottom;
	float length;
	float angle;
	float speed;
	bool isOn;
	float count;
	int alpha;
};