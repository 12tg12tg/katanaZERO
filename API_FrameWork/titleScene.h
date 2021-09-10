#pragma once
#include "gameNode.h"
class titleScene : public gameNode
{
private:
	image* _bg;
	image* _gr;
	image* _pl;
	image* _katana;
	animation* _grassani;

	int _offsetTop;
	int _offsetBottom;

	bool _neonOn;
	bool _fortextNeon;
	RECT _pressBox;
	int _count;

	bool _nextScene;
	int _fadeOffset;
public:
	titleScene();
	~titleScene();

	HRESULT init();
	void release();
	void update();
	void render();

};

