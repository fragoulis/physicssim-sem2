#include "MainWindow.h"
#include "MainApp.h"
#include "CSceneManager.h"
#include "CPhysicsManager.h"
#include "Time/Clock.h"
#include "Util/CRecorder.h"
#include "Util/CReplayer.h"
#include "Math/Random.h"

#include "Util/CLogger.h"
#include "Util/Config.h"

using namespace tlib;
using namespace tlib::math;
using namespace tlib::time;
using namespace tlib::gocs;

#include <iostream>
#include <fstream>

const float DEG_TO_RAD = (float)M_PI / 180.0f;
const float ROTATION_SPEED = 350.0f;

//-----------------------------------------------------------------------------
MainWindow::MainWindow():
m_fAccumX(0.0f),
m_fAccumY(0.0f),
m_iMouseX(0),
m_iMouseY(0),
m_AppState(AS_NORMAL),
m_bWireframe(0),
m_bShowControls(0),
m_bTextured(1)
{
    // TODO: Read window stuff from config
    
    CFG_CLIENT_OPEN;
    CFG_LOAD("Window_Attributes");
    CFG_1f("fov", m_fFovY);
    CFG_2fv("planes", m_fPlanes);

    int w, h;
    CFG_2i("size", w, h);
    SetSize( w, h );
    
    bool fullscreen;
    CFG_1b("fullscreen", fullscreen);
    SetFullscreen(fullscreen);

    m_fDimRatio  = float(Width()) / float(Height());
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

    Clock::_Get().Start( MGRTimeSrc::SRC_CLOCK );
    InputRec::_Get().Clear(m_RecData);
    InputReplay::_Get();

    // Initialize camera's position and direction
    m_Camera.SetPosition( Vec3f( 0.0f, 0.25f, 1.25f ) );
    m_Camera.LookAt( Vec3f( 0.0f, 0.0f, 0.0f ) );

    // Setup lighting
#define LIGHT( index, vec ) \
    m_Lights[index].SetId(index); \
    m_Lights[index].SetPosition( vec ); \
    m_Lights[index].TurnOn();

    LIGHT( 0, Vec3f( 0.0f, 0.0f, 0.4f ) );

    MainApp::GetPhysics().Start(); // Safely start the physics thread
}

//-----------------------------------------------------------------------------
void MainWindow::OnDisplay()
{
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    SetupView();
    m_Camera.Apply();
    m_Lights[0].Apply();

    //RenderHelpGrid( 40, 0.5f );
    MGRScene::Get().Render();
    PrintStats();

    SwapBuffers();
}

//-----------------------------------------------------------------------------
void MainWindow::OnIdle()
{
    //float delta = (float)Clock::Get().GetTimeDelta();

    // =========================================================================
    //if( m_AppState == AS_REPLAY )
    //{
    //    HandleReplay();
    //}
    // =========================================================================

    //if( !MainApp::Get().IsPaused() )
    //{
    //    RotateCube( delta );
    //    MGRPhysics::Get().Update( delta );
    //}

    // =========================================================================
    //if( m_AppState == AS_RECORD )
    //{
    //    m_RecData.time = Clock::Get().GetCurrentFeed();
    //    m_RecData.mouse = m_bIsMouseDown;
    //    InputRec::Get().Record( m_RecData );
    //}
    // =========================================================================

    Redraw();
}
//-----------------------------------------------------------------------------
void MainWindow::HandleReplay()
{
    if( !InputReplay::Get().Read( m_RecData ) )
    {
        Clock::Get().Start( MGRTimeSrc::SRC_CLOCK );
        m_AppState = AS_NORMAL;
        return;
    }

    CommonKeyboard( m_RecData.key );
    ActMouseButton( (m_RecData.mouse==1)?true:false ); // used macro to get rid of the warning
    ActMouseMove( m_RecData.x, m_RecData.y );
}

//-----------------------------------------------------------------------------
void MainWindow::OnKeyboard( int key, bool down )
{
    key = tolower(key);
    MainApp::Get().SetKey( key, down );

    if( down ) return;

    // Handle general input
    switch(key)
    {
    case 'q': Quit(); break;
    case 'z': m_bShowControls = !m_bShowControls; break;
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

    //case 187: MGRPhysics::Get().MultTimeStep(2.0f); break; // '='
    //case 189: MGRPhysics::Get().MultTimeStep(0.5f); break; // '-'
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
        //Reset();
    }
    else if( key == 'o' )
    {   
        // Enable replay if recorded data exist
        if( !InputReplay::Get().Begin() ) return;
    
        //Reset();
        m_AppState = AS_REPLAY;
        Clock::Get().Start( MGRTimeSrc::SRC_FILE );
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
        InputRec::Get().End();
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
        Clock::Get().Start( MGRTimeSrc::SRC_CLOCK );
        m_AppState = AS_NORMAL;
    }
}

