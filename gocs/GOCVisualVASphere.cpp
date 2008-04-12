#ifdef _WIN32
#include <windows.h>
#endif
#include <GL/GL.h>

#include "GOCVisualVASphere.h"
#include "Templates/GOCTVisualVASphere.h"
#include "CGameObject.h"
using namespace tlib::gocs;

GOCVisualVASphere::GOCVisualVASphere( const GOCTVisualVASphere * const tpl ):
m_fRadius(tpl->GetRadius()),
m_iStacks(tpl->GetStacks()),
m_iSlices(tpl->GetSlices())
{}

void GOCVisualVASphere::RenderObject() const 
{
    glPushMatrix();
    {
        const Vec3f &vPos = GetOwner()->GetPosition();
        glTranslatef( vPos.x(), vPos.y(), vPos.z() );

        //float m[16];
        //const Quatf &vOri = GetOwner()->GetOrientation();
        //vOri.ToMatrix(m);
        //glMultMatrixf(m);

        // Draw sphere
        glDrawElements(GL_TRIANGLE_FAN, m_iSlices+2, GL_UNSIGNED_INT, &m_IndexArray[0]);
        for (int i = 0; i < (m_iStacks-2); i++) { 
            glDrawElements(GL_TRIANGLE_STRIP, (m_iSlices+1)*2, GL_UNSIGNED_INT, &m_IndexArray[m_iSlices+2+i*(m_iSlices+1)*2]);
        }
        glDrawElements(GL_TRIANGLE_FAN, m_iSlices+2, GL_UNSIGNED_INT, &m_IndexArray[m_iSlices+2+(m_iStacks-2)*(m_iSlices+1)*2]);
    }
    glPopMatrix();

} // end Render()

bool GOCVisualVASphere::Create()
{
    int array_size;
    float stack_inc;
    float slice_inc;
    float x, y, z;
    int vertex_count;
    int index_count;
    int temp_vc;
    float temp_tex;
    float temp_rad;
    int stacks = m_iStacks;
    int slices = m_iSlices;

    array_size = (slices+1)*(stacks-1)+2;	// +2 is for the top and bottom vertex

    m_VertexArray   = new float[array_size][3];
    m_NormalArray   = new float[array_size][3];
    m_IndexArray    = new GLuint[2+(stacks-1)*(slices+1)*2];
    m_TexArray      = new float[array_size][2];

    if ((stacks < 2) & (slices <2)) return false;

    stack_inc = 1.0f/(float)stacks;
    slice_inc = float(M_PI)*2.0f/slices;

    // define the vertex array

    // top point
    vertex_count = 0;
    m_VertexArray[vertex_count][0] = 0.0f; 
    m_VertexArray[vertex_count][1] = m_fRadius; 
    m_VertexArray[vertex_count][2] = 0.0f; 

    m_NormalArray[vertex_count][0] = 0.0f; 
    m_NormalArray[vertex_count][1] = 1.0f; 
    m_NormalArray[vertex_count][2] = 0.0f;

    m_TexArray[vertex_count][0] = 0; 
    m_TexArray[vertex_count++][1] = 1; 

    // bottom point
    m_VertexArray[vertex_count][0] = 0.0f; 
    m_VertexArray[vertex_count][1] = -m_fRadius; 
    m_VertexArray[vertex_count][2] = 0.0f;

    m_NormalArray[vertex_count][0] = 0.0f; 
    m_NormalArray[vertex_count][1] = -1.0f; 
    m_NormalArray[vertex_count][2] = 0.0f;

    m_TexArray[vertex_count][0] = 0; 
    m_TexArray[vertex_count++][1] = 0; 


    for (int i = 1; i < stacks; i++) {
	    y = sin(float(M_PI)*(1/2.0f - stack_inc*(float)i));
	    temp_rad = cos(float(M_PI)*(1/2.0f - stack_inc*(float)i));
	    temp_vc = vertex_count;
	    temp_tex = 1.0f - stack_inc*(float)i;
	    for(int j = 0; j < slices; j++) {
		    x = cos((float)j*slice_inc); 
		    z = -sin((float)j*slice_inc);  
		    m_VertexArray[vertex_count][0] = m_fRadius*temp_rad*x;
		    m_VertexArray[vertex_count][1] = m_fRadius*y;
		    m_VertexArray[vertex_count][2] = m_fRadius*temp_rad*z;
		    m_NormalArray[vertex_count][0] = temp_rad*x;
		    m_NormalArray[vertex_count][1] = y;
		    m_NormalArray[vertex_count][2] = temp_rad*z;
		    m_TexArray[vertex_count][0] = (float)j/(float)slices;
		    m_TexArray[vertex_count++][1] = temp_tex;
	    };
	    m_VertexArray[vertex_count][0] = m_VertexArray[temp_vc][0];
	    m_VertexArray[vertex_count][1] = m_VertexArray[temp_vc][1];
	    m_VertexArray[vertex_count][2] = m_VertexArray[temp_vc][2];
	    m_NormalArray[vertex_count][0] = m_NormalArray[temp_vc][0];
	    m_NormalArray[vertex_count][1] = m_NormalArray[temp_vc][1];
	    m_NormalArray[vertex_count][2] = m_NormalArray[temp_vc][2];
	    m_TexArray[vertex_count][0] = 1;
	    m_TexArray[vertex_count++][1] = temp_tex;
    };

    // now generate the index array
    // start with triangle fans for the top
    index_count = 0;
    vertex_count =2;
    m_IndexArray[index_count++] = 0; // very top vertex
    for(int j = 0; j<= slices; j++) {
	    m_IndexArray[index_count++] = vertex_count++;
    };
    vertex_count -= (slices+1);
    // now do the main strips
    for(int i = 0; i< (stacks-2); i++) {
	    for(int j = 0; j<= slices; j++) {
		    m_IndexArray[index_count++] = vertex_count++;
		    m_IndexArray[index_count++] = slices+vertex_count;
	    };
    };

    m_IndexArray[index_count++] = 1; // very bottom vertex
    for(int j = 0; j<= slices; j++) {
	    m_IndexArray[index_count++] = vertex_count+slices-j;
    };

    return true;
} // end Create()