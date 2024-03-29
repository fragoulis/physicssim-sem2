#include "../MainApp.h"
#include "CPhysicsThread.h"
#include "../CPhysicsManager.h"
#include "../Time/Clock.h"
#include "../Util/CLogger.h"
#include "../Util/Config.h"
#include "../Math/Random.h"
#include "../ObjectMutex.h"
#include "../goc_includes.h"
#include "../Network/CPacket.h"
#include "../CSceneManager.h"

using namespace tlib::math;
using namespace tlib::time;

#include "../GOCS/Interfaces/IGOCVisualVertexArray.h"
using tlib::gocs::IGOCVisualVertexArray;

CPhysicsThread::CPhysicsThread():
m_bPause(false),
m_bReset(false),
m_bRestartClockFromFile(false),
m_bRestartClock(false),
m_LastSphere(0),
m_bIsReady(false)
{}

//-----------------------------------------------------------------------------
void CPhysicsThread::Run( void *lpArgs )
{
    Clock::_Get().Start( MGRTimeSrc::SRC_CLOCK );

    while(IsRunning())
    {
        if( m_bRestartClockFromFile ) 
        {
            Clock::Get().Start( MGRTimeSrc::SRC_FILE );
            m_bRestartClockFromFile = false;
        }
        else if( m_bRestartClock )
        {
            Clock::Get().Start( MGRTimeSrc::SRC_CLOCK );
            m_bRestartClock = false;
        }

        Clock::Get().FrameStep();
        float delta = (float)Clock::Get().GetTimeDelta();

        HandleInput();

        if( m_bReset ) 
        {
            Reset();
            m_bReset = false;
        }

        if( !m_bPause )
        {
            RotateCube( delta );
            MGRPhysics::Get().Update( delta );

			GOCBoundingDWBox *bnd = GET_OBJ_GOC( m_Cloth, GOCBoundingDWBox, "BoundingVolume" );
			assert(bnd);
			bnd->WrapObject();
        }

        Sleep(1);
    }
} // Run()

#define DOWN( index ) myinput.keys[index]
// ----------------------------------------------------------------------------
void CPhysicsThread::HandleInput()
{
    input_t myinput;
    MainApp::Get().GetInput( myinput );

    // Handles all controls except general and replay
	if( DOWN('0') ) m_bReset = true;
    if( DOWN('p') ) m_bPause = !m_bPause;
#ifdef _DEBUG
    if( DOWN(187) ) MGRPhysics::Get().MultTimeStep(2.0f); // '='
    else if( DOWN(189) ) MGRPhysics::Get().MultTimeStep(0.5f); // '-'
#endif
    if( !m_bPause )
    {
        if( DOWN('1') )
        {
            if( RandBoolean() ) AddBigSphere(); 
            else
                AddSmallSphere(); 
        }
        else if( DOWN('2') ) RemoveLastSphere();
        if( DOWN('3') ) { /* Toggle jelly */ }
        if( DOWN('s') ) ToggleClothShelf();
    }

    MainApp::Get().ClearInput();
}

// ----------------------------------------------------------------------------
void CPhysicsThread::OnStart()
{
    m_iNumOfSmallSpheres = 0;
    m_iNumOfBigSpheres = 0;

    InitTemplates();
    InitPlanes();
    InitCloth();
    InitShelf();
	InitFloor();

	m_bIsReady = true;
}

// ----------------------------------------------------------------------------
void CPhysicsThread::OnEnd()
{
    // Delete spheres
    ObjectList::iterator iter = m_Spheres.begin();
    for(; iter != m_Spheres.end(); ++iter )
        delete (*iter);

    // Delete planes
    for( int i=0; i<MAX_PLANES; ++i )
        delete m_Planes[i];

    // Delete other objects
    delete m_Cloth;
    delete m_Shelf;
	delete m_Floor;
}

// ----------------------------------------------------------------------------
void CPhysicsThread::ToggleClothShelf()
{
    m_Cloth->Toggle();
    m_Shelf->Toggle();
}

