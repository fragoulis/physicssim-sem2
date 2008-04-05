#include "MainApp.h"

#include "CSceneManager.h"      // We need these to destroy the managers from the
#include "CPhysicsManager.h"    // app instead of the window
#include "GOCS/CGOCManager.h"   // The component manager

// Visual component templates
#include "GOCS/Templates/GOCTVisualVASphere.h"  
#include "GOCS/Templates/GOCTVisualVAPlane.h"
#include "GOCS/Templates/GOCTVisualIMQuad.h"
// Physical component templates
#include "GOCS/Templates/GOCTPhysicsPoint.h"
#include "GOCS/Templates/GOCTPhysicsCloth.h"
// Collision componenet templates
#include "GOCS/Templates/GOCTBoundingSphere.h"
#include "GOCS/Templates/GOCTBoundingPlane.h"
#include "GOCS/Templates/GOCTBoundingDWBox.h"

// The world object class
#include "GOCS/CGameObject.h"

#include "Util/Config.h"

using namespace tlib::gocs;
using namespace tlib;

static MainApp g_MainApp;

CPhysicsThread MainApp::m_tPhysics;

// ----------------------------------------------------------------------------
MainApp& MainApp::Get() { return g_MainApp; }

// ----------------------------------------------------------------------------
void MainApp::OnCreate()
{
    m_iNumOfSmallSpheres = 0;
    m_iNumOfBigSpheres = 0;
    m_bPause = false;

    CGOCManager::_Get();
    InitTemplates();
    InitPlanes();
    InitCloth();

    m_tPhysics.Start();
}

// ----------------------------------------------------------------------------
void MainApp::OnDestroy()
{
    CGOCManager::Destroy();

    // Delete spheres
    ObjectList::iterator iter = m_Spheres.begin();
    for(; iter != m_Spheres.end(); ++iter )
        delete (*iter);

    // Delete planes
    for( int i=0; i<MAX_PLANES; ++i )
        delete m_Planes[i];

    // Delete cloth
    delete m_Cloth;

    // Destroy managers after all objects are destroy !!!
    MGRScene::Destroy();
    MGRPhysics::Destroy();

    m_tPhysics.Terminate();
}

// ----------------------------------------------------------------------------
void MainApp::GetNumOfSpheres( int &ss, int &bs ) 
{ 
    ss = m_iNumOfSmallSpheres;
    bs = m_iNumOfBigSpheres;
}

// ----------------------------------------------------------------------------
void MainApp::RemoveLastSphere()
{
    if( m_Spheres.size() )
    {
        // Get last element
        CGameObject *el = m_Spheres[ m_Spheres.size() - 1 ];
        
        if( el->Is("MyBigSphere") ) {
            m_iNumOfBigSpheres--;
        } else {
            m_iNumOfSmallSpheres--;
        }

        delete el;
        el = 0;

        m_Spheres.pop_back();
    }
}

// ----------------------------------------------------------------------------
void MainApp::AddBigSphere()
{
    CGameObject *pSphere = new CGameObject("BigSphere");
    ADD_GOC( pSphere, "VisualBigSphere" );
    ADD_GOC( pSphere, "PhysicsBigPoint"   );
    ADD_GOC( pSphere, "BoundingBigSphere" );

    m_Spheres.push_back(pSphere);
    m_iNumOfBigSpheres++;
}

// ----------------------------------------------------------------------------
void MainApp::AddSmallSphere()
{
    CGameObject *pSphere = new CGameObject("SmallSphere");
    ADD_GOC( pSphere, "VisualSmallSphere" );
    ADD_GOC( pSphere, "PhysicsSmallPoint"   );
    ADD_GOC( pSphere, "BoundingSmallSphere" );

    m_Spheres.push_back(pSphere);
    m_iNumOfSmallSpheres++;
}

// ----------------------------------------------------------------------------
void MainApp::Reset()
{
    // Remove all spheres
    while( m_Spheres.size() )
    {
        CGameObject *el = m_Spheres[ m_Spheres.size() - 1 ];
        delete el;
        el = 0;
        m_Spheres.pop_back();
    }
    m_iNumOfBigSpheres = 0;
    m_iNumOfSmallSpheres = 0;

    // Hide deformable !
}

