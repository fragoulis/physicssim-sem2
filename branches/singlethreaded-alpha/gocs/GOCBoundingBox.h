#pragma once
#include "Interfaces/IGOCBoundingVolume.h"
#include "../Math/TVector3.h"
using tlib::math::Vec3f;

namespace tlib
{
namespace gocs
{

    class GOCTBoundingBox;

/**
 * @class GOCBoundingBox
 *
 */
class GOCBoundingBox : public IGOCBoundingVolume
{
private:
    //! Volume's half box
    Vec3f m_vHalfSize;

public:
    GOCBoundingBox();
    GOCBoundingBox( const GOCTBoundingBox * const tpl );

    virtual ComponentId_t GetID() const {
        return CHash::_("BoundingBox");
    }

    Vec3f& GetHalfSize() { return m_vHalfSize; }
    const Vec3f& GetHalfSize() const { return m_vHalfSize; }
    void SetHalfSize( const Vec3f &s ) { m_vHalfSize = s; }
};

} // end namespace gocs
} // end namespace tlib