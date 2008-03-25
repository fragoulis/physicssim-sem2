#include "GOCTVisualIMQuad.h"
#include "../GOCVisualIMQuad.h"
using namespace tlib::gocs;

GOCTVisualIMQuad::GOCTVisualIMQuad( const char *id ):
IGOCTemplate(id)
{}

IGOComponent* GOCTVisualIMQuad::MakeComponent() const
{
    GOCVisualIMQuad *goc = new GOCVisualIMQuad(this);
    goc->Reset();
    return goc;
}