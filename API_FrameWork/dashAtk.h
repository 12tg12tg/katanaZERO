#pragma once
#include "Cbullet.h"
#define MAXLEN 150
class boss;
class dashAtk : public Cbullet
{
private:
	boss* _boss;
public:
	dashAtk();
	~dashAtk();

	void fire(boss* boss);
	void release();
	void update();
	void render();
};

