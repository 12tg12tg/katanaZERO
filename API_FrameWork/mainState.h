#pragma once
class mainState : public Singleton<mainState>
{
private:
	MAINSTATE _state;
	MAINSTATE _changeState = MAINSTATE::NONE;
	bool _isSlow;
public:
	mainState();
	~mainState();

	MAINSTATE getMainState() { return _state; }
	MAINSTATE getChangeState() { return _changeState; }
	void setMainState(MAINSTATE mainState) { _state = mainState; }
	void changeMainState(MAINSTATE mainState) { _changeState = mainState; }
	bool getIsSlow() { return _isSlow; }
	void setIsSlow(bool isSlow) { _isSlow = isSlow; }
};

