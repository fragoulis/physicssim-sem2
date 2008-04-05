#pragma once
#include "../Interfaces/IGOCTemplate.h"
#include "../../Physics/CParticle.h"
using tlib::physics::CParticle;

namespace tlib
{
namespace gocs
{

class GOCPhysicsPoint;

/**
 * @class GOCTPhysicsPoint
 * 
 */
class GOCTPhysicsPoint : public IGOCTemplate
{
private:
    CParticle m_Body;

public:
    GOCTPhysicsPoint( const char *id );

    virtual ComponentId_t GetFamilyID() const {
        return CHash::_("Physics");
    }

    const CParticle& GetBody() const { return m_Body; }
    CParticle& GetBody() { return m_Body; }

    //! Returns a new component based on this template
    virtual IGOComponent* MakeComponent() const;

}; // end GOCTPhysicsPoint

} // end namespace gocs
} // end namespace tlib