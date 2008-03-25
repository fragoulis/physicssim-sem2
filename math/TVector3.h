#ifndef __TLIB_MATH_VECTOR3_H__
#define __TLIB_MATH_VECTOR3_H__
#pragma once
#define _USE_MATH_DEFINES
#include <cmath>
#include <iostream>

namespace tlib
{
namespace math
{

/**
 * @class TVector3
 * Basic 3d templated vector class.
 * Although the class is templated and components can be of virtualy any type,
 * the proposed uses are with type of integers( short, long and unsigned), 
 * floats and doubles. 
 * Also note that functions that return a type that is 
 * not in actual relation with the template's Type, e.g. Length(), return float. 
 */
template<class Type>
class TVector3
{
private:
    // This will make the class a little better to look at
    typedef TVector3<Type> TVec;

    Type m_xyz[3];

public:
    // Constructors
    explicit TVector3() {}  // Default constructor is set to do nothing
    explicit TVector3( const Type xyz[] )       { Set( xyz ); }
    explicit TVector3( Type x, Type y, Type z ) { Set( x, y, z ); }

    // Accessors
    Type x() const { return m_xyz[0]; }
    Type y() const { return m_xyz[1]; }
    Type z() const { return m_xyz[2]; }
    const Type* xyz() const { return m_xyz; }

    // Mutators
    void x( Type f ) { m_xyz[0] = f; }
    void y( Type f ) { m_xyz[1] = f; }
    void z( Type f ) { m_xyz[2] = f; }
    void Clear() { x(Type(0.0)); y(Type(0.0)); z(Type(0.0)); }
    void Set( const Type xyz[] ) { 
        Set( xyz[0], xyz[1], xyz[2] );
    }
    void Set( Type x, Type y, Type z ) { 
        m_xyz[0] = x; m_xyz[1] = y; m_xyz[2] = z; 
    }

    //! Returns the vector's squared magnitude
    float SqrLength() const  { return x()*x() + y()*y() + z()*z(); }

    //! Returns the vector's magnitude
    float Length() const     { return float(sqrt(SqrLength())); }

    //! Normalizes the vector
    inline void Normalize();

    //! Returns the dot product between two vectors
    float Dot( const TVec &rh ) const {
        return x()*rh.x() + y()*rh.y() + z()*rh.z();
    }

    //! Returns the cross product between two vectors
    inline TVec Cross( const TVec& ) const;

    //! Passes the cross product between two vectors in the parametre
    inline void Cross( const TVec&, TVec& ) const;

    //! Inverts the vector's components
    void Invert() { Set( -x(), -y(), -z() ); }

    //! Adds a vectors multiplied by a scale
    inline void AddScaledVector( const TVec&, Type );

    // Operators
    inline TVec& operator+=( const TVec& );
    inline TVec& operator-=( const TVec& );
    inline TVec& operator*=( const TVec& );
    inline TVec& operator+=( Type );
    inline TVec& operator-=( Type );
    inline TVec& operator*=( Type );
    
