#pragma once
#include "Interfaces/IGOCBoundingVolume.h"

namespace tlib
{
namespace gocs
{

    class GOCTBoundingSphere;

/**
 * @class GOCBoundingSphere
 *
 */
class GOCBoundingSphere : public IGOCBoundingVolume
{
private:
    //! Bounding sphere's radius
    float m_fRadius;

public:
    GOCBoundingSphere( const GOCTBoundingSphere * const tpl );

	virtual void Init();
    virtual ComponentId_t GetID() const {
        return CHash::_("BoundingSphere");
    }

    float GetRadius() const { return m_fRadius; }

};

} // end namespace gocs
} // end namespace tlib