#pragma once
#include "gameNode.h"
#include "UI.h"
#include "Caretaker.h"

//----------------------------
#include "CollisionTestScene.h"
#include "textMap.h"
#include "textMap2.h"
#include "textMap3.h"
#include "textMap4.h"
//-----------------------------



class KatanaZero : public gameNode
{
private:
	image* _cameraBuffer;
	UI* m_ui;
	//------------------------
	CollisionTestScene* _test;
	textMap* _testmap1;
	textMap2* _testmap2;
	textMap3* _testmap3;
	textMap4* _testmap4;
	//-------------------------

	//���÷��� bool
	bool _firstFadeOut;
	bool _secondFadeIn;
	bool _replayDone;
	int _RPcount;
	bool _RPfadeIn;





	//���λ���
	MAINSTATE _state;
	//�޸���ȿ��
	Caretaker* _caretaker;
	//�ٽý����̹���
	image* _restartBox;
	image* _restartBox_over;
	RECT _restartTextRc;
	bool _fortextNeon;
	bool _startRollback;
	int _rollbackstartCount;
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
	void sceneInit();
	void collisionInit();
	void release();
	void update();
	void render();

	void dropFrame();
	void restartBoxupdate();
	void restartBoxrender();

	void showReplay();
	void updateReplay();

	void setIsDebug(bool isDebug)
	{
		_isDebug = isDebug;
		COLLISION->setIsDebug(isDebug);
		SCENE->curScene()->setIsDebug(isDebug);
	}
};

