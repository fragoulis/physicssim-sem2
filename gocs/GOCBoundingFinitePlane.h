#pragma once
#include "GOCBoundingPlane.h"
#include "../Math/TVector2.h"
using tlib::math::Vec2f;

namespace tlib
{
namespace gocs
{

    class GOCTBoundingFinitePlane;

/**
 * @class GOCBoundingFinitePlane
 *
 */
class GOCBoundingFinitePlane : public GOCBoundingPlane
{
private:
    //! Planes half size
    Vec2f m_vHalfSize;

    //! The plane's normal
    Vec3f m_vBinormal;

public:
    GOCBoundingFinitePlane( const GOCTBoundingFinitePlane * const tpl );

    virtual ComponentId_t GetID() const {
        return CHash::_("BoundingFinitePlane");
    }

    const Vec2f& GetHalfSize() const { return m_vHalfSize; }
    const Vec3f& GetBinormal() const { return m_vBinormal; }
    void SetBinormal( const Vec3f &n ) { m_vBinormal = n; }
};

} // end namespace gocs
} // end namespace tlib