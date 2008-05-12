#pragma once
#include "Interfaces/IGOCBoundingVolume.h"
#include "../Math/TVector3.h"
using tlib::math::Vec3f;

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


struct b_area 
{
	static const int areaSideMaxCount = 10;
	int *indices;
	int cx, cy, sidex, sidey;
	b_area(): indices(0), cx(0), cy(0), sidex(0), sidey(0) {}
	~b_area() { delete[] indices; indices = 0;}
};

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

	b_area *m_areas;
	int m_iNumOfAreas;

private:
    int m_iParticleIndex;

	void InitAreas();

public:
    GOCBoundingDeformable( const IGOCTBounding * const tpl );
    virtual ~GOCBoundingDeformable();

    virtual ComponentId_t GetID() const {
        return CHash::_("BoundingDeformable");
    }
    virtual void Init();

    //! Returns a particle each time increasine the stack index
	int GetArea(const Vec3f &pos);
    CParticle* GetParticle();
	CParticle* GetParticle(int areaIndex);
    IGOCBoundingVolume* GetPrimitive() { return m_Primitive; }
};

} // end namespace gocs
} // end namespace tlib