#pragma once
#include "../Interfaces/IGOCTBounding.h"

namespace tlib
{
namespace gocs
{

    class GOCBoundingSphere;

/**
 * @class GOCTBoundingSphere
 * 
 */
class GOCTBoundingSphere : public IGOCTBounding
{
private:
    //! Bounding sphere's radius
    float m_fRadius;

public:
    GOCTBoundingSphere( const char *id );

    //! Returns a new component based on this template
    virtual IGOComponent* MakeComponent() const;

    float GetRadius() const { return m_fRadius; }
    void SetRadius( float fRadius ) { m_fRadius = fRadius; }

}; // end GOCTBoundingSphere

} // end namespace gocs
} // end namespace tlib