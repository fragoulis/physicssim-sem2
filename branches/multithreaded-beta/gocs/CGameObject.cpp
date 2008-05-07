#include "CGameObject.h"
#include "Interfaces/IGOComponent.h"
using namespace tlib::gocs;

// ----------------------------------------------------------------------------
CGameObject::CGameObject( const char *id ):
m_ID( CHash::_(id) ),
m_bActive(true)
{}

// ----------------------------------------------------------------------------
CGameObject::~CGameObject()
{
    ClearGOCs();
}

// ----------------------------------------------------------------------------
IGOComponent* CGameObject::SetGOC( IGOComponent* newGOC ) 
{
    newGOC->SetOwner(this);

    // Cache the family id
    const ComponentId_t familyId = newGOC->GetFamilyID();

    IGOComponent *oldGOC = m_Components[ familyId ];
    m_Components[ familyId ] = newGOC;

    return oldGOC;
}

// ----------------------------------------------------------------------------
void CGameObject::ClearGOCs()
{
    component_map_t::iterator i = m_Components.begin();
    for(; i!=m_Components.end(); ++i )
    {
        delete i->second;
        i->second = 0;
    }
}