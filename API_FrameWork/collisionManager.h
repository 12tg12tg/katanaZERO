#pragma once
#include "singleton.h"
#include "Collider.h"
/*
비트연산 이용
충돌체 종류 enum은 마지막이 32이고,
0부터 시작하여 1씩 증가함.
이때 각각을 (1<<enum) 연산해주면, 앞에서부터
0000 0000
0000 0001
0000 0010
0000 0100	이런식으로 표현가능.
m_arrCheck라는 배열에서는 unsigned int를 하나씩 가지는데, 처음엔 32개 모두 0임.
clearCheck 함수를 돌면서 앞에서부터 충돌확인해야하는 놈의 (1<<enum)을 확인해서
0 | (1<<enum)를 해주면, 충돌확인해야하는놈의 위치가 전부 1로 바뀜.
예를들어 clearCheck의 0번째 요소가 0000 1010 이라면,
얘는 1번 enum과 3번 enum이랑 충돌체크를 해야한다는 뜻이됨.

참고로, 충돌체끼리 쌍방중첩확인될 염려가없음.
누가더 작은 enum값인지 체크하고 그녀석의 m_arrCheck에 입력하기 때문임.
*/

union COL_ID
{
	//따로입력해서
	struct {
		DWORD left;
		DWORD right;
	};
	//합쳐서호출
	unsigned long long ID;
};

class collisionManager : public Singleton<collisionManager>
{
private:
	UINT m_arrCheck[(UINT)COLLIDER_TYPE::END];	//충돌체 종류 조합 마스크
	map<unsigned long long, bool> m_mapID;		//충돌일어난 콜라이더조합

	map<UINT, list<Collider*>> m_totalCollider;

	bool _isDebug;
public:
	collisionManager();
	~collisionManager();
	
	Collider* addCollider(vector2 pos, vector2 size, COLLIDER_TYPE type, float z, bool isRotate = false, float angle = 0);
	void update();
	void release();
	void render();

	void erase(Collider* collider);

	void CollisionCheck(COLLIDER_TYPE left, COLLIDER_TYPE right);
	void clearLog() { memset(m_arrCheck, 0, sizeof(UINT) * (UINT)COLLIDER_TYPE::END); }
	bool isCollision(Collider* pleft, Collider* pright);

	void CollisionGroup(UINT left, UINT right);
	void firstTimeBoolClear();

	map<UINT, list<Collider*>>& getTotalCollider() { return m_totalCollider; }

	void setIsDebug(bool isDebug) { _isDebug = isDebug; }
};