// ----------------------------------------------------------------------------
void CPhysicsThread::WritePacket( CPacket &packet )
{
    // push planes
    for( int i=0; i<MAX_PLANES; i++ ) {
        packet.push<transform_t>( m_Planes[i]->GetTransform() );
    }

    // push cloth/shelf status
    packet.push<bool>( m_Cloth->IsActive() );

    if( m_Cloth->IsActive() ) {
        // push the cloth
        IGOCVisualVertexArray *visualData = (IGOCVisualVertexArray*)m_Cloth->GetGOC("Visual");
        
        const int bytes = visualData->GetNumOfVertices()*3*sizeof(float);
        packet.push<float>( &visualData->GetVertex(0)[0], bytes );
    }
    else {
        // push the shelf
        packet.push<transform_t>( m_Shelf->GetTransform() );
    }

    // push the balls
    packet.push<int>(m_iNumOfBigSpheres);
    packet.push<int>(m_iNumOfSmallSpheres);
    ObjectList::iterator i = m_Spheres.begin();
    for(; i != m_Spheres.end(); ++i )
    {
        CGameObject *sphere = *i;
        packet.push<position_t>(sphere->GetPosition());
        bool isSmall = true;
        if( sphere->Is("BigSphere") ) isSmall = false;
        packet.push<bool>(isSmall);
    }

}

// ----------------------------------------------------------------------------
void CPhysicsThread::ReadPacket( CPacket &packet )
{
    input_t myinput;
    packet.pull<bool>(myinput.keys, sizeof(myinput.keys));
	
	if( !MainApp::Get().AccumInput(myinput) ) {
		_LOG("Failed to accumulate input from client!!!")
	}

    float angleX, angleY;
    packet.pull<float>(angleX);
    packet.pull<float>(angleY);

	const float rotCeil = 0.4f;
	if(angleX > rotCeil) angleX = rotCeil;
	else if(angleX < -rotCeil) angleX = -rotCeil;
	if(angleY > rotCeil) angleY = rotCeil;
	else if(angleY < -rotCeil) angleY = -rotCeil;

	if( !m_cube.AddAngles(angleX,angleY) ) {
		_LOG("Failed to accumulate cube angles!!!")
	}
}

// ----------------------------------------------------------------------------
void CPhysicsThread::RemoveLastSphere()
{
    if( !m_LastSphere ) return;
    
    if( m_LastSphere->Is("BigSphere") ) {
        m_iNumOfBigSpheres--;
    } else {
        m_iNumOfSmallSpheres--;
    }

    if( ObjectMutex::IsWritable() )
    {
        __TRY { delete m_LastSphere; m_LastSphere = 0; }
        __FINALLY { ObjectMutex::ReleaseAll(); }
    }

    m_Spheres.pop_back();

    // Get last element
    if( !m_Spheres.size() ) return;

    m_LastSphere = m_Spheres[ m_Spheres.size() - 1 ];
}

// ----------------------------------------------------------------------------
void CPhysicsThread::AddBigSphere()
{
    CGameObject *pSphere = new CGameObject("BigSphere");
    if( ObjectMutex::IsWritable() )
    {
        __TRY { ADD_GOC( pSphere, "VisualBigSphere" ); }
        __FINALLY { ObjectMutex::ReleaseAll(); }
    }
    
    ADD_GOC( pSphere, "PhysicsBigPoint"   );
    ADD_GOC( pSphere, "BoundingBigSphere" );

    m_Spheres.push_back(pSphere);
    m_iNumOfBigSpheres++;

    m_LastSphere = pSphere;
}

// ----------------------------------------------------------------------------
void CPhysicsThread::AddSmallSphere()
{
    CGameObject *pSphere = new CGameObject("SmallSphere");
    if( ObjectMutex::IsWritable() )
    {
        __TRY { ADD_GOC( pSphere, "VisualSmallSphere" ); }
        __FINALLY { ObjectMutex::ReleaseAll(); }
    }
    ADD_GOC( pSphere, "PhysicsSmallPoint"   );
    ADD_GOC( pSphere, "BoundingSmallSphere" );

    m_Spheres.push_back(pSphere);
    m_iNumOfSmallSpheres++;

    m_LastSphere = pSphere;
}

