#ifdef _WIN32
#include <windows.h>
#endif
#include <GL/GL.h>

#include "GOCVisualVAPlane.h"
#include "Templates/GOCTVisualVAPlane.h"
#include "CGameObject.h"
using namespace tlib::gocs;

GOCVisualVAPlane::GOCVisualVAPlane( const GOCTVisualVAPlane * const tpl ):
m_vHalfSize(tpl->GetHalfSize()),
m_iStacks(tpl->GetStacks()),
m_iSlices(tpl->GetSlices())
{}

void GOCVisualVAPlane::RenderObject() const 
{
    glPushMatrix();
    {
        //const Vec3f &vPos = GetOwner()->GetTransform().GetPosition();
        //glTranslatef( vPos.x(), vPos.y(), vPos.z() );

        //float m[16];
        //const Quatf &vOri = GetOwner()->GetTransform().GetOrientation();
        //vOri.ToMatrix(m);
        //glMultMatrixf(m);

        glDisable(GL_CULL_FACE);
        glDrawElements( GL_TRIANGLES, (m_iStacks-1)*(m_iSlices-1)*6, GL_UNSIGNED_INT, m_IndexArray );
        glEnable(GL_CULL_FACE);
    }
    glPopMatrix();

} // end Render()

bool GOCVisualVAPlane::Create()
{
    const int stacks = m_iStacks;
    const int slices = m_iSlices;
    const int array_size = slices*stacks;

    m_VertexArray   = new float[array_size][3];
    m_NormalArray   = new float[array_size][3];
    m_IndexArray    = new GLuint[(stacks-1)*(slices-1)*6];
    m_TexArray      = new float[array_size][2];

    if ((stacks < 1) && (slices <1)) return false;

    const float stack_inc = 1.0f/(float)(stacks-1);
    const float slice_inc = 1.0f/(float)(slices-1);

    const Quatf &qOri = GetOwner()->GetTransform().GetOrientation();
    const Vec3f &vPos = GetOwner()->GetTransform().GetPosition();

    const float sizeX = 2.0f * m_vHalfSize.x() * stack_inc;
    const float sizeY = 2.0f * m_vHalfSize.y() * slice_inc;

    // define the vertex array
    int vertex = 0;
    for( int j=0; j<slices; ++j )
    {
        float temp_pos_y = sizeY * j - m_vHalfSize.y();
        float temp_tex_y = (float)j / (slices-1);

        for( int i=0; i<stacks; ++i )
        {
            int index = i+j*stacks;
            
            Vec3f temp( sizeX * i - m_vHalfSize.x(), temp_pos_y, 0.0f );
            qOri.Rotate( temp );
            temp += vPos;

            m_VertexArray[index][0] = temp.x();
            m_VertexArray[index][1] = temp.y();
            m_VertexArray[index][2] = temp.z();

            temp.Set( 0.0f, 0.0f, 1.0f );
            qOri.Rotate( temp );

            m_NormalArray[index][0] = temp.x();
            m_NormalArray[index][1] = temp.y();
            m_NormalArray[index][2] = temp.z();

            m_TexArray[index][0] = (float)i / (stacks-1);
            m_TexArray[index][1] = temp_tex_y;
            
            //std::cout << "Vertex[" << index << "]=(" << m_VertexArray[index][0] << ", " << m_VertexArray[index][1] << ") -- ";
            //std::cout << "Texture[" << index << "]=(" << m_TexArray[index][0] << ", " << m_TexArray[index][1] << ")" << std::endl;
        }
    }
    
    // now generate the index array
    int index = 0;
    for( int j=0; j<slices-1; ++j )
    {
        for( int i=0; i<stacks-1; ++i )
        {
            m_IndexArray[index++] = i+j*stacks;
            m_IndexArray[index++] = i+1+j*stacks;
            m_IndexArray[index++] = i+(j+1)*stacks;

            m_IndexArray[index++] = i+1+j*stacks;
            m_IndexArray[index++] = i+1+(j+1)*stacks;
            m_IndexArray[index++] = i+(j+1)*stacks;
        }
    }

    return true;
} // end Create()