#include "GOCTVisualIMQuad.h"
#include "../GOCVisualIMQuad.h"
using namespace tlib::gocs;

GOCTVisualIMQuad::GOCTVisualIMQuad( const char *id ):
IGOCTemplate(id),
m_bDoublesided(false)
{}

IGOComponent* GOCTVisualIMQuad::MakeComponent() const
{
    GOCVisualIMQuad *goc = new GOCVisualIMQuad(this);
    goc->Reset();
    return goc;
}