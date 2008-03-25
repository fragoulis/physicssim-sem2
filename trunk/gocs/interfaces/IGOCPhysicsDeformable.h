#pragma once
#include "IGOCPhysics.h"

namespace tlib
{

namespace physics
{
    class CParticle;
    class CSpringDamper;
}
using physics::CParticle;
using physics::CSpringDamper;

namespace gocs
{

    class IGOCVisualVertexArray;

/**
 * @class IGOCPhysicsDeformable
 *
 * Multiple mass points.
 * This class holds the data the describe the multiple mass points
 * and the interconnected springs of a deformable objects.
 */
class IGOCPhysicsDeformable : public IGOCPhysics
{
protected:
    //! Array of body's points of mass
    CParticle *m_Particles;

    //! The collection of springs
    CSpringDamper *m_Springs;

    //! A direct pointer to the visual components for faster com
    IGOCVisualVertexArray *m_Visual;

    int m_iNumOfSprings, m_iNumOfParticles;
    int m_iStacks, m_iSlices;
    float m_fMass;

public:
    virtual ~IGOCPhysicsDeformable();
    virtual ComponentId_t GetID() const { return CHash::_("PhysicsDeformable"); }
    virtual void Update( float delta );
    virtual void Init();

    CParticle* GetParticle( int index );
    int GetNumOfParticles() const { return m_iNumOfParticles; }

protected:
    //! Sets up the particles and springs
    virtual void Setup() = 0;

private:
    void UpdateNormals();
};

} // end namespace gocs
} // end namespace tlib