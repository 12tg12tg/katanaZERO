#pragma once
class mainState : public Singleton<mainState>
{
private:
	MAINSTATE _state;
	bool _isSlow;
public:
	mainState();
	~mainState();

	MAINSTATE getMainState() { return _state; }
	void setMainState(MAINSTATE mainState) { _state = mainState; }
	bool getIsSlow() { return _isSlow; }
	void setIsSlow(bool isSlow) { _isSlow = isSlow; }
};

