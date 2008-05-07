#include "GOCTBoundingDWBox.h"
#include "../GOCBoundingDWBox.h"
using namespace tlib::gocs;

GOCTBoundingDWBox::GOCTBoundingDWBox( const char *id ):
IGOCTBounding(id)
{}

IGOComponent* GOCTBoundingDWBox::MakeComponent() const
{
    GOCBoundingDWBox *goc = new GOCBoundingDWBox(this);
    goc->Reset();
    return goc;
}