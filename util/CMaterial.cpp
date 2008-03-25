#ifdef _WIN32
#include <windows.h>
#endif
#include <GL/GL.h>
#include "CMaterial.h"
using namespace tlib::util;

CMaterial::CMaterial(): 
    m_Ambient(CColor(0.2f,0.2f,0.2f,1.0f)),
    m_Diffuse(CColor(0.8f,0.8f,0.8f,1.0f)),
    m_Specular(CColor::black()),
    m_Emission(CColor::black()),
    m_fShininess(0.0f)
{}

// ------------------------------------------------------------------------
void CMaterial::Apply() const 
{
    glMaterialfv( GL_FRONT, GL_AMBIENT,   m_Ambient.rgba() );
    glMaterialfv( GL_FRONT, GL_DIFFUSE,   m_Diffuse.rgba() );
    glMaterialfv( GL_FRONT, GL_EMISSION,  m_Emission.rgba() );
    glMaterialfv( GL_FRONT, GL_SPECULAR,  m_Specular.rgba() );
    glMaterialf(  GL_FRONT, GL_SHININESS, m_fShininess );
}