#ifdef _WIN32
#include <windows.h>
#endif
#include <GL/GL.h>
#include "IVertexArray.h"
using namespace tlib::util;

IVertexArray::IVertexArray( const VArrayId &id, int stacks, int slices ):
m_id(id),
m_iStacks(stacks),
m_iSlices(slices),
m_Vertices(0),
m_Normals(0),
m_TexCoords(0),
m_Indices(0),
m_bIsTextured(true)
{}

IVertexArray::~IVertexArray()
{
    delete[] m_Vertices;
    delete[] m_Normals;
    delete[] m_TexCoords;
    delete[] m_Indices;
}

void IVertexArray::Begin()
{   
    if( m_bIsTextured )
    {
        glEnableClientState (GL_TEXTURE_COORD_ARRAY);
        glTexCoordPointer   (2, GL_FLOAT, 0, m_TexCoords);
    }
    glEnableClientState (GL_NORMAL_ARRAY);
    glNormalPointer     (GL_FLOAT, 0, m_Normals);
    glEnableClientState (GL_VERTEX_ARRAY);
    glVertexPointer     (3, GL_FLOAT, 0, m_Vertices);
}

void IVertexArray::End()
{
    if( m_bIsTextured )
    {
        glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    }
    glDisableClientState(GL_NORMAL_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);
}