// ----------------------------------------------------------------------------
void MainApp::InitTemplates()
{
    CFG_CLIENT_OPEN;

    float radius, mass, elasticity, friction, size[2], normal[3];
    int stacks, slices;

    // ------------------------------------------------------------------------
    // CUSTOM SPHERE TEMPLATE
    CFG_LOAD("VisualSphere");

    CFG_1f("BigRadius", radius);
    CFG_1i("BigStacks", stacks);
    CFG_1i("BigSlices", slices);
    GOCTVisualVASphere *tplBig = new GOCTVisualVASphere("VisualBigSphere");
    tplBig->SetRadius(radius);
    tplBig->SetStacks(stacks);
    tplBig->SetSlices(slices);
    CGOCManager::Get().SetTemplate( tplBig );

    CFG_1f("SmallRadius", radius);
    CFG_1i("SmallStacks", stacks);
    CFG_1i("SmallSlices", slices);
    GOCTVisualVASphere *tplSmall = new GOCTVisualVASphere("VisualSmallSphere");;
    tplSmall->SetRadius(radius);
    tplSmall->SetStacks(stacks);
    tplSmall->SetSlices(slices);
    CGOCManager::Get().SetTemplate( tplSmall );


    // ------------------------------------------------------------------------
    // CUSTOM PHYSICAL POINT TEMPLATE
    CFG_LOAD("PhysicsPoint");

    CFG_1f("BigMass", mass);
    GOCTPhysicsPoint *tplPhyPoi = new GOCTPhysicsPoint("PhysicsBigPoint");
    tplPhyPoi->GetBody().SetMass(mass);
    CGOCManager::Get().SetTemplate( tplPhyPoi );

    CFG_1f("SmallMass", mass);
    tplPhyPoi = new GOCTPhysicsPoint("PhysicsSmallPoint");
    tplPhyPoi->GetBody().SetMass(mass);
    CGOCManager::Get().SetTemplate( tplPhyPoi );


    // ------------------------------------------------------------------------
    // CUSTOM BOUNDING SPHERE TEMPLATE
    CFG_LOAD("BoundingSphere");

    CFG_1f("BigRadius", radius);
    CFG_1f("BigElasticity", elasticity);
    CFG_1f("BigFriction", friction);
    GOCTBoundingSphere *tplBsph = new GOCTBoundingSphere("BoundingBigSphere");
    tplBsph->SetRadius(radius);
    tplBsph->SetElasticity(elasticity);
    tplBsph->SetFriction(friction);
    CGOCManager::Get().SetTemplate( tplBsph );
    
    CFG_1f("SmallRadius", radius);
    CFG_1f("SmallElasticity", elasticity);
    CFG_1f("SmallFriction", friction);
    tplBsph = new GOCTBoundingSphere("BoundingSmallSphere");
    tplBsph->SetRadius(radius);
    tplBsph->SetElasticity(elasticity);
    tplBsph->SetFriction(friction);
    CGOCManager::Get().SetTemplate( tplBsph );


    // ------------------------------------------------------------------------
    // PLANE TEMPLATES
    CFG_LOAD("Plane");

    CFG_2fv("Halfsize", size);
    CFG_3fv("Normal", normal);
    CFG_1f("Elasticity", elasticity);
    CFG_1f("Friction", friction);

    GOCTVisualIMQuad *tplPlane = new GOCTVisualIMQuad("VisualPlane");
    tplPlane->SetHalfSize( Vec2f( size ) );
    CGOCManager::Get().SetTemplate( tplPlane );

    GOCTBoundingPlane *tplBpln = new GOCTBoundingPlane("BoundingPlane");
    tplBpln->SetHalfSize( Vec2f( size ) );
    tplBpln->SetNormal( Vec3f( normal ) );
    tplBpln->SetElasticity(elasticity);
    tplBpln->SetFriction(friction);
    CGOCManager::Get().SetTemplate( tplBpln );

    // ------------------------------------------------------------------------
    // CLOTH TEMPLATES
    CFG_LOAD("Cloth");

    CFG_2fv("Halfsize", size);
    CFG_1f("SmallMass", mass);
    CFG_1i("Stacks", stacks);
    CFG_1i("Slices", slices);
    CFG_1f("Elasticity", elasticity);
    CFG_1f("Friction", friction);

    GOCTVisualVAPlane *vaPlane = new GOCTVisualVAPlane("ClothVisualPlane");
    vaPlane->SetHalfSize( Vec2f( size ) );
    vaPlane->SetStacks(stacks);
    vaPlane->SetSlices(slices);
    CGOCManager::Get().SetTemplate( vaPlane );

    GOCTBoundingDWBox *tplBbox = new GOCTBoundingDWBox("ClothBoundingDef");
    //tplBbox->SetHalfSize( Vec3f( 0.25f, 0.5f, 0.2f ) );
    tplBbox->SetElasticity(elasticity);
    tplBbox->SetFriction(friction);
    CGOCManager::Get().SetTemplate( tplBbox );

    GOCTPhysicsCloth *tplDfmr = new GOCTPhysicsCloth("ClothDeformable");
    tplDfmr->SetHalfSize( Vec2f( size ) );
    tplDfmr->SetStacks(stacks);
    tplDfmr->SetSlices(slices);
    tplDfmr->SetMass(mass);
    CGOCManager::Get().SetTemplate( tplDfmr );
} // end InitTemplates()

