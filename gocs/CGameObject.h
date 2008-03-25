#ifndef __TLIB_GOCS_IGAMEOBJECT_H__
#define __TLIB_GOCS_IGAMEOBJECT_H__
#pragma once
#include <map>
#include "CTransform.h"
#include "CHash.h"

namespace tlib
{
namespace gocs
{

class IGOComponent;

/**
 * @class CGameObject
 *
 * Abstract game object class for all world objects. 
 * Every object has a list of components and all objects have a 
 * transformation component by default.
 */
class CGameObject
{
protected:
    typedef unsigned ObjectId_t;
    typedef unsigned ComponentId_t;

private:
    //! Object's transformation node
    CTransform m_Transform;

    //! Object's unique identifier
    ObjectId_t m_ID;

    typedef std::map< const ComponentId_t, IGOComponent* > component_map_t;
    //! Object's component map
    // This is declared as mutable to allow as to declare the GetGOC function
    // as a const, the way it should be, because std::map operator[] is not const.
    mutable component_map_t m_Components;

public:
    //! Constructor
    CGameObject( const char *id );

    //! Destructor
    virtual ~CGameObject();

    // Accessors
    ObjectId_t GetID() const { return m_ID; }
    const CTransform& GetTransform() const { return m_Transform; }
    CTransform& GetTransform() { return m_Transform; }

    // Mutators
    void SetID( ObjectId_t id ) { m_ID = id; }
    void SetTransform( const CTransform &tr ) { m_Transform = tr; }

    //! Adds a component to the object's component list
    //! @param newGOC Pointer to the new component
    //! @return The component that was replaced or NULL if no component pre-existed
    IGOComponent* SetGOC( IGOComponent* newGOC );

    //! Returns a component from the object's list with the matching id
    //! @param familyID The component's identifier
    //! @return The requested component or NULL is no such component exists
    IGOComponent* GetGOC( const char *familyID ) const {
        return m_Components[ CHash::_(familyID) ];
    }

    //! Clears the object's components
    void ClearGOCs();
};

} // end namespace gocs
} // end namespace tlib

#endif // __TLIB_GOCS_IGAMEOBJECT_H__