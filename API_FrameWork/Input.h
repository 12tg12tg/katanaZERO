#pragma once
#include "singleton.h"
#include<bitset>
#define KEYMAX 256
class Input : public Singleton<Input>
{
private:
	bitset<KEYMAX> _keyUp;
	bitset<KEYMAX> _keyDown;
	bool _isAnyKeyDown;
public:
	Input();
	~Input();

	HRESULT init();
	void release();

	bool isOnceKeyDown(int key); //�ѹ��� ������
	bool isOnceKeyUp(int key); //�ѹ������ٰ��³�
	bool isStayKeyDown(int key); //�������ֳ�
	bool isToggleKey(int key);//���Ű��

	void setIsAnyKeyDown(bool isanykeydown) { _isAnyKeyDown = isanykeydown; }
	bool isAnyKeyDown() { return _isAnyKeyDown; }
};

