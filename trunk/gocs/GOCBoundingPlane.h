#pragma once
#include "Interfaces/IGOCBoundingVolume.h"
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
    //! Double-sided plane flag
    bool m_bDoublesided;

    //! The plane's normal
    Vec3f m_vNormal;

public:
    GOCBoundingPlane( const GOCTBoundingPlane * const tpl );
    virtual ~GOCBoundingPlane() {}

    virtual ComponentId_t GetID() const {
        return CHash::_("BoundingPlane");
    }

    virtual void Init();

    bool IsDoublesided() const { return m_bDoublesided; }
    const Vec3f& GetNormal() const { return m_vNormal; }
    void SetNormal( const Vec3f &n ) { m_vNormal = n; }
};

} // end namespace gocs
} // end namespace tlib