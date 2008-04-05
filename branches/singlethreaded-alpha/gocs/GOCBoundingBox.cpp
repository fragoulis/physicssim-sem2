#include "GOCBoundingBox.h"
#include "Templates/GOCTBoundingBox.h"
using namespace tlib::gocs;

GOCBoundingBox::GOCBoundingBox()
{
    SetVolumeType(VT_BOX);
}

GOCBoundingBox::GOCBoundingBox( const GOCTBoundingBox * const tpl ):
IGOCBoundingVolume(tpl),
m_vHalfSize(tpl->GetHalfSize())
{
    SetVolumeType(VT_BOX);
}