//-----------------------------------------------------------------------------
void CPhysicsThread::RotateCube( float delta )
{
    m_cube.ReadAngles();

    if( m_cube.horizontalAngle == 0.0f &&
        m_cube.verticalAngle == 0.0f ) 
        return;

    Quatf qHRot, qVRot;
    qHRot.FromVector( m_cube.horizontalAngle * delta, Vec3f( 0.0f, 0.0f, 1.0f ) );
    qVRot.FromVector( m_cube.verticalAngle * delta, Vec3f( 1.0f, 0.0f, 0.0f ) );
    Quatf qCirRot = qVRot * qHRot;
    
    RotateCube( qCirRot );

    m_cube.rotationAccum = qCirRot * m_cube.rotationAccum; // Accumulate rotation
    m_cube.CheckAngles();

} // RotateCube()

//-----------------------------------------------------------------------------
void CPhysicsThread::RotateCube( const Quatf &qCirRot )
{
    RotatePlanes( qCirRot );
    RotateCloth( qCirRot );
    RotateShelf( qCirRot );
}

// ----------------------------------------------------------------------------
void CPhysicsThread::RotateCloth( const Quatf& qRot )
{
    Vec3f vPos = m_Cloth->GetPosition();
    qRot.Rotate( vPos );
    m_Cloth->SetPosition( vPos );

    GOCPhysicsCloth *physics = GET_OBJ_GOC( m_Cloth, GOCPhysicsCloth, "Physics" );
    assert(physics);
    physics->Rotate(qRot);
}

// ----------------------------------------------------------------------------
void CPhysicsThread::RotateShelf( const Quatf& qRot )
{
    // Rotate plane
    Vec3f vPos = m_Shelf->GetPosition();
    qRot.Rotate( vPos );
    m_Shelf->SetPosition( vPos );

    // Add the rotation
    Quatf qOri = m_Shelf->GetOrientation();
    qOri = qRot * qOri;
    m_Shelf->SetOrientation( qOri );

    // Corrent normal
    GOCBoundingFinitePlane *bnd = GET_OBJ_GOC( m_Shelf, GOCBoundingFinitePlane, "BoundingVolume" );
    Vec3f normal = bnd->GetNormal();
    qRot.Rotate( normal );
    normal.Normalize();
    bnd->SetNormal( normal );

    normal = bnd->GetBinormal();
    qRot.Rotate( normal );
    normal.Normalize();
    bnd->SetBinormal( normal );
}

// ----------------------------------------------------------------------------
void CPhysicsThread::RotatePlanes( const Quatf& qRot )
{
    for( int i=0; i<MAX_PLANES; ++i )
    {
        // Must rotate the orientation 
        // Then we must also notify interested components about that ??? 
        // [Inter-component message systems?]
        // OnPositionChange() && OnOrientationChange() ???

        CGameObject *plane = m_Planes[i];
        
        // Rotate plane
        Vec3f vPos = plane->GetPosition();
        qRot.Rotate( vPos );
        plane->SetPosition( vPos );

        // Add the rotation
        Quatf qOri = plane->GetOrientation();
        qOri = qRot * qOri;
        plane->SetOrientation( qOri );

        // Corrent normal
        GOCBoundingPlane *bnd = GET_OBJ_GOC( plane, GOCBoundingPlane, "BoundingVolume" );
        Vec3f normal = bnd->GetNormal();
        qRot.Rotate( normal );
        normal.Normalize();
        bnd->SetNormal( normal );
    } // for( ... )
}

//-----------------------------------------------------------------------------
void CPhysicsThread::ResetCube()
{
    RotateCube( ~m_cube.rotationAccum );
    m_cube.rotationAccum.Clear();
    
    m_bPause = false;
    m_cube.angles.Clear();
}

