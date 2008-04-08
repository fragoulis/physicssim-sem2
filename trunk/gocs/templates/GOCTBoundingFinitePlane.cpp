#include "GOCTBoundingFinitePlane.h"
#include "../GOCBoundingFinitePlane.h"
using namespace tlib::gocs;

GOCTBoundingFinitePlane::GOCTBoundingFinitePlane( const char *id ):
GOCTBoundingPlane(id)
{}

IGOComponent* GOCTBoundingFinitePlane::MakeComponent() const
{
    GOCBoundingFinitePlane *goc = new GOCBoundingFinitePlane(this);
    goc->Reset();
    return goc;
}