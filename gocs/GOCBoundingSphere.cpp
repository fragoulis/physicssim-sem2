#include "GOCBoundingSphere.h"
#include "Templates/GOCTBoundingSphere.h"
#include "Interfaces/IGOCPhysics.h"
#include "CGameObject.h"
#include <cassert>
using namespace tlib::gocs;

GOCBoundingSphere::GOCBoundingSphere( const GOCTBoundingSphere * const tpl ):
IGOCBoundingVolume(tpl),
m_fRadius(tpl->GetRadius())
{
    SetVolumeType(VT_SPHERE);
}

void GOCBoundingSphere::Init()
{
	IGOCBoundingVolume::Init();

	m_PhysicsGOC = GET_GOC(IGOCPhysics, "Physics");
    assert(m_PhysicsGOC);
}