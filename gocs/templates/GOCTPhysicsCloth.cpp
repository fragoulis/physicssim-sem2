#include "GOCTPhysicsCloth.h"
#include "../GOCPhysicsCloth.h"
using namespace tlib::gocs;

GOCTPhysicsCloth::GOCTPhysicsCloth( const char *id ):
IGOCTemplate(id)
{}

IGOComponent* GOCTPhysicsCloth::MakeComponent() const
{
    GOCPhysicsCloth *goc = new GOCPhysicsCloth(this);
    goc->Reset();
    return goc;
}