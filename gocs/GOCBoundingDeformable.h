#pragma once
#include "Interfaces/IGOCBoundingVolume.h"

namespace tlib
{

namespace physics
{
    class CParticle;
}
using physics::CParticle;

namespace gocs
{
    class IGOCTBounding;
    class IGOCPhysicsDeformable;
    class IGOCVisualVertexArray;

/**
 * @class GOCBoundingDeformable
 *
 */
class GOCBoundingDeformable : public IGOCBoundingVolume
{
protected:
    //! A wrapping primitive volume for quick exclusions
    IGOCBoundingVolume *m_Primitive;

    //! Direct access pointer to physical component
    IGOCPhysicsDeformable *m_Body;

    //! The visual representation of the deformable
    IGOCVisualVertexArray *m_Visual;

private:
    int m_iParticleIndex;

public:
    GOCBoundingDeformable( const IGOCTBounding * const tpl );
    virtual ~GOCBoundingDeformable();

    virtual ComponentId_t GetID() const {
        return CHash::_("BoundingDeformable");
    }
    virtual void Init();

    //! Returns a particle each time increasine the stack index
    CParticle* GetParticle();
    IGOCBoundingVolume* GetPrimitive() { return m_Primitive; }
};

} // end namespace gocs
} // end namespace tlib