#pragma once
#include "Singleton.h"
#include "Physics/CCollisionDataStack.h"
#include <vector>
using namespace tlib::physics;

namespace tlib
{

namespace gocs
{
    class IGOCPhysics;
    class IGOCBoundingVolume;
}
using gocs::IGOCPhysics;
using gocs::IGOCBoundingVolume;

/**
 * @class CPhysicsManager
 *
 */
class CPhysicsManager : public Singleton<CPhysicsManager>
{
private:
    typedef std::vector<IGOCPhysics*>           BodyList;
    typedef std::vector<IGOCBoundingVolume*>    CollidableBodyList;
    
    //! Collision data information container
    CCollisionDataStack m_CollisionStack;

    //! Array of active physic components
    BodyList m_Bodies;

    //! Array of collidable spheres
    CollidableBodyList m_CollidableSpheres;
    
    //! Array of collidable planes
    CollidableBodyList m_CollidablePlanes;

    //! The collidable box
    // This should normaly be a list of collidable boxes, but since we know
    // we are only going to use one box, there is not need for list here
    IGOCBoundingVolume *m_Cloth;

    //! Physics' fixed time step
    float m_fTimeStep;

    //! Default world gravity
    float m_fGravity;

public:
    //! Updates all bodies
    void Update( float delta );

    //! Adds a new physical body to the list
    void RegisterPhysical( IGOCPhysics* body );

    //! Adds a new collidable body to one of the list
    void RegisterCollidable( IGOCBoundingVolume* body );

    //! Removes an physical body from the list
    void UnregisterPhysical( IGOCPhysics* body );

    //! Removes a collidable body from the list
    void UnregisterCollidable( IGOCBoundingVolume* body );

    //! Initialize the class
    void Init();

private:
    friend Singleton<CPhysicsManager>;
    CPhysicsManager();

    //! Adds a new collidable body to the list
    void RegisterCollidableSphere( IGOCBoundingVolume* body ) {
        m_CollidableSpheres.push_back(body);
    }

    //! Adds a new collidable body to the list
    void RegisterCollidablePlane( gocs::IGOCBoundingVolume* body ) {
        m_CollidablePlanes.push_back(body);
    }

    //! Checks bodies for collisions 
    void CheckCollisions();

}; // end CPhysicsManager

typedef CPhysicsManager MGRPhysics;

} // end namespace tlib