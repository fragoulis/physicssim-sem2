#include "MainWindow.h"
#include "MainApp.h"
#include "CSceneManager.h"
#include "CPhysicsManager.h"
#include "Time/Clock.h"
#include "Util/CRecorder.h"
#include "Util/CReplayer.h"
#include "Math/Random.h"

#include "GOCS/CGameObject.h"       // TODO: This dependecies exist because of the lack
#include "GOCS/GOCBoundingPlane.h"  //       of an inter-component event system.
#include "GOCS/GOCBoundingDWBox.h"
#include "GOCS/GOCBoundingBox.h"
#include "GOCS/GOCPhysicsCloth.h"

#include "Util/CLogger.h"

using namespace tlib;
using namespace tlib::math;
using namespace tlib::time;
using namespace tlib::gocs;

#include <iostream>
#include <fstream>

const float DEG_TO_RAD = (float)M_PI / 180.0f;
const float ROTATION_STEP = 0.025f;
const float ROTATION_SPEED = 350.0f;

//-----------------------------------------------------------------------------
MainWindow::MainWindow():
m_fCubeHorizontalAngle(0.0f),
m_fCubeVerticalAngle(0.0f),
m_fAccumX(0.0f),
m_fAccumY(0.0f),
m_iMouseX(0),
m_iMouseY(0),
m_bPause(0),
m_AppState(AS_NORMAL),
m_bWireframe(0),
m_bShowControls(0)
{
    // TODO: Read window stuff from config

    // Initialize window
    SetSize( 768, 768 );
    
    //SetFullscreen(true);
    m_fFovY      = 80.0f;
    m_fDimRatio  = float(Width()) / float(Height());
    m_fPlanes[0] = 0.1f;
    m_fPlanes[1] = 1000.0f;
}

//-----------------------------------------------------------------------------
void MainWindow::OnCreate()
{
    GLWindowEx::OnCreate();
    _CLEAR_LOG

    glClearColor( 0.15f, 0.15f, 0.15f, 1.0f );
    glEnable        (GL_CULL_FACE);
    glEnable        (GL_DEPTH_TEST);
    glShadeModel    (GL_SMOOTH);

    MGRScene::_Instance().Init();
    MGRPhysics::_Instance().Init();
    Clock::_Instance().Start( MGRTimeSrc::SRC_CLOCK );
    InputRec::_Instance().Clear(m_RecData);
    InputReplay::_Instance();

    // Initialize camera's position and direction
    m_Camera.SetPosition( Vec3f( 0.0f, 0.25f, 1.25f ) );
    m_Camera.LookAt( Vec3f( 0.0f, 0.0f, 0.0f ) );

    // Setup lighting
#define LIGHT( index, vec ) \
    m_Lights[index].SetId(index); \
    m_Lights[index].SetPosition( vec ); \
    m_Lights[index].TurnOn();

    LIGHT( 0, Vec3f( 0.0f, 0.0f, 0.4f ) );
}

//-----------------------------------------------------------------------------
void MainWindow::OnDisplay()
{
    Clock::Instance().FrameStep();

    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    SetupView();
    m_Camera.Apply();
    m_Lights[0].Apply();

    //RenderHelpGrid( 40, 0.5f );
    MGRScene::Instance().Render();
    PrintStats();

    CGameObject *cloth = MainApp::Get().GetCloth();
    GOCBoundingDWBox *bnd = GET_OBJ_GOC( cloth, GOCBoundingDWBox, "BoundingVolume" );
    assert(bnd);

    SwapBuffers();
}

//-----------------------------------------------------------------------------
void MainWindow::OnIdle()
{
    float delta = (float)Clock::Instance().GetTimeDelta();
    
    // =========================================================================
    if( m_AppState == AS_REPLAY )
    {
        HandleReplay();
    }
    // =========================================================================

    if( !m_bPause )
    {
        RotateCube( delta );
        MGRPhysics::Instance().Update( delta );
    }

    // =========================================================================
    if( m_AppState == AS_RECORD )
    {
        m_RecData.time = Clock::Instance().GetCurrentFeed();
        m_RecData.mouse = m_bIsMouseDown;
        InputRec::Instance().Record( m_RecData );
    }
    // =========================================================================

    Redraw();
}
//-----------------------------------------------------------------------------
void MainWindow::HandleReplay()
{
    if( !InputReplay::Instance().Read( m_RecData ) )
    {
        Clock::Instance().Start( MGRTimeSrc::SRC_CLOCK );
        m_AppState = AS_NORMAL;
        return;
    }

    CommonKeyboard( m_RecData.key );
    ActMouseButton( (m_RecData.mouse==1)?true:false );
    ActMouseMove( m_RecData.x, m_RecData.y );
}

