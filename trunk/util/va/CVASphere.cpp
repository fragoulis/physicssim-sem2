#ifdef _WIN32
#include <windows.h>
#endif
#include <GL/GL.h>
#define _USE_MATH_DEFINES
#include <cmath>

#include "CVASphere.h"
using namespace tlib::util;

CVASphere::CVASphere( const VArrayId &id, 
                      float radius, 
                      int stacks, 
                      int slices ):
IVertexArray(id,stacks,slices),
m_fRadius(radius)
{}

void CVASphere::Render() const
{
    glDrawElements(GL_TRIANGLE_FAN, m_iSlices+2, GL_UNSIGNED_INT, &m_Indices[0]);
    for (int i = 0; i < (m_iStacks-2); i++) { 
        glDrawElements(GL_TRIANGLE_STRIP, (m_iSlices+1)*2, GL_UNSIGNED_INT, &m_Indices[m_iSlices+2+i*(m_iSlices+1)*2]);
    };
    glDrawElements(GL_TRIANGLE_FAN, m_iSlices+2, GL_UNSIGNED_INT, &m_Indices[m_iSlices+2+(m_iStacks-2)*(m_iSlices+1)*2]);
}

bool CVASphere::Create( bool bTextured )
{
    m_bIsTextured = bTextured;

    const int stacks = m_iStacks;
    const int slices = m_iSlices;

    if ((stacks < 2) & (slices <2)) return false;

    m_iNumOfVertices = (slices+1)*(stacks-1)+2;
    m_iNumOfIndices = 2+(stacks-1)*(slices+1)*2;

    m_Vertices  = new float[m_iNumOfVertices][3];
    m_Normals   = new float[m_iNumOfVertices][3];
    m_Indices   = new unsigned int[m_iNumOfIndices];
    m_TexCoords = new float[m_iNumOfVertices][2];

    const float stack_inc = 1.0f/(float)stacks;
    const float slice_inc = float(M_PI)*2.0f/slices;

    // top point
    int vertex_count = 0;
    m_Vertices[vertex_count][0] = 0.0f; 
    m_Vertices[vertex_count][1] = m_fRadius; 
    m_Vertices[vertex_count][2] = 0.0f; 

    m_Normals[vertex_count][0] = 0.0f; 
    m_Normals[vertex_count][1] = 1.0f; 
    m_Normals[vertex_count][2] = 0.0f;

    m_TexCoords[vertex_count][0] = 0; 
    m_TexCoords[vertex_count++][1] = 1; 

    // bottom point
    m_Vertices[vertex_count][0] = 0.0f; 
    m_Vertices[vertex_count][1] = -m_fRadius; 
    m_Vertices[vertex_count][2] = 0.0f;

    m_Normals[vertex_count][0] = 0.0f; 
    m_Normals[vertex_count][1] = -1.0f; 
    m_Normals[vertex_count][2] = 0.0f;

    m_TexCoords[vertex_count][0] = 0; 
    m_TexCoords[vertex_count++][1] = 0; 


    float x, y, z;
    int temp_vc;
    float temp_tex;
    float temp_rad;
    for (int i = 1; i < stacks; i++) {
	    y = sin(float(M_PI)*(1/2.0f - stack_inc*(float)i));
	    temp_rad = cos(float(M_PI)*(1/2.0f - stack_inc*(float)i));
	    temp_vc = vertex_count;
	    temp_tex = 1.0f - stack_inc*(float)i;
	    for(int j = 0; j < slices; j++) {
		    x = cos((float)j*slice_inc); 
		    z = -sin((float)j*slice_inc);  
		    m_Vertices[vertex_count][0] = m_fRadius*temp_rad*x;
		    m_Vertices[vertex_count][1] = m_fRadius*y;
		    m_Vertices[vertex_count][2] = m_fRadius*temp_rad*z;
		    m_Normals[vertex_count][0] = temp_rad*x;
		    m_Normals[vertex_count][1] = y;
		    m_Normals[vertex_count][2] = temp_rad*z;
		    m_TexCoords[vertex_count][0] = (float)j/(float)slices;
		    m_TexCoords[vertex_count++][1] = temp_tex;
	    };
	    m_Vertices[vertex_count][0] = m_Vertices[temp_vc][0];
	    m_Vertices[vertex_count][1] = m_Vertices[temp_vc][1];
	    m_Vertices[vertex_count][2] = m_Vertices[temp_vc][2];
	    m_Normals[vertex_count][0] = m_Normals[temp_vc][0];
	    m_Normals[vertex_count][1] = m_Normals[temp_vc][1];
	    m_Normals[vertex_count][2] = m_Normals[temp_vc][2];
	    m_TexCoords[vertex_count][0] = 1;
	    m_TexCoords[vertex_count++][1] = temp_tex;
    };

    // now generate the index array
    // start with triangle fans for the top
    int index_count = 0;
    vertex_count = 2;
    m_Indices[index_count++] = 0; // very top vertex
    for(int j = 0; j<= slices; j++) {
	    m_Indices[index_count++] = vertex_count++;
    };
    vertex_count -= (slices+1);
    // now do the main strips
    for(int i = 0; i< (stacks-2); i++) {
	    for(int j = 0; j<= slices; j++) {
		    m_Indices[index_count++] = vertex_count++;
		    m_Indices[index_count++] = slices+vertex_count;
	    };
    };

    m_Indices[index_count++] = 1; // very bottom vertex
    for(int j = 0; j<= slices; j++) {
	    m_Indices[index_count++] = vertex_count+slices-j;
    };

    if( !bTextured )
    {
        delete[] m_TexCoords;
        m_TexCoords = 0;
    }

    return true;

} // Create()