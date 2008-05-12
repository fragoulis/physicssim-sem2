#include "Network/CPacket.h"
#include "MainApp.h"
#include "ObjectMutex.h"
#include "goc_includes.h"
#include "Util/Config.h"
#include "CSceneManager.h"
#include "GOCS/CGOCManager.h"    // The component manager
#include "Util/CLogger.h"

static MainApp g_MainApp;

CBitmapThread MainApp::m_tBitmap;
CSendThread MainApp::m_tClientSend;
CRecvThread MainApp::m_tClientRecv;

#include "GOCS/Interfaces/IGOCVisualVertexArray.h"
using tlib::gocs::IGOCVisualVertexArray;

// ----------------------------------------------------------------------------
MainApp& MainApp::Get() { return g_MainApp; }

// ----------------------------------------------------------------------------
void MainApp::ReadPacket( CPacket &packet )
{
    // camera status
    int state;
    packet.pull<int>(state);
    m_tMain.SetState(state);

    // box data
    transform_t tr;
    for( int i=0; i<MAX_PLANES; i++ ) {
        packet.pull<transform_t>(tr);
        m_Planes[i]->SetTransform(tr);
    }

    // cloth / shelf data
    bool cloth;
    packet.pull<bool>(cloth);
    
    if(cloth)
    {
        m_Cloth->Activate();
        m_Shelf->Deactivate();

        IGOCVisualVertexArray *visualData = (IGOCVisualVertexArray*)m_Cloth->GetGOC("Visual");
        const int bytes = visualData->GetNumOfVertices()*3*sizeof(float);
        packet.pull<float>( &visualData->GetVertex(0)[0], bytes );

        visualData->UpdateNormals();
    }
    else 
    {
        m_Cloth->Deactivate();
        m_Shelf->Activate();

        packet.pull<transform_t>(tr);
        m_Shelf->SetTransform(tr);
    }

    // sphere data    
    packet.pull<int>(m_iNumOfBigSpheres);
    packet.pull<int>(m_iNumOfSmallSpheres);
    int numOfSpheres = m_iNumOfBigSpheres + m_iNumOfSmallSpheres;

    if( 0 == numOfSpheres && m_Spheres.empty() )
    { /* do nothing */ }
    else if( 0 == numOfSpheres && !m_Spheres.empty() )
    {
        ClearSpheres();
    }
    else
    {
        position_t sphere_pos;
        bool sphere_isSmall;

        int currentSize = (int)m_Spheres.size();
        for( int i=0; i<numOfSpheres && i<currentSize; i++ )
        {
            // pull out a sphere
            packet.pull<position_t>(sphere_pos);
            packet.pull<bool>(sphere_isSmall);

            m_Spheres[i]->SetPosition(sphere_pos);
        }

        if( numOfSpheres < currentSize )
        {
            // remove the remaining spheres
            if( ObjectMutex::IsWritable() )
            {
                __TRY 
                {
                    // save last sphere
                    m_LastSphere = m_Spheres[numOfSpheres-1];

                    for( int i=currentSize; i>numOfSpheres; i-- )
                    {
                        CGameObject *obj = m_Spheres[i-1];
                        delete obj; obj = 0;
                        m_Spheres.pop_back();
                    }
                }
                __FINALLY { ObjectMutex::ReleaseAll(); }
            }
        }
        else if( numOfSpheres > currentSize )
        {
            // add the new spheres
            for( int i=currentSize; i<numOfSpheres; i++ )
            {
                packet.pull<position_t>(sphere_pos);
                packet.pull<bool>(sphere_isSmall);
                
                if(sphere_isSmall) AddSmallSphere();
                else
                    AddBigSphere();

                m_LastSphere->SetPosition(sphere_pos);
            }
        }
    }

} // ReadPacket()

// ----------------------------------------------------------------------------
void MainApp::WritePacket( CPacket &packet )
{
    input_t myinput;
    if( !m_input.Get(myinput) )
		_LOG("Failed to read input in order to write the packet!!!");

    // push general keyboard input
    packet.push<bool>(myinput.keys, sizeof(myinput.keys));

    // push cume accumulated angles
    packet.push<float>(m_tMain.GetAngleX());
    packet.push<float>(m_tMain.GetAngleY());

    if( !m_input.Clear() )
		_LOG("Failed to clear input after sending");
} 

// ----------------------------------------------------------------------------
void MainApp::OnCreate()
{
    //MGRScene::_Get().Init();
    CGOCManager::_Get();

    m_iNumOfSmallSpheres = 0;
    m_iNumOfBigSpheres = 0;

    InitTemplates();
    InitPlanes();
    InitCloth();
    InitShelf();
	InitFloor();
}

// ----------------------------------------------------------------------------
void MainApp::OnDestroy()
{
    // Delete spheres
    ClearSpheres();

    // Delete planes
    for( int i=0; i<MAX_PLANES; ++i )
        delete m_Planes[i];

    // Delete other objects
    delete m_Cloth;
    delete m_Shelf;
	delete m_Floor;

    CGOCManager::Destroy();
    MGRScene::Destroy();
}

// ----------------------------------------------------------------------------
void MainApp::ClearSpheres()
{
    // Delete spheres
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
        m_LastSphere = 0;

        __FINALLY { ObjectMutex::ReleaseAll(); }
    }
}

