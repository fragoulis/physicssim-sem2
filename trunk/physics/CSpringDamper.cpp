#include "CSpringDamper.h"
#include "CParticle.h"
#include "../Util/assert.h"
using namespace tlib::physics;

const float FORCE_LIMIT = 500.0f;

CSpringDamper::CSpringDamper():
m_Ks(100.0f),
m_Kd(1.0f)
{}

void CSpringDamper::Compute()
{
    // Current length
    const Vec3f vDist = m_Particles[1]->GetPosition() - m_Particles[0]->GetPosition();
    const float fLength = vDist.Length();
    //massert( fLength>0.0f, "Spring length cannot be negative or zero!" );
    Vec3f vNormal = vDist / fLength;

    // Spring force
    const float fSpring = m_Ks * ( fLength - m_fRestLength );

    // Current velocities
    const Vec3f vVel = m_Particles[0]->GetVelocity() - m_Particles[1]->GetVelocity();

    // Damping force
    const float fDamper = m_Kd * vNormal.Dot( vVel );

    // Total spring-damper force
    Vec3f vTotal = ( fSpring - fDamper ) * vNormal;
    //Vec3f vTotal = fSpring * vNormal;

//#ifdef _DEBUG
    //massert( vTotal.Length()<10.0f, "Too big force!" );
    //const float force = vTotal.SqrLength();
    //if( force > FORCE_LIMIT )
    //{
    //    vTotal *= 1.0f / force;
    //}
//#endif

    // Apply to particles
    m_Particles[0]->AddForce( vTotal );
    m_Particles[1]->AddForce(~vTotal );
}