    //! Writes vector data to a stream
    void write( std::ostream &cout ) const {
        cout << "(" << x() << "," << y() << "," << z() << ")";
    }

}; // end TVector3

template<class Type>
void TVector3<Type>::Normalize()
{
    float len = Length();
    if( len > 0.0f )
    {
        len = 1.0f / len;
        *this *= len;
    }
}

template<class Type>
TVector3<Type> TVector3<Type>::Cross( const TVec &rh ) const {
    return TVec(
        y() * rh.z() - z() * rh.y(),
        z() * rh.x() - x() * rh.z(),
        x() * rh.y() - y() * rh.x()
        );
}

template<class Type>
void TVector3<Type>::Cross( const TVec &rh, TVec &vResult ) const {
    vResult.Set(
        y() * rh.z() - z() * rh.y(),
        z() * rh.x() - x() * rh.z(),
        x() * rh.y() - y() * rh.x()
        );
}

template<class Type>
TVector3<Type>& TVector3<Type>::operator+=( const TVec& rh ) {
    m_xyz[0] += rh.x();
    m_xyz[1] += rh.y();
    m_xyz[2] += rh.z();
    return *this;
}

template<class Type>
TVector3<Type>& TVector3<Type>::operator-=( const TVec& rh ) {
    m_xyz[0] -= rh.x();
    m_xyz[1] -= rh.y();
    m_xyz[2] -= rh.z();
    return *this;
}

template<class Type>
TVector3<Type>& TVector3<Type>::operator*=( const TVec& rh ) {
    m_xyz[0] *= rh.x();
    m_xyz[1] *= rh.y();
    m_xyz[2] *= rh.z();
    return *this;
}

template<class Type>
TVector3<Type>& TVector3<Type>::operator+=( Type rh ) {
    m_xyz[0] += rh;
    m_xyz[1] += rh;
    m_xyz[2] += rh;
    return *this;
}

template<class Type>
TVector3<Type>& TVector3<Type>::operator-=( Type rh ) {
    m_xyz[0] -= rh;
    m_xyz[1] -= rh;
    m_xyz[2] -= rh;
    return *this;
}

template<class Type>
TVector3<Type>& TVector3<Type>::operator*=( Type rh ) {
    m_xyz[0] *= rh;
    m_xyz[1] *= rh;
    m_xyz[2] *= rh;
    return *this;
}

template<class Type>
void TVector3<Type>::AddScaledVector( const TVec& vVector, Type fScale )
{
    m_xyz[0] += vVector.x() * fScale;
    m_xyz[1] += vVector.y() * fScale;
    m_xyz[2] += vVector.z() * fScale;
}

// 
// Global operators
// 
template<class Type> 
inline 
TVector3<Type> operator~( const TVector3<Type>& lh ) {
    return TVector3<Type>( -lh.x(), -lh.y(), -lh.z() );
}

template<class Type> 
inline 
TVector3<Type> operator+( const TVector3<Type>& lh, const TVector3<Type>& rh ) {
    return TVector3<Type>(
        lh.x() + rh.x(),
        lh.y() + rh.y(),
        lh.z() + rh.z()
        );
}

template<class Type>
inline
TVector3<Type> operator+( const TVector3<Type>& lh, Type rh ) {
    return TVector3<Type>(
        lh.x() + rh,
        lh.y() + rh,
        lh.z() + rh
        );
}

template<class Type>
inline
TVector3<Type> operator+( Type lh, const TVector3<Type>& rh ) {
    return rh + lh;
}

template<class Type>
inline
TVector3<Type> operator-( const TVector3<Type>& lh, const TVector3<Type>& rh ) {
    return TVector3<Type>(
        lh.x() - rh.x(),
        lh.y() - rh.y(),
        lh.z() - rh.z()
        );
}

//! Overloads subtraction operator for a vector and a scalar
template<class Type>
inline
TVector3<Type> operator-( const TVector3<Type>& lh, Type rh ) {
    return TVector3<Type>(
        lh.x() - rh,
        lh.y() - rh,
        lh.z() - rh
        );
}

template<class Type>
inline
TVector3<Type> operator-( Type lh, const TVector3<Type>& rh ) {
    return TVector3<Type>(
        lh - rh.x(),
        lh - rh.y(),
        lh - rh.z()
        );
}

//! Overloads multiplication operator for two vectors
template<class Type>
inline
TVector3<Type> operator*( const TVector3<Type>& lh, const TVector3<Type>& rh ) {
    return TVector3<Type>(
        lh.x() * rh.x(),
        lh.y() * rh.y(),
        lh.z() * rh.z()
        );
}

//! Overloads multiplication operator for a vector and a scalar
template<class Type>
inline
TVector3<Type> operator*( const TVector3<Type>& lh, Type rh ) {
    return TVector3<Type>(
        lh.x() * rh,
        lh.y() * rh,
        lh.z() * rh
        );
}

template<class Type>
inline
TVector3<Type> operator*( Type rh, const TVector3<Type>& lh ) {
    return TVector3<Type>(
        lh.x() * rh,
        lh.y() * rh,
        lh.z() * rh
        );
}

//! Overloads division operator for a vector and a scalar
template<class Type>
inline
TVector3<Type> operator/( const TVector3<Type>& lh, Type rh ) {
    return TVector3<Type>(
        lh.x() / rh,
        lh.y() / rh,
        lh.z() / rh
        );
}

template<class Type>
inline
TVector3<Type> operator/( Type rh, const TVector3<Type>& lh ) {
    return TVector3<Type>(
        lh.x() / rh,
        lh.y() / rh,
        lh.z() / rh
        );
}

//! Overloads operator << in order to stream out the vector
template<class Type>
inline
std::ostream& operator<<( std::ostream &cout, const TVector3<Type> &rh ) 
{
    rh.write( cout );
    return cout;
}

typedef TVector3<float> Vec3f;
//typedef TVector3<float> Vector3f;

} // end namespace math
} // end namespace tlib

#endif // __TLIB_MATH_VECTOR3F_H__
