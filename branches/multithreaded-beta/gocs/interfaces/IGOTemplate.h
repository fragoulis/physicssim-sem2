#ifndef __TLIB_GOCS_IGOTEMPLATE_H__
#define __TLIB_GOCS_IGOTEMPLATE_H__
#pragma once

namespace tlib
{
namespace gocs
{

class IGOCTemplate;

/**
 * @class 
 * 
 */
class IGOTemplate
{
    typedef std::string ComponentId_t;
    typedef std::list<IGOCTemplate*> GOCTList_t;

protected:
    //! The game object template's name
    std::string m_sName;

    //! The list of component templates
    GOCTList_t m_Components;

protected:
    //! Constructor
    IGOTemplate( const std::string& name );

public:
    //! Destructor
    virtual ~IGOTemplate();

    // Accessors
    const std::string& GetName() const  { return m_sName; }
    GOCTList& Components() const        { return m_Components; }

    // Mutators
    void SetName( const std::string& sName ) { m_sName = sName; }

    //! Clears the component template list
    void Clear() { m_Components.clear(); }

    //! Adds a component template to the object's template
    void AddGOCTemplate( const IGOCTemplate* gcTemplate ) {
        m_Components.push_back();
    }

    //! Returns the requested template by name, if it exists
    IGOCTemplate* GetGOCTemplate( const ComponentId_t& id ) const {

    }

}; // end IGOTemplate

} // end namespace gocs
} // end namespace tlib

#endif // __TLIB_GOCS_IGOTEMPLATE_H__