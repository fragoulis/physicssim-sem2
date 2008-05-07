#pragma once
#include "../CHash.h"

namespace tlib
{
namespace gocs
{

// A small helper
#define GET_GOC( goc_type, goc_id ) \
    static_cast<goc_type*>( GetOwner()->GetGOC(goc_id) )
    /* end */

#define GET_OBJ_GOC( obj, goc_type, goc_id ) \
    static_cast<goc_type*>( obj->GetGOC(goc_id) )
    /* end */

class CGameObject;

/**
 * @class IGOComponent
 * 
 * Game object component class.
 * Abstract class where all game object components are derived from.
 */
class IGOComponent
{
protected:
    typedef unsigned ComponentId_t;

private:
    //! Component's owner game object
    CGameObject *m_Owner;
    
public:
    //! Constructor
    IGOComponent();

    //! Virtual destructor
    virtual ~IGOComponent() = 0 {}
    
    bool IsOwnerActive() const;
    CGameObject* GetOwner() const { return m_Owner; }
    void SetOwner( CGameObject* go ) 
    { 
        m_Owner = go; 
        Init();
    }

    //! Override this to return the component's identifier
    virtual ComponentId_t GetID() const = 0;

    //! Override this to return the component's family identifier
    virtual ComponentId_t GetFamilyID() const = 0;

    //! Override this to reset the component's values
    virtual void Reset() {}

private:
    //! Override this to add immediate functionality after the component
    //! has been properly added to its owner
    virtual void Init() {}
};

} // end namespace gocs
} // end namespace tlib