#include "GOCBoundingFinitePlane.h"
#include "Templates/GOCTBoundingFinitePlane.h"
#include "CGameObject.h"
using namespace tlib::gocs;

GOCBoundingFinitePlane::GOCBoundingFinitePlane( const GOCTBoundingFinitePlane * const tpl ):
GOCBoundingPlane(tpl),
m_vHalfSize(tpl->GetHalfSize()),
m_vBinormal(tpl->GetBinormal())
{
    SetVolumeType(VT_FIN_PLANE);
}