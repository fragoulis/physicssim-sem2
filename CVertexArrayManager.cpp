//#include "CVertexArrayManager.h"
//
//CVertexArrayManager::~CVertexArrayManager()
//{
//    VAList::iterator i = m_Arrays.begin();
//    for(; i != m_Arrays.end(); ++i )
//        delete (*i);
//}
//
//void CVertexArrayManager::Enable( const VAKey &key )
//{
//    m_Arrays[ key ]->Enable();
//    
//    glEnableClientState (GL_TEXTURE_COORD_ARRAY);
//    glTexCoordPointer   (2, GL_FLOAT, 0, m_TexArray);
//    glEnableClientState (GL_NORMAL_ARRAY);
//    glNormalPointer     (GL_FLOAT, 0, m_NormalArray);
//    glEnableClientState (GL_VERTEX_ARRAY);
//    glVertexPointer     (3, GL_FLOAT, 0, m_VertexArray);
//}
//
//void CVertexArrayManager::Disable()
//{
//    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
//    glDisableClientState(GL_NORMAL_ARRAY);
//    glDisableClientState(GL_VERTEX_ARRAY);
//}