// ----------------------------------------------------------------------------
void MainApp::AddBigSphere()
{
    CGameObject *pSphere = new CGameObject("BigSphere");
    if( ObjectMutex::IsWritable() )
    {
        __TRY { ADD_GOC( pSphere, "VisualBigSphere" ); }
        __FINALLY { ObjectMutex::ReleaseAll(); }
    }
	else 
		cout << "Big Sphere timeout!!!" << endl;

    m_Spheres.push_back(pSphere);
    m_LastSphere = pSphere;
}

// ----------------------------------------------------------------------------
void MainApp::AddSmallSphere()
{
    CGameObject *pSphere = new CGameObject("SmallSphere");
    if( ObjectMutex::IsWritable() )
    {
        __TRY { ADD_GOC( pSphere, "VisualSmallSphere" ); }
        __FINALLY { ObjectMutex::ReleaseAll(); }
    }else 
		cout << "Small Sphere timeout!!!" << endl;

    m_Spheres.push_back(pSphere);
    m_LastSphere = pSphere;
}

// ----------------------------------------------------------------------------
void MainApp::InitTemplates()
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
    // PLANE TEMPLATES
    CFG_LOAD("Plane");

    CFG_2fv("Halfsize", size);
    CFG_3fv("Normal", normal);
    CFG_1f("Elasticity", elasticity);
    CFG_1f("Friction", friction);

    GOCTVisualIMQuad *tplPlane = new GOCTVisualIMQuad("VisualPlane");
    tplPlane->SetHalfSize( Vec2f( size ) );
    CGOCManager::Get().SetTemplate( tplPlane );

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

} // end InitTemplates()

// ----------------------------------------------------------------------------
void MainApp::InitPlanes()
{
    // TODO: Read all values from config file

    const float POSITION = 0.5f;

    // Back plane
    int iP = 0;
    m_Planes[iP] = new CGameObject("Backplane");
    m_Planes[iP]->GetTransform().GetPosition().Set( 0.0f, 0.0f, -POSITION );
    ADD_GOC( m_Planes[iP], "VisualPlane" );

    // Left plane
    ++iP;
    m_Planes[iP] = new CGameObject("Plane");
    m_Planes[iP]->GetTransform().GetPosition().Set( -POSITION, 0.0f, 0.0f );
    m_Planes[iP]->GetTransform().GetOrientation().FromVector( float(M_PI_2), Vec3f( 0.0f, 1.0f, 0.0f ) );
    ADD_GOC( m_Planes[iP], "VisualPlane" );

    // Right plane
    ++iP;
    m_Planes[iP] = new CGameObject("Plane");
    m_Planes[iP]->GetTransform().GetPosition().Set( POSITION, 0.0f, 0.0f );
    m_Planes[iP]->GetTransform().GetOrientation().FromVector( -float(M_PI_2), Vec3f( 0.0f, 1.0f, 0.0f ) );
    ADD_GOC( m_Planes[iP], "VisualPlane" );

    // Top plane
    ++iP;
    m_Planes[iP] = new CGameObject("Plane");
    m_Planes[iP]->GetTransform().GetPosition().Set( 0.0f, POSITION, 0.0f );
    m_Planes[iP]->GetTransform().GetOrientation().FromVector( float(M_PI_2), Vec3f( 1.0f, 0.0f, 0.0f ) );
    ADD_GOC( m_Planes[iP], "VisualPlane" );

    // Bottom plane
    ++iP;
    m_Planes[iP] = new CGameObject("Plane");
    m_Planes[iP]->GetTransform().GetPosition().Set( 0.0f, -POSITION, 0.0f );
    m_Planes[iP]->GetTransform().GetOrientation().FromVector( -float(M_PI_2), Vec3f( 1.0f, 0.0f, 0.0f ) );
    ADD_GOC( m_Planes[iP], "VisualPlane" );

    // Front plane
    ++iP;
    m_Planes[iP] = new CGameObject("Plane");
    m_Planes[iP]->GetTransform().GetPosition().Set( 0.0f, 0.0f, POSITION );
    m_Planes[iP]->GetTransform().GetOrientation().FromVector( -float(M_PI), Vec3f( 0.0f, 1.0f, 0.0f ) );
    ADD_GOC( m_Planes[iP], "VisualPlane" );
}

// ----------------------------------------------------------------------------
void MainApp::InitFloor()
{
	Quatf qOri( -float(M_PI_2), Vec3f( 1.0f, 0.0f, 0.0f ) );
	qOri.FromSelf();

	m_Floor = new CGameObject("Floor");
	m_Floor->SetPosition( Vec3f( 0.0f, -1.0f, -1.0f ) );
    m_Floor->SetOrientation( qOri );
    ADD_GOC( m_Floor, "VisualPlane" );
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
}

// ----------------------------------------------------------------------------
void MainApp::InitShelf()
{
    // TODO: Read all values from config file

    m_Shelf = new CGameObject("Shelf");
    m_Shelf->Deactivate();
    m_Shelf->GetTransform().GetPosition().Set( -0.25f, 0.0f, 0.0f );
    m_Shelf->GetTransform().GetOrientation().FromVector( -float(M_PI_2), Vec3f( 1.0f, 0.0f, 0.0f ) );
    ADD_GOC( m_Shelf, "VisualShelf" );
}