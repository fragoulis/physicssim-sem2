#include "CPhysicsManager.h"
#include "Physics/CollisionDetector.h"
#include "GOCS/Interfaces/IGOCPhysics.h"
//#include "GOCS/Interfaces/IGOCBoundingVolume.h"
#include "GOCS/GOCBoundingDeformable.h"
#include "Util/CLogger.h"
#include <algorithm>
using namespace tlib;
using tlib::gocs::GOCBoundingDeformable;

// Helps us erase an item from a vector
#define ERASE( vector, item ) \
    vector.erase( \
        std::find( vector.begin(), vector.end(), item ) \
        );
        // ----- //

// ----------------------------------------------------------------------------
CPhysicsManager::CPhysicsManager()
{
    m_fGravity  = -5.81f; // this MUST be in the constructor
    m_fTimeStep = 0.001f;
}

void CPhysicsManager::Init()
{
    m_CollisionStack.Init(600);   
}

// ----------------------------------------------------------------------------
void CPhysicsManager::Update( float delta ) 
{
    // Integration
    if( delta <= m_fTimeStep )
    {
        BodyList::const_iterator i = m_Bodies.begin();
            for(; i!= m_Bodies.end(); ++i )
                (*i)->Update( delta );
    } 
    else {
        float accum = delta;
        while( accum >= m_fTimeStep )
        {
            BodyList::const_iterator i = m_Bodies.begin();
            for(; i!= m_Bodies.end(); ++i )
                (*i)->Update( m_fTimeStep );

            CheckCollisions();
            m_CollisionStack.ResolveCollisions( m_fTimeStep );

            accum -= m_fTimeStep;
        }
    }

    //for(; i!= m_Bodies.end(); ++i )
    //    (*i)->Update( delta );

    // Collision detection
    //CheckCollisions();

    // Collision resoving
    //m_CollisionStack.ResolveCollisions( delta );
}

// ----------------------------------------------------------------------------
void CPhysicsManager::CheckCollisions()
{
    CCollisionData *data;

    // Check each sphere...
    CollidableBodyList::const_iterator i = m_CollidableSpheres.begin();
    for(; i != m_CollidableSpheres.end(); ++i )
    {
        IGOCBoundingVolume *sphere = *i;
        
        
        // ... with all planes ...
        CollidableBodyList::const_iterator j = m_CollidablePlanes.begin();
        for(; j != m_CollidablePlanes.end(); ++j )
        {
            data = m_CollisionStack.GetFreeSpot();
            if( sphere->Check( *j, data ) )
                m_CollisionStack.AddCollision( data );
        } // end planes

        // ... with each sphere ...
        for( j = i+1; j != m_CollidableSpheres.end(); ++j )
        {
            data = m_CollisionStack.GetFreeSpot();
            if( sphere->Check( *j, data ) )
                m_CollisionStack.AddCollision( data );
        } // end spheres

        // ... with the cloth ...
        GOCBoundingDeformable *cloth = (GOCBoundingDeformable*)m_Cloth;
        if( CollisionDetector::CheckPrimitiveSphereBox( 
            sphere, 
            cloth->GetPrimitive() ) 
            )
        {
            // Check sphere with all particles of the cloth
            while( CParticle *p = cloth->GetParticle() )
            {
                data = m_CollisionStack.GetFreeSpot();
                if( CollisionDetector::CheckSphereParticle( sphere, p, data ) )
                    m_CollisionStack.AddCollision( data );
            }
        } // end cloth

    } // end for( spheres ... )

    // Check planes with all particles of the cloth
    for( i = m_CollidablePlanes.begin(); i != m_CollidablePlanes.end(); ++i )
    {
        GOCBoundingDeformable *cloth = static_cast<GOCBoundingDeformable*>(m_Cloth);
        while( CParticle *p = cloth->GetParticle() )
        {
            data = m_CollisionStack.GetFreeSpot();
            if( CollisionDetector::CheckPlaneParticle( *i, p, data ) )
                m_CollisionStack.AddCollision( data );
        }
    } // end for( planes ... )

} // CheckCollisions()

// ----------------------------------------------------------------------------
void CPhysicsManager::RegisterPhysical( IGOCPhysics* body ) 
{ 
    body->SetAcceleration( Vec3f( 0.0f, m_fGravity, 0.0f ) );
    m_Bodies.push_back(body);
}

// ----------------------------------------------------------------------------
void CPhysicsManager::UnregisterPhysical( IGOCPhysics* body )
{
    ERASE(m_Bodies, body);
}

// ----------------------------------------------------------------------------
void CPhysicsManager::RegisterCollidable( IGOCBoundingVolume* body )
{
    switch( body->GetVolumeType() )
    {
    case IGOCBoundingVolume::VT_SPHERE:
        RegisterCollidableSphere( body );
        break;

    case IGOCBoundingVolume::VT_PLANE:
        RegisterCollidablePlane( body );
        break;

    case IGOCBoundingVolume::VT_DEFORMABLE:
        m_Cloth = body;
        break;
    }
}

// ----------------------------------------------------------------------------
void CPhysicsManager::UnregisterCollidable( IGOCBoundingVolume* body )
{
    switch( body->GetVolumeType() )
    {
    case IGOCBoundingVolume::VT_SPHERE:
        ERASE(m_CollidableSpheres, body);
        break;

    case IGOCBoundingVolume::VT_PLANE:
        ERASE(m_CollidablePlanes, body);
        break;
    }
}