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

using namespace tlib::gocs;
using namespace tlib;

static MainApp g_MainApp;

// ----------------------------------------------------------------------------
MainApp& MainApp::Get() { return g_MainApp; }

// ----------------------------------------------------------------------------
void MainApp::OnCreate()
{
    m_iNumOfSmallSpheres = 0;
    m_iNumOfBigSpheres = 0;

    CGOCManager::_Get();
    InitTemplates();
    InitPlanes();
    InitCloth();
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
    CGameObject *pSphere = new CGameObject("MyBigSphere");
    ADD_GOC( pSphere, "VisualBigSphere" );
    ADD_GOC( pSphere, "PhysicsBigPoint"   );
    ADD_GOC( pSphere, "BoundingBigSphere" );

    m_Spheres.push_back(pSphere);
    m_iNumOfBigSpheres++;
}

// ----------------------------------------------------------------------------
void MainApp::AddSmallSphere()
{
    CGameObject *pSphere = new CGameObject("MySmallSphere");
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
    // TODO: Read all values from config file

    // CUSTOM SPHERE TEMPLATE
    GOCTVisualVASphere *tplBig = new GOCTVisualVASphere("VisualBigSphere");
    tplBig->SetRadius(0.05f);
    tplBig->SetSlices(15);
    tplBig->SetStacks(15);
    CGOCManager::Get().SetTemplate( tplBig );

    GOCTVisualVASphere *tplSmall = new GOCTVisualVASphere("VisualSmallSphere");;
    tplSmall->SetRadius(0.02f);
    tplSmall->SetSlices(10);
    tplSmall->SetStacks(10);
    CGOCManager::Get().SetTemplate( tplSmall );


    // CUSTOM PHYSICAL POINT TEMPLATE
    GOCTPhysicsPoint *tplPhyPoi = new GOCTPhysicsPoint("PhysicsBigPoint");
    tplPhyPoi->GetBody().SetMass(1.0f);
    CGOCManager::Get().SetTemplate( tplPhyPoi );

    tplPhyPoi = new GOCTPhysicsPoint("PhysicsSmallPoint");
    tplPhyPoi->GetBody().SetMass(0.25f);
    CGOCManager::Get().SetTemplate( tplPhyPoi );


    // CUSTOM BOUNDING SPHERE TEMPLATE
    GOCTBoundingSphere *tplBsph = new GOCTBoundingSphere("BoundingBigSphere");
    tplBsph->SetRadius(0.05f);
    tplBsph->SetElasticity(0.95f);
    tplBsph->SetFriction(0.1f);
    CGOCManager::Get().SetTemplate( tplBsph );
    
    tplBsph = new GOCTBoundingSphere("BoundingSmallSphere");
    tplBsph->SetRadius(0.02f);
    tplBsph->SetElasticity(0.95f);
    tplBsph->SetFriction(0.1f);
    CGOCManager::Get().SetTemplate( tplBsph );


    // PLANE TEMPLATES
    GOCTVisualIMQuad *tplPlane = new GOCTVisualIMQuad("VisualPlane");
    tplPlane->SetHalfSize( Vec2f( 0.5f, 0.5f ) );
    CGOCManager::Get().SetTemplate( tplPlane );

    GOCTBoundingPlane *tplBpln = new GOCTBoundingPlane("BoundingPlane");
    tplBpln->SetHalfSize( Vec2f( 0.5f, 0.5f ) );
    tplBpln->SetNormal( Vec3f( 0.0f, 0.0f, 1.0f ) );
    tplBpln->SetElasticity(0.95f);
    tplBpln->SetFriction(0.1f);
    CGOCManager::Get().SetTemplate( tplBpln );

    // CLOTH TEMPLATES
    GOCTVisualVAPlane *vaPlane = new GOCTVisualVAPlane("ClothVisualPlane");
    vaPlane->SetHalfSize( Vec2f( 0.25f, 0.5f ) );
    vaPlane->SetStacks(10);
    vaPlane->SetSlices(10);
    CGOCManager::Get().SetTemplate( vaPlane );

    GOCTBoundingDWBox *tplBbox = new GOCTBoundingDWBox("ClothBoundingDef");
    //tplBbox->SetHalfSize( Vec3f( 0.25f, 0.5f, 0.2f ) );
    tplBbox->SetElasticity(0.0f);
    tplBbox->SetFriction(1.0f);
    CGOCManager::Get().SetTemplate( tplBbox );

    GOCTPhysicsCloth *tplDfmr = new GOCTPhysicsCloth("ClothDeformable");
    tplDfmr->SetHalfSize( Vec2f( 0.25f, 0.5f ) );
    tplDfmr->SetStacks(10);
    tplDfmr->SetSlices(10);
    tplDfmr->SetMass(1.0f);
    CGOCManager::Get().SetTemplate( tplDfmr );
}

