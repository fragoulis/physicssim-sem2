#pragma once
#include <string>
#include "../../Math/TVector2.h"
#include "../../Math/TVector3.h"
using namespace tlib::math;

namespace tlib
{
namespace util
{

//! Definition of a triangle
struct Triangle
{
    Vec3f a, b, c;
};

/**
 * @class IVertexArray
 * Simple vertex array interface.
 */
class IVertexArray
{
protected:
    typedef std::string VArrayId;
    VArrayId m_id;

    //! The object's vertices
    float (*m_Vertices)[3];

    //! The vertices' normals 
    float (*m_Normals)[3];

    //! The object's texture coordinates
    float (*m_TexCoords)[2];

    //! The object's index array that defines the polygon
    unsigned int *m_Indices;
    
    bool m_bIsTextured;
    int m_iNumOfVertices, m_iNumOfIndices;
    int m_iStacks, m_iSlices;

public:
    IVertexArray( const VArrayId &id, int stacks, int slices );

    virtual ~IVertexArray();

    virtual bool Create( bool bTextured ) = 0;
    virtual void Render() const = 0;
    void Begin();
    void End();

    // Accessors
    int GetNumOfVertices() const { return m_iNumOfVertices; }
    int GetNumOfIndices() const { return m_iNumOfIndices; }
    int GetStacks() const { return m_iStacks; }
    int GetSlices() const { return m_iSlices; }
    void SetStacks( int s ) { m_iStacks = s; }
    void SetSlices( int s ) { m_iSlices = s; }

    void GetVertex( int index, Vec3f &v ) { v.Set( m_Vertices[index] ); }
    void GetNormal( int index, Vec3f &v ) { v.Set( m_Normals[index] ); }
    void GetVertex( int stack, int slice, Vec3f &v ) { GetVertex( stack+slice*m_iStacks, v ); }
    void GetNormal( int stack, int slice, Vec3f &v ) { GetNormal( stack+slice*m_iStacks, v ); }

    // Mutators
    void SetVertex( int index, const Vec3f &v ) 
    { 
        m_Vertices[index][0] = v.x();
        m_Vertices[index][1] = v.y();
        m_Vertices[index][2] = v.z();
    }
    void SetNormal( int index, const Vec3f &v ) 
    { 
        m_Normals[index][0] = v.x();
        m_Normals[index][1] = v.y();
        m_Normals[index][2] = v.z();
    }
    void SetVertex( int stack, int slice, const Vec3f &v ) { SetVertex(stack+slice*m_iStacks, v); }
    void SetNormal( int stack, int slice, const Vec3f &v ) { SetNormal(stack+slice*m_iStacks, v); }

}; // end IVertexArray

} // end namespace util
} // end namespace tlib