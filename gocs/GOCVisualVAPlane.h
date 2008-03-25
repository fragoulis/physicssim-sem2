#pragma once
#include "Interfaces/IGOCVisualVertexArray.h"
#include "../Math/TVector2.h"
using tlib::math::Vec2f;

namespace tlib
{
namespace gocs
{

class GOCTVisualVAPlane;

/**
 * @class GOCVisualVAPlane
 * 
 */
class GOCVisualVAPlane : public IGOCVisualVertexArray
{
private:
    Vec2f m_vHalfSize;
    int m_iStacks, m_iSlices;

public:
    //! Template based constructor
    GOCVisualVAPlane( const GOCTVisualVAPlane * const tpl );

    virtual ComponentId_t GetID() const {
        return CHash::_("VisualVAPlane");
    }

private:
    //! Renders the plane
    virtual void RenderObject() const;

    //! Creates the vertex array
    virtual bool Create();

}; // end GOCVisualVAPlane

} // end namespace gocs
} // end namespace tlib