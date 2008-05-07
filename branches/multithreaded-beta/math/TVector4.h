#ifndef __TLIB_MATH_VECTOR4_H__
#define __TLIB_MATH_VECTOR4_H__
#pragma once
#include <cmath>
#include <iostream>

namespace tlib
{
namespace math
{

/**
 * @class TVector4
 * Basic 4d templated vector class.
 * Although the class is templated and components can be of virtualy any type,
 * the proposed uses are with type of integers( short, long and unsigned), 
 * floats and doubles. 
 * Also note that functions that return a type that is 
 * not in actual relation with the template's Type, e.g. Length(), return float. 
 */
template<class Type>
class TVector4
{
private:
    Type m_xyzw[4];

public:
    // Constructors
    explicit TVector4() {}  // Default constructor is set to do nothing
    explicit TVector4( const Type xyzw[] ) { 
        Set( xyzw[0], xyzw[1], xyzw[2], xyzw[3] ); 
    }
    explicit TVector4( Type x, Type y, Type z, Type w ) { 
        Set( x, y, z, w ); 
    }

    // Accessors
    Type x() const { return m_xyzw[0]; }
    Type y() const { return m_xyzw[1]; }
    Type z() const { return m_xyzw[2]; }
    Type w() const { return m_xyzw[3]; }
    const Type* xyzw() const { return m_xyzw; }

    // Mutators
    void x( Type f ) { m_xyzw[0] = f; }
    void y( Type f ) { m_xyzw[1] = f; }
    void z( Type f ) { m_xyzw[2] = f; }
    void w( Type f ) { m_xyzw[3] = f; }
    void Clear() { Set( Type(0), Type(0), Type(0), Type(0) ); }
    void Set( Type x, Type y, Type z, Type w ) { 
        m_xyzw[0] = x; m_xyzw[1] = y; m_xyzw[2] = z; m_xyzw[3] = w;
    }

    //! Returns the vector's squared magnitude
    float SqrLength() const  { return x()*x() + y()*y() + z()*z() + w()*w(); }

    //! Returns the vector's magnitude
    float Length() const { return float(sqrt(SqrLength())); }

    //! Normalizes the vector
    inline void Normalize();

    //! Returns the dot product between two vectors
    float Dot( const TVector4<Type>& ) const {
        return x()*rh.x() + y()*rh.y() + z()*rh.z() + w()*rh.w();
    }

    //! Inverts the vector's components
    void Invert() { Set( -x(), -y(), -z(), -w() ); }

    //! Adds a vectors multiplied by a scale
    inline void AddScaledVector( const TVector4<Type>&, Type );

    // Operators
    inline TVector4<Type>& operator+=( const TVector4<Type>& );
    inline TVector4<Type>& operator-=( const TVector4<Type>& );
    inline TVector4<Type>& operator*=( const TVector4<Type>& );
    inline TVector4<Type>& operator+=( Type );
    inline TVector4<Type>& operator-=( Type );
    inline TVector4<Type>& operator*=( Type );
    
