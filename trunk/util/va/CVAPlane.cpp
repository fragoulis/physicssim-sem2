#ifdef _WIN32
#include <windows.h>
#endif
#include <GL/GL.h>
#define _USE_MATH_DEFINES
#include <cmath>

#include "CVAPlane.h"
using namespace tlib::util;

CVAPlane::CVAPlane( const VArrayId &id, 
                    const Vec2f &halfsize, 
                    int stacks, 
                    int slices ):
IVertexArray(id,stacks,slices),
m_vHalfSize(halfsize)
{}

void CVAPlane::Render() const
{
    glDrawElements( GL_TRIANGLES, m_iNumOfIndices, GL_UNSIGNED_INT, m_Indices );
}

bool CVAPlane::Create( bool bTextured )
{
    m_bIsTextured = bTextured;

    const int stacks = m_iStacks;
    const int slices = m_iSlices;

    if( (stacks<1) && (slices<1) ) return false;

    m_iNumOfVertices = slices*stacks;
    m_iNumOfIndices = (stacks-1)*(slices-1)*6;

    m_Vertices  = new float[m_iNumOfVertices][3];
    m_Normals   = new float[m_iNumOfVertices][3];
    m_Indices   = new unsigned int[m_iNumOfIndices];
    m_TexCoords = new float[m_iNumOfVertices][2];

    const float stack_inc = 1.0f/(float)(stacks-1);
    const float slice_inc = 1.0f/(float)(slices-1);

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

            m_Vertices[index][0] = temp.x();
            m_Vertices[index][1] = temp.y();
            m_Vertices[index][2] = temp.z();

            temp.Set( 0.0f, 0.0f, 1.0f );

            m_Normals[index][0] = temp.x();
            m_Normals[index][1] = temp.y();
            m_Normals[index][2] = temp.z();

            m_TexCoords[index][0] = (float)i / (stacks-1);
            m_TexCoords[index][1] = temp_tex_y;
        }
    }
    
    // now generate the index array
    int index = 0;
    for( int j=0; j<slices-1; ++j )
    {
        for( int i=0; i<stacks-1; ++i )
        {
            m_Indices[index++] = i+j*stacks;
            m_Indices[index++] = i+1+j*stacks;
            m_Indices[index++] = i+(j+1)*stacks;

            m_Indices[index++] = i+1+j*stacks;
            m_Indices[index++] = i+1+(j+1)*stacks;
            m_Indices[index++] = i+(j+1)*stacks;
        }
    }

    if( !bTextured )
    {
        delete[] m_TexCoords;
        m_TexCoords = 0;
    }

    return true;

} // Create()