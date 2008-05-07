#pragma once
#include "../Singleton.h"
#include "CHash.h"
#include <map>

#define ADD_GOC( go, goc ) { go->SetGOC( CGOCManager::Get().CreateGOC( goc ) ); }

namespace tlib
{
namespace gocs
{

class IGOCTemplate;
class IGOComponent;

/**
 * @class CGOCManager
 * 
 */
class CGOCManager : public Singleton<CGOCManager>
{
private:
    typedef unsigned GOCTemplateId_t;
    typedef std::map< const GOCTemplateId_t, IGOCTemplate* > template_map_t;

    //! Associative array that holds the objects' templates
    mutable template_map_t m_Templates;

public:
    //! Returns a new game object
    IGOComponent* CreateGOC( const char *id ) const;

    //! Adds a template to the template array
    IGOCTemplate* SetTemplate( IGOCTemplate *newTpl );

private:
    friend Singleton<CGOCManager>;
    CGOCManager();
    ~CGOCManager();

    //! Returns the template for that given object id
    IGOCTemplate* GetTemplate( const char *compId ) const {
        return m_Templates[ CHash::_( compId ) ];
    }

}; // end CGOCManager

} // end namespace gocs
} // end namespace tlib