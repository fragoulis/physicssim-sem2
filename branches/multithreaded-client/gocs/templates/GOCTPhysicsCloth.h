#pragma once
#include "../Interfaces/IGOCTemplate.h"
#include "../../Math/TVector2.h"
using tlib::math::Vec2f;

namespace tlib
{
namespace gocs
{

/**
 * @class GOCTPhysicsCloth
 * 
 */
class GOCTPhysicsCloth : public IGOCTemplate
{
private:
    Vec2f m_vHalfSize;
    int m_iStacks, m_iSlices;
    float m_fMass;

public:
    GOCTPhysicsCloth( const char *id );

    virtual ComponentId_t GetFamilyID() const {
        return CHash::_("Physics");
    }

    // Accessors
    const Vec2f& GetHalfSize() const { return m_vHalfSize; }
    int GetSlices() const   { return m_iSlices; }
    int GetStacks() const   { return m_iStacks; }
    float GetMass() const   { return m_fMass; }

    // Mutators
    void SetHalfSize( const Vec2f &v ) { m_vHalfSize = v; }
    void SetSlices( int s ) { m_iSlices = s; }
    void SetStacks( int s ) { m_iStacks = s; }
    void SetMass( float m ) { m_fMass = m; }

    //! Returns a new component based on this template
    virtual IGOComponent* MakeComponent() const;

}; // end GOCTPhysicsCloth

} // end namespace gocs
} // end namespace tlib