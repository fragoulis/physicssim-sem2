#pragma once

namespace tlib
{
namespace physics
{

class CParticle;

/**
 * @class CSpringDamper
 *
 * Spring simulator.
 * This structures holds the necessary physics information
 * to simulate the spring physics between two particles.
 */
class CSpringDamper
{
private:
    //! Spring's two particles
    CParticle *m_Particles[2];

    //! Spring coefficient
    float m_Ks;

    //! Damping factor
    float m_Kd;

    //! Spring's rest length
    float m_fRestLength;

public:
    CSpringDamper();
    
    // Accessors
    float GetKs() const { return m_Ks; }
    float GetKd() const { return m_Kd; }
    float GetRestLength() const { return m_fRestLength; }

    // Mutators
    void SetParticle( int index, CParticle *p ) { m_Particles[index] = p; }
    void SetParticles( CParticle *p1, CParticle *p2 ) { 
        m_Particles[0] = p1; m_Particles[1] = p2; 
    }

    void SetKs( float k ) { m_Ks = k; }
    void SetKd( float d ) { m_Kd = d; }
    void SetRestLength( float l ) { m_fRestLength = l; }
    
    //! Computes the forces acting on the particles
    void Compute();
};

} // end namespace physics
} // end namespace tlib
