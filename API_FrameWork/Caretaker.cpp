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
		//_vplayerhistory.push_back(PLAYER->save());
		_vZorderhistory.push_back(ZORDER->save());
		_vCamerahistory.push_back(CAMERA->save());
	}
	ZORDER->ZorderSaveClear();
}

void Caretaker::replay()
{
	if (!isReplay) {
		isReplay = true;
		//_iplayerhistory = _vplayerhistory.begin();
		_iZorderhistory = _vZorderhistory.begin();
		_iCamerahistory = _vCamerahistory.begin();
		saveCount = 0;
	}
	else {
		//if (_iplayerhistory == _vplayerhistory.end()) {
		//	isReplay = false;
		//	replayDone = true;
		//}
		//else {
		//	PLAYER->restore(*_iplayerhistory);
		//	++_iplayerhistory;
		//	//...
		//}
		if (_iZorderhistory == _vZorderhistory.end()) {
			//복구끝 - 변수초기화
			isReplay = false;
			replayDone = true;
		}
		else {
			//복구
			ZORDER->restore(*_iZorderhistory, true);
			CAMERA->restore(*_iCamerahistory);
			++_iZorderhistory;
			++_iCamerahistory;
		}
	}
}

void Caretaker::rollback()
{
	if (!isRollback) {
		isRollback = true;
		//i = _vplayerhistory.size()-1;
		i = _vZorderhistory.size()-1;
		saveCount = 0;
	}
	else {
		if (i < 0) {
			isRollback = false;
			rollbackDone = true;
		}
		//else {
		//	PLAYER->restore(_vplayerhistory[i]);
		//	//...
		//	i -= 3;
		//}
		else {
			ZORDER->restore(_vZorderhistory[i], false);
			CAMERA->restore(_vCamerahistory[i]);
			i -= 3;
		}
	}
}

void Caretaker::allVectorClear()
{
	_vZorderhistory.clear();
	_vCamerahistory.clear();
	//_vplayerhistory.clear();
	//...
}
