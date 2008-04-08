#pragma once
#include "IGOComponent.h"

namespace tlib
{

namespace physics
{
    class CCollisionData;
}
using physics::CCollisionData;

namespace gocs
{

    class IGOCTBounding;

/**
 * @class IGOCBoundingVolume
 * 
 * Collision components' family interface.
 */
class IGOCBoundingVolume : public IGOComponent
{
public:
    enum VolumeType { VT_PLANE = 1, VT_FIN_PLANE, VT_SPHERE, VT_BOX, VT_DEFORMABLE };

protected:
    //! Set the body's volume type and register's the object
    //! with the appropriate containers
    void SetVolumeType( VolumeType iType );

private:
    //! Volume type flag
    VolumeType m_iType;

    // Volume's collision data
    //! Elasticity coefficient
    float m_fElasticity;

    //! Friction coefficient
    float m_fFriction;

public:
    IGOCBoundingVolume();
    IGOCBoundingVolume( const IGOCTBounding * const tpl );

    //! Virtual destructor
    virtual ~IGOCBoundingVolume();

    //! Returns the visual's family identifier
    virtual ComponentId_t GetFamilyID() const {
        return CHash::_("BoundingVolume");
    }

    //! Override this function
    bool Check( IGOCBoundingVolume *other, CCollisionData *coldata );
    virtual void Init();

    int GetVolumeType() const   { return m_iType; }
    float GetElasticity() const { return m_fElasticity; }
    float GetFriction() const   { return m_fFriction; }

    void SetElasticity( float e )   { m_fElasticity = e; }
    void SetFriction( float f )     { m_fFriction = f; }

}; // end IGOCBoundingVolume

} // end namespace gocs
} // end namespace tlib