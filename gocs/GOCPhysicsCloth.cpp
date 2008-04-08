#include "GOCPhysicsCloth.h"
#include "CGameObject.h"
#include "Templates/GOCTPhysicsCloth.h"
#include "Interfaces/IGOCVisualVertexArray.h"
using namespace tlib::gocs;

#include "../Physics/CParticle.h"
#include "../Physics/CSpringDamper.h"
using tlib::physics::CParticle;
using tlib::physics::CSpringDamper;

#include "../Util/Config.h"
#include <cassert>

GOCPhysicsCloth::GOCPhysicsCloth( const GOCTPhysicsCloth * const tpl ):
m_Static(0)
{
    m_fMass     = tpl->GetMass();    
    m_iStacks   = tpl->GetStacks();
    m_iSlices   = tpl->GetSlices();
    m_vHalfSize = tpl->GetHalfSize();

    // Compute number of particles and springs
    // [for now, we only do structural springs]

    m_iNumOfParticles = m_iStacks * m_iSlices;
    m_iNumOfSprings = 2 * m_iNumOfParticles - m_iStacks - m_iSlices;
}

// ----------------------------------------------------------------------------
GOCPhysicsCloth::~GOCPhysicsCloth()
{
    delete[] m_Static;
}

// ----------------------------------------------------------------------------
void GOCPhysicsCloth::Init()
{
    IGOCPhysicsDeformable::Init();
}

// ----------------------------------------------------------------------------
void GOCPhysicsCloth::Setup()
{
    float Ks, Kd, damp, drag;

    CFG_SERVER_OPEN;
    CFG_LOAD("Spring_Attributes");
    CFG_1f("spring", Ks);
    CFG_1f("damper", Kd);

    CFG_LOAD("Particle_Attributes");
    CFG_1f("damp", damp);
    CFG_1f("drag", drag);

    m_Particles = new CParticle[ m_iNumOfParticles ];
    m_Springs = new CSpringDamper[ m_iNumOfSprings ];

    // Get visual component
    IGOCVisualVertexArray *va = m_Visual;
    assert(va);

    // Copy position for each particle as well as mass
    // Here we know that the number of particles is the same as 
    // the number of vertices
    const float fIMass = m_iNumOfParticles / m_fMass;
    const float fRestLenX = 2.0f * m_vHalfSize.x() / (float)m_iStacks;
    const float fRestLenY = 2.0f * m_vHalfSize.y() / (float)m_iSlices;
    int spring_cnt = 0;

    for( int j=0; j<m_iSlices; ++j )
    {
        for( int i=0; i<m_iStacks; ++i )
        {
            int index = i+j*m_iStacks;

            m_Particles[index].SetPosition( Vec3f( 
                va->GetVertex(index)[0], 
                va->GetVertex(index)[1], 
                va->GetVertex(index)[2] 
            ) 
            );

            m_Particles[index].SetInverseMass( fIMass );

            // Setup springs
            if( i<m_iStacks-1)
            {
                int right = (i+1) + j*m_iStacks;
                m_Springs[spring_cnt].SetKs(Ks);
                m_Springs[spring_cnt].SetKd(Kd);
                m_Springs[spring_cnt].SetRestLength(fRestLenX);
                m_Springs[spring_cnt++].SetParticles( 
                    &m_Particles[index], 
                    &m_Particles[right] 
                );
            }

            if( j<m_iSlices-1) 
            {
                int bottom = i + (j+1)*m_iStacks;
                m_Springs[spring_cnt].SetKs(Ks);
                m_Springs[spring_cnt].SetKd(Kd);
                m_Springs[spring_cnt].SetRestLength(fRestLenY);
                m_Springs[spring_cnt++].SetParticles( 
                    &m_Particles[index], 
                    &m_Particles[bottom] 
                );
            }

            m_Particles[index].SetAcceleration( m_vAcceleration );
            m_Particles[index].SetDamping(damp);
            m_Particles[index].SetDrag(drag);

        } // for( ... i )
    } // for( ... j )
    
    SetupStaticPoints();

} // Setup()

// ----------------------------------------------------------------------------
void GOCPhysicsCloth::SetupStaticPoints()
{
    // TODO: These are to be read from file
    m_iNumOfStatics = 9;
    m_Static = new int[m_iNumOfStatics];

    // Manually set the mass of the static points to zero
    // [here the four corners]
    const int points[] =
    {
        0,
        m_iStacks/2,
        m_iStacks-1,

        m_iStacks*(m_iSlices-m_iSlices/4),
        m_iStacks*(m_iSlices-m_iSlices/2),
        m_iStacks*(m_iSlices-3*m_iSlices/4),

        m_iStacks*(m_iSlices-1),
        m_iStacks*m_iSlices-m_iStacks/2,
        m_iStacks*m_iSlices-1
    };

    for( int i=0; i<m_iNumOfStatics; ++i )
    {
        m_Static[i] = points[i];
        assert(m_Static[i]<m_iNumOfParticles);
        m_Particles[m_Static[i]].SetInverseMass(0.0f);
    }
}

// ----------------------------------------------------------------------------
void GOCPhysicsCloth::Rotate( const Quatf &rot )
{
    for( int i=0; i<m_iNumOfStatics; ++i )
    {
        rot.Rotate( m_Particles[m_Static[i]].GetPosition() );
    }
}

// ----------------------------------------------------------------------------
const Vec3f& GOCPhysicsCloth::GetStaticPos( int index )
{
    assert(index<m_iNumOfStatics);
    return m_Particles[m_Static[index]].GetPosition();
}