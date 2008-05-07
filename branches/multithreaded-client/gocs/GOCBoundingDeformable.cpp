#include "GOCBoundingDeformable.h"
#include "Interfaces/IGOCTBounding.h"
#include "Interfaces/IGOCPhysicsDeformable.h"
#include "Interfaces/IGOCVisualVertexArray.h"
#include "CGameObject.h"
#include <cassert>
using namespace tlib::gocs;
using tlib::physics::CParticle;

GOCBoundingDeformable::GOCBoundingDeformable( const IGOCTBounding * const tpl ):
IGOCBoundingVolume(tpl),
m_iParticleIndex(0),
m_Primitive(0)
{
    SetVolumeType(VT_DEFORMABLE);
}

GOCBoundingDeformable::~GOCBoundingDeformable()
{
    delete m_Primitive;
    m_Primitive = 0;
}

void GOCBoundingDeformable::Init()
{
    IGOCBoundingVolume::Init();

    m_Body = GET_GOC( IGOCPhysicsDeformable, "Physics" );
    assert(m_Body);
    
    m_Visual = GET_GOC( IGOCVisualVertexArray, "Visual" );
    assert(m_Visual);

    assert(m_Primitive);
    m_Primitive->SetOwner(GetOwner());
}

CParticle* GOCBoundingDeformable::GetParticle()
{
    ++m_iParticleIndex;

    if( m_iParticleIndex == m_Body->GetNumOfParticles()+1 )
    {
        m_iParticleIndex = 0;
        return 0;
    }

    return m_Body->GetParticle( m_iParticleIndex-1 );
}