//-----------------------------------------------------------------------------
void MainWindow::OnKeyboard( int key, bool down )
{
    if( down ) return;
    key = tolower(key);

    // Handle general input
    switch(key)
    {
    case 'q': Quit(); break;
    case 's': m_bShowControls = !m_bShowControls; break;
    case 'w': 
        {
            m_bWireframe = !m_bWireframe;
            if( m_bWireframe )
                glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
            else
                glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
        }
        break;

    case 'f':
        {
            if( GetFullscreen() ) SetFullscreen(false);
            else
                SetFullscreen(true);
        }
        break;
    }

    if( m_AppState == AS_NORMAL ) {
        KeyboardOnNormal( key );
    }
    else if( m_AppState == AS_RECORD ) {
        KeyboardOnRecord( key );
    }
    else {
        KeyboardOnReplay( key );
    }

    Redraw();
} // OnKeyboard()

//-----------------------------------------------------------------------------
void MainWindow::KeyboardOnNormal( int key )
{
    if( key == 'r' )
    {
        // Enable recording
        m_AppState = AS_RECORD;
        Reset();
    }
    else if( key == 'o' )
    {   
        // Enable replay if recorded data exist
        if( !InputReplay::Instance().Begin() ) return;
    
        Reset();
        m_AppState = AS_REPLAY;
        Clock::Instance().Start( MGRTimeSrc::SRC_FILE );
    }
    else
    {
        CommonKeyboard( key );
    }
}

//-----------------------------------------------------------------------------
void MainWindow::KeyboardOnRecord( int key )
{
    if( key == 'r' )
    {
        // Disable recording
        m_AppState = AS_NORMAL;
        InputRec::Instance().End();
    }
    else if( key == 'o' ) { /* Do nothing for 'o' */ }
    else
    {
        // Record currect key
        m_RecData.key = key;

        CommonKeyboard( key );
    }
}

//-----------------------------------------------------------------------------
void MainWindow::KeyboardOnReplay( int key )
{
    // Handle replay
    // Ignores all controls except the general
    if( key == 'o' )
    {
        // Disable replay
        Clock::Instance().Start( MGRTimeSrc::SRC_CLOCK );
        m_AppState = AS_NORMAL;
    }
}

//-----------------------------------------------------------------------------
void MainWindow::CommonKeyboard( int key )
{
    // Handles all controls except general and replay
    switch(key)
    {
    case '0': Reset(); break;
    case 'p': m_bPause = !m_bPause; break;
    }

    if( !m_bPause )
    {
        switch(key)
        {
        case '1': MainApp::Get().AddBigSphere(); break;
        case '2': MainApp::Get().RemoveLastSphere(); break;
        case '3': /* Toggle jelly */ break;
        case 's': /* Toggle solid/cloth */ break;
        case 'v': /* Toggle video screen */ break;
        case 'c': /* Toggle cameras */ break;
        }
    }
}

//-----------------------------------------------------------------------------
void MainWindow::OnMouseButton( MouseButton button, bool down )
{
    // Skip mouse input on replay
    if( m_AppState == AS_REPLAY ) return;
    
    if( button == MBLeft ) ActMouseButton( down );

    Redraw();
}

//-----------------------------------------------------------------------------
void MainWindow::ActMouseButton( bool down )
{
    m_bIsMouseDown = down;
}

//-----------------------------------------------------------------------------
void MainWindow::OnMouseMove( int x, int y )
{
    // Skip mouse input on replay
    if( m_AppState == AS_REPLAY ) return;
    if( m_AppState == AS_RECORD )
    {
        m_RecData.x = x;
        m_RecData.y = y;
    }

    ActMouseMove( x, y );

    Redraw();
}

