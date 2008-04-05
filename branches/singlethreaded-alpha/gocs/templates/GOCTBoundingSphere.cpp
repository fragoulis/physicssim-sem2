#include "GOCTBoundingSphere.h"
#include "../GOCBoundingSphere.h"
using namespace tlib::gocs;

GOCTBoundingSphere::GOCTBoundingSphere( const char *id ):
IGOCTBounding(id)
{}

IGOComponent* GOCTBoundingSphere::MakeComponent() const
{
    GOCBoundingSphere *goc = new GOCBoundingSphere(this);
    goc->Reset();
    return goc;
}