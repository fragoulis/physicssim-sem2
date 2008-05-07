#pragma once
#include "../Math/TVector3.h"
using tlib::math::Vec3f;

namespace tlib
{
namespace physics
{

    class CParticle;

/**
 * @class CCollisionData
 *
 * Simple storage class.
 * This class holds pointers to two collided objects with information about 
 * their collision.
 */
class CCollisionData
{
private:
    //! Pointer to the two objects
    CParticle *m_Bodies[2];

    //! The restitution coefficient
    float m_fRestitution;

    //! The friction coefficient
    float m_fFriction;

    //! The amount of penetration
    float m_fPenetration;

    //! The contact normal
    Vec3f m_vContactNormal;

public:
    // Mutators
    void SetBody( int index, CParticle *body )  { m_Bodies[ index ] = body; }
    void SetRestitution( float r )              { m_fRestitution = r; }
    void SetFriction( float f )                 { m_fFriction = f; }
    void SetPenetration( float p )              { m_fPenetration = p; }
    void SetNormal( const Vec3f &n )            { m_vContactNormal = n; }

    //! Calculates penetration and relative velocities
    void Resolve( float delta );

    //! Calculates the separating velocity between the two objects
    float CalculateSeperationVelocity() const;

private:
    void ResolveVelocity( float delta );
    void ResolveInterpenetration( float delta );
};

} // end namespace physics
} // end namespace tlib