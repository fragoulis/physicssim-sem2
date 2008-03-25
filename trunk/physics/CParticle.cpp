#include <assert.h>
#include "CParticle.h"
#include "../Time/Clock.h"
using namespace tlib::physics;
using tlib::time::Clock;

// Global constant that represents the gravitational acceleration
const float DRAG    = 0.2f;
const float DAMPING = 0.8f;

// Gravity integration and small velocity damping by default
CParticle::CParticle():
m_vVelocity(0.0f, 0.0f, 0.0f),
m_vForceAccum(0.0f, 0.0f, 0.0f),
m_vAcceleration(0.0f, 0.0f, 0.0f),
m_fDamping(DAMPING),
m_fInverseMass(0.0f)
{}

void CParticle::Integrate( float delta )
{
    //assert(delta>0.0f); ?????
    IntegrateEuler( delta );
}

void CParticle::IntegrateEuler( float delta )
{
    if( m_fInverseMass <= 0.0f ) 
    {
        m_vForceAccum.Clear();
        return;
    }

    // Work out the acceleration from the force
    Vec3f vResultingAcc = m_vAcceleration;
    vResultingAcc.AddScaledVector( m_vForceAccum, m_fInverseMass );
    
    // Impose drag
    vResultingAcc.AddScaledVector( m_vVelocity, -DRAG*m_fInverseMass );

    // Update linear velocity from the acceleration
    m_vVelocity.AddScaledVector( vResultingAcc, delta );

    // Velocity damping
    m_vVelocity *= powf( m_fDamping, delta );

    // Update linear position
    m_vPosition.AddScaledVector( m_vVelocity, delta );
    m_vPosition.AddScaledVector( vResultingAcc, 0.5f * delta * delta );

    m_vForceAccum.Clear();
}