#ifndef __TLIB_UTIL_CCAMERA_H__
#define __TLIB_UTIL_CCAMERA_H__
#pragma once
#include "../Math/TVector3.h"
using tlib::math::Vec3f;

namespace tlib
{
namespace util
{

/**
 * @class CCamera
 *
 */
class CCamera
{
private:
    //! Camera's position
    Vec3f m_vPosition;

    //! Camera's view direction
    Vec3f m_vView;

    //! Camera's up vector
    Vec3f m_vUp;

public:
    CCamera();

    // Accessors
    const Vec3f& GetPosition() const    { return m_vPosition; }
    const Vec3f& GetView() const        { return m_vView; }
    const Vec3f& GetUp() const          { return m_vUp; }

    // Mutators
    void SetPosition( const Vec3f &vec )    { m_vPosition = vec; }
    void SetView( const Vec3f &vec )        { m_vView = vec; }
    void SetUp( const Vec3f &vec )          { m_vUp = vec; }

    //! Apply camera to the modelview matrix
    void Apply() const;

    //! Recalculates the camera's vectors to look at the given point
    void LookAt( const Vec3f &vLookAt );

}; // end CCamera

} // end namespace util
} // end namespace tlib

#endif // __TLIB_UTIL_CCAMERA_H__