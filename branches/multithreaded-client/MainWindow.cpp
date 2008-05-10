#include "MainWindow.h"
#include "MainApp.h"
#include "CSceneManager.h"
#include "Math/Random.h"
#include "GOCS/CGOCManager.h"    // The component manager

#include "Util/CLogger.h"
#include "Util/Config.h"

using namespace tlib;
using namespace tlib::math;
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
m_bWireframe(false),
m_bShowControls(false),
m_bBallCamActive(false),
m_bTextured(true),
m_ActiveCam(0)
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

    MGRScene::_Get().Init();

    // Initialize camera's position and direction
    m_FrontCamera.SetPosition( Vec3f( 0.0f, 0.25f, 1.25f ) );
    m_FrontCamera.LookAt( Vec3f( 0.0f, 0.0f, 0.0f ) );
    m_ActiveCam = &m_FrontCamera;

    // Setup lighting
    m_Lights[0].SetId(0);
    m_Lights[0].SetPosition( Vec3f( 0.0f, 0.0f, 0.4f ) );
    m_Lights[0].TurnOn();

    //MainApp::GetBitmap().Start();
    MainApp::GetClientRecv().Start();
    MainApp::GetClientSend().Start();
}

//-----------------------------------------------------------------------------
void MainWindow::OnDisplay()
{
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    SetupView();
    UpdateActiveCamera();
    m_Lights[0].Apply();

    //RenderHelpGrid( 40, 0.5f );
    MGRScene::Get().Render();
    PrintStats();

    SwapBuffers();
}

//-----------------------------------------------------------------------------
void MainWindow::OnIdle()
{
    Redraw();
}

//-----------------------------------------------------------------------------
void MainWindow::OnKeyboard( int key, bool down )
{
    key = tolower(key);

    // ugly hack!!
    switch(key)
    {
    case 'q': 
    case 'z': 
    case 'w': 
    case 'f':
    case 'v':
    case 'c': break;
    default: MainApp::Get().SetKey(key, down); break;
    }

    if( down ) return;

    // Handle general input
    switch(key)
    {
    case 'q': Close(); break;
    case 'z': m_bShowControls = !m_bShowControls; break;
    case 'w': 
        {
            if( MGRScene::Get().IsTextured() && !m_bWireframe )
            {
                // Textured and solid -> solid
                MGRScene::Get().ToggleTextured();
            }
            else if( !MGRScene::Get().IsTextured() && !m_bWireframe )
            {
                // Solid -> wireframe
                glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
                m_bWireframe = true;
            }
            else
            {
                // Wireframe -> reset
                MGRScene::Get().ToggleTextured();
                glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
                m_bWireframe = false;
            }
        }
        break;

    case 'f':
        {
            if( GetFullscreen() ) SetFullscreen(false);
            else
                SetFullscreen(true);
        }
        break;

    case 'v': 
        {
            MainApp::GetBitmap().Toggle(); 
            MGRScene::Get().ClearBackplaneTexture();
        }
        break;

    case 'c':
        {
            m_bBallCamActive = !m_bBallCamActive;
            if( m_bBallCamActive )
                m_ActiveCam = &m_BallCamera;
            else
                m_ActiveCam = &m_FrontCamera;
        }
        break;
    }

    Redraw();
} // OnKeyboard()

//-----------------------------------------------------------------------------
void MainWindow::OnMouseButton( MouseButton button, bool down )
{
    
    if( button == MBLeft ) m_bIsMouseDown = down;

    Redraw();
}

//-----------------------------------------------------------------------------
void MainWindow::OnMouseMove( int x, int y )
{
    // Dont act if camera follows a ball
    if( m_bBallCamActive ) return;

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

        m_fCubeHorizontalAngle = ROTATION_SPEED * pcX * DEG_TO_RAD;
        m_fCubeVerticalAngle   = ROTATION_SPEED * pcY * DEG_TO_RAD;

        m_fAccumX += dX;
        m_fAccumY += dY;
    }
    else {
        m_fAccumX = 0.0f;
        m_fAccumY = 0.0f;

        m_fCubeHorizontalAngle = 0.0f;
        m_fCubeVerticalAngle   = 0.0f;
    }

    m_iMouseX = x;
    m_iMouseY = y;

    Redraw();
}

//-----------------------------------------------------------------------------
void MainWindow::OnDestroy()
{
    MainApp::GetClientSend().Terminate();
    MainApp::GetClientRecv().Terminate();
    MainApp::GetBitmap().Terminate();
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

    int smallspheres, bigspheres;
    MainApp::Get().GetNumOfSpheres( smallspheres, bigspheres );
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

//-----------------------------------------------------------------------------
void MainWindow::UpdateActiveCamera()
{
    if( m_bBallCamActive )
    {
        // Get last sphere trasformation data if exists
        CGameObject *sphere = MainApp::Get().GetLastSphere();
        if( !sphere ) {
            // fallback
            m_bBallCamActive = false;
            m_ActiveCam = &m_FrontCamera;
        }
        else {
            // Rotate view and up vector
            Vec3f view  = m_BallCamera.GetView();
            Vec3f up    = m_BallCamera.GetUp();

            sphere->GetOrientation().Rotate( view );
            sphere->GetOrientation().Rotate( up );

            m_BallCamera.SetView( view );
            m_BallCamera.SetUp( up );

            // Set position
            m_BallCamera.SetPosition( sphere->GetPosition() - 0.15f * view );
        }
    }

    m_ActiveCam->Apply();
}