#pragma once
#include "gameNode.h"
#include "progressBar.h"
class UI : public gameNode
{
private:
	//ui 이미지
	image* _topUi;
	image* _cursor;
	image* _battery;
	int _batteryFrameY;

	//3개 네온
	image* _topleft;
	image* _topmid;
	image* _topright;
	int neonNum_left;
	int neonPeriod_left;
	bool isNeon_left;
	int neonCount_left;
	int neonNumCount_left;
	bool isNeonOn_left;
	int neonNum_mid;
	int neonPeriod_mid;
	bool isNeon_mid;
	int neonCount_mid;
	int neonNumCount_mid;
	bool isNeonOn_mid;
	int neonNum_right;
	int neonPeriod_right;
	bool isNeon_right;
	int neonCount_right;
	int neonNumCount_right;
	bool isNeonOn_right;

	//중단프로그래스바
	progressBar* _timerPg;
public:
	UI();
	~UI();

	HRESULT init();
	void update();
	void render();
	void release();

	void neonInit();
	void neonUpdate();
	void neonRender();


};

