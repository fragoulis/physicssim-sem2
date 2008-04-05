#include "CGOCManager.h"
#include "Interfaces/IGOCTemplate.h"
using namespace tlib::gocs;

CGOCManager::CGOCManager()
{}

CGOCManager::~CGOCManager()
{
    template_map_t::iterator i = m_Templates.begin();
    for(; i!=m_Templates.end(); ++i )
    {
        delete i->second;
        i->second = 0;
    }
}

IGOComponent* CGOCManager::CreateGOC( const char *id ) const
{
    IGOCTemplate *newTpl = GetTemplate( id );
    IGOComponent *newGoc = newTpl->MakeComponent();
    return newGoc;
}

IGOCTemplate* CGOCManager::SetTemplate( IGOCTemplate *newTpl )
{
    const GOCTemplateId_t& compId = newTpl->GetComponentID();

    IGOCTemplate *oldTpl = m_Templates[ compId ];
    m_Templates[ compId ] = newTpl;
    //m_Templates.insert( std::make_pair( compId, newTpl ) );

    return oldTpl;
}