#pragma once
#include "IGOCTemplate.h"

namespace tlib
{
namespace gocs
{

/**
 * @class GOCTBounding
 * 
 * Holds the basic material properties of an object such
 * as elasticity and friction.
 */
class IGOCTBounding : public IGOCTemplate
{
private:
    //! Elasticity coefficient
    float m_fElasticity;

    //! Friction coefficient
    float m_fFriction;

public:
    IGOCTBounding( const char *id );
    virtual ~IGOCTBounding() = 0 {}

    virtual ComponentId_t GetFamilyID() const {
        return CHash::_("BoundingVolume");
    }

    float GetElasticity() const     { return m_fElasticity; }
    float GetFriction() const       { return m_fFriction; }

    void SetElasticity( float e )   { m_fElasticity = e; }
    void SetFriction( float f )     { m_fFriction = f; }

}; // end IGOCTBounding

} // end namespace gocs
} // end namespace tlib