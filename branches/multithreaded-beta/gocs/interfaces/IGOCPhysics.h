#pragma once
#include "IGOComponent.h"
#include "../../Math/TVector3.h"
using tlib::math::Vec3f;

namespace tlib
{
namespace gocs
{

/**
 * @class IGOCPhysics
 * 
 * Physics components' family interface.
 */
class IGOCPhysics : public IGOComponent
{
protected:
    //! Default acceleration vector for every physical body
    Vec3f m_vAcceleration;

public:
    //! Constructor
    IGOCPhysics();

    //! Virtual destructor
    virtual ~IGOCPhysics();

    //! Returns the visual's family identifier
    virtual ComponentId_t GetFamilyID() const {
        return CHash::_("Physics");
    }

    //! Override this function
    virtual void Update( float delta ) = 0;
    virtual void UpdateOwner() {}

    const Vec3f& GetAcceleration() const { return m_vAcceleration; }
    Vec3f& GetAcceleration() { return m_vAcceleration; }
    void SetAcceleration( const Vec3f &a ) { m_vAcceleration = a; }

}; // end IGOCPhysics

} // end namespace gocs
} // end namespace tlib