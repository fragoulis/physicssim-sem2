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
    //! Double-sided plane flag
    bool m_bDoublesided;

    //! The quads half size
    math::Vec2f m_vHalfSize; 

public:
    //! Template based constructor
    GOCVisualIMQuad( const GOCTVisualIMQuad * const tpl );
    ~GOCVisualIMQuad();

    virtual ComponentId_t GetID() const {
        return CHash::_("VisualIMQuad");
    }

    bool IsDoublesided() const { return m_bDoublesided; }
    const Vec2f& GetHalfSize() const { return m_vHalfSize; }

    //! Renders the plane
    virtual void Render() const;

}; // end GOCVisualIMQuad

} // end namespace gocs
} // end namespace tlib