// ----------------------------------------------------------------------------
void MainApp::InitPlanes()
{
    // TODO: Read all values from config file

    const float POSITION = 0.5f;

    // Back plane
    int iP = 0;
    m_Planes[iP] = new CGameObject("MyPlane");
    m_Planes[iP]->GetTransform().GetPosition().Set( 0.0f, 0.0f, -POSITION );
    ADD_GOC( m_Planes[iP], "VisualPlane" );
    ADD_GOC( m_Planes[iP], "BoundingPlane" );

    // Left plane
    ++iP;
    m_Planes[iP] = new CGameObject("MyPlane");
    m_Planes[iP]->GetTransform().GetPosition().Set( -POSITION, 0.0f, 0.0f );
    m_Planes[iP]->GetTransform().GetOrientation().FromVector( float(M_PI_2), Vec3f( 0.0f, 1.0f, 0.0f ) );
    ADD_GOC( m_Planes[iP], "VisualPlane" );
    ADD_GOC( m_Planes[iP], "BoundingPlane" );

    // Right plane
    ++iP;
    m_Planes[iP] = new CGameObject("MyPlane");
    m_Planes[iP]->GetTransform().GetPosition().Set( POSITION, 0.0f, 0.0f );
    m_Planes[iP]->GetTransform().GetOrientation().FromVector( -float(M_PI_2), Vec3f( 0.0f, 1.0f, 0.0f ) );
    ADD_GOC( m_Planes[iP], "VisualPlane" );
    ADD_GOC( m_Planes[iP], "BoundingPlane" );

    // Top plane
    ++iP;
    m_Planes[iP] = new CGameObject("MyPlane");
    m_Planes[iP]->GetTransform().GetPosition().Set( 0.0f, POSITION, 0.0f );
    m_Planes[iP]->GetTransform().GetOrientation().FromVector( float(M_PI_2), Vec3f( 1.0f, 0.0f, 0.0f ) );
    ADD_GOC( m_Planes[iP], "VisualPlane" );
    ADD_GOC( m_Planes[iP], "BoundingPlane" );

    // Bottom plane
    ++iP;
    m_Planes[iP] = new CGameObject("MyPlane");
    m_Planes[iP]->GetTransform().GetPosition().Set( 0.0f, -POSITION, 0.0f );
    m_Planes[iP]->GetTransform().GetOrientation().FromVector( -float(M_PI_2), Vec3f( 1.0f, 0.0f, 0.0f ) );
    ADD_GOC( m_Planes[iP], "VisualPlane" );
    ADD_GOC( m_Planes[iP], "BoundingPlane" );

    // Front plane
    ++iP;
    m_Planes[iP] = new CGameObject("MyPlane");
    m_Planes[iP]->GetTransform().GetPosition().Set( 0.0f, 0.0f, POSITION );
    m_Planes[iP]->GetTransform().GetOrientation().FromVector( -float(M_PI), Vec3f( 0.0f, 1.0f, 0.0f ) );
    ADD_GOC( m_Planes[iP], "VisualPlane" );
    ADD_GOC( m_Planes[iP], "BoundingPlane" );
}

// ----------------------------------------------------------------------------
void MainApp::InitCloth()
{
    // TODO: Read all values from config file

    m_Cloth = new CGameObject("MyCloth");
    m_Cloth->GetTransform().GetPosition().Set( -0.25f, 0.0f, 0.0f );
    //m_Cloth->GetTransform().GetPosition().Set( 0.0f, 0.4f, 0.0f );
    m_Cloth->GetTransform().GetOrientation().FromVector( -float(M_PI_2), Vec3f( 1.0f, 0.0f, 0.0f ) );
    ADD_GOC( m_Cloth, "ClothVisualPlane" );
    ADD_GOC( m_Cloth, "ClothDeformable" );
    ADD_GOC( m_Cloth, "ClothBoundingDef" );
}