//-----------------------------------------------------------------------------
void MainWindow::ActMouseMove( int x, int y )
{
    // Zero values in reality are almost non-existen, so
    // we say zero values are not acceptable, which helps
    // us with the replay
    if( !x && !y ) return;

    if( m_bIsMouseDown )
    {
        const int dX = x - m_iMouseX;
        const int dY = y - m_iMouseY;

        const float pcX = float(m_fAccumX) / Width();
        const float pcY = -float(m_fAccumY) / Height();

        m_fCubeHorizontalAngle = ROTATION_SPEED * pcX * DEG_TO_RAD;
        m_fCubeVerticalAngle   = ROTATION_SPEED * pcY * DEG_TO_RAD;

        m_fAccumX += dX;
        m_fAccumY += dY;
    }
    else {
        m_fAccumX = 0.0f;
        m_fAccumY = 0.0f;
    }

    m_iMouseX = x;
    m_iMouseY = y;
}

//-----------------------------------------------------------------------------
void MainWindow::Reset()
{
    MainApp::Get().Reset();
    RotateCube( ~m_qRotationAccum );
    m_qRotationAccum.Clear();
    
    m_bPause = false;
    m_fCubeHorizontalAngle = 0.0f;
    m_fCubeVerticalAngle = 0.0f;
}

//-----------------------------------------------------------------------------
void MainWindow::RotateCube( float delta )
{
    if( m_fCubeHorizontalAngle == 0.0f &&
        m_fCubeVerticalAngle == 0.0f ) 
        return;

    Quatf qHRot, qVRot;
    qHRot.FromVector( m_fCubeHorizontalAngle * delta, Vec3f( 0.0f, 0.0f, 1.0f ) );
    qVRot.FromVector( m_fCubeVerticalAngle * delta, Vec3f( 1.0f, 0.0f, 0.0f ) );
    Quatf qCirRot = qVRot * qHRot;
    
    RotateCube( qCirRot );

    m_qRotationAccum = qCirRot * m_qRotationAccum; // Accumulate rotation

    CheckRotationAngles();

} // RotateCube()

//-----------------------------------------------------------------------------
void MainWindow::RotateCube( const Quatf &qCirRot )
{
    for( int i=0; i<MainApp::MAX_PLANES; ++i )
    {
        // Must rotate the orientation 
        // Then we must also notify interested components about that ??? 
        // [Inter-component message systems?]
        // OnPositionChange() && OnOrientationChange() ???

        CGameObject *plane = MainApp::Get().GetPlane(i);
        
        // Rotate plane
        Vec3f &vPos = plane->GetTransform().GetPosition();
        qCirRot.Rotate( vPos );

        // Add the rotation
        Quatf &qRot = plane->GetTransform().GetOrientation();
        qRot = qCirRot * qRot;

        // Corrent normal
        GOCBoundingPlane *bnd = GET_OBJ_GOC( plane, GOCBoundingPlane, "BoundingVolume" );
        qCirRot.Rotate( bnd->GetNormal() );
        bnd->GetNormal().Normalize();
    } // for( ... )

    // Rotate cloth as well
    Vec3f &vPos = MainApp::Get().GetCloth()->GetTransform().GetPosition();
    qCirRot.Rotate( vPos );
    //Quatf &qRot = MainApp::Get().GetCloth()->GetTransform().GetOrientation();
    //qRot = qCirRot * qRot;

    CGameObject *cloth = MainApp::Get().GetCloth();
    GOCPhysicsCloth *physics = GET_OBJ_GOC( cloth, GOCPhysicsCloth, "Physics" );
    assert(physics);
    physics->Rotate(qCirRot);

    //GOCBoundingDWBox *bnd = GET_OBJ_GOC( cloth, GOCBoundingDWBox, "BoundingVolume" );
    //assert(bnd);
    //bnd->WrapObject();
}

