#include "IGOComponent.h"
#include "../CGameObject.h"
using namespace tlib::gocs;

IGOComponent::IGOComponent():
m_Owner(0)
{}

bool IGOComponent::IsOwnerActive() const
{
    return m_Owner->IsActive();
}