#include "IGOCBoundingVolume.h"
#include "IGOCTBounding.h"
#include "../../CPhysicsManager.h"
#include "../../Physics/CollisionDetector.h"
#include <cassert>
using namespace tlib::gocs;
using tlib::physics::CCollisionData;
using tlib::physics::CollisionDetector;

IGOCBoundingVolume::IGOCBoundingVolume():
m_fElasticity(1.0f),
m_fFriction(0.0f)
{}

IGOCBoundingVolume::IGOCBoundingVolume( const IGOCTBounding * const tpl ):
m_fElasticity(tpl->GetElasticity()),
m_fFriction(tpl->GetFriction())
{}

IGOCBoundingVolume::~IGOCBoundingVolume()
{
    MGRPhysics::_Get().UnregisterCollidable(this);
}

// ----------------------------------------------------------------------------
void IGOCBoundingVolume::SetVolumeType( VolumeType iType )
{
    m_iType = iType;
}

// ----------------------------------------------------------------------------
void IGOCBoundingVolume::Init()
{
    MGRPhysics::_Get().RegisterCollidable(this);
}

// ----------------------------------------------------------------------------
bool IGOCBoundingVolume::Check( IGOCBoundingVolume *other, 
                                CCollisionData     *coldata )
{
    if( !coldata ) return false;

    assert(m_iType&&other->GetVolumeType());
    
    return CollisionDetector::Check( this, other, coldata );
}