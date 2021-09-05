#pragma once
#include "singleton.h"
class UI;
class mainState : public Singleton<mainState>
{
private:
	UI* _ui;

	MAINSTATE _state;
	MAINSTATE _changeState = MAINSTATE::NONE;
	bool _isSlow;
	bool _cantSlow;
public:
	mainState();
	~mainState();

	MAINSTATE getMainState() { return _state; }
	MAINSTATE getChangeState() { return _changeState; }
	void setMainState(MAINSTATE mainState) { _state = mainState; }
	void changeMainState(MAINSTATE mainState) { _changeState = mainState; }
	bool getIsSlow() { return _isSlow; }
	void setIsSlow(bool isSlow) { _isSlow = isSlow; }
	void setUIlink(UI* ui) { _ui = ui; }
	UI* getUIlink() { return _ui; }
	bool getCantSlow() { return _cantSlow; }
	void setCantSlow(bool cs) { _cantSlow = cs; }
};

