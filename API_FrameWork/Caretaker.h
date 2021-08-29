#pragma once
class Caretaker
{
private:
	vector<player::Memento>				_vplayerhistory;
	vector<player::Memento>::iterator	_iplayerhistory;
	//...

private:
	bool isReplay;
	bool replayDone;
	bool isRollback;
	bool rollbackDone;
	int i;
	
	float saveCount;
public:
	Caretaker();
	~Caretaker();

	HRESULT init(/*�÷��̾�� ����, ...*/);		//����, ���� ���� ��������.
	void snapshot();	//��������. ��ī��Ʈ����.
	void replay();		//���;տ������� ���鼭 end���� ���� �� Ŭ���ϸ� �����ʱ�ȭ
	void rollback();	//���͸��������� ���鼭 begin���� ���� �� ���� �ʱ�ȭ

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
