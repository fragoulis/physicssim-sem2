#ifdef _WIN32
#include <windows.h>
#endif
#include <GL/GLu.h>
#include "CCamera.h"
#include "../Math/TQuaternion.h"
using namespace tlib::util;

CCamera::CCamera()
{
    m_vPosition.Set( 0.0f, 0.0f, 0.0f );
    m_vView.Set( 0.0f, 0.0f, 1.0f );
    m_vUp.Set( 0.0f, 1.0f, 0.0f );
}

// ----------------------------------------------------------------------------
void CCamera::Apply() const
{
    const Vec3f vCenter = m_vPosition + m_vView;

    gluLookAt( 
        m_vPosition.x(), m_vPosition.y(), m_vPosition.z(), 
        vCenter.x(),     vCenter.y(),     vCenter.z(),
        m_vUp.x(),       m_vUp.y(),       m_vUp.z() 
        );
}

// ----------------------------------------------------------------------------
void CCamera::LookAt( const Vec3f &vLookAt )
{
    Vec3f vNewView = vLookAt - m_vPosition;
    vNewView.Normalize();
    m_vView.Normalize();

    // The two vectors are in one plane. This gives us the cosine of the angle between
    // these two vectors.
    float dotProd = vNewView.Dot( m_vView );
    if( dotProd < -1.0f || dotProd > 1.0f )
        return;

    // By getting the arc cosine of this, we get the angle in radians between the two
    // vectors. But we only get an angle between 0 and pi, that will be a problem.
    float alpha = acosf(dotProd);

    // Here we get the normal to these two vectors, we will need to rotate our front
    // vector around this normal.
    Vec3f normal = vNewView.Cross( m_vView );
    normal.Normalize();

    math::Quatf qRot;
    qRot.FromVector( alpha, normal );
    qRot.Rotate(m_vUp);

    // As I said, we have a problem: as we only get an angle between 0 and pi,
    // we don't know if we need to rotate clockwise or counterclockwise ...
    // We solve this problem by just trying it out.
    Vec3f viewTest = m_vView;
    qRot.Rotate(viewTest);

    // If the dot product is near to one, it means we were right. with our guess
    // rotating counterclockwise.
    // Else, we must rotate clockwise.
    dotProd = viewTest.Dot( vNewView );
    if( fabs(dotProd) > 0.999f ) 
    {
        qRot.FromVector( alpha, normal );
        m_vView = viewTest;
        qRot.Rotate(m_vUp);
    } 
    else 
    {
        qRot.FromVector( -alpha, normal );
        qRot.Rotate(m_vView);
        qRot.Rotate(m_vUp);
    }
    m_vView = vNewView;

    // If the dot product is near to -1, it means we look the wrong way.
    // To correct this, we just rotate pi radians (a half circle).
    if( dotProd < -0.999f )
    {
        qRot.FromVector( (float)M_PI, m_vUp );
        qRot.Rotate(m_vView);
    }
}