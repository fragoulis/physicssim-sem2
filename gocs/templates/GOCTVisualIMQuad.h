#pragma once
#include "../Interfaces/IGOCTemplate.h"
#include "../../Math/TVector2.h"
using tlib::math::Vec2f;

namespace tlib
{
namespace gocs
{

/**
 * @class GOCTVisualIMQuad
 * 
 */
class GOCTVisualIMQuad : public IGOCTemplate
{
private:
    //! Double-sided plane flag
    bool m_bDoublesided;

    //! The quads half size
    math::Vec2f m_vHalfSize; 

public:
    GOCTVisualIMQuad( const char *id );
    
    virtual ComponentId_t GetFamilyID() const {
        return CHash::_("Visual");
    }

    bool IsDoublesided() const { return m_bDoublesided; }
    void setDoublesided( bool ds ) { m_bDoublesided = ds; }

    const Vec2f& GetHalfSize() const { return m_vHalfSize; }
    void SetHalfSize( const Vec2f &hs ) { m_vHalfSize = hs; }

    //! Returns a new component based on this template
    virtual IGOComponent* MakeComponent() const;

}; // end GOCTVisualIMQuad

} // end namespace gocs
} // end namespace tlib