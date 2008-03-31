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
    Setup();
    m_Visual = GET_GOC( IGOCVisualVertexArray, "Visual" );
    assert(m_Visual);
}

CParticle* IGOCPhysicsDeformable::GetParticle( int index ) 
{
    assert((index>=0)&&(index<m_iNumOfParticles));
    return &m_Particles[index];
}

void IGOCPhysicsDeformable::Update( float delta )
{
    assert(m_Visual);

    // Update springs
    for( int i=0; i<m_iNumOfSprings; ++i )
        m_Springs[i].Compute();

    // Update particles
    for( int i=0; i<m_iNumOfParticles; ++i )
    {    
        m_Particles[i].SetAcceleration(m_vAcceleration);
        m_Particles[i].Integrate(delta);

        // Update vertices along with integration
        m_Visual->SetVertex( i, m_Particles[i].GetPosition().xyz() );
    }

    UpdateNormals();

    // After updating copy the particle's position to the object's
    //GetOwner()->GetTransform().SetPosition( m_Body.GetPosition() );

} // Update()

void IGOCPhysicsDeformable::UpdateNormals()
{
    for( int j=0; j<m_iSlices; ++j )
    {
        for( int i=0; i<m_iStacks; ++i )
        {
            int index = i+j*m_iStacks;
            Vec3f u, v1, v2;

            int bottom  = i+(j+1)*m_iStacks;
            int top     = i+(j-1)*m_iStacks;

            // Pick the left vertex
            if( i > 0 ) v1.Set( m_Visual->GetVertex(index-1) );
            else v1.Set( m_Visual->GetVertex(index) );

            // Pick the right vertex
            if( i < m_iStacks-1 ) v2.Set( m_Visual->GetVertex(index+1) );
            else v2.Set( m_Visual->GetVertex(index) );

            u = v2 - v1;

            // Pick the top
            if( j > 0 ) v1.Set( m_Visual->GetVertex(index - m_iStacks) );
            else v1.Set( m_Visual->GetVertex(index) );

            // Pick the bottom
            if( j < m_iSlices-1 ) v2.Set( m_Visual->GetVertex(index + m_iStacks) );
            else v2.Set( m_Visual->GetVertex(index) );

            v2 -= v1;

            // Calculate normal
            u.Cross(v2, v1);
            
            // Correct it
            v1.Normalize();
            m_Visual->SetNormal( index, v1.xyz() );

        } // end for( ... )
    } // end for( ... )

} // UpdateNormals()