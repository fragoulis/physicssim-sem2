#pragma once
#include "GOCBoundingDeformable.h"
#include "../Math/TQuaternion.h"
using tlib::math::Quatf;

namespace tlib
{
namespace gocs
{

    class GOCTBoundingDWBox;

/**
 * @class GOCBoundingDWBox
 *
 */
class GOCBoundingDWBox : public GOCBoundingDeformable
{
public:
    GOCBoundingDWBox( const GOCTBoundingDWBox * const tpl );

    virtual ComponentId_t GetID() const {
        return CHash::_("BoundingDWBox");
    }

    void WrapObject();
};

} // end namespace gocs
} // end namespace tlib