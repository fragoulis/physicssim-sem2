#ifdef _WIN32
#include <windows.h>
#endif
#include <GL/GL.h>
#define _USE_MATH_DEFINES
#include <cmath>

#include "GOCVisualIMQuad.h"
#include "Templates/GOCTVisualIMQuad.h"
#include "CGameObject.h"
using namespace tlib::gocs;

GOCVisualIMQuad::GOCVisualIMQuad( const GOCTVisualIMQuad * const tpl ):
IGOCVisual(),
m_vHalfSize(tpl->GetHalfSize())
{}

GOCVisualIMQuad::~GOCVisualIMQuad() 
{}

void GOCVisualIMQuad::Render() const 
{
    glPushMatrix();
    {
        const Vec3f &vPos = GetOwner()->GetTransform().GetPosition();
        glTranslatef( vPos.x(), vPos.y(), vPos.z() );

        float m[16];
        const Quatf &vOri = GetOwner()->GetTransform().GetOrientation();
        vOri.ToMatrix(m);
        glMultMatrixf(m);

        glBegin(GL_QUADS);
        {
            glNormal3f( 0.0f, 0.0f, 1.0f );
            glTexCoord2f( 0.0f, 0.0f ); glVertex3f( -m_vHalfSize.x(), -m_vHalfSize.y(), 0.0f );
            glTexCoord2f( 1.0f, 0.0f ); glVertex3f(  m_vHalfSize.x(), -m_vHalfSize.y(), 0.0f );
            glTexCoord2f( 1.0f, 1.0f ); glVertex3f(  m_vHalfSize.x(),  m_vHalfSize.y(), 0.0f );
            glTexCoord2f( 0.0f, 1.0f ); glVertex3f( -m_vHalfSize.x(),  m_vHalfSize.y(), 0.0f );
        }
        glEnd();
    }
    glPopMatrix();

} // end Render()