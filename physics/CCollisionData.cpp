#include "CCollisionData.h"
#include "CParticle.h"
using namespace tlib::physics;

void CCollisionData::Resolve( float delta )
{
    ResolveVelocity( delta );
    ResolveInterpenetration( delta );
}

// ----------------------------------------------------------------------------
float CCollisionData::CalculateSeperationVelocity() const
{
    Vec3f vRelativeVelocity = m_Bodies[0]->GetVelocity();
    if( m_Bodies[1] )
        vRelativeVelocity -= m_Bodies[1]->GetVelocity();

    // Find the velocity in the direction of the contact.
    return m_vContactNormal.Dot( vRelativeVelocity );
}

void CCollisionData::ResolveVelocity( float delta )
{
    // Find the velocity in the direction of the contact.
    float fSeparatingVelocity = CalculateSeperationVelocity();

    // Check whether it needs to be resolved.
    if( fSeparatingVelocity > 0 )
    {
        // The contact is either separating or stationary - there’s
        // no impulse required.
        return;
    }
    
    // Calculate the new separating velocity.
    float fNewSepVelocity = -fSeparatingVelocity * m_fRestitution;

    // Check the velocity build-up due to acceleration only.
    Vec3f vAccCausedVelocity = m_Bodies[0]->GetAcceleration();
    if( m_Bodies[1] ) 
        vAccCausedVelocity -= m_Bodies[1]->GetAcceleration();
    
    float fAccCausedSepVelocity = vAccCausedVelocity.Dot( m_vContactNormal ) * delta;

    // If we’ve got a closing velocity due to acceleration build-up,
    // remove it from the new separating velocity.
    if( fAccCausedSepVelocity < 0 )
    {
        fNewSepVelocity += m_fRestitution * fAccCausedSepVelocity;
        
        // Make sure we haven’t removed more than was
        // there to remove.
        if( fNewSepVelocity < 0 ) 
            fNewSepVelocity = 0;
    }

    float fDeltaVelocity = fNewSepVelocity - fSeparatingVelocity;
    
    // We apply the change in velocity to each object in proportion to
    // its inverse mass (i.e., those with lower inverse mass [higher
    // actual mass] get less change in velocity).
    float fTotalInverseMass = m_Bodies[0]->GetInverseMass();
    
    if( m_Bodies[1] ) fTotalInverseMass += m_Bodies[1]->GetInverseMass();

    // If all particles have infinite mass, then impulses have no effect.
    if( fTotalInverseMass <= 0 ) return;

    // Calculate the impulse to apply.
    const float fImpulse = fDeltaVelocity / fTotalInverseMass;

    // Find the amount of impulse per unit of inverse mass.
    const Vec3f vImpulsePerIMass = m_vContactNormal * fImpulse;

    // Apply impulses: they are applied in the direction of the contact,
    // and are proportional to the inverse mass.
    m_Bodies[0]->SetVelocity( 
        m_Bodies[0]->GetVelocity() +
        vImpulsePerIMass * m_Bodies[0]->GetInverseMass()
    );

    //Vec3f velocity = m_Bodies[0]->GetVelocity() * ( 1.0f - m_vContactNormal );
    //m_Bodies[0]->AddForce( m_fFriction * ~velocity );

    if( m_Bodies[1] )
    {
        // Particle 1 goes in the opposite direction.
        m_Bodies[1]->SetVelocity(
            m_Bodies[1]->GetVelocity() +
            vImpulsePerIMass * -m_Bodies[1]->GetInverseMass()
        );

        //velocity = m_Bodies[1]->GetVelocity() * ( 1.0f - m_vContactNormal );
        //m_Bodies[1]->AddForce( m_fFriction * ~velocity );
    }
} // end of ResolveVelocity()

// ----------------------------------------------------------------------------
void CCollisionData::ResolveInterpenetration( float delta )
{
    // If we don’t have any m_fPenetration, skip this step.
    if( m_fPenetration <= 0 ) return;
    
    // The movement of each object is based on its inverse mass, so
    // total that.
    float fTotalInverseMass = m_Bodies[0]->GetInverseMass();
    if( m_Bodies[1] ) fTotalInverseMass += m_Bodies[1]->GetInverseMass();
    
    // If all particles have infinite mass, then we do nothing.
    if( fTotalInverseMass <= 0 ) return;
    
    const float fPenetration = m_fPenetration / fTotalInverseMass;

    // Find the amount of penetration resolution per unit of inverse mass
    const Vec3f vMovePerIMass = m_vContactNormal * fPenetration;
    
    // Apply the m_fPenetration resolution.
    m_Bodies[0]->SetPosition(
        m_Bodies[0]->GetPosition() +
        vMovePerIMass * m_Bodies[0]->GetInverseMass());
    
    if( m_Bodies[1] )
    {
        m_Bodies[1]->SetPosition(
            m_Bodies[1]->GetPosition() -
            vMovePerIMass * m_Bodies[1]->GetInverseMass());
    }
} // end of ResolveInterpenetration()
