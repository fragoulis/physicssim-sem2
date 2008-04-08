#include "GOCTBoundingPlane.h"
#include "../GOCBoundingPlane.h"
using namespace tlib::gocs;

GOCTBoundingPlane::GOCTBoundingPlane( const char *id ):
IGOCTBounding(id),
m_bDoublesided(false)
{}

IGOComponent* GOCTBoundingPlane::MakeComponent() const
{
    GOCBoundingPlane *goc = new GOCBoundingPlane(this);
    goc->Reset();
    return goc;
}