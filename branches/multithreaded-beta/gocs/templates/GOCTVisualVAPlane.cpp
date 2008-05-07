#include "GOCTVisualVAPlane.h"
#include "../GOCVisualVAPlane.h"
using namespace tlib::gocs;

GOCTVisualVAPlane::GOCTVisualVAPlane( const char *id ):
IGOCTemplate(id)
{}

IGOComponent* GOCTVisualVAPlane::MakeComponent() const
{
    GOCVisualVAPlane *goc = new GOCVisualVAPlane(this);
    goc->Reset();
    return goc;
}