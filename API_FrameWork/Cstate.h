#pragma once
#include "CFSM.h"
class Cstate
{
protected:
	CFSM* m_pFSM;	//��ü���º���
	string* owner;	//����key
	
public:
	virtual void update() = 0;
	virtual void init() = 0;
	virtual void release() = 0;

};

