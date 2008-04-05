#include "GOCTBoundingBox.h"
#include "../GOCBoundingBox.h"
using namespace tlib::gocs;

GOCTBoundingBox::GOCTBoundingBox( const char *id ):
IGOCTBounding(id)
{}

IGOComponent* GOCTBoundingBox::MakeComponent() const
{
    GOCBoundingBox *goc = new GOCBoundingBox(this);
    goc->Reset();
    return goc;
}