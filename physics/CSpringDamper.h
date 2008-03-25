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
    float m_k;

    //! Damping factor
    float m_d;

    //! Spring's rest length
    float m_fRestLength;

public:
    CSpringDamper();
    
    // Accessors
    float GetK() const { return m_k; }
    float GetD() const { return m_d; }
    float GetRestLength() const { return m_fRestLength; }

    // Mutators
    void SetParticle( int index, CParticle *p ) { m_Particles[index] = p; }
    void SetParticles( CParticle *p1, CParticle *p2 ) { 
        m_Particles[0] = p1; m_Particles[1] = p2; 
    }

    void SetK( float k ) { m_k = k; }
    void SetD( float d ) { m_d = d; }
    void SetRestLength( float l ) { m_fRestLength = l; }
    
    //! Computes the forces acting on the particles
    void Compute();
};

} // end namespace physics
} // end namespace tlib
