#include "framework.h"
#include "collisionManager.h"

collisionManager::collisionManager()
{
}

collisionManager::~collisionManager()
{
}

Collider* collisionManager::addCollider(vector2 pos, vector2 size, COLLIDER_TYPE type, float z, bool isRotate, float angle)
{
	Collider* newCollider = new Collider;
	newCollider->init(pos, size, type, z, isRotate, angle);

	auto iter = m_totalCollider.find((UINT)type);
	if (iter == m_totalCollider.end()){
		list<Collider*> lCollider;
		lCollider.push_back(newCollider);
		m_totalCollider.insert(make_pair((UINT)type, lCollider));
	}
	else {
		iter->second.push_back(newCollider);
	}

	return newCollider;
}

void collisionManager::update()
{
	for (UINT i = 0; i < (UINT)COLLIDER_TYPE::END; ++i)
	{
		for (UINT j = i; j < (UINT)COLLIDER_TYPE::END; ++j)
		{
			if (m_arrCheck[i] & (1 << j))
			{
				CollisionGroup(i, j);
			}
		}
	}
}

void collisionManager::release()
{
	auto iter = m_totalCollider.begin();
	for (iter; iter != m_totalCollider.end(); ++iter)
	{
		auto listIter = iter->second.begin();
		for (listIter; listIter != iter->second.end(); ++listIter)
		{
			SAFE_DELETE(*listIter);
		}
		iter->second.clear();
	}
	m_totalCollider.clear();
	clearLog();
}

void collisionManager::render()
{
	if (!_isDebug) return;
	auto iter = m_totalCollider.begin();
	for (iter; iter != m_totalCollider.end(); ++iter)
	{
		auto listIter = iter->second.begin();
		for (listIter; listIter != iter->second.end(); ++listIter)
		{
			(*listIter)->render();
		}
	}
}

void collisionManager::erase(Collider* collider)
{
	auto mIter = m_totalCollider.find((UINT)collider->getType());
	if (mIter == m_totalCollider.end()) return;
	list<Collider*>& ltCollider = mIter->second;
	list<Collider*>::iterator ltIter = ltCollider.begin();
	for (ltIter; ltIter != ltCollider.end(); ++ltIter)
	{
		if ((*ltIter)->getID() == collider->getID()) {
			SAFE_DELETE(*ltIter);
			ltCollider.erase(ltIter);
			return;
		}
	}
}

void collisionManager::CollisionCheck(COLLIDER_TYPE left, COLLIDER_TYPE right)
{
	UINT iIdx = (UINT)((left < right) ? left : right);
	UINT iBitIdx = (iIdx == (UINT)left) ? (UINT)right : (UINT)left;
	m_arrCheck[iIdx] |= (1 << iBitIdx);
}

bool collisionManager::isCollision(Collider* pleft, Collider* pright)
{
	if (!pleft->_isRotate && !pright->_isRotate) {
		float fDist = abs(pleft->getPos().x - pright->getPos().x);
		float fSize = pleft->getSize().x / 2.f + pright->getSize().x / 2.f;

		//x축으로 겹치는지 먼저 확인
		if (fDist < fSize)
		{
			//y축으로도 겹치는지 확인
			fDist = abs(pleft->getPos().y - pright->getPos().y);
			fSize = pleft->getSize().y / 2.f + pright->getSize().y / 2.f;

			if (fDist < fSize)
			{
				return true;
			}
		}
		return false;
	}
	else {
		return OBB->isOBBCollision(pleft->getRect(), pleft->_rotate, pright->getRect(), pright->_rotate);
	}
}

void collisionManager::CollisionGroup(UINT left, UINT right)
{
	//아직 충돌체가 없는 enum이라면 나간다.
	auto mapiter1 = m_totalCollider.find(left);
	auto mapiter2 = m_totalCollider.find(right);
	if (mapiter1 == m_totalCollider.end() || mapiter2 == m_totalCollider.end()) return;

	//둘다 충돌체를 하나이상 보유하고 있다면,
	list<Collider*> ltLeft = mapiter1->second;
	list<Collider*> ltRight = mapiter2->second;
	list<Collider*>::iterator listIter1 = ltLeft.begin();
	list<Collider*>::iterator listIter2 = ltRight.begin();

	COL_ID colID;
	for (listIter1; listIter1 != ltLeft.end(); ++listIter1)
	{
		for (listIter2; listIter2 != ltRight.end(); ++listIter2)
		{
			colID.left = (*listIter1)->getID();
			colID.right = (*listIter2)->getID();

			map<unsigned long long, bool>::iterator idIter = m_mapID.find(colID.ID);

			//exit 초기화
			(*listIter1)->_isExit = false;
			(*listIter2)->_isExit = false;

			//충돌체크
			if (isCollision((*listIter1), (*listIter2)))
			{
				//충돌인데, 충돌기록(id)에 없다면
				if (idIter == m_mapID.end())
				{
					m_mapID.insert(make_pair(colID.ID, true));
					(*listIter1)->_isEnter = true;
					(*listIter2)->_isEnter = true;
				}
				//충돌기록은 있는데, 이전에 충돌하지 않고있었다면
				else if (idIter->second == false)
				{
					idIter->second = true;
					(*listIter1)->_isEnter = true;
					(*listIter2)->_isEnter = true;
				}
				//충돌기록도있고, 이전에 충돌중이었다면
				else {
					(*listIter1)->_isEnter = false;
					(*listIter2)->_isEnter = false;
					(*listIter1)->_isIng = true;
					(*listIter2)->_isIng = true;
				}
			}
			//충돌하고있지 않는데
			else {
				//이전 충돌기록이 있으며, 아직 true인 경우
				if (idIter != m_mapID.end() && idIter->second == true) {
					idIter->second = false;
					(*listIter1)->_isIng = false;
					(*listIter2)->_isIng = false;
					(*listIter1)->_isExit = true;
					(*listIter2)->_isExit = true;
				}
			}
		}
	}
}
