#pragma once
#include "Interfaces/IGOCBoundingVolume.h"
#include "../Math/TVector2.h"
using tlib::math::Vec2f;
#include "../Math/TVector3.h"
using tlib::math::Vec3f;

namespace tlib
{
namespace gocs
{

    class GOCTBoundingPlane;

/**
 * @class GOCBoundingPlane
 *
 */
class GOCBoundingPlane : public IGOCBoundingVolume
{
private:
    //! Planes half size
    Vec2f m_vHalfSize;

    //! The plane's normal
    Vec3f m_vNormal;

public:
    GOCBoundingPlane( const GOCTBoundingPlane * const tpl );

    virtual ComponentId_t GetID() const {
        return CHash::_("BoundingPlane");
    }

    virtual void Init();

    const Vec2f& GetHalfSize() const { return m_vHalfSize; }
    const Vec3f& GetNormal() const { return m_vNormal; }
    Vec3f& GetNormal() { return m_vNormal; }
};

} // end namespace gocs
} // end namespace tlib