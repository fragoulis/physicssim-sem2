#pragma once
#include "../CHash.h"

namespace tlib
{
namespace gocs
{

class IGOComponent;

/**
 * @class IGOCTemplate
 * 
 */
class IGOCTemplate
{
protected:
    typedef unsigned ComponentId_t;

private:
    //! The template's id
    ComponentId_t m_Id;

public:
    //! Constructor
    IGOCTemplate( const char *id );

    //! Destructor
    virtual ~IGOCTemplate() = 0 {}

    //! The component id that this template can create
    ComponentId_t GetComponentID() { return m_Id; }

    //! In which family the component belongs
    virtual ComponentId_t GetFamilyID() const = 0;

    virtual IGOComponent* MakeComponent() const = 0;

}; // end IGOCTemplate

} // end namespace gocs
} // end namespace tlib