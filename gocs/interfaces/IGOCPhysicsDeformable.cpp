#include "IGOCPhysicsDeformable.h"
#include "IGOCVisualVertexArray.h"
#include "../CGameObject.h"

#include "../../Physics/CParticle.h"
#include "../../Physics/CSpringDamper.h"
using tlib::physics::CParticle;
using tlib::physics::CSpringDamper;
using namespace tlib::gocs;

#include <cassert>

IGOCPhysicsDeformable::~IGOCPhysicsDeformable()
{
    delete[] m_Particles;
    delete[] m_Springs;
}

void IGOCPhysicsDeformable::Init()
{
    m_Visual = GET_GOC( IGOCVisualVertexArray, "Visual" );
    assert(m_Visual);
    Setup();
}

CParticle* IGOCPhysicsDeformable::GetParticle( int index ) 
{
    assert((index>=0)&&(index<m_iNumOfParticles));
    return &m_Particles[index];
}

void IGOCPhysicsDeformable::Update( float delta )
{
    if( !IsOwnerActive() ) return;

    const float step = 0.0002f;
    while( delta >= step )
    {
        // Update springs
        for( int i=0; i<m_iNumOfSprings; ++i )
            m_Springs[i].Compute();

        // Update particles
        for( int i=0; i<m_iNumOfParticles; ++i )
            m_Particles[i].Integrate(step);

        delta -= step;
    }

} // Update()

void IGOCPhysicsDeformable::UpdateOwner() 
{
    assert(m_Visual);

    // Update vertices and normals
    for( int i=0; i<m_iNumOfParticles; ++i )
        m_Visual->SetVertex( i, m_Particles[i].GetPosition() );

    m_Visual->UpdateNormals();
}