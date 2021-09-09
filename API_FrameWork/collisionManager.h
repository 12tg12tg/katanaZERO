#pragma once
#include "singleton.h"
#include "Collider.h"
/*
��Ʈ���� �̿�
�浹ü ���� enum�� �������� 32�̰�,
0���� �����Ͽ� 1�� ������.
�̶� ������ (1<<enum) �������ָ�, �տ�������
0000 0000
0000 0001
0000 0010
0000 0100	�̷������� ǥ������.
m_arrCheck��� �迭������ unsigned int�� �ϳ��� �����µ�, ó���� 32�� ��� 0��.
clearCheck �Լ��� ���鼭 �տ������� �浹Ȯ���ؾ��ϴ� ���� (1<<enum)�� Ȯ���ؼ�
0 | (1<<enum)�� ���ָ�, �浹Ȯ���ؾ��ϴ³��� ��ġ�� ���� 1�� �ٲ�.
������� clearCheck�� 0��° ��Ұ� 0000 1010 �̶��,
��� 1�� enum�� 3�� enum�̶� �浹üũ�� �ؾ��Ѵٴ� ���̵�.

�����, �浹ü���� �ֹ���øȮ�ε� ����������.
������ ���� enum������ üũ�ϰ� �׳༮�� m_arrCheck�� �Է��ϱ� ������.
*/

union COL_ID
{
	//�����Է��ؼ�
	struct {
		DWORD left;
		DWORD right;
	};
	//���ļ�ȣ��
	unsigned long long ID;
};

class collisionManager : public Singleton<collisionManager>
{
private:
	UINT m_arrCheck[(UINT)COLLIDER_TYPE::END];	//�浹ü ���� ���� ����ũ
	map<unsigned long long, bool> m_mapID;		//�浹�Ͼ �ݶ��̴�����

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

