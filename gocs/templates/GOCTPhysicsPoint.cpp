#include "GOCTPhysicsPoint.h"
#include "../GOCPhysicsPoint.h"
#include "../../Util/Config.h"
using namespace tlib::gocs;

GOCTPhysicsPoint::GOCTPhysicsPoint( const char *id ):
IGOCTemplate(id)
{
    float damp, drag;
    CFG_SERVER_OPEN;
    CFG_LOAD("Particle_Attributes");
    CFG_1f("damp", damp);
    CFG_1f("drag", drag);

    m_Body.SetDamping(damp);
    m_Body.SetDrag(drag);
}

IGOComponent* GOCTPhysicsPoint::MakeComponent() const
{
    GOCPhysicsPoint *goc = new GOCPhysicsPoint(this);
    goc->Reset();
    return goc;
}