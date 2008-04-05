#pragma once
#include "Interfaces/IGOCPhysics.h"
#include "../Physics/CParticle.h"
using tlib::physics::CParticle;

namespace tlib
{
namespace gocs
{

    class GOCTPhysicsPoint;
/**
 * @class GOCPhysicsPoint
 *
 * Single mass point object.
 * This class describes the physics of an object with a single point of mass,
 * without angular velocities etc.
 */
class GOCPhysicsPoint : public IGOCPhysics
{
private:
    //! The body's single mass point
    CParticle m_Body;

public:
    GOCPhysicsPoint( const GOCTPhysicsPoint * const tpl );
    
    virtual void Update( float delta );

    virtual ComponentId_t GetID() const {
        return CHash::_("PhysicsPoint");
    }

    CParticle& GetBody() { return m_Body; }
    
private:
    virtual void Init();
};

} // end namespace gocs
} // end namespace tlib