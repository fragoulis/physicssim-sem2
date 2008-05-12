#include "CSpringDamper.h"
#include "CParticle.h"
using namespace tlib::physics;

const float FORCE_LIMIT = 1500.0f;

#define _USE_DAMPER

CSpringDamper::CSpringDamper():
m_Ks(100.0f),
m_Kd(1.0f)
{}

void CSpringDamper::Compute()
{
    // Current length
    const Vec3f vDist = m_Particles[1]->GetPosition() - m_Particles[0]->GetPosition();
    const float fLength = vDist.Length();
    Vec3f vNormal = vDist / fLength;

    // Spring force
    const float fSpring = m_Ks * ( fLength - m_fRestLength );

    // Current velocities
    const Vec3f vVel = m_Particles[0]->GetVelocity() - m_Particles[1]->GetVelocity();

#ifdef _USE_DAMPER
    // Damping force
    const float fDamper = m_Kd * vNormal.Dot( vVel );

    // Total spring-damper force
    Vec3f vTotal = ( fSpring - fDamper ) * vNormal;
#else
	Vec3f vTotal = fSpring * vNormal;
#endif

#ifdef _DEBUG
    const float force = vTotal.SqrLength();
    if( force > FORCE_LIMIT )
    {
        vTotal *= 1.0f / force;
    }
#endif

    // Apply to particles
    m_Particles[0]->AddForce( vTotal );
    m_Particles[1]->AddForce(~vTotal );
}