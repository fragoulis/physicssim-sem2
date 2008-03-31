#include "IGOCPhysics.h"
#include "../../CPhysicsManager.h"
using namespace tlib::gocs;

IGOCPhysics::IGOCPhysics():
m_vAcceleration(0.0f, 0.0f, 0.0f)
{
    // Automatically register this physics component with
    // the physics engine

    MGRPhysics::_Get().RegisterPhysical(this);
}

IGOCPhysics::~IGOCPhysics()
{
    MGRPhysics::_Get().UnregisterPhysical(this);
}