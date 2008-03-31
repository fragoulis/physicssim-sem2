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
    MGRPhysics::_Get().RegisterCollidable(this);
}

// ----------------------------------------------------------------------------
bool IGOCBoundingVolume::Check( IGOCBoundingVolume *other, 
                                CCollisionData     *coldata )
{
    if( !coldata ) return false;

    assert(m_iType&&other->GetVolumeType());

    if( m_iType == VT_SPHERE && other->GetVolumeType() == VT_SPHERE )
    {
        return CollisionDetector::CheckSphereSphere( this, other, coldata );
    }

    // --- SPHERE | PLANE ---
    else if( m_iType == VT_SPHERE && other->GetVolumeType() == VT_PLANE )
    {
        return CollisionDetector::CheckSpherePlane( this, other, coldata );
    }
    //else if( m_iType == VT_PLANE && other->GetVolumeType() == VT_SPHERE )
    //{
    //    return CollisionDetector::CheckSpherePlane( other, this, coldata );
    //}

    // --- SPHERE | BOX ---
    //else if( m_iType == VT_SPHERE && other->GetVolumeType() == VT_BOX )
    //{
    //    return CollisionDetector::CheckSpherePlane( this, other, coldata );
    //}
    //else if( m_iType == VT_BOX && other->GetVolumeType() == VT_SPHERE )
    //{
    //    return CollisionDetector::CheckSpherePlane( other, this, coldata );
    //}

    // --- SPHERE | DEFORMABLE ---
    //else if( m_iType == VT_SPHERE && other->GetVolumeType() == VT_DEFORMABLE )
    //{
    //    return CollisionDetector::CheckSphereDeformable( this, other, coldata );
    //}
    //else if( m_iType == VT_DEFORMABLE && other->GetVolumeType() == VT_SPHERE )
    //{
    //    return CollisionDetector::CheckSphereDeformable( other, this, coldata );
    //}

    // --- PLANE | DEFORMABLE ---
    //else if( m_iType == VT_PLANE && other->GetVolumeType() == VT_DEFORMABLE )
    //{
    //    return CollisionDetector::CheckPlaneDeformable( this, other, coldata );
    //}
    //else if( m_iType == VT_DEFORMABLE && other->GetVolumeType() == VT_PLANE )
    //{
    //    return CollisionDetector::CheckPlaneDeformable( other, this, coldata );
    //}

    return false;
}