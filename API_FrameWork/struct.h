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
	int destinationY;
	float angle;
	CHASEPOINT cpoint;
	bool laserDie;
	int laserX;
	bool laserDieover;
	int attackDelay;
};

struct bulletSpec
{
	int x, y;
	BYTE alpha;
};

struct tagBullet
{
	image* img;
	image* bwimg;
	FOWARD foward;
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
	bool firstTime;
	list<bulletSpec> specList;
};