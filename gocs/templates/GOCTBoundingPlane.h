#pragma once
#include "../Interfaces/IGOCTBounding.h"
#include "../../Math/TVector2.h"
using tlib::math::Vec2f;
#include "../../Math/TVector3.h"
using tlib::math::Vec3f;

namespace tlib
{
namespace gocs
{

class GOCBoundingPlane;

/**
 * @class GOCTBoundingPlane
 * 
 */
class GOCTBoundingPlane : public IGOCTBounding
{
private:
    //! Planes half size
    Vec2f m_vHalfSize;

    //! The plane's normal
    Vec3f m_vNormal;

public:
    GOCTBoundingPlane( const char *id );

    //! Returns a new component based on this template
    virtual IGOComponent* MakeComponent() const;

    const Vec2f& GetHalfSize() const { return m_vHalfSize; }
    void SetHalfSize( const Vec2f &hs ) { m_vHalfSize = hs; }

    const Vec3f& GetNormal() const { return m_vNormal; }
    void SetNormal( const Vec3f &n ) { m_vNormal = n; }

}; // end GOCTBoundingPlane

} // end namespace gocs
} // end namespace tlib