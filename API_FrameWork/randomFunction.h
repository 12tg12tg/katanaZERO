#pragma once
#include "singleton.h"

class RandomFunction : public Singleton<RandomFunction>
{
public:
	RandomFunction() { srand(GetTickCount64()); };
	~RandomFunction() {};

	inline int getInt(int num) { return rand() % num; }

	inline int getFromTo(int fromNum, int toNum) 
	{
		return rand() % (toNum - fromNum + 1) + fromNum;
	}
	inline float getFloat(float num)
	{
		//rand() => 0 ~ 32767
		//RNAD_MAX => 32767
		return ((float)rand() / (float)RAND_MAX) * num;
	}
	inline float getFloatFromTo(float min, float max)
	{
		float rnd = ((float)rand() / (float)RAND_MAX);
		return (rnd * (max - min)) + min;
	}
};