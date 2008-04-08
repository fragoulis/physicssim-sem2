#pragma once
#include "../Interfaces/IGOCTemplate.h"
#include "../../Math/TVector3.h"
using tlib::math::Vec3f;

namespace tlib
{
namespace gocs
{

/**
 * @class GOCTVisualVASphere
 * 
 */
class GOCTVisualVASphere : public IGOCTemplate
{
private:
    // The sphere's radius, stacks and slices
    float m_fRadius; 
    int m_iStacks, m_iSlices;

public:
    GOCTVisualVASphere( const char *id );

    virtual ComponentId_t GetFamilyID() const {
        return CHash::_("Visual");
    }

    // Accessors
    float GetRadius() const { return m_fRadius; }
    int GetSlices() const   { return m_iSlices; }
    int GetStacks() const   { return m_iStacks; }

    // Mutators
    void SetRadius( float r )   { m_fRadius = r; }
    void SetSlices( int s )     { m_iSlices = s; }
    void SetStacks( int s )     { m_iStacks = s; }

    //! Returns a new component based on this template
    virtual IGOComponent* MakeComponent() const;

}; // end GOCTVisualVASphere

} // end namespace gocs
} // end namespace tlib