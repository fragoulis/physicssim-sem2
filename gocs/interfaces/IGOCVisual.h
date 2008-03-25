#pragma once
#include "IGOComponent.h"

namespace tlib
{
namespace gocs
{

/**
 * @class IGOCVisual
 * 
 * Visual components' family interface.
 * All visual components are derived from this interface.
 */
class IGOCVisual : public IGOComponent
{
public:
    //! Constructor
    IGOCVisual();

    //! Virtual destructor
    virtual ~IGOCVisual();

    //! Returns the visual's family identifier, which "Visual"
    virtual ComponentId_t GetFamilyID() const {
        return CHash::_("Visual");
    }

    //! Override this function
    virtual void Render() const = 0;

}; // end IGOCVisual

} // end namespace gocs
} // end namespace tlib