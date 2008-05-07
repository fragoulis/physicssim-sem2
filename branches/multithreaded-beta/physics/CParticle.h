#pragma once
#include "../Math/TVector3.h"
using tlib::math::Vec3f;

namespace tlib
{
namespace physics
{

    struct State
    {
        Vec3f x, v;
    };

    struct Derivative
    {
        Vec3f dx, dv;
    };

/**
 * @class CParticle
 *
 * The particle class describes a generic particle unit. 
 */
class CParticle
{
private:
    //! The position of the particle
    Vec3f m_vPosition;

    //! The velocity of the particle
    Vec3f m_vVelocity;;

    //! The acceleration of the particle computed by the forces acting on it
    Vec3f m_vAcceleration;

    //! The sum of forces acting on the object
    Vec3f m_vForceAccum;

    //! The damping controls how velocity is left after the update
    float m_fDamping;

    //! The inverse mass ( 1 / mass ) of the particle.
    //! Inverse mass of zero means the object is unmovable, no forces can act on it.
    float m_fInverseMass;

    //! Common drag factor for particles
    float m_fDrag;

public:
    CParticle();

    //! Updates the particle's position and velocity
    void Integrate( float delta );

    // Accessors
    const Vec3f& GetPosition() const        { return m_vPosition; }
    const Vec3f& GetVelocity() const        { return m_vVelocity; }
    const Vec3f& GetAcceleration() const    { return m_vAcceleration; }
    const Vec3f& GetForceAccum() const      { return m_vForceAccum; }
    float GetDamping() const                { return m_fDamping; }
    float GetMass() const                   { return ( 1.0f / m_fInverseMass ); }
    float GetInverseMass() const            { return m_fInverseMass; }

    Vec3f& GetPosition() { return m_vPosition; } // non-const [for speed]

    // Mutators
    void SetPosition( const Vec3f &vPosition )          { m_vPosition = vPosition; }
    void SetVelocity( const Vec3f &vVelocity )          { m_vVelocity = vVelocity; }
    void SetAcceleration( const Vec3f &vAcceleration )  { m_vAcceleration = vAcceleration; }
    void SetDamping( float fDamping )                   { m_fDamping = fDamping; }
    void SetDrag( float fDrag )                         { m_fDrag = fDrag; }
    void SetInverseMass( float fInvMass )               { m_fInverseMass = fInvMass; }
    void SetMass( float fMass )                         { m_fInverseMass = 1.0f / fMass; }

    //! Adds a force to the accumulator
    void AddForce( const Vec3f &vForce ) { m_vForceAccum += vForce; }

    //! Replaces all forces in the accumulator
    void ApplyForce( const Vec3f &vForce ) { m_vForceAccum = vForce; }

    //! Checks if the particle has infinite mass
    bool HasInfiniteMass() const { return m_fInverseMass>0.0f?false:true; }

private:
    //! Updates particle with Euler 1st order accurate method
    void IntegrateEuler( float delta );
    
    // Trial integrators
    void IntegrateMidpoint( float delta );
    void IntegrateRK4( float delta );

    Derivative Evaluate( const State &initial );
    Derivative Evaluate( const State &initial, float dt, const Derivative &d );
    Vec3f Acceleration( const State &state );

}; // end CParticle

} // end namespace physics
} // end namespace tlib