#include "GOCTPhysicsPoint.h"
#include "../GOCPhysicsPoint.h"
using namespace tlib::gocs;

GOCTPhysicsPoint::GOCTPhysicsPoint( const char *id ):
IGOCTemplate(id)
{}

IGOComponent* GOCTPhysicsPoint::MakeComponent() const
{
    GOCPhysicsPoint *goc = new GOCPhysicsPoint(this);
    goc->Reset();
    return goc;
}