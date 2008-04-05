#pragma once
#include "IGOCVisual.h"

namespace tlib
{
namespace gocs
{

/**
 * @class IGOCVisualVertexArray
 * 
 */
class IGOCVisualVertexArray : public IGOCVisual
{
protected:
    //! The object's vertices
    float (*m_VertexArray)[3];
    
    //! The vertices' normals
    float (*m_NormalArray)[3];

    //! The object's texture coordinates
    float (*m_TexArray)[2];

    //! The object's index array that defines the polygon
    unsigned int *m_IndexArray;

public:
    IGOCVisualVertexArray();
    virtual ~IGOCVisualVertexArray();

    virtual ComponentId_t GetID() const {
        return CHash::_("VisualVertexArray");
    }

    //! Renders the vertex array
    virtual void Render() const;
    virtual void Init();

    const float* GetVertex( int index ) const { return m_VertexArray[index]; }
    void SetVertex( int index, const float *xyz ) { 
        m_VertexArray[index][0] = xyz[0];
        m_VertexArray[index][1] = xyz[1];
        m_VertexArray[index][2] = xyz[2];
    }

    void SetNormal( int index, const float *xyz ) { 
        m_NormalArray[index][0] = xyz[0];
        m_NormalArray[index][1] = xyz[1];
        m_NormalArray[index][2] = xyz[2];
    }


protected:
    //! Creates the vertex array
    virtual bool Create() = 0;

    //! Individual vertex array rendering code
    virtual void RenderObject() const = 0;

}; // end IGOCVisualVertexArray

} // end namespace gocs
} // end namespace tlib