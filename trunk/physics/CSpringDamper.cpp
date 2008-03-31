#include "CSpringDamper.h"
#include "CParticle.h"
using namespace tlib::physics;

CSpringDamper::CSpringDamper()
{
    m_k = 200.0f;
    m_d = 1.0f;
}

void CSpringDamper::Compute()
{
    // Current length
    const Vec3f vDist = m_Particles[1]->GetPosition() - m_Particles[0]->GetPosition();
    const float fLength = vDist.Length();
    const Vec3f vNormal = vDist / fLength;

    // Spring force
    float fSpring = m_k * ( fLength - m_fRestLength );

    //// Current velocities
    //float fVel1 = vNormal.Dot( m_Particles[0]->GetVelocity() );
    //float fVel2 = vNormal.Dot( m_Particles[1]->GetVelocity() );

    //// Damping force
    //float fDamper = m_d * ( fVel1 - fVel2 );

    //// Total spring-damper force
    //Vec3f vTotal = ( fSpring - fDamper ) * vNormal;
    Vec3f vTotal = fSpring * vNormal;

    // Apply to particles
    m_Particles[0]->AddForce( vTotal );
    m_Particles[1]->AddForce(~vTotal );
}