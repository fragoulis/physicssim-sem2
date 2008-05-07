#pragma once
#include "TVector3.h"

namespace tlib
{
namespace math
{

/**
 * @class TTriangle
 * Triangle class.
 * Simple templated triangle class.
 */
template<class Type>
class TTriangle
{
private:
    typedef TVector3<Type> TVec;
    
    //! The three triangle points
    TVec m_points[3];

public:
    explicit TTriangle() {}
    explicit TTriangle( const TVec &a, const TVec &b, const TVec &c ) {
        Set( a, b, c );
    }

    void SetA( const TVec &v ) { m_points[0] = v; }
    void SetB( const TVec &v ) { m_points[1] = v; }
    void SetC( const TVec &v ) { m_points[2] = v; }
    void Set( const TVec &a, const TVec &b, const TVec &c ) {
        m_points[0] = a; m_points[1] = b; m_points[2] = c;
    }

    const TVec& GetA() const { return m_points[0]; }
    const TVec& GetB() const { return m_points[1]; }
    const TVec& GetC() const { return m_points[2]; }

    //void GetA( TVec &a ) { return m_points[0]; }
    //void GetA( TVec &b ) { return m_points[1]; }
    //void GetA( TVec &c ) { return m_points[2]; }

    //! Returns the normal of the triangle surface
    void GetNormal( TVec &normal )
    {
        TVec cross = (m_points[1]-m_points[0]).Cross((m_points[2]-m_points[0]));
        normal = cross / cross.Length();
        normal.Normalize();
    }

}; // end TTriangle

typedef TTriangle<float> Trianglef;

} // end namespace math
} // end namespace tlib