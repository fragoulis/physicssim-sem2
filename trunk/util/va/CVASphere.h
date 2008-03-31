#pragma once
#include "IVertexArray.h"

namespace tlib
{
namespace util
{

class CVASphere : public IVertexArray
{
private:
    float m_fRadius;

public:
    CVASphere( const VArrayId &id, 
               float radius, 
               int stacks, 
               int slices );

    virtual bool Create( bool bTextured );
    virtual void Render() const;

    float GetRadius() const { return m_fRadius; }
    void SetRadius( float radius ) { m_fRadius = radius; }
};

} // end namespace util
} // end namespace tlib