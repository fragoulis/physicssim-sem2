#include <assert.h>
#include "CParticle.h"
using namespace tlib::physics;

// Gravity integration and small velocity damping by default
CParticle::CParticle():
m_vVelocity(0.0f, 0.0f, 0.0f),
m_vForceAccum(0.0f, 0.0f, 0.0f),
m_vAcceleration(0.0f, 0.0f, 0.0f),
m_fInverseMass(0.0f)
{
    m_fDamping = 0.3f;
    m_fDrag = 0.2f;
}

// ----------------------------------------------------------------------------
void CParticle::Integrate( float delta )
{
    //TODO: Remove from update lists the static particles
    //assert(delta>0.0f); ?????
    if( m_fInverseMass <= 0.0f || delta <= 0.0f ) 
    {
        m_vForceAccum.Clear();
        return;
    }

    IntegrateEuler( delta );
    //IntegrateMidpoint( delta );
    //IntegrateRK4( delta );

    m_vForceAccum.Clear();
}

// ----------------------------------------------------------------------------
void CParticle::IntegrateEuler( float delta )
{
    // Work out the acceleration from the force
    Vec3f vResultingAcc = m_vAcceleration;
    vResultingAcc.AddScaledVector( m_vForceAccum, m_fInverseMass );
    
    // Impose drag
    vResultingAcc.AddScaledVector( m_vVelocity, -m_fDrag*m_fInverseMass );

    // Update linear velocity from the acceleration
    m_vVelocity.AddScaledVector( vResultingAcc, delta );

    // Update linear position
    m_vPosition.AddScaledVector( m_vVelocity, delta );
    m_vPosition.AddScaledVector( vResultingAcc, 0.5f * delta * delta );

    // Velocity damping
    m_vVelocity *= powf( m_fDamping, delta );
}

// ----------------------------------------------------------------------------
void CParticle::IntegrateMidpoint( float delta )
{
    State state;
    state.x = m_vPosition;
    state.v = m_vVelocity;

    Derivative a = Evaluate(state);
    Derivative b = Evaluate(state, delta*0.5f, a);

    state.x = state.x + b.dx * delta;
    state.v = state.v + b.dv * delta;
    state.v *= powf( m_fDamping, delta );

    m_vPosition = state.x;
    m_vVelocity = state.v;
}

// ----------------------------------------------------------------------------
void CParticle::IntegrateRK4( float delta )
{
    State state;
    state.x = m_vPosition;
    state.v = m_vVelocity;

    Derivative a = Evaluate(state);
    Derivative b = Evaluate(state, delta*0.5f, a);
    Derivative c = Evaluate(state, delta*0.5f, b);
    Derivative d = Evaluate(state, delta, c);

    const Vec3f dxdt = (a.dx + 2.0f*(b.dx + c.dx) + d.dx) / 6.0f;
    const Vec3f dvdt = (a.dv + 2.0f*(b.dv + c.dv) + d.dv) / 6.0f;

    state.x = state.x + dxdt * delta;
    state.v = state.v + dvdt * delta;
    state.v *= powf( m_fDamping, delta );

    m_vPosition = state.x;
    m_vVelocity = state.v;
}

// ----------------------------------------------------------------------------
Derivative CParticle::Evaluate( const State &initial )
{
    Derivative output;
    output.dx = initial.v;
    output.dv = Acceleration(initial);
    return output;
}

// ----------------------------------------------------------------------------
Derivative CParticle::Evaluate( const State &initial, float dt, const Derivative &d )
{
    State state;
    state.x = initial.x + d.dx*dt;
    state.v = initial.v + d.dv*dt;

    Derivative output;
    output.dx = state.v;
    output.dv = Acceleration(state);
    return output;
}

// ----------------------------------------------------------------------------
Vec3f CParticle::Acceleration( const State &state )
{
    //const float k = 10;
    //const float b = 1;
    //return -k * state.x - b*state.v;

    // Work out the acceleration from the force
    Vec3f vResultingAcc = m_vAcceleration;
    vResultingAcc.AddScaledVector( m_vForceAccum, m_fInverseMass );
    
    // Impose drag
    vResultingAcc.AddScaledVector( state.v, -m_fDrag*m_fInverseMass );

    return vResultingAcc;
}