// ----------------------------------------------------------------------------
void MainApp::InitPlanes()
{
    // TODO: Read all values from config file

    const float POSITION = 0.5f;

    // Back plane
    int iP = 0;
    m_Planes[iP] = new CGameObject("Plane");
    m_Planes[iP]->GetTransform().GetPosition().Set( 0.0f, 0.0f, -POSITION );
    ADD_GOC( m_Planes[iP], "VisualPlane" );
    ADD_GOC( m_Planes[iP], "BoundingPlane" );

    // Left plane
    ++iP;
    m_Planes[iP] = new CGameObject("Plane");
    m_Planes[iP]->GetTransform().GetPosition().Set( -POSITION, 0.0f, 0.0f );
    m_Planes[iP]->GetTransform().GetOrientation().FromVector( float(M_PI_2), Vec3f( 0.0f, 1.0f, 0.0f ) );
    ADD_GOC( m_Planes[iP], "VisualPlane" );
    ADD_GOC( m_Planes[iP], "BoundingPlane" );

    // Right plane
    ++iP;
    m_Planes[iP] = new CGameObject("Plane");
    m_Planes[iP]->GetTransform().GetPosition().Set( POSITION, 0.0f, 0.0f );
    m_Planes[iP]->GetTransform().GetOrientation().FromVector( -float(M_PI_2), Vec3f( 0.0f, 1.0f, 0.0f ) );
    ADD_GOC( m_Planes[iP], "VisualPlane" );
    ADD_GOC( m_Planes[iP], "BoundingPlane" );

    // Top plane
    ++iP;
    m_Planes[iP] = new CGameObject("Plane");
    m_Planes[iP]->GetTransform().GetPosition().Set( 0.0f, POSITION, 0.0f );
    m_Planes[iP]->GetTransform().GetOrientation().FromVector( float(M_PI_2), Vec3f( 1.0f, 0.0f, 0.0f ) );
    ADD_GOC( m_Planes[iP], "VisualPlane" );
    ADD_GOC( m_Planes[iP], "BoundingPlane" );

    // Bottom plane
    ++iP;
    m_Planes[iP] = new CGameObject("Plane");
    m_Planes[iP]->GetTransform().GetPosition().Set( 0.0f, -POSITION, 0.0f );
    m_Planes[iP]->GetTransform().GetOrientation().FromVector( -float(M_PI_2), Vec3f( 1.0f, 0.0f, 0.0f ) );
    ADD_GOC( m_Planes[iP], "VisualPlane" );
    ADD_GOC( m_Planes[iP], "BoundingPlane" );

    // Front plane
    ++iP;
    m_Planes[iP] = new CGameObject("Plane");
    m_Planes[iP]->GetTransform().GetPosition().Set( 0.0f, 0.0f, POSITION );
    m_Planes[iP]->GetTransform().GetOrientation().FromVector( -float(M_PI), Vec3f( 0.0f, 1.0f, 0.0f ) );
    ADD_GOC( m_Planes[iP], "VisualPlane" );
    ADD_GOC( m_Planes[iP], "BoundingPlane" );
}

// ----------------------------------------------------------------------------
void MainApp::InitCloth()
{
    // TODO: Read all values from config file

    m_Cloth = new CGameObject("Cloth");
    m_Cloth->GetTransform().GetPosition().Set( -0.25f, 0.0f, 0.0f );
    //m_Cloth->GetTransform().GetPosition().Set( 0.0f, 0.4f, 0.0f );
    m_Cloth->GetTransform().GetOrientation().FromVector( -float(M_PI_2), Vec3f( 1.0f, 0.0f, 0.0f ) );
    ADD_GOC( m_Cloth, "ClothVisualPlane" );
    ADD_GOC( m_Cloth, "ClothDeformable" );
    ADD_GOC( m_Cloth, "ClothBoundingDef" );
}