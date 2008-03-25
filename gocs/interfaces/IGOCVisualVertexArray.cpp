#ifdef _WIN32
#include <windows.h>
#endif
#include <GL/GL.h>

#include "IGOCVisualVertexArray.h"
using namespace tlib::gocs;

IGOCVisualVertexArray::~IGOCVisualVertexArray()
{
    delete[] m_VertexArray;
    delete[] m_NormalArray;
    delete[] m_TexArray;
    delete[] m_IndexArray;
}

void IGOCVisualVertexArray::Init()
{
    Create();
}

void IGOCVisualVertexArray::Render() const
{
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