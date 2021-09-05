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
	image* img;
	image* bwimg;
	animation* ani;
	Collider* col;
	float bottom;
	bool isDeath;
	ENEMYSTATE state;
	float count;
	bool isfirstTime;
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