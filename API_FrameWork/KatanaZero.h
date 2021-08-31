#pragma once
#include "gameNode.h"
#include "UI.h"
#include "Caretaker.h"

//----------------------------
#include "CollisionTestScene.h"
//-----------------------------



class KatanaZero : public gameNode
{
private:
	image* _cameraBuffer;
	UI* m_ui;
	//------------------------
	CollisionTestScene* _test;
	//-------------------------







	//���λ���
	MAINSTATE _state;
	//�޸���ȿ��
	Caretaker* _caretaker;

	//���ο�
	BYTE _slowAlpha;
	bool _isSlow;
	//������ؽ�Ʈ�ڽ�
	RECT m_debugRc;	
	bool _isDebug;
public:
	KatanaZero();
	~KatanaZero();

	HRESULT init();
	void release();
	void update();
	void render();

	void dropFrame();
	void setIsDebug(bool isDebug)
	{
		_isDebug = isDebug;
		COLLISION->setIsDebug(isDebug);
	}
};

