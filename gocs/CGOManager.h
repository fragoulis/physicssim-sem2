#pragma once
#include "../Singleton.h"
#include "CHash.h"
#include <map>

namespace tlib
{
namespace gocs
{

class IGOCTemplate;
class IGOComponent;

/**
 * @class CGOManager
 * 
 */
class CGOManager : public Singleton<CGOManager>
{
private:
    typedef unsigned GOTemplateId_t;
    typedef std::map< const GOTemplateId_t, IGOCTemplate* > template_map_t;

    //! Associative array that holds the objects' templates
    mutable template_map_t m_Templates;

public:
    //! Returns a new game object
    IGOComponent* CreateGOC( const char *id ) const;

    //! Adds a template to the template array
    IGOCTemplate* SetTemplate( IGOCTemplate *newTpl );

private:
    friend Singleton<CGOManager>;
    CGOManager();
    ~CGOManager();

    //! Returns the template for that given object id
    IGOCTemplate* GetTemplate( const char *compId ) const {
        return m_Templates[ CHash::_( compId ) ];
    }

}; // end CGOManager

} // end namespace gocs
} // end namespace tlib