//-----------------------------------------------------------------------------
void MainWindow::CommonKeyboard( int key )
{
    // Handles all controls except general and replay
    //switch(key)
    //{
    //case '0': Reset(); break;
    //case 'p': MainApp::GetPhysics().TogglePause(); break;
    //}

    //if( !MainApp::GetPhysics().IsPaused() )
    //{
    //    switch(key)
    //    {
    //    case '1': 
    //        {
    //            if( RandBoolean() )
    //                MainApp::Get().AddBigSphere(); 
    //            else
    //                MainApp::Get().AddSmallSphere(); 
    //        }
    //        break;
    //    case '2': MainApp::Get().RemoveLastSphere(); break;
    //    case '3': /* Toggle jelly */ break;
    //    case 's': MainApp::Get().ToggleClothShelf(); break;
    //    case 'v': /* Toggle video screen */ break;
    //    case 'c': /* Toggle cameras */ break;
    //    }
    //}
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
    MainApp::Get().SetMButton( down );
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
    // Zero values in reality are almost non-existent, so
    // we say zero values are not acceptable, which helps
    // us with the replay
    if( !x && !y ) return;

    if( m_bIsMouseDown )
    {
        const int dX = x - m_iMouseX;
        const int dY = y - m_iMouseY;

        const float pcX = float(m_fAccumX) / Width();
        const float pcY = -float(m_fAccumY) / Height();

        float cubeHorizontalAngle = ROTATION_SPEED * pcX * DEG_TO_RAD;
        float cubeVerticalAngle   = ROTATION_SPEED * pcY * DEG_TO_RAD;

        MainApp::GetPhysics().SetCubeAngles( cubeHorizontalAngle, cubeVerticalAngle );

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
void MainWindow::Quit()
{
    if( m_AppState == AS_RECORD ) {
        InputRec::Get().End(); // Stop recoding
    }

    MainApp::GetPhysics().Stop();
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

    float deltaY = 0.05f, startY = 1.0f;
    #define TEXT_Y startY-=deltaY

    glRasterPos2f( -1.0f, TEXT_Y );
    Printf( "Delta: %.4f", (float)Clock::Get().GetTimeDelta() );
    glRasterPos2f( -1.0f, TEXT_Y );
    Printf( "TimeStep: %.4f", MGRPhysics::Get().GetTimeStep() );

    int smallspheres, bigspheres;
    MainApp::GetPhysics().GetNumOfSpheres( smallspheres, bigspheres );
    glRasterPos2f( -1.0f, TEXT_Y );
    Printf( "Big spheres: %i", bigspheres );
    glRasterPos2f( -1.0f, TEXT_Y );
    Printf( "Small Spheres: %i", smallspheres );
    
    glRasterPos2f( -1.0f, TEXT_Y );
    string gamestate("Normal");
    switch(m_AppState)
    {
    case AS_RECORD: gamestate = "Record"; break;
    case AS_REPLAY: gamestate = "Replay"; break;
    }
    Printf( "Gamestate: %s", gamestate.c_str() );

    //if( m_bShowControls )
    //{
    //    glRasterPos2f( -1.0f, TEXT_Y );
    //    Printf( "Hold left mouse button and drag to rotate the cube" );
    //    glRasterPos2f( -1.0f, TEXT_Y );
    //    Printf( "'1' to add sphere" );
    //    glRasterPos2f( -1.0f, TEXT_Y );
    //    Printf( "'2' to remove the last ball added" );
    //    glRasterPos2f( -1.0f, TEXT_Y );
    //    Printf( "'P' to pause" );
    //    glRasterPos2f( -1.0f, TEXT_Y );
    //    Printf( "'R' to toggle record mode" );
    //    glRasterPos2f( -1.0f, TEXT_Y );
    //    Printf( "'O' to replay the last recorded try" );
    //    glRasterPos2f( -1.0f, TEXT_Y );
    //    Printf( "'W' toggle wireframe" );
    //    glRasterPos2f( -1.0f, TEXT_Y );
    //    Printf( "'F' toggle fullscreen" );
    //    glRasterPos2f( -1.0f, TEXT_Y );
    //    Printf( "'Q' to quit" );
    //    glRasterPos2f( -1.0f, TEXT_Y );
    //    Printf( "'S' to close help" );
    //}
    //else
    //{
    //    glRasterPos2f( -1.0f, TEXT_Y );
    //    Printf( "'S' to show help" );
    //}

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