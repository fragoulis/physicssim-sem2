#ifndef __TLIB_GOCS_GOCVISUALIMQUAD_H__
#define __TLIB_GOCS_GOCVISUALIMQUAD_H__
#pragma once
#include "Interfaces/IGOCVisual.h"
#include "../Math/TVector2.h"
using tlib::math::Vec2f;

namespace tlib
{
namespace gocs
{

class GOCTVisualIMQuad;

/**
 * @class GOCVisualIMQuad
 * 
 */
class GOCVisualIMQuad : public IGOCVisual
{
private:
    //! The quads half size
    math::Vec2f m_vHalfSize; 

public:
    //! Template based constructor
    GOCVisualIMQuad( const GOCTVisualIMQuad * const tpl );
    ~GOCVisualIMQuad();

    virtual ComponentId_t GetID() const {
        return CHash::_("VisualIMQuad");
    }

    const Vec2f& GetHalfSize() const { return m_vHalfSize; }

    //! Renders the plane
    virtual void Render() const;

}; // end GOCVisualIMQuad

} // end namespace gocs
} // end namespace tlib

#endif // __TLIB_GOCS_GOCVISUALIMQUAD_H__