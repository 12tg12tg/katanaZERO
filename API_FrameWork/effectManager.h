#pragma once
#include "singleton.h"
class effect;

struct tagParticle
{
	string imgKey;
	float angle;
	float z;
	float x, y;
	int frameX, frameY;
	bool isAlpha;
	BYTE alpha;
	int count;
	float speed;
	bool isPlay;
	float gravity;
	bool isUI;
};


class effectManager : public Singleton<effectManager>
{
private:
	//이펙트 클래스 담은 벡터
	typedef vector<effect*> vEffect;
	typedef vector<effect*>::iterator viEffect;

	//이펙트 클래스 담은 벡터를 담은 맵
	typedef map<string, vEffect> mEffect;
	typedef map<string, vEffect>::iterator miEffect;

	//모든 이펙트를 관리할 벡터
	typedef vector<mEffect> vTotalEffect;
	typedef vector<mEffect>::iterator viTotalEffect;


private:
	//실제 모든 이펙트가 담겨져 있는 녀석
	vTotalEffect m_vTotalEffect;
	//파티클벡터
	vector<tagParticle> _vParticle;
public:
	effectManager();
	~effectManager();

	HRESULT init();
	void release();
	void update();
	void render();

	void addEffect(string effectKey, char* imageName, int imageW, int imageH, int frameW, int frameH, int fps, float elapsedTime, int buffer, bool isRotate = false , bool isAlpha = false, bool isSave = false, image* bwImage = nullptr);
	effect* play(string effectKey, float z, int x, int y, float radian = 0, BYTE alpha = 255);

	HRESULT addParticle(string key, float z, float x, float y, float power, float angle, int count, float gravity, bool isAlpha = false, BYTE alpha = 0, bool isUI = false);
	void deleteParticle() { _vParticle.clear(); }
};

