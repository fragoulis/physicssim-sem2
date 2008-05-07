#pragma once
#include "GOCTBoundingPlane.h"
#include "../../Math/TVector2.h"
using tlib::math::Vec2f;

namespace tlib
{
namespace gocs
{

/**
 * @class GOCTBoundingFinitePlane
 * 
 */
class GOCTBoundingFinitePlane : public GOCTBoundingPlane
{
private:
    //! Planes half size
    Vec2f m_vHalfSize;

    //! The plane binormal
    Vec3f m_vBinormal;

public:
    GOCTBoundingFinitePlane( const char *id );

    //! Returns a new component based on this template
    virtual IGOComponent* MakeComponent() const;

    const Vec2f& GetHalfSize() const { return m_vHalfSize; }
    void SetHalfSize( const Vec2f &hs ) { m_vHalfSize = hs; }

    const Vec3f& GetBinormal() const { return m_vBinormal; }
    void SetBinormal( const Vec3f &n ) { m_vBinormal = n; }

}; // end GOCTBoundingFinitePlane

} // end namespace gocs
} // end namespace tlib