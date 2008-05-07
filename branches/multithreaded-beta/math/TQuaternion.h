#pragma once
#define _USE_MATH_DEFINES
#include <cmath>
#include <iostream>

namespace tlib
{
namespace math
{

template<class Type> class TVector3;

/**
 * @class TQuaterion
 * Basic Quaternion class
 */
template<class Type>
class TQuaternion
{
private:
    // This will make the class a little better to look at
    typedef TQuaternion<Type>   TQuat;
    typedef TVector3<Type>      TVec;

    Type m_wxyz[4];

public:
    // Constructors
    explicit TQuaternion() { Clear(); }
    explicit TQuaternion( Type wxyz[] ) {
        Set( wxyz[0], wxyz[1], wxyz[2], wxyz[3] ); 
    }
    explicit TQuaternion( Type w, Type x, Type y, Type z ) {
        Set( w, x, y, z );
    }
    explicit TQuaternion( Type w, const TVec &copy ) {
        Set( w, copy );
    }

    // Accessors
    Type w() const { return m_wxyz[0]; }
    Type x() const { return m_wxyz[1]; }
    Type y() const { return m_wxyz[2]; }
    Type z() const { return m_wxyz[3]; }
    const Type* wxyz() const { return _wxyz; }
    TVec xyz() const { return TVec( x(), y(), z() ); }

    // Mutators
    void w( Type in ) { m_wxyz[0] = in; }
    void x( Type in ) { m_wxyz[1] = in; }
    void y( Type in ) { m_wxyz[2] = in; }
    void z( Type in ) { m_wxyz[3] = in; }
    void Set( Type w, Type x, Type y, Type z ) {
        m_wxyz[0] = w; m_wxyz[1] = x; m_wxyz[2] = y; m_wxyz[3] = z;
    }
    void Set( Type w, const TVec &copy ) {
        Set( w, copy.x(), copy.y(), copy.z() );
    }
    void Clear() {
        Set( Type(1), Type(0), Type(0), Type(0) );
    }
    
    //! Returns the quaternion's squared length
    float SqrLength() const { return w()*w() + x()*x() + y()*y() + z()*z(); }

    //! Returns the quaternion's length
    float Length() const { return float(sqrt(SqrLength())); }

    //! Normalizes the quaternion
    inline void Normalize();

    //! Calculates the rotation from the (w)component and a vector
    void FromVector( float w, const TVec &xyz );

    //! Calculates the rotation from three angles
    void FromEuler( float fPitch, float fYaw, float fRoll );

    //! Calculates the rotation from its own values
    void FromSelf();

    //! Creates a matrix from the quaternion
    void ToMatrix( Type *m ) const;
    
    //! Rotates a vector
    void Rotate( TVec &Vec ) const;

    //! Simulates spherical interpolation
    void Slerp( const TQuat &qFinal, float t, TQuat &qResult ) const;

    //! Multiplies two quaternions and returns the result
    inline TQuat& operator*=( Type rh );

    inline TQuat operator*( const TQuat &rh ) const;

    //! Returns the conjugate of this quaternion
    TQuat operator~() const { 
        return TQuat( w(), -x(), -y(), -z() );
    }

