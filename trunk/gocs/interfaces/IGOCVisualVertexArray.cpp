#ifdef _WIN32
#include <windows.h>
#endif
#include <GL/GL.h>

#include "IGOCVisualVertexArray.h"
using namespace tlib::gocs;

IGOCVisualVertexArray::IGOCVisualVertexArray( int stacks, int slices ):
m_iStacks(stacks),
m_iSlices(slices),
m_VertexArray(0),
m_NormalArray(0),
m_TexArray(0),
m_IndexArray(0)
{}

IGOCVisualVertexArray::~IGOCVisualVertexArray()
{
    delete[] m_VertexArray;
    delete[] m_NormalArray;
    delete[] m_TexArray;
    delete[] m_IndexArray;
}

void IGOCVisualVertexArray::Init()
{
    IGOCVisual::Init();
    Create();
}

void IGOCVisualVertexArray::Render() const
{
    if( !IsOwnerActive() ) return;

    // Enable client states
    glEnableClientState (GL_TEXTURE_COORD_ARRAY);
    glTexCoordPointer   (2, GL_FLOAT, 0, m_TexArray);
    glEnableClientState (GL_NORMAL_ARRAY);
    glNormalPointer     (GL_FLOAT, 0, m_NormalArray);
    glEnableClientState (GL_VERTEX_ARRAY);
    glVertexPointer     (3, GL_FLOAT, 0, m_VertexArray);

    RenderObject();

    // Disable client states
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);
}

void IGOCVisualVertexArray::UpdateNormals()
{
    for( int j=0; j<m_iSlices; ++j )
    {
        for( int i=0; i<m_iStacks; ++i )
        {
            int index = i+j*m_iStacks;
            Vec3f u, v1, v2;

            // Pick the left vertex
            if( i > 0 ) v1.Set( m_VertexArray[index-1] );
            else v1.Set( m_VertexArray[index] );

            // Pick the right vertex
            if( i < m_iStacks-1 ) v2.Set( m_VertexArray[index+1] );
            else v2.Set( m_VertexArray[index] );

            u = v2 - v1;

            // Pick the top
            if( j > 0 ) v1.Set( m_VertexArray[index-m_iStacks] );
            else v1.Set( m_VertexArray[index] );

            // Pick the bottom
            if( j < m_iSlices-1 ) v2.Set( m_VertexArray[index+m_iStacks] );
            else v2.Set( m_VertexArray[index] );

            v2 -= v1;

            // Calculate normal
            u.Cross(v2, v1);
            
            // Correct it
            v1.Normalize();
            SetNormal( index, v1 );

        } // end for( ... )
    } // end for( ... )

} // UpdateNormals()