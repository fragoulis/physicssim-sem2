#pragma once
#include "Singleton.h"
#include <map>
#include <string>
using tlib::physics::CCollisionData;

namespace tlib
{

class CVertexArrayManager : public Singleton<CVertexArrayManager>
{
private:
    //! Definition of a vertex array
    struct SVertexArray
    {
        //! The object's vertices
        float (*m_VertexArray)[3];
    
        //! The vertices' normals
        float (*m_NormalArray)[3];

        //! The object's texture coordinates
        float (*m_TexArray)[2];

        //! The object's index array that defines the polygon
        unsigned int *m_IndexArray;
    };
    
    typedef unsigned int VAKey;
    typedef std::map< VAKey, SVertexArray* > VAList;
    
    //! List of vertex arrays
    VAList m_Arrays;

public:
    //! Enables a vertex array
    void Enable( const VAKey &key );
    
    //! Adds a vertex array to the list
    void Register( 
private:
    friend Singleton<CVertexArrayManager>;
    ~CVertexArrayManager();
};

} // end namespace tlib