#include "GOCBoundingPlane.h"
#include "Templates/GOCTBoundingPlane.h"
#include "CGameObject.h"
using namespace tlib::gocs;

GOCBoundingPlane::GOCBoundingPlane( const GOCTBoundingPlane * const tpl ):
IGOCBoundingVolume(tpl),
m_bDoublesided(tpl->IsDoublesided()),
m_vNormal(tpl->GetNormal())
{
    SetVolumeType(VT_PLANE);
}

void GOCBoundingPlane::Init()
{
    IGOCBoundingVolume::Init();

    // Get owner's original orientation
    const Quatf &qRot = GetOwner()->GetTransform().GetOrientation();

    // Rotate the plane's normal accordingly
    qRot.Rotate( m_vNormal );
}