#pragma once
#include "IGOCVisual.h"
#include "../../Math/TVector3.h"
using tlib::math::Vec3f;

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

    int m_iStacks, m_iSlices;
    int m_iNumOfVertices, m_iNumOfIndices, m_iNumOfTriangles;

public:
    IGOCVisualVertexArray( int stacks, int slices );
    virtual ~IGOCVisualVertexArray();

    virtual ComponentId_t GetID() const {
        return CHash::_("VisualVertexArray");
    }

    //! Renders the vertex array
    virtual void Render() const;
    virtual void Init();

    // Direct memory const accessor
    const float *GetVertex( int index ) const { return m_VertexArray[index]; }

    // Regular accessors
    void GetVertex( int index, Vec3f &v ) const { v.Set( m_VertexArray[index] ); }
    void GetNormal( int index, Vec3f &v ) const { v.Set( m_NormalArray[index] ); }
    void GetVertex( int stack, int slice, Vec3f &v ) const { GetVertex( stack+slice*m_iStacks, v ); }
    void GetNormal( int stack, int slice, Vec3f &v ) const { GetNormal( stack+slice*m_iStacks, v ); }

    // TODO: Add GetTriangle()

    // Mutators
    void SetVertex( int index, const Vec3f &v ) 
    { 
        m_VertexArray[index][0] = v.x();
        m_VertexArray[index][1] = v.y();
        m_VertexArray[index][2] = v.z();
    }
    void SetNormal( int index, const Vec3f &v ) 
    { 
        m_NormalArray[index][0] = v.x();
        m_NormalArray[index][1] = v.y();
        m_NormalArray[index][2] = v.z();
    }
    void SetVertex( int stack, int slice, const Vec3f &v ) { SetVertex(stack+slice*m_iStacks, v); }
    void SetNormal( int stack, int slice, const Vec3f &v ) { SetNormal(stack+slice*m_iStacks, v); }

    int GetNumOfVertices() const { return m_iNumOfVertices; }

protected:
    //! Creates the vertex array
    virtual bool Create() = 0;

    //! Individual vertex array rendering code
    virtual void RenderObject() const = 0;

}; // end IGOCVisualVertexArray

} // end namespace gocs
} // end namespace tlib