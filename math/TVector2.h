#ifndef __TLIB_MATH_VECTOR2_H__
#define __TLIB_MATH_VECTOR2_H__
#pragma once
#define _USE_MATH_DEFINES
#include <cmath>
#include <iostream>

namespace tlib
{
namespace math
{

/**
 * @class TVector2
 * Basic 2d templated vector class.
 * Although the class is templated and components can be of virtualy any type,
 * the proposed uses are with type of integers( short, long and unsigned), 
 * floats and doubles. 
 * Also note that functions that return a type that is 
 * not in actual relation with the template's Type, e.g. Length(), return float. 
 */
template<class Type>
class TVector2
{
private:
    // This will make the class a little better to look at
    typedef TVector2<Type> TVec;

    Type m_xy[2];

public:
    // Constructors
    explicit TVector2() {}  // Default constructor is set to do nothing
    explicit TVector2( const Type xy[] )       { Set( xyz[0], xyz[1] ); }
    explicit TVector2( Type x, Type y ) { Set( x, y ); }

    // Accessors
    Type x() const { return m_xy[0]; }
    Type y() const { return m_xy[1]; }
    const Type* xy() const { return m_xy; }

    // Mutators
    void x( Type f ) { m_xy[0] = f; }
    void y( Type f ) { m_xy[1] = f; }
    void Clear() { x(Type(0.0)); y(Type(0.0)); }
    void Set( Type x, Type y ) { m_xy[0] = x; m_xy[1] = y; }

    //! Returns the vector's squared magnitude
    float SqrLength() const  { return x()*x() + y()*y(); }

    //! Returns the vector's magnitude
    float Length() const     { return float(sqrt(SqrLength())); }

    //! Normalizes the vector
    inline void Normalize();

    //! Inverts the vector's components
    void Invert() { Set( -x(), -y() ); }

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
        cout << "(" << x() << "," << y() << ")";
    }

}; // end TVector2

template<class Type>
void TVector2<Type>::Normalize()
{
    float len = Length();
    if( len > 0.0f )
    {
        len = 1.0f / len;
        *this *= len;
    }
}

template<class Type>
TVector2<Type>& TVector2<Type>::operator+=( const TVec& rh ) {
    m_xy[0] += rh.x();
    m_xy[1] += rh.y();
    return *this;
}

template<class Type>
TVector2<Type>& TVector2<Type>::operator-=( const TVec& rh ) {
    m_xy[0] -= rh.x();
    m_xy[1] -= rh.y();
    return *this;
}

template<class Type>
TVector2<Type>& TVector2<Type>::operator*=( const TVec& rh ) {
    m_xy[0] *= rh.x();
    m_xy[1] *= rh.y();
    return *this;
}

template<class Type>
TVector2<Type>& TVector2<Type>::operator+=( Type rh ) {
    m_xy[0] += rh;
    m_xy[1] += rh;
    return *this;
}

template<class Type>
TVector2<Type>& TVector2<Type>::operator-=( Type rh ) {
    m_xy[0] -= rh;
    m_xy[1] -= rh;
    return *this;
}

template<class Type>
TVector2<Type>& TVector2<Type>::operator*=( Type rh ) {
    m_xy[0] *= rh;
    m_xy[1] *= rh;
    return *this;
}

template<class Type>
void TVector2<Type>::AddScaledVector( const TVec& vVector, Type fScale )
{
    m_xy[0] += vVector.x() * fScale;
    m_xy[1] += vVector.y() * fScale;
}

// 
// Global operators
// 
template<class Type> 
inline 
TVector2<Type> operator+( const TVector2<Type>& lh, const TVector2<Type>& rh ) {
    return TVector2<Type>(
        lh.x() + rh.x(),
        lh.y() + rh.y()
        );
}

template<class Type>
inline
TVector2<Type> operator+( const TVector2<Type>& lh, Type rh ) {
    return TVector2<Type>(
        lh.x() + rh,
        lh.y() + rh
        );
}

template<class Type>
inline
TVector2<Type> operator+( Type lh, const TVector2<Type>& rh ) {
    return rh + lh;
}

template<class Type>
inline
TVector2<Type> operator-( const TVector2<Type>& lh, const TVector2<Type>& rh ) {
    return TVector2<Type>(
        lh.x() - rh.x(),
        lh.y() - rh.y()
        );
}

//! Overloads subtraction operator for a vector and a scalar
template<class Type>
inline
TVector2<Type> operator-( const TVector2<Type>& lh, Type rh ) {
    return TVector2<Type>(
        lh.x() - rh,
        lh.y() - rh
        );
}

//! Overloads multiplication operator for two vectors
template<class Type>
inline
TVector2<Type> operator*( const TVector2<Type>& lh, const TVector2<Type>& rh ) {
    return TVector2<Type>(
        lh.x() * rh.x(),
        lh.y() * rh.y()
        );
}

//! Overloads multiplication operator for a vector and a scalar
template<class Type>
inline
TVector2<Type> operator*( const TVector2<Type>& lh, Type rh ) {
    return TVector2<Type>(
        lh.x() * rh,
        lh.y() * rh
        );
}

template<class Type>
inline
TVector2<Type> operator*( Type rh, const TVector2<Type>& lh ) {
    return TVector2<Type>(
        lh.x() * rh,
        lh.y() * rh
        );
}

//! Overloads operator << in order to stream out the vector
template<class Type>
inline
std::ostream& operator<<( std::ostream &cout, const TVector2<Type> &rh ) 
{
    rh.write( cout );
    return cout;
}

typedef TVector2<float> Vec2f;
//typedef TVector2<float> Vector3f;

} // end namespace math
} // end namespace tlib

#endif // __TLIB_MATH_VECTOR2F_H__
