#pragma once
#include "CCollisionData.h"
#include <vector>

namespace tlib
{
namespace physics
{

/**
 * @class CCollisionDataStack
 */
class CCollisionDataStack
{
private:
    typedef std::vector<CCollisionData*> CollisionDataList;

    //! The maximum number of collisions that can be resolved in a frame
    int m_iMaxCollisionsPerFrame;

    //! Collision data container
    CCollisionData *m_CollisionData;

    //! The list of potential collision couples
    CollisionDataList m_CollisionList;

public:
    CCollisionDataStack();
    ~CCollisionDataStack();

    //! Initializes the structure
    void Init( int iMaxCollisions );

    //! Retuns the number of free
    int HaveSpots() { 
        return int(m_iMaxCollisionsPerFrame-m_CollisionList.size()); 
    }
    
    //! Returns the next available collisions data address
    CCollisionData* GetFreeSpot() 
    { 
        if( HaveSpots() <= 0 ) return 0;
        return &m_CollisionData[m_CollisionList.size()]; 
    }

    //! Adds a collision
    void AddCollision( CCollisionData *col ) { 
        m_CollisionList.push_back(col); 
    }

    //! Resolves collisions between objects
    void ResolveCollisions( float delta );
};

} // end namespace physics
} // end namespace tlib