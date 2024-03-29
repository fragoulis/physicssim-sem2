#include "CollisionDetector.h"
#include "CCollisionData.h"
#include "CParticle.h"

#include "../GOCS/CGameObject.h"
#include "../GOCS/GOCBoundingSphere.h"
#include "../GOCS/GOCBoundingPlane.h"
#include "../GOCS/GOCBoundingFinitePlane.h"
#include "../GOCS/GOCBoundingBox.h"

#include "../GOCS/GOCPhysicsPoint.h"
#include <cassert>

using namespace tlib::physics;
using namespace tlib::gocs;

#define E_ZERO 0.00001f

bool CollisionDetector::Check( IGOCBoundingVolume *a, 
                               IGOCBoundingVolume *b, 
                               CCollisionData     *data )
{
    if( a->GetVolumeType() == IGOCBoundingVolume::VT_SPHERE && 
        b->GetVolumeType() == IGOCBoundingVolume::VT_SPHERE )
    {
        return CheckSphereSphere( a, b, data );
    }

    // --- SPHERE | PLANE ---
    else if( a->GetVolumeType() == IGOCBoundingVolume::VT_SPHERE && 
             b->GetVolumeType() == IGOCBoundingVolume::VT_PLANE )
    {
        return CheckSpherePlane( a, b, data );
    }
    else if( a->GetVolumeType() == IGOCBoundingVolume::VT_SPHERE && 
             b->GetVolumeType() == IGOCBoundingVolume::VT_FIN_PLANE )
    {
        return CheckSphereFinitePlane( a, b, data );
    }

    // --- SPHERE | PRIMITIVE BOX ---
    //else if( a->GetVolumeType() == IGOCBoundingVolume::VT_SPHERE && 
    //         b->GetVolumeType() == IGOCBoundingVolume::VT_BOX )
    //{
    //    return CheckPrimitiveSphereBox( a, b );
    //}

    return false;
}

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
bool CollisionDetector::CheckSphereSphere( IGOCBoundingVolume *a, 
                                           IGOCBoundingVolume *b, 
                                           CCollisionData     *data )
{
    // Get Bounding components
    GOCBoundingSphere *sphere1 = (GOCBoundingSphere*)(a);
    GOCBoundingSphere *sphere2 = (GOCBoundingSphere*)(b);

    // Get physics components
	CParticle *point1 = &((GOCPhysicsPoint*)a->GetPhysicsGOC())->GetBody();
    CParticle *point2 = &((GOCPhysicsPoint*)b->GetPhysicsGOC())->GetBody();

    // Check if the absolute distance between the spheres is less than the sum of 
    // their radii
    const float fRadiiSum   = sphere1->GetRadius() + sphere2->GetRadius();
    Vec3f vDist             = point1->GetPosition() - point2->GetPosition();
    const float fDistance   = vDist.SqrLength();

    if( fRadiiSum * fRadiiSum <= fDistance ) return false;
    
    vDist.Normalize(); // Normalized distance equals collision normal
    
    // Fill contact data
    data->SetBody(0, point1 );
    data->SetBody(1, point2 );
    data->SetPenetration( fRadiiSum - sqrt(fDistance) );
    data->SetRestitution( ( sphere1->GetElasticity() + sphere2->GetElasticity() ) * 0.5f );
    data->SetFriction( ( sphere1->GetFriction() + sphere2->GetFriction() ) * 0.5f );
    data->SetNormal(vDist);

    return true;
} // end CheckSphereSphere()

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
bool CollisionDetector::CheckSpherePlane( IGOCBoundingVolume *a, 
                                          IGOCBoundingVolume *b, 
                                          CCollisionData     *data ) 
{
    // Get Bounding components
    GOCBoundingSphere *sphere = (GOCBoundingSphere*)(a);
    GOCBoundingPlane   *plane = (GOCBoundingPlane*)(b);

    // Get physics component
    CParticle *point = &((GOCPhysicsPoint*)a->GetPhysicsGOC())->GetBody();

    // Sphere data 
    const Vec3f &vSpherePos = point->GetPosition();
    const float fRadius     = sphere->GetRadius();
    // Plane data
    const Vec3f &vPlaneCenter = plane->GetOwner()->GetPosition();
    const Vec3f &vNormal      = plane->GetNormal();

    // Find the distance from the plane
    float fBallDistance = vNormal.Dot( vSpherePos - vPlaneCenter ) - fRadius;
    
    if( fBallDistance >= 0 ) return false;

    // Fill contact data
    data->SetBody(0, point );
    data->SetBody(1, NULL );
    data->SetPenetration( -fBallDistance );
    data->SetRestitution( ( sphere->GetElasticity() + plane->GetElasticity() ) * 0.5f );
    data->SetFriction( ( sphere->GetFriction() + plane->GetFriction() ) * 0.5f );
    data->SetNormal(vNormal);

    return true;

} // end CheckSpherePlane()

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
bool CollisionDetector::CheckSphereFinitePlane( IGOCBoundingVolume *a, 
                                                IGOCBoundingVolume *b, 
                                                CCollisionData     *data ) 
{
    // Get bounding components
    GOCBoundingSphere     *sphere = static_cast<GOCBoundingSphere*>(a);
    GOCBoundingFinitePlane *plane = static_cast<GOCBoundingFinitePlane*>(b);

    // Get physics component
    GOCPhysicsPoint *physics = (GOCPhysicsPoint*)a->GetPhysicsGOC();
    CParticle *point = &(physics->GetBody());

    // Sphere
    const Vec3f &vSpherePos = point->GetPosition();
    const float fRadius     = sphere->GetRadius();

    // Plane data
    const Vec3f &vPlaneCenter = plane->GetOwner()->GetPosition();
    const Vec2f &vHalfSize    = plane->GetHalfSize();

    const Vec3f pos_diff_now = vSpherePos - vPlaneCenter;

    // Check if the sphere is withing the planes size
    const Vec3f &Nx = plane->GetBinormal();     // Check binoral direction
    const float DistX = Nx.Dot( pos_diff_now ) - fRadius - vHalfSize.x();
    if( DistX >= 0.0f ) return false;
    
    const Vec3f vNormal = plane->GetNormal();
    const Vec3f Ny      = vNormal.Cross(Nx);          // Check tangent direction
    const float DistY   = Ny.Dot( pos_diff_now ) - fRadius - vHalfSize.y();
    if( DistY >= 0.0f ) return false;
    
	// Find the distance from the plane
    float fBallDistance1 = vNormal.Dot( pos_diff_now ) - fRadius;
    
	Vec3f vRevNormal = ~vNormal;
	float fBallDistance2 = vRevNormal.Dot( pos_diff_now ) - fRadius;

	if( fBallDistance1 >= 0 || fBallDistance2 >= 0 ) return false;

	float penetration = penetration = -fBallDistance1;
	Vec3f normal = vNormal;

	if( fBallDistance2 > fBallDistance1 ) 
	{
		penetration = -fBallDistance2;
		normal = vRevNormal;
	}

    // Fill contact data
    data->SetBody(0, point );
    data->SetBody(1, NULL );
    data->SetPenetration( penetration );
    data->SetRestitution( ( sphere->GetElasticity() + plane->GetElasticity() ) * 0.5f );
    data->SetFriction( ( sphere->GetFriction() + plane->GetFriction() ) * 0.5f );
    data->SetNormal(normal);

    return true;

} // end CheckSphereFinitePlane()

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
bool CollisionDetector::CheckPlaneParticle( IGOCBoundingVolume *a, 
                                            CParticle *b, 
                                            CCollisionData *data ) 
{
    if( !data ) return false;
    if( b->GetInverseMass() <= 0 ) return false;

    GOCBoundingPlane *plane = static_cast<GOCBoundingPlane*>(a);

    // Sphere data
    const float fParticleRadius = 0.01f; /* make particle a little bigger */

    // Plane data
    const Vec3f &vPlaneCenter = plane->GetOwner()->GetPosition();
    const Vec3f &vNormal      = plane->GetNormal();

    // Find the distance from the plane
    float fDistance = vNormal.Dot( b->GetPosition() ) - 
                      fParticleRadius - vNormal.Dot( vPlaneCenter );
    
    if( fDistance >= 0 ) return false;

    GOCPhysicsPoint *p1 = (GOCPhysicsPoint*)a->GetOwner()->GetGOC("Physics");

    // Fill contact data
    data->SetBody(0, b );
    data->SetBody(1, NULL );
    data->SetPenetration( -fDistance );
    data->SetRestitution( plane->GetElasticity() );
    data->SetFriction( plane->GetFriction() );
    data->SetNormal(vNormal);

    return true;

} // end CheckPlaneParticle()

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
bool CollisionDetector::CheckSphereParticle( IGOCBoundingVolume *a, 
                                             CParticle *b, 
                                             CCollisionData *data ) 
{
    if( !data ) return false;

    // Get bounding component
    GOCBoundingSphere *sphere = (GOCBoundingSphere*)(a);

    // Get physics component
    //CParticle *point = &((GOCPhysicsPoint*)a->GetOwner()->GetGOC("Physics"))->GetBody();
	CParticle *point = &((GOCPhysicsPoint*)a->GetPhysicsGOC())->GetBody();

    // Sphere data
	const float extraRadius = 0.028f;
    const float fRadiiSum   = sphere->GetRadius() + extraRadius; /* make the sphere a little bigger */
	//const float fRadiiSum = 0.075f;
    Vec3f vDistance = point->GetPosition() - b->GetPosition();
    const float fDistance = vDistance.SqrLength();

    if( fDistance >= fRadiiSum * fRadiiSum ) return false;

    vDistance.Normalize();

    // Fill contact data
    data->SetBody(0, point );
    data->SetBody(1, b );
    data->SetPenetration( fRadiiSum - sqrt(fDistance) );
    data->SetRestitution( 1.0f );
    data->SetFriction( 0.0f );
    data->SetNormal(vDistance);

    return true;

} // end CheckSphereDeformable()

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
bool CollisionDetector::CheckPrimitiveSphereBox( IGOCBoundingVolume *a, 
                                                 IGOCBoundingVolume *b ) 
{
    GOCBoundingSphere *sphere = (GOCBoundingSphere*)(a);
    GOCBoundingBox *box = (GOCBoundingBox*)(b);

    const Vec3f &vBoxPos = box->GetOwner()->GetPosition();
    const Vec3f &vRelPos = sphere->GetOwner()->GetPosition();

    const Vec3f boxEx = box->GetHalfSize() + 0.01f;

    if( vRelPos.x() - sphere->GetRadius() > vBoxPos.x() + boxEx.x() ) return false;
    if( vRelPos.x() + sphere->GetRadius() < vBoxPos.x() - boxEx.x() ) return false;

    if( vRelPos.y() - sphere->GetRadius() > vBoxPos.y() + boxEx.y() ) return false;
    if( vRelPos.y() + sphere->GetRadius() < vBoxPos.y() - boxEx.y() ) return false;

    if( vRelPos.z() - sphere->GetRadius() > vBoxPos.z() + boxEx.z() ) return false;
    if( vRelPos.z() + sphere->GetRadius() < vBoxPos.z() - boxEx.z() ) return false;

    return true;
}