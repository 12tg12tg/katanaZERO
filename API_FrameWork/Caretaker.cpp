#include "framework.h"
#include "Caretaker.h"

Caretaker::Caretaker()
{
}

Caretaker::~Caretaker()
{
}

HRESULT Caretaker::init()
{
	saveCount = 0;
	isReplay = false;
	isRollback = false;
	replayDone = false;
	rollbackDone = false;
	//...
	return S_OK;
}

void Caretaker::snapshot()
{
	saveCount += 1 * TIME->getGameTimeRate();
	if (saveCount >= 1) {
		saveCount = 0;
		_vplayerhistory.push_back(PLAYER->save());
	}
}

void Caretaker::replay()
{
	if (!isReplay) {
		isReplay = true;
		_iplayerhistory = _vplayerhistory.begin();
		saveCount = 0;
	}
	else {
		if (_iplayerhistory == _vplayerhistory.end()) {
			isReplay = false;
			replayDone = true;
		}
		else {
			PLAYER->restore(*_iplayerhistory);
			++_iplayerhistory;
			//...


		}
	}
}

void Caretaker::rollback()
{
	if (!isRollback) {
		isRollback = true;
		i = _vplayerhistory.size()-1;
		saveCount = 0;
	}
	else {
		if (i < 0) {
			isRollback = false;
			rollbackDone = true;
		}
		else {
			PLAYER->restore(_vplayerhistory[i]);
			//...
			i -= 3;
		}
	}
}

void Caretaker::allVectorClear()
{
	_vplayerhistory.clear();
	//...
}
