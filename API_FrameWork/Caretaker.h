#pragma once
class Caretaker
{
private:
	//vector<player::Memento>				_vplayerhistory;
	//vector<player::Memento>::iterator	_iplayerhistory;
	//...

	vector<Zorder::Memento>				_vZorderhistory;
	vector<Zorder::Memento>::iterator	_iZorderhistory;
	vector<camera::Memento>				_vCamerahistory;
	vector<camera::Memento>::iterator	_iCamerahistory;
private:
	bool isReplay;
	bool replayDone;
	bool isRollback;
	bool rollbackDone;
	int i;
	int rollbackNum;
	
	float saveCount;
public:
	Caretaker();
	~Caretaker();

	HRESULT init(/*플레이어는 전역, ...*/);		//블릿, 몬스터 정보 가져오기.
	void snapshot();	//순간저장. 매카운트실행.
	void replay();		//벡터앞에서부터 돌면서 end까지 실행 후 클릭하면 벡터초기화
	void rollback();	//벡터마지막부터 돌면서 begin까지 실행 후 벡터 초기화

	void allVectorClear();

	//gettersetter
	bool getIsReplay() { return isReplay; }
	bool getIsRollback() { return isRollback; }
	bool getReplayDone() { return replayDone; }
	bool getRollbackDone() { return rollbackDone; }
	void setIsReplay(bool isReplay) { this->isReplay = isReplay; }
	void setIsRollback(bool isRollback) { this->isRollback = isRollback; }
	void setReplayDone(bool replayDone) { this->replayDone = replayDone; }
	void setRollbackDone(bool rollbackDone) { this->rollbackDone = rollbackDone; }
};

