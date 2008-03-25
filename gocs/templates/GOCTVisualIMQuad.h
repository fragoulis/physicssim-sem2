#ifndef __TLIB_GOCS_GOCTVISUALIMQUAD_H__
#define __TLIB_GOCS_GOCTVISUALIMQUAD_H__
#pragma once
#include "../Interfaces/IGOCTemplate.h"
#include "../../Math/TVector2.h"
using tlib::math::Vec2f;

namespace tlib
{
namespace gocs
{

class GOCVisualIMQuad;

/**
 * @class GOCTVisualIMQuad
 * 
 */
class GOCTVisualIMQuad : public IGOCTemplate
{
private:
    //! The quads half size
    math::Vec2f m_vHalfSize; 

public:
    GOCTVisualIMQuad( const char *id );
    
    virtual ComponentId_t GetFamilyID() const {
        return CHash::_("Visual");
    }

    const Vec2f& GetHalfSize() const { return m_vHalfSize; }
    void SetHalfSize( const Vec2f &hs ) { m_vHalfSize = hs; }

    //! Returns a new component based on this template
    virtual IGOComponent* MakeComponent() const;

}; // end GOCTVisualIMQuad

} // end namespace gocs
} // end namespace tlib

#endif // __TLIB_GOCS_GOCTVISUALIMQUAD_H__