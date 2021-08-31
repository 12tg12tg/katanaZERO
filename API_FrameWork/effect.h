#pragma once
#include "gameNode.h"
class effect :public gameNode
{
private:
	image* _effectImage;		//이펙트 이미지
	animation* _effectAni;		//이펙트 애니메이션
	
	int _x, _y;					//이펙트 터뜨릴 좌표
	float _elapsedTime;			//이펙트 경과시간(속도)
	bool _isRunning;			//재생중?
	
	int _frameW, _frameH;
	float _z;					//20210812 - Zorder와 함께 추가.

	bool _isRotate;				//20210831 - 회전이펙트
	float _radian;				//20210831 - 회전이펙트

	image* _bwImage;			//20210831 - 카타나제로를위한 흑백이미지
	bool _isSave;				//20210831 - 저장할꺼냐

	bool _isAlpha;
	BYTE _alpha;
public:
	effect();
	~effect();

	HRESULT init(image* effectImage, int frameW, int frameH, int FPS, float elapsedTime, bool isSave = false, image* bwImg = nullptr);
	HRESULT initAlpha(image* effectImage,  int frameW, int frameH, int FPS, float elapsedTime, bool isSave = false, image* bwImg = nullptr);
	HRESULT initRotate(image* effectImage, int frameW, int frameH, int FPS, float elapsedTime, bool isSave = false, image* bwImg = nullptr);
	HRESULT initRotateAlpha(image* effectImage, int frameW, int frameH, int FPS, float elapsedTime, bool isSave = false, image* bwImg = nullptr);
	void release();
	void update();
	void render();
	
	//원하는 좌표에 이펙트 터뜨리기
	void startEffect(float z, int x, int y, float radian = 0, BYTE alpha = 255);
	//재생중인 좌표 수정
	void moveEffect(float x, float y);
	//이펙트 멈추기
	void stopEffect();
	//이펙트 재생중인지 상태값 가져오기
	bool getIsRunning()const {	return _isRunning;	}

};

