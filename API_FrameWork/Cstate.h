#pragma once
#include "CFSM.h"
class Cstate
{
protected:
	CFSM* m_pFSM;	//전체상태벡터
	string* owner;	//주인key
	
public:
	virtual void update() = 0;
	virtual void init() = 0;
	virtual void release() = 0;

};

