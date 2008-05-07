#pragma once

namespace tlib
{

namespace gocs
{
    class IGOCBoundingVolume;
}
using gocs::IGOCBoundingVolume;

namespace physics
{

    class CCollisionData;
    class CParticle;

/**
 * @class CollisionDetector
 *
 */
class CollisionDetector
{
public:
    static bool Check( IGOCBoundingVolume *a, IGOCBoundingVolume *b, CCollisionData *data );

    // Basics collision detection
    static bool CheckSphereSphere( IGOCBoundingVolume *a, IGOCBoundingVolume *b, CCollisionData *data );
    static bool CheckSpherePlane( IGOCBoundingVolume *a, IGOCBoundingVolume *b, CCollisionData *data );
    static bool CheckSphereFinitePlane( IGOCBoundingVolume *a, IGOCBoundingVolume *b, CCollisionData *data );

    static bool CheckPlaneParticle( IGOCBoundingVolume *a, CParticle *b, CCollisionData *data );
    static bool CheckSphereParticle( IGOCBoundingVolume *a, CParticle *b, CCollisionData *data );

    // Simple primitive checks. No collision data.
    static bool CheckPrimitiveSphereBox( IGOCBoundingVolume *a, IGOCBoundingVolume *b );

private:
    
};

} // end namespace physics
} // end namespace tlib