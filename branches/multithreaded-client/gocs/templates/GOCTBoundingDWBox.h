#pragma once
#include "../Interfaces/IGOCTBounding.h"
#include "../../Math/TVector3.h"
using tlib::math::Vec3f;

namespace tlib
{
namespace gocs
{

/**
 * @class GOCTBoundingDWBox
 * 
 */
class GOCTBoundingDWBox : public IGOCTBounding
{
private:
    //! Volume's half box
    Vec3f m_vHalfSize;

public:
    GOCTBoundingDWBox( const char *id );

    //! Returns a new component based on this template
    virtual IGOComponent* MakeComponent() const;

    const Vec3f& GetHalfSize() const { return m_vHalfSize; }
    void SetHalfSize( const Vec3f &n ) { m_vHalfSize = n; }

}; // end GOCTBoundingDWBox

} // end namespace gocs
} // end namespace tlib