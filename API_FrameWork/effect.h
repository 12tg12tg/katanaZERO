#pragma once
#include "gameNode.h"
class effect :public gameNode
{
private:
	image* _effectImage;		//����Ʈ �̹���
	animation* _effectAni;		//����Ʈ �ִϸ��̼�
	
	int _x, _y;					//����Ʈ �Ͷ߸� ��ǥ
	float _elapsedTime;			//����Ʈ ����ð�(�ӵ�)
	bool _isRunning;			//�����?
	
	int _frameW, _frameH;
	float _z;					//20210812 - Zorder�� �Բ� �߰�.

	bool _isRotate;				//20210831 - ȸ������Ʈ
	float _radian;				//20210831 - ȸ������Ʈ

	image* _bwImage;			//20210831 - īŸ�����θ����� ����̹���
	bool _isSave;				//20210831 - �����Ҳ���

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
	
	//���ϴ� ��ǥ�� ����Ʈ �Ͷ߸���
	void startEffect(float z, int x, int y, float radian = 0, BYTE alpha = 255);
	//������� ��ǥ ����
	void moveEffect(float x, float y);
	//����Ʈ ���߱�
	void stopEffect();
	//����Ʈ ��������� ���°� ��������
	bool getIsRunning()const {	return _isRunning;	}

};