    //! Writes vector data to a stream
    void write( std::ostream &cout ) const {
        cout << "(" << x() << "," << y() << "," << z() << "," << w() << ")";
    }

}; // end TVector4

template<class Type>
void TVector4<Type>::Normalize()
{
    float len = Length();
    if( len > 0.0f )
    {
        len = 1.0f / len;
        *this *= len;
    }
}

template<class Type>
TVector4<Type>& TVector4<Type>::operator+=( const TVector4<Type>& rh ) {
    m_xyzw[0] += rh.x();
    m_xyzw[1] += rh.y();
    m_xyzw[2] += rh.z();
    m_xyzw[3] += rh.w();
    return *this;
}

template<class Type>
TVector4<Type>& TVector4<Type>::operator-=( const TVector4<Type>& rh ) {
    m_xyzw[0] -= rh.x();
    m_xyzw[1] -= rh.y();
    m_xyzw[2] -= rh.z();
    m_xyzw[3] -= rh.w();
    return *this;
}

template<class Type>
TVector4<Type>& TVector4<Type>::operator*=( const TVector4<Type>& rh ) {
    m_xyzw[0] *= rh.x();
    m_xyzw[1] *= rh.y();
    m_xyzw[2] *= rh.z();
    m_xyzw[3] *= rh.w();
    return *this;
}

template<class Type>
TVector4<Type>& TVector4<Type>::operator+=( Type rh ) {
    m_xyzw[0] += rh;
    m_xyzw[1] += rh;
    m_xyzw[2] += rh;
    m_xyzw[3] += rh;
    return *this;
}

template<class Type>
TVector4<Type>& TVector4<Type>::operator-=( Type rh ) {
    m_xyzw[0] -= rh;
    m_xyzw[1] -= rh;
    m_xyzw[2] -= rh;
    m_xyzw[3] -= rh;
    return *this;
}

template<class Type>
TVector4<Type>& TVector4<Type>::operator*=( Type rh ) {
    m_xyzw[0] *= rh;
    m_xyzw[1] *= rh;
    m_xyzw[2] *= rh;
    m_xyzw[3] *= rh;
    return *this;
}

template<class Type>
void TVector4<Type>::AddScaledVector( const TVector4<Type>& vVector, Type fScale )
{
    m_xyzw[0] += vVector.x() * fScale;
    m_xyzw[1] += vVector.y() * fScale;
    m_xyzw[2] += vVector.z() * fScale;
    m_xyzw[3] += vVector.w() * fScale;
}

// 
// Global operators
// 
template<class Type> 
inline 
TVector4<Type> operator+( const TVector4<Type>& lh, const TVector4<Type>& rh ) {
    return TVector4<Type>(
        lh.x() + rh.x(),
        lh.y() + rh.y(),
        lh.z() + rh.z(),
        lh.w() + rh.w()
        );
}

template<class Type>
inline
TVector4<Type> operator+( const TVector4<Type>& lh, Type rh ) {
    return TVector4<Type>(
        lh.x() + rh,
        lh.y() + rh,
        lh.z() + rh,
        lh.w() + rh
        );
}

template<class Type>
inline
TVector4<Type> operator+( Type lh, const TVector4<Type>& rh ) {
    return rh + lh;
}

template<class Type>
inline
TVector4<Type> operator-( const TVector4<Type>& lh, const TVector4<Type>& rh ) {
    return TVector4<Type>(
        lh.x() - rh.x(),
        lh.y() - rh.y(),
        lh.z() - rh.z(),
        lh.w() - rh.w()
        );
}

//! Overloads subtraction operator for a vector and a scalar
template<class Type>
inline
TVector4<Type> operator-( const TVector4<Type>& lh, Type rh ) {
    return TVector4<Type>(
        lh.x() - rh,
        lh.y() - rh,
        lh.z() - rh,
        lh.w() - rh
        );
}

//! Overloads multiplication operator for two vectors
template<class Type>
inline
TVector4<Type> operator*( const TVector4<Type>& lh, const TVector4<Type>& rh ) {
    return TVector4<Type>(
        lh.x() * rh.x(),
        lh.y() * rh.y(),
        lh.z() * rh.z(),
        lh.w() * rh.w()
        );
}

//! Overloads multiplication operator for a vector and a scalar
template<class Type>
inline
TVector4<Type> operator*( const TVector4<Type>& lh, Type rh ) {
    return TVector4<Type>(
        lh.x() * rh,
        lh.y() * rh,
        lh.z() * rh,
        lh.w() * rh
        );
}

//! Overloads operator << in order to stream out the vector
template<class Type>
inline
std::ostream& operator<<( std::ostream &cout, const TVector4<Type> &rh ) 
{
    rh.write( cout );
    return cout;
}

typedef TVector4<s16>   Vec4s;
typedef TVector4<u16>   Vec4us;
typedef TVector4<s32>   Vec4i;
typedef TVector4<u32>   Vec4ui;
typedef TVector4<float>   Vec4f;
typedef TVector4<f64>   Vec4d;

//typedef TVector4<float>         Vector3f;

} // end namespace math
} // end namespace tlib

#endif // __TLIB_MATH_VECTOR4_H__
