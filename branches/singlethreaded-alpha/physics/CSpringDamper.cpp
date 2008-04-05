#include "CSpringDamper.h"
#include "CParticle.h"
#include "../Util/Config.h"
using namespace tlib::physics;

CSpringDamper::CSpringDamper()
{
    CFG_SERVER_OPEN;
    CFG_LOAD("Spring_Attributes");
    CFG_1f("spring", m_Ks);
    CFG_1f("damper", m_Kd);
}

void CSpringDamper::Compute()
{
    // Current length
    const Vec3f vDist = m_Particles[1]->GetPosition() - m_Particles[0]->GetPosition();
    const float fLength = vDist.Length();
    const Vec3f vNormal = vDist / fLength;

    // Spring force
    float fSpring = m_Ks * ( fLength - m_fRestLength );

    //// Current velocities
    //float fVel1 = vNormal.Dot( m_Particles[0]->GetVelocity() );
    //float fVel2 = vNormal.Dot( m_Particles[1]->GetVelocity() );

    //// Damping force
    //float fDamper = m_Kd * ( fVel1 - fVel2 );

    //// Total spring-damper force
    //Vec3f vTotal = ( fSpring - fDamper ) * vNormal;
    Vec3f vTotal = fSpring * vNormal;

    // Apply to particles
    m_Particles[0]->AddForce( vTotal );
    m_Particles[1]->AddForce(~vTotal );
}