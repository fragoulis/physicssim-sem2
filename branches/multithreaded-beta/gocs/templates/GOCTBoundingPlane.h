#pragma once
#include "../Interfaces/IGOCTBounding.h"
#include "../../Math/TVector3.h"
using tlib::math::Vec3f;

namespace tlib
{
namespace gocs
{

/**
 * @class GOCTBoundingPlane
 * 
 */
class GOCTBoundingPlane : public IGOCTBounding
{
protected:
    //! Double-sided plane flag
    bool m_bDoublesided;

    //! The plane's normal
    Vec3f m_vNormal;

public:
    GOCTBoundingPlane( const char *id );
    virtual ~GOCTBoundingPlane() {}

    //! Returns a new component based on this template
    virtual IGOComponent* MakeComponent() const;

    bool IsDoublesided() const { return m_bDoublesided; }
    void setDoublesided( bool ds ) { m_bDoublesided = ds; }

    const Vec3f& GetNormal() const { return m_vNormal; }
    void SetNormal( const Vec3f &n ) { m_vNormal = n; }

}; // end GOCTBoundingPlane

} // end namespace gocs
} // end namespace tlib