//-----------------------------------------------------------------------------
void MainWindow::CheckRotationAngles()
{
    // Check cube rotation angles
    if( m_fCubeHorizontalAngle > ROTATION_STEP )
        m_fCubeHorizontalAngle -= ROTATION_STEP;
    else if( m_fCubeHorizontalAngle <= -ROTATION_STEP )
        m_fCubeHorizontalAngle += ROTATION_STEP;
    else
        m_fCubeHorizontalAngle = 0.0f;

    if( m_fCubeVerticalAngle > ROTATION_STEP )
        m_fCubeVerticalAngle -= ROTATION_STEP;
    else if( m_fCubeVerticalAngle <= -ROTATION_STEP )
        m_fCubeVerticalAngle += ROTATION_STEP;
    else
        m_fCubeVerticalAngle = 0.0f;
}

//-----------------------------------------------------------------------------
void MainWindow::Quit()
{
    if( m_AppState == AS_RECORD ) {
        InputRec::Instance().End(); // Stop recoding
    }

    Close();
}

//-----------------------------------------------------------------------------
void MainWindow::OnDestroy()
{
    Clock::Destroy();
    InputRec::Destroy();
    InputReplay::Destroy();
}

//-----------------------------------------------------------------------------
void MainWindow::OnResize( int w, int h )
{
    m_fDimRatio = float(w) / float(h);
    SetupView();
}

//-----------------------------------------------------------------------------
void MainWindow::PrintStats()
{
    glDisable( GL_LIGHTING );
    glColor3f( 0.0f, 1.0f, 0.0f );

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D( -1.0, 1.0, -1.0, 1.0 ); 
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // TODO: Change projection to ortho
    glRasterPos2f( -1.0f, 0.95f );
    Printf( "Number of Balls: %i", MainApp::Get().GetBallCount() );
    
    glRasterPos2f( -1.0f, 0.9f );
    string gamestate("Normal");
    switch(m_AppState)
    {
    case AS_RECORD: gamestate = "Record"; break;
    case AS_REPLAY: gamestate = "Replay"; break;
    }
    Printf( "Gamestate: %s", gamestate.c_str() );

    if( m_bShowControls )
    {
        glRasterPos2f( -1.0f, 0.85f );
        Printf( "Hold left mouse button and drag to rotate the cube" );
        glRasterPos2f( -1.0f, 0.80f );
        Printf( "'1' to add sphere" );
        glRasterPos2f( -1.0f, 0.75f );
        Printf( "'2' to remove the last ball added" );
        glRasterPos2f( -1.0f, 0.70f );
        Printf( "'P' to pause" );
        glRasterPos2f( -1.0f, 0.65f );
        Printf( "'R' to toggle record mode" );
        glRasterPos2f( -1.0f, 0.60f );
        Printf( "'O' to replay the last recorded try" );
        glRasterPos2f( -1.0f, 0.55f );
        Printf( "'W' toggle wireframe" );
        glRasterPos2f( -1.0f, 0.50f );
        Printf( "'F' toggle fullscreen" );
        glRasterPos2f( -1.0f, 0.45f );
        Printf( "'Q' to quit" );
        glRasterPos2f( -1.0f, 0.40f );
        Printf( "'S' to close help" );
    }
    else
    {
        glRasterPos2f( -1.0f, 0.85f );
        Printf( "'S' to show help" );
    }

    glEnable( GL_LIGHTING );
}

//-----------------------------------------------------------------------------
void MainWindow::SetupView() const
{
    // Setup projection matrix
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective( m_fFovY, m_fDimRatio, m_fPlanes[0], m_fPlanes[1] );

    // Setup modelview matrix
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glViewport( 0, 0, Width(), Height() );
}

//-----------------------------------------------------------------------------
void MainWindow::RenderHelpGrid( int lines, float size ) const
{
    // Render a helper grid
    glBegin(GL_LINES);
    {
        glColor3f( 1.0f, 1.0f, 0.0f );
        const float init_pos = -size*lines*0.5f;
        const float y = -0.6f;
        for( int kk=0; kk<lines; ++kk )
        {
            const float temp = init_pos + kk * size;

            glVertex3f( temp, y, init_pos );
            glVertex3f( temp, y, init_pos + size*lines );

            glVertex3f( init_pos, y, temp );
            glVertex3f( init_pos + size*lines, y, temp );
        }
    }
    glEnd();
}