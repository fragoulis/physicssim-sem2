#pragma once
#include "Interfaces/IGOCPhysicsDeformable.h"
#include "../Math/TVector2.h"
using tlib::math::Vec2f;
#include "../Math/TQuaternion.h"
using tlib::math::Quatf;

namespace tlib
{

namespace physics
{
    class CParticle;
}

namespace gocs
{

class GOCTPhysicsCloth;

/**
 * @class GOCPhysicsCloth
 *
 */
class GOCPhysicsCloth : public IGOCPhysicsDeformable
{
private:
    //! The cloth's halfsize [assumes cloth is quad]
    Vec2f m_vHalfSize;

    //! List of indices to the static points
    int *m_Static, m_iNumOfStatics;

public:
    GOCPhysicsCloth( const GOCTPhysicsCloth * const tpl );
    ~GOCPhysicsCloth();

    virtual ComponentId_t GetID() const {
        return CHash::_("PhysicsCloth");
    }

    virtual void Init();
    void Rotate( const Quatf &rot );
    
    const Vec3f& GetStaticPos( int index );
    int GetNumOfStatics() const { return m_iNumOfStatics; }

private:
    //! Sets up the particles and springs
    virtual void Setup();
    
    //! Saves the static points
    void SetupStaticPoints();
};

} // end namespace gocs
} // end namespace tlib