#pragma once
#include "gameNode.h"
#include "progressBar.h"
class UI : public gameNode
{
private:
	//ui �̹���
	image* _topUi;
	image* _cursor;
	image* _battery;
	image* _rightClick;
	int _rightClickCount;
	animation* _rightClickAni;
	int _batteryFrameY;

	//3�� �׿�
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

	//�ߴ����α׷�����
	progressBar* _timerPg;
	bool _isTimeFade;
	float _fadeAlphaMax;
	int _fadePeriod;
	float _minus;
	float _fadeAlpha;
	bool _isDecrease;

	//�������α׷�����
	float _slowGauge;
	float _maxSlow;
	int _slowDelay;
	int _delay;
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
	
	void slowGauge();
	void reinit();
	void progressFade();

	void setMapTimer(float cur, float max){	_timerPg->setGauge(cur, max);}
};

