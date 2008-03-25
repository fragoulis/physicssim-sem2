#pragma once
#include "../math/TVector3.h"
#include "../math/TQuaternion.h"

namespace tlib
{

using math::Vec3f;
using math::Quatf;

/**
 * @class CTransform
 *
 * Represents the transformation of an object.
 * The node structure holds information about position and orientation 
 * in the world. It represents the transformation [position and rotation] 
 * of an object but can be used for more arbitrary causes as well.
 */
class CTransform
{
private:
    //! The position of the node
    Vec3f m_vPosition;

    //! The orientation of the node
    Quatf m_qOrientation;

public:
    CTransform()
    {
        m_vPosition.Clear();
        m_qOrientation.Clear();
    }

    CTransform( const Vec3f &vPos, const Quatf &qOri ):
    m_vPosition(vPos),
    m_qOrientation(qOri)
    {}

    const Vec3f& GetPosition() const { return m_vPosition; }
    Vec3f& GetPosition() { return m_vPosition; }

    const Quatf& GetOrientation() const { return m_qOrientation; }
    Quatf& GetOrientation() { return m_qOrientation; }

    void SetPosition( const Vec3f &vPos )       { m_vPosition = vPos; }
    void SetOrientation( const Quatf &qOri )    { m_qOrientation = qOri; }
};

} // end namespace tlib