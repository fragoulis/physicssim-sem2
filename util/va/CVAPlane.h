#pragma once
#include "IVertexArray.h"
#include "../../Math/TVector2.h"
using tlib::math::Vec2f;

namespace tlib
{
namespace util
{

class CVAPlane : public IVertexArray
{
private:
    Vec2f m_vHalfSize;

public:
    CVAPlane( const VArrayId &id, 
              const Vec2f &halfsize, 
              int stacks, 
              int slices );

    virtual bool Create( bool bTextured );
    virtual void Render() const;

    const Vec2f& GetHalfSize() const { return m_vHalfSize; }
    void SetHalfSize( const Vec2f &v ) { m_vHalfSize = v; }
};

} // end namespace util
} // end namespace tlib