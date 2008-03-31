#include "CollisionDetector.h"
#include "CCollisionData.h"
#include "CParticle.h"

#include "../GOCS/CGameObject.h"
#include "../GOCS/GOCBoundingSphere.h"
#include "../GOCS/GOCBoundingPlane.h"
#include "../GOCS/GOCBoundingBox.h"

#include "../GOCS/GOCPhysicsPoint.h"

using namespace tlib::physics;
using namespace tlib::gocs;

#define E_ZERO 0.01f

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
bool CollisionDetector::CheckSphereSphere( IGOCBoundingVolume *a, 
                                           IGOCBoundingVolume *b, 
                                           CCollisionData     *data )
{
    GOCBoundingSphere *sphere1 = static_cast<GOCBoundingSphere*>(a);
    GOCBoundingSphere *sphere2 = static_cast<GOCBoundingSphere*>(b);

    // Check if the absolute distance between the spheres is less than the sum of 
    // their radii
    const float fRadiiSum   = sphere1->GetRadius() + sphere2->GetRadius();
    const Vec3f &vPosA      = sphere1->GetOwner()->GetTransform().GetPosition();
    const Vec3f &vPosB      = sphere2->GetOwner()->GetTransform().GetPosition();
    Vec3f vDist             = vPosA - vPosB;
    const float fDistance   = vDist.SqrLength();

    if( fRadiiSum * fRadiiSum <= fDistance ) return false;
    
    vDist.Normalize(); // Normalized distance equals collision normal

    // Get physics components
    GOCPhysicsPoint *p1 = (GOCPhysicsPoint*)a->GetOwner()->GetGOC("Physics");
    GOCPhysicsPoint *p2 = (GOCPhysicsPoint*)b->GetOwner()->GetGOC("Physics");
    
    // Fill contact data
    data->SetBody(0, &(p1->GetBody()) );
    data->SetBody(1, &(p2->GetBody()) );
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
    GOCBoundingSphere *sphere = static_cast<GOCBoundingSphere*>(a);
    GOCBoundingPlane *plane = static_cast<GOCBoundingPlane*>(b);

    // Sphere data 
    const Vec3f &vSpherePos = sphere->GetOwner()->GetTransform().GetPosition();
    const float fRadius     = sphere->GetRadius();
    // Plane data
    const Vec3f &vPlaneCenter = plane->GetOwner()->GetTransform().GetPosition();
    const Vec3f &vNormal      = plane->GetNormal();

    // Find the distance from the plane
    float fBallDistance = vNormal.Dot( vSpherePos ) - 
                          fRadius - vNormal.Dot( vPlaneCenter );
    
    if( fBallDistance >= 0 ) return false;

    GOCPhysicsPoint *p1 = (GOCPhysicsPoint*)a->GetOwner()->GetGOC("Physics");

    // Fill contact data
    data->SetBody(0, &(p1->GetBody()) );
    data->SetBody(1, NULL );
    data->SetPenetration( -fBallDistance );
    data->SetRestitution( ( sphere->GetElasticity() + plane->GetElasticity() ) * 0.5f );
    data->SetFriction( ( sphere->GetFriction() + plane->GetFriction() ) * 0.5f );
    data->SetNormal(vNormal);

    return true;

} // end CheckSpherePlane()

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
    const float fParticleRadius   = 0.01f; /* make particle a little bigger */

    // Plane data
    const Vec3f &vPlaneCenter = plane->GetOwner()->GetTransform().GetPosition();
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

} // end CheckPlaneDeformable()

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
bool CollisionDetector::CheckSphereParticle( IGOCBoundingVolume *a, 
                                             CParticle *b, 
                                             CCollisionData *data ) 
{
    if( !data ) return false;
    GOCBoundingSphere *sphere = static_cast<GOCBoundingSphere*>(a);

    // Sphere data
    const float fRadiiSum   = sphere->GetRadius() + 0.02f; /* make the sphere a little bigger */
    const Vec3f &vSpherePos = sphere->GetOwner()->GetTransform().GetPosition();

    Vec3f vDistance = vSpherePos - b->GetPosition();
    const float fDistance = vDistance.SqrLength();

    if( fDistance >= fRadiiSum * fRadiiSum ) return false;

    vDistance.Normalize();

    GOCPhysicsPoint *p1 = (GOCPhysicsPoint*)a->GetOwner()->GetGOC("Physics");

    // Fill contact data
    data->SetBody(0, &(p1->GetBody()) );
    data->SetBody(1, b );
    data->SetPenetration( fRadiiSum - sqrt(fDistance) );
    data->SetRestitution( 1.0f );
    data->SetFriction( 0.0f );
    data->SetNormal(vDistance);

    return true;

} // end CheckSphereDeformable()

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
bool CollisionDetector::CheckParticleParticle( CParticle *a, 
                                               CParticle *b, 
                                               CCollisionData *data ) 
{
    if( !data ) return false;

    // Sphere data
    const float fRadiiSum   = 0.01f + 0.01f; /* make the sphere a little bigger */

    Vec3f vDistance = a->GetPosition() - b->GetPosition();
    const float fDistance = vDistance.SqrLength();

    if( fDistance >= fRadiiSum * fRadiiSum ) return false;

    vDistance.Normalize();

    // Fill contact data
    data->SetBody(0, a );
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
    //GOCBoundingSphere *sphere = static_cast<GOCBoundingSphere*>(a);
    //GOCBoundingBox *box = static_cast<GOCBoundingBox*>(b);

    //const Quatf &qBoxOri = box->GetOwner()->GetTransform().GetOrientation();
    //const Vec3f &vBoxPos = box->GetOwner()->GetTransform().GetPosition();

    //Vec3f vRelPos = sphere->GetOwner()->GetTransform().GetPosition();
    //vRelPos -= vBoxPos;
    //(~qBoxOri).Rotate(vRelPos);

    ////std::cout << "Relative ... " << vRelPos << std::endl;

    //if( fabs(vRelPos.x()) - sphere->GetRadius() > box->GetHalfSize().x() ||
    //    fabs(vRelPos.y()) - sphere->GetRadius() > box->GetHalfSize().y() ||
    //    fabs(vRelPos.z()) - sphere->GetRadius() > box->GetHalfSize().z() )
    //{
    //    return false;
    //}

    //static int num = 0;
    //std::cout << "Collision Detected ... " << num << std::endl;
    //num++;

    return true;
}