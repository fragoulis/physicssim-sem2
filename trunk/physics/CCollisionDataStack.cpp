#include "CCollisionDataStack.h"
using namespace tlib::physics;

CCollisionDataStack::CCollisionDataStack():
m_iMaxCollisionsPerFrame(0),
m_CollisionData(0)
{}

CCollisionDataStack::~CCollisionDataStack()
{
    delete[] m_CollisionData;
}

void CCollisionDataStack::Init( int iMaxCollisions )
{
    if( iMaxCollisions <= 0 ) return;
    
    m_iMaxCollisionsPerFrame = iMaxCollisions;

    // Allocate memory for the collision data and pass the pointers
    m_CollisionData = new CCollisionData[m_iMaxCollisionsPerFrame];
}

void CCollisionDataStack::ResolveCollisions( float delta )
{
    CollisionDataList::iterator i = m_CollisionList.begin();
    for(; i != m_CollisionList.end(); ++i )
    {
        (*i)->Resolve( delta );
    }
    m_CollisionList.clear();
}