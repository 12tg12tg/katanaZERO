#pragma once
class mainState : public Singleton<mainState>
{
private:
	MAINSTATE _state;
	MAINSTATE _change = MAINSTATE::NONE;
	bool _isSlow;
public:
	mainState();
	~mainState();

	MAINSTATE getMainState() { return _state; }
	MAINSTATE getChangeState() { return _change; }
	void setMainState(MAINSTATE mainState) { _state = mainState; }
	void changeMainState(MAINSTATE mainState) { _change = mainState; }
	bool getIsSlow() { return _isSlow; }
	void setIsSlow(bool isSlow) { _isSlow = isSlow; }
};

