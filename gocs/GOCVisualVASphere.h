#pragma once
#include "Interfaces/IGOCVisualVertexArray.h"

namespace tlib
{
namespace gocs
{

class GOCTVisualVASphere;

/**
 * @class GOCVisualVASphere
 * 
 */
class GOCVisualVASphere : public IGOCVisualVertexArray
{
private:
    // The sphere's radius, stacks and slices
    float m_fRadius; 
    int m_iStacks, m_iSlices;

public:
    //! Template based constructor
    GOCVisualVASphere( const GOCTVisualVASphere * const tpl );

    virtual ComponentId_t GetID() const {
        return CHash::_("VisualVASphere");
    }

    float GetRadius() const { return m_fRadius; }

private:
    //! Renders the sphere
    virtual void RenderObject() const;

    //! Creates the vertex array
    virtual bool Create();

}; // end GOCVisualVASphere

} // end namespace gocs
} // end namespace tlib