// ----------------------------------------------------------------------------
void CPhysicsThread::Reset()
{
    if( ObjectMutex::IsWritable() )
    {
        __TRY
        {
            // Remove all spheres
            ObjectList::iterator i = m_Spheres.begin();
            for(; i != m_Spheres.end(); ++i )
            {
                CGameObject *obj = *i;
                delete obj;
                obj = 0;
            }
            m_Spheres.clear();
        }
        __FINALLY { ObjectMutex::ReleaseAll(); }
    }
    else
        return;

    m_iNumOfBigSpheres = 0;
    m_iNumOfSmallSpheres = 0;

    m_Cloth->Activate();
    m_Shelf->Deactivate();
    m_LastSphere = 0;

    // Hide deformable !

    ResetCube();
}

// ----------------------------------------------------------------------------
void CPhysicsThread::InitTemplates()
{
    CFG_CLIENT_OPEN;

    float radius, mass, elasticity, friction, size[2], bbox[3], normal[3], binormal[3];
    int stacks, slices;
    bool doublesided;

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
    tplBpln->SetNormal( Vec3f( normal ) );
    tplBpln->SetElasticity(elasticity);
    tplBpln->SetFriction(friction);
    CGOCManager::Get().SetTemplate( tplBpln );


    // ------------------------------------------------------------------------
    // CLOTH TEMPLATES
    CFG_LOAD("Cloth");

    CFG_2fv("Halfsize", size);
    CFG_3fv("BoundingBox", bbox);
    CFG_1f("SmallMass", mass);
    CFG_1i("Stacks", stacks);
    CFG_1i("Slices", slices);
    CFG_1f("Elasticity", elasticity);
    CFG_1f("Friction", friction);

#ifdef _DEBUG
    stacks = slices = 10;
#endif

    GOCTVisualVAPlane *vaPlane = new GOCTVisualVAPlane("ClothVisualPlane");
    vaPlane->SetHalfSize( Vec2f( size ) );
    vaPlane->SetStacks(stacks);
    vaPlane->SetSlices(slices);
    CGOCManager::Get().SetTemplate( vaPlane );

    GOCTBoundingDWBox *tplBbox = new GOCTBoundingDWBox("ClothBoundingDef");
    tplBbox->SetHalfSize( Vec3f( bbox ) );
    tplBbox->SetElasticity(elasticity);
    tplBbox->SetFriction(friction);
    CGOCManager::Get().SetTemplate( tplBbox );

    GOCTPhysicsCloth *tplDfmr = new GOCTPhysicsCloth("ClothDeformable");
    tplDfmr->SetHalfSize( Vec2f( size ) );
    tplDfmr->SetStacks(stacks);
    tplDfmr->SetSlices(slices);
    tplDfmr->SetMass(mass);
    CGOCManager::Get().SetTemplate( tplDfmr );

    // ------------------------------------------------------------------------
    // SHELF TEMPLATES
    CFG_LOAD("Shelf");

    CFG_2fv("Halfsize", size);
    CFG_3fv("Normal", normal);
    CFG_3fv("Binormal", binormal);
    CFG_1f("Elasticity", elasticity);
    CFG_1f("Friction", friction);
    CFG_1b("Doublesided", doublesided);

    tplPlane = new GOCTVisualIMQuad("VisualShelf");
    tplPlane->SetHalfSize( Vec2f( size ) );
    tplPlane->setDoublesided(doublesided);
    CGOCManager::Get().SetTemplate( tplPlane );

    GOCTBoundingFinitePlane *tplBfpln = new GOCTBoundingFinitePlane("DoubleSidePlane");
    tplBfpln->setDoublesided(doublesided);
    tplBfpln->SetHalfSize( Vec2f( size ) );
    tplBfpln->SetNormal( Vec3f( normal ) );
    tplBfpln->SetBinormal( Vec3f( binormal ) );
    tplBfpln->SetElasticity(elasticity);
    tplBfpln->SetFriction(friction);
    CGOCManager::Get().SetTemplate( tplBfpln );

} // end InitTemplates()