    //! Writes quaternion data to a stream
    void write( std::ostream &cout ) const {
        cout << "(" << w() << "," 
                    << x() << "," 
                    << y() << "," 
                    << z() << ")";
    }

}; // end TQuaternion

//--- Operations ---------------------------------------------------------------
template<class Type>
void TQuaternion<Type>::Normalize()
{
    float len = Length();
    if( len > 0.0f )
    {
        len = 1.0f / len;
        *this *= len;
    }
}

//! Calculates the rotation from the (w)component and a vector
template<class Type>
void TQuaternion<Type>::FromVector( float w, const TVec &xyz )
{
    w *= 0.5f; // angle / 2
    const float mysin = sin(w);

    // Setup rotation quaternion
    this->w( cos(w) );
    x( xyz.x() * mysin );
    y( xyz.y() * mysin );
    z( xyz.z() * mysin );

} // end toRotation()

//! Calculates the rotation from three angles
template<class Type>
void TQuaternion<Type>::FromEuler( float fPitch, float fYaw,
                                   float fRoll )
{
    float
        cr = cos( fRoll * 0.5f ),
        cp = cos( fPitch * 0.5f ),
        cy = cos( fYaw * 0.5f ),

        sr = sin( fRoll * 0.5f ),
        sp = sin( fPitch * 0.5f ),
        sy = sin( fYaw * 0.5f ),
	
        cpcy = cp * cy,
        spsy = sp * sy;


    w( cr * cpcy + sr * spsy );
    x( sr * cpcy - cr * spsy );
    y( cr * sp * cy + sr * cp * sy );
    z( cr * cp * sy - sr * sp * cy );

} // end toRotation()

//! Calculates the rotation from its own values
template<class Type>
void TQuaternion<Type>::FromSelf()
{
    w( w() * 0.5f ); // angle / 2
    const T mysin = sin( w() );

    // Setup rotation quaternion
    w( cos( w() ) );
    x( x() * mysin );
    y( y() * mysin );
    z( z() * mysin );

} // end toRotation()

//! Creates a matrix corresponding to the quaternion values
template<class Type>
void TQuaternion<Type>::ToMatrix( Type *m ) const 
{
    if( !m ) return;

    // First row
    m[ 0] = 1.0f - 2.0f * ( y() * y() + z() * z() );
    m[ 1] = 2.0f * (x() * y() + z() * w());
    m[ 2] = 2.0f * (x() * z() - y() * w());
    m[ 3] = 0.0f;
	
    // Second row
    m[ 4] = 2.0f * ( x() * y() - z() * w() );
    m[ 5] = 1.0f - 2.0f * ( x() * x() + z() * z() );
    m[ 6] = 2.0f * (z() * y() + x() * w() );
    m[ 7] = 0.0f;

    // Third row
    m[ 8] = 2.0f * ( x() * z() + y() * w() );
    m[ 9] = 2.0f * ( y() * z() - x() * w() );
    m[10] = 1.0f - 2.0f * ( x() * x() + y() * y() );
    m[11] = 0.0f;

    // Fourth row
    m[12] = 0;
    m[13] = 0;
    m[14] = 0;
    m[15] = 1.0f;
}

//! Rotates a vector by the angle represented by the quaternion
template<class Type>
void TQuaternion<Type>::Rotate( TVec &Vec ) const
{
    TQuat qTemp( Type(0), Vec );
    qTemp = *this * qTemp * ~(*this);
    Vec.Set( qTemp.x(), qTemp.y(), qTemp.z() );
}

//! Creates a quaternion with values interpolated spericaly between this the the 
//! qFinal parameter quaternion based on the value of parameter t
template<class Type>
void TQuaternion<Type>::Slerp( const TQuat &qFinal, float t, TQuat &qResult ) const 
{
    float to1[4];

    // calc cosine
    float cosom = x() * qFinal.x() + 
                  y() * qFinal.y() + 
                  z() * qFinal.z() + 
                  w() * qFinal.w();

    // adjust signs (if necessary)
    if( cosom < 1e-6f ) { 
        cosom = -cosom; 
        to1[0] = -qFinal.x();
        to1[1] = -qFinal.y();
        to1[2] = -qFinal.z();
        to1[3] = -qFinal.w();
    } else  {
        to1[0] = qFinal.x();
        to1[1] = qFinal.y();
        to1[2] = qFinal.z();
        to1[3] = qFinal.w();
    }

    // calculate coefficients
    float  omega, sinom, scale0, scale1;
    if( ( 1.0f - cosom ) > 1e-3f ) {
        // standard case (slerp)
        omega   = acos(cosom);
        sinom   = 1.0f / sin(omega);
        scale0  = sin( ( 1.0f - fTime ) * omega ) * sinom;
        scale1  = sin( fTime * omega ) * sinom;
    } else {         
    // "from" and "to" quaternions are very close 
    //  ... so we can do a linear interpolation
            scale0 = 1.0f - fTime;
            scale1 = fTime;
    }

    // calculate final values
    qResult.x( scale0 * x() + scale1 * to1[0] );
    qResult.y( scale0 * y() + scale1 * to1[1] );
    qResult.z( scale0 * z() + scale1 * to1[2] );
    qResult.w( scale0 * w() + scale1 * to1[3] );
}

//--- Operators ---------------------------------------------------------------
template<class Type>
TQuaternion<Type>& TQuaternion<Type>::operator*=( Type rh )
{
    m_wxyz[0] += rh;
    m_wxyz[1] += rh;
    m_wxyz[2] += rh;
    m_wxyz[3] += rh;
    return *this;
}

template<class Type>
TQuaternion<Type> TQuaternion<Type>::operator*( const TQuat &rh ) const 
{
    float
        A = (w() + x())*(rh.w() + rh.x()),
        B = (z() - y())*(rh.y() - rh.z()),
        C = (w() - x())*(rh.y() + rh.z()), 
        D = (y() + z())*(rh.w() - rh.x()),
        E = (x() + z())*(rh.x() + rh.y()),
        F = (x() - z())*(rh.x() - rh.y()),
        G = (w() + y())*(rh.w() - rh.z()),
        H = (w() - y())*(rh.w() + rh.z());

    return TQuat( 
        B + (-E - F + G + H ) * (Type)0.5,
        A - ( E + F + G + H ) * (Type)0.5,
        C + ( E - F + G - H ) * (Type)0.5,
        D + ( E - F - G + H ) * (Type)0.5 
        );
}

//! Overloads operator << in order to stream out the quaternion
template<class Type>
inline
std::ostream& operator<<( std::ostream &cout, const TQuaternion<Type> &rh ) 
{
    rh.write( cout );
    return cout;
}

typedef TQuaternion<float> Quatf;

} // end namespace math
} // end namespace tlib