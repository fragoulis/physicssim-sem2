#ifdef _WIN32
#include <windows.h>
#endif
#include <GL/GL.h>
#include "CLight.h"
using namespace tlib::util;

CLight::CLight():
m_fW(1.0f),
m_iLightId(-1),
m_bIsOn(false),
m_Ambient(CColor( 0.0f, 0.0f, 0.0f, 1.0f )),
m_Diffuse(CColor( 1.0f, 1.0f, 1.0f, 1.0f )),
m_Specular(CColor( 1.0f, 1.0f, 1.0f, 1.0f )),
m_fConstantAttenuation(1.0f),
m_fLinearAttenuation(0.0f),
m_fQuadraticAttenuation(0.0f)
{}

// ----------------------------------------------------------------------------
void CLight::Reset() 
{
    glLightfv( GL_LIGHT0+m_iLightId, GL_AMBIENT,  m_Ambient.rgba() );
    glLightfv( GL_LIGHT0+m_iLightId, GL_DIFFUSE,  m_Diffuse.rgba() );
    glLightfv( GL_LIGHT0+m_iLightId, GL_SPECULAR, m_Diffuse.rgba() );

    glLightf(GL_LIGHT0+m_iLightId, GL_CONSTANT_ATTENUATION,  m_fConstantAttenuation );
    glLightf(GL_LIGHT0+m_iLightId, GL_LINEAR_ATTENUATION,    m_fLinearAttenuation   );
    glLightf(GL_LIGHT0+m_iLightId, GL_QUADRATIC_ATTENUATION, m_fQuadraticAttenuation);
}

// ----------------------------------------------------------------------------
void CLight::Apply() const
{
    // Update the light's position
    float vfPos[] = { 
        m_vPosition.x(), 
        m_vPosition.y(), 
        m_vPosition.z(), 
        m_fW 
    };
    glLightfv( GL_LIGHT0+m_iLightId, GL_POSITION, vfPos );
}

// ----------------------------------------------------------------------------
void CLight::TurnOff()
{
    // Disable the light source
    glDisable( GL_LIGHT0 + m_iLightId );
    // Set flag to false
    m_bIsOn = false;
}

// ----------------------------------------------------------------------------
void CLight::TurnOn()
{
    // Set flag to true
    m_bIsOn = true;
    // Enable lighting
    glEnable( GL_LIGHTING );
    // Enable the light source
    glEnable( GL_LIGHT0 + m_iLightId );
}

// ----------------------------------------------------------------------------
void CLight::SetPosition( const Vec3f &vPos )
{
    // Update the object's position
    m_vPosition = vPos;

    // Update the light's position
    float vfPos[] = { vPos.x(), vPos.y(), vPos.z(), m_fW };
    glLightfv( GL_LIGHT0+m_iLightId, GL_POSITION, vfPos );
}

// ----------------------------------------------------------------------------
void CLight::SetAmbient( const CColor &ambient ) {
	m_Ambient = ambient;
	glLightfv(GL_LIGHT0+m_iLightId, GL_AMBIENT, m_Ambient.rgba());
}		

// ----------------------------------------------------------------------------
void CLight::SetDiffuse( const CColor &diffuse ) {
	m_Diffuse = diffuse;
	glLightfv( GL_LIGHT0+m_iLightId, GL_DIFFUSE, m_Diffuse.rgba() );
}

// ----------------------------------------------------------------------------
void CLight::SetSpecular( const CColor &specular ) {
	m_Specular = specular;
	glLightfv( GL_LIGHT0+m_iLightId, GL_SPECULAR, m_Diffuse.rgba() );
}

// ----------------------------------------------------------------------------
void CLight::SetConstantAttenuation( float fConstantAttenuation ) {
    m_fConstantAttenuation = fConstantAttenuation;
    glLightf(GL_LIGHT0+m_iLightId, GL_CONSTANT_ATTENUATION, m_fConstantAttenuation);
}

// ----------------------------------------------------------------------------
void CLight::SetLinearAttenuation( float fLinearAttenuation ) {
    m_fLinearAttenuation = fLinearAttenuation;
    glLightf(GL_LIGHT0+m_iLightId, GL_LINEAR_ATTENUATION, m_fLinearAttenuation);
}

// ----------------------------------------------------------------------------
void CLight::SetQuadraticAttenuation( float fQuadraticAttenuation ) {
    m_fQuadraticAttenuation = fQuadraticAttenuation;
    glLightf(GL_LIGHT0+m_iLightId, GL_QUADRATIC_ATTENUATION, m_fQuadraticAttenuation);
}