// ----------------------------------------------------------------------------
void CPhysicsThread::InitPlanes()
{
    // TODO: Read all values from config file

    const float POSITION = 0.5f;
	Quatf qOri;

    // Back plane
    int iP = 0;
    m_Planes[iP] = new CGameObject("Backplane");
    m_Planes[iP]->SetPosition( Vec3f( 0.0f, 0.0f, -POSITION ) );
    ADD_GOC( m_Planes[iP], "VisualPlane" );
    ADD_GOC( m_Planes[iP], "BoundingPlane" );

    // Left plane
    ++iP;
    m_Planes[iP] = new CGameObject("Plane");
    m_Planes[iP]->SetPosition( Vec3f( -POSITION, 0.0f, 0.0f ) );
    qOri.FromVector( float(M_PI_2), Vec3f( 0.0f, 1.0f, 0.0f ) );
	m_Planes[iP]->SetOrientation(qOri);
    ADD_GOC( m_Planes[iP], "VisualPlane" );
    ADD_GOC( m_Planes[iP], "BoundingPlane" );

    // Right plane
    ++iP;
    m_Planes[iP] = new CGameObject("Plane");
    m_Planes[iP]->SetPosition( Vec3f( POSITION, 0.0f, 0.0f ) );
    qOri.FromVector( -float(M_PI_2), Vec3f( 0.0f, 1.0f, 0.0f ) );
	m_Planes[iP]->SetOrientation(qOri);
    ADD_GOC( m_Planes[iP], "VisualPlane" );
    ADD_GOC( m_Planes[iP], "BoundingPlane" );

    // Top plane
    ++iP;
    m_Planes[iP] = new CGameObject("Plane");
    m_Planes[iP]->SetPosition( Vec3f( 0.0f, POSITION, 0.0f ) );
    qOri.FromVector( float(M_PI_2), Vec3f( 1.0f, 0.0f, 0.0f ) );
	m_Planes[iP]->SetOrientation(qOri);
    ADD_GOC( m_Planes[iP], "VisualPlane" );
    ADD_GOC( m_Planes[iP], "BoundingPlane" );

    // Bottom plane
    ++iP;
    m_Planes[iP] = new CGameObject("Plane");
    m_Planes[iP]->SetPosition( Vec3f( 0.0f, -POSITION, 0.0f ) );
    qOri.FromVector( -float(M_PI_2), Vec3f( 1.0f, 0.0f, 0.0f ) );
	m_Planes[iP]->SetOrientation(qOri);
    ADD_GOC( m_Planes[iP], "VisualPlane" );
    ADD_GOC( m_Planes[iP], "BoundingPlane" );

    // Front plane
    ++iP;
    m_Planes[iP] = new CGameObject("Plane");
    m_Planes[iP]->SetPosition( Vec3f( 0.0f, 0.0f, POSITION ) );
    qOri.FromVector( -float(M_PI), Vec3f( 0.0f, 1.0f, 0.0f ) );
	m_Planes[iP]->SetOrientation(qOri);
    ADD_GOC( m_Planes[iP], "VisualPlane" );
    ADD_GOC( m_Planes[iP], "BoundingPlane" );
}

// ----------------------------------------------------------------------------
void CPhysicsThread::InitFloor()
{
	Quatf qOri( -float(M_PI_2), Vec3f( 1.0f, 0.0f, 0.0f ) );
	qOri.FromSelf();

	m_Floor = new CGameObject("Floor");
	m_Floor->SetPosition( Vec3f( 0.0f, -1.0f, -1.0f ) );
    m_Floor->SetOrientation( qOri );
    ADD_GOC( m_Floor, "VisualPlane" );
}

// ----------------------------------------------------------------------------
void CPhysicsThread::InitCloth()
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

// ----------------------------------------------------------------------------
void CPhysicsThread::InitShelf()
{
    // TODO: Read all values from config file
    m_Shelf = new CGameObject("Shelf");
    m_Shelf->Deactivate();
    m_Shelf->GetTransform().GetPosition().Set( -0.25f, 0.0f, 0.0f );
    m_Shelf->GetTransform().GetOrientation().FromVector( -float(M_PI_2), Vec3f( 1.0f, 0.0f, 0.0f ) );
    ADD_GOC( m_Shelf, "VisualShelf" );
    ADD_GOC( m_Shelf, "DoubleSidePlane" );
}