#include "GOCTVisualVASphere.h"
#include "../GOCVisualVASphere.h"
using namespace tlib::gocs;

GOCTVisualVASphere::GOCTVisualVASphere( const char *id ):
IGOCTemplate(id)
{}

IGOComponent* GOCTVisualVASphere::MakeComponent() const
{
    GOCVisualVASphere *goc = new GOCVisualVASphere(this);
    goc->Reset();
    return goc;
}