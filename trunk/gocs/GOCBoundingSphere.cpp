#include "GOCBoundingSphere.h"
#include "Templates/GOCTBoundingSphere.h"
using namespace tlib::gocs;

GOCBoundingSphere::GOCBoundingSphere( const GOCTBoundingSphere * const tpl ):
IGOCBoundingVolume(tpl),
m_fRadius(tpl->GetRadius())
{
    SetVolumeType(